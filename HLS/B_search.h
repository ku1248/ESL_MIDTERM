#ifndef SOBEL_FILTER_H_
#define SOBEL_FILTER_H_
#include <systemc>
using namespace sc_core;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

#include "b_search_def.h"
#define V 6
class B_search: public sc_module
{
public:
	sc_in_clk i_clk;
	sc_in < bool >  i_rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<8> >::in i_map;
	cynw_p2p< sc_dt::sc_uint<8> >::out o_result;
#else
	sc_fifo_in< sc_dt::sc_uint<8> > i_map;
	sc_fifo_out< sc_dt::sc_uint<8> > o_result;
#endif

	SC_HAS_PROCESS( B_search );
	B_search( sc_module_name n );
	~B_search();
private:
	sc_dt::sc_uint<8> map[8];
	sc_dt::sc_uint<8> tar_num;
	sc_dt::sc_uint<8> ans;
	sc_dt::sc_uint<1> tar_read;
	sc_dt::sc_uint<1> find;
	sc_dt::sc_uint<8> n;
	void do_b_search();
};
#endif
