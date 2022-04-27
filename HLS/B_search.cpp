#include <cmath>
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "B_search.h"

B_search::B_search( sc_module_name n ): sc_module( n )
{
#ifndef NATIVE_SYSTEMC
	HLS_FLATTEN_ARRAY(map);
#endif
	SC_THREAD( do_b_search );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);
        
#ifndef NATIVE_SYSTEMC
	i_map.clk_rst(i_clk, i_rst);
  o_result.clk_rst(i_clk, i_rst);
#endif
}

B_search::~B_search() {}

void B_search::do_b_search() {
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_map.reset();
		o_result.reset();
#endif
		wait();
	}
	while (true) {
		tar_read = 0;
		if (tar_read == 0) {
#ifndef NATIVE_SYSTEMC
			{
				HLS_DEFINE_PROTOCOL("input");
				tar_num = i_map.get();
				wait();
			}
#else
			tar_num = i_map.read();
#endif
#ifndef NATIVE_SYSTEMC
			{
				HLS_DEFINE_PROTOCOL("input");
				n = i_map.get();
				wait();
			}
#else
			n = i_map.read();
#endif
			tar_read = 1;
		}
		for (unsigned int i = 0; i<8; ++i) {
			HLS_CONSTRAIN_LATENCY(0, 1, "lat00");
			map[i] = 0;
		}
		find = 0;
		ans = 0;
		sc_dt::sc_uint<8> n_ = n;
		if ((n.range(2, 0) || 0) == 0) {
			n = n >> 3;
		}
		else {
			n = (n >> 3) + 1;
		}
		
		for (int i = 0; i < n; i++) {
			#if defined(II)
			HLS_PIPELINE_LOOP( SOFT_STALL, II, "main_loop_pipeline" ); 
			#endif
			{for (unsigned int j = 0; j<8; ++j) {
			
#ifndef NATIVE_SYSTEMC
				{
					HLS_DEFINE_PROTOCOL("input");
					map[j] = i_map.get();
					int b = map[j];
					wait();
				}
#else
				map[j] = i_map.read();
#endif
			}
			}
			#ifdef UNROLL
			#   define MAIN_LOOP  HLS_UNROLL_LOOP(ALL, "my_loop")
			#endif
			#ifndef UNROLL
			#   define MAIN_LOOP
			#endif
			MAIN_LOOP;
			{
			sc_dt::sc_uint<8> mid;
			sc_dt::sc_uint<8> start = 0;
			sc_dt::sc_uint<8> end = 7;
			for (unsigned int k = 0; k < 4; k++) {						
				mid = (start + end) >> 1;
				HLS_CONSTRAIN_LATENCY(0, 2, "lat01");
				if(map[mid] == tar_num && ans < n_) {
					ans = 8 * i+ mid;
					find = 1;
				}
				else if (map[mid] > tar_num) {
					end = mid - 1;
				}
				else {
					start = mid + 1;
				}
			}
			}
		}
		{
#ifndef NATIVE_SYSTEMC
		{
			HLS_DEFINE_PROTOCOL("output");
			o_result.put(find);
			wait();
		}
#else
		o_result.write(find);
#endif
#ifndef NATIVE_SYSTEMC
		{
			HLS_DEFINE_PROTOCOL("output");
			o_result.put(ans);
			wait();
		}
#else
		o_result.write(ans);
#endif
	}
	}
}
