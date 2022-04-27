#ifndef SOBEL_FILTER_H_
#define SOBEL_FILTER_H_
#include <systemc>
using namespace sc_core;

#include <tlm>
#include <tlm_utils/simple_target_socket.h>

#include "search_def.h"
#define V 6
class B_search : public sc_module {
public:
  tlm_utils::simple_target_socket<B_search> t_skt;

  sc_fifo<unsigned char> i_map1;
  sc_fifo<unsigned char> i_map2;
  sc_fifo<unsigned char> o_result;

  SC_HAS_PROCESS(B_search);
  B_search(sc_module_name n);
  ~B_search();

private:
  void do_b_search();
  unsigned char map[8];
  void blocking_transport(tlm::tlm_generic_payload &payload,
                          sc_core::sc_time &delay);
  unsigned char tar_num;
  unsigned char n;
  unsigned char tar_read;
  unsigned char find;
  unsigned char ans;
};
#endif
