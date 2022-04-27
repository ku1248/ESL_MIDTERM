#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <string>
using namespace std;

#include <systemc>
using namespace sc_core;

#include "Initiator.h"
#include "MemoryMap.h"
#include "search_def.h"
#define NODE 6
class Testbench : public sc_module {
public:
  Initiator initiator;

  SC_HAS_PROCESS(Testbench);

  Testbench(sc_module_name n);
  ~Testbench();

  int read_map(string infile_name);
  int write_map(string outfile_name);

private:
  unsigned char *source_map;
  unsigned char *target_map;
  void do_search();
  unsigned char num;
	unsigned char tar;
	unsigned char tar_ans;
	unsigned char is_find;
};
#endif
