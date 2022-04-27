#ifndef SYSTEM_H_
#define SYSTEM_H_
#include <systemc>
using namespace sc_core;

#include "Testbench.h"
#ifndef NATIVE_SYSTEMC
#include "B_search_wrap.h"
#else
#include "B_search.h"
#endif

class System: public sc_module
{
public:
	SC_HAS_PROCESS( System );
	System( sc_module_name n, std::string input_bmp, std::string output_bmp );
	~System();
private:
  Testbench tb;
#ifndef NATIVE_SYSTEMC
	B_search_wrapper b_search;
#else
	B_search b_search;
#endif
	sc_clock clk;
	sc_signal<bool> rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<8> > map;
	cynw_p2p< sc_dt::sc_uint<8> > result;
#else
	sc_fifo< sc_dt::sc_uint<8> > map;
	sc_fifo< sc_dt::sc_uint<8> > result;
#endif

	std::string _output_bmp;
};
#endif
