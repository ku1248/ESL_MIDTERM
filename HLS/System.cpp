#include "System.h"
System::System( sc_module_name n, string input_bmp, string output_bmp ): sc_module( n ), 
	tb("tb"), b_search("b_search"), clk("clk", CLOCK_PERIOD, SC_NS), rst("rst"), _output_bmp(output_bmp)
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	b_search.i_clk(clk);
	b_search.i_rst(rst);
	tb.o_map(map);
	tb.i_result(result);
	b_search.i_map(map);
	b_search.o_result(result);

  tb.read_map(input_bmp);
}

System::~System() {
  tb.write_map(_output_bmp);
}
