#include <cassert>
#include <cstdio>
#include <cstdlib>
using namespace std;

#include "Testbench.h"

Testbench::Testbench(sc_module_name n)
    : sc_module(n), initiator("initiator") {
  SC_THREAD(do_search);
}

Testbench::~Testbench() {}

int Testbench::read_map(string infile_name) {
  FILE *fp_s = NULL; // source file handler
  fp_s = fopen(infile_name.c_str(), "rb");
  if (fp_s == NULL) {
    printf("fopen %s error\n", infile_name.c_str());
    return -1;
  }
  //fscanf(fp_s, "%d", &NODE);
  fscanf(fp_s, "%d", &num);
  source_map = (unsigned char *)malloc((size_t)num);
  for (int i = 0; i < num; i++) {
    fscanf(fp_s, "%d", &source_map[i]);
    //printf("%d ", source_map[i]);
  }
  fscanf(fp_s, "%d", &tar);
  fclose(fp_s);
  return 0;
}

int Testbench::write_map(string outfile_name) {
  FILE *fp_t = NULL; // target file handler

  fp_t = fopen(outfile_name.c_str(), "wb");
  if (fp_t == NULL) {
    printf("fopen %s error\n", outfile_name.c_str());
    return -1;
  }
  if (is_find)
    fprintf(fp_t, "The number is found at index %d.\n", tar_ans);
  else
    fprintf(fp_t, "The number is not found.\n");

  fclose(fp_t);
  return 0;
}

void Testbench::do_search() {
  int i, j; // for loop counter
  word data;
  unsigned char mask[4];
  wait(5 * CLOCK_PERIOD, SC_NS);

  data.uc[0] = tar;
  data.uc[1] = num;
  mask[0] = 0xff;
  mask[1] = 0xff;
  mask[2] = 0;
  mask[3] = 0;
  initiator.write_to_socket(SEARCH_MM_BASE + SEARCH_R_ADDR, mask, data.uc, 4);
  wait(1 * CLOCK_PERIOD, SC_NS);

  for (j = 0; j != num / 8; j++) {
    for (i = 0; i != 8; ++i) {
		  data.uc[0] = *(source_map + j * 8 + i);
      mask[0] = 0xff;
      mask[1] = 0;
      mask[2] = 0;
      mask[3] = 0;
      initiator.write_to_socket(SEARCH_MM_BASE + SEARCH_R_ADDR, mask, data.uc, 4);
      wait(1 * CLOCK_PERIOD, SC_NS);
    }
  }
  for (i = 0; i != 8; ++i) {
    if (i < num % 8) {
		  data.uc[0] = *(source_map + j * 8 + i);
      mask[0] = 0xff;
      mask[1] = 0;
      mask[2] = 0;
      mask[3] = 0;
      initiator.write_to_socket(SEARCH_MM_BASE + SEARCH_R_ADDR, mask, data.uc, 4);
      wait(1 * CLOCK_PERIOD, SC_NS);
    }
    else {
      data.uc[0] = 255;
      mask[0] = 0xff;
      mask[1] = 0;
      mask[2] = 0;
      mask[3] = 0;
      initiator.write_to_socket(SEARCH_MM_BASE + SEARCH_R_ADDR, mask, data.uc, 4);
      wait(1 * CLOCK_PERIOD, SC_NS);
    }
  }

  bool done=false;
  int output_num=0;
  while(!done){
    initiator.read_from_socket(SEARCH_MM_BASE + SEARCH_CHECK_ADDR, mask, data.uc, 4);
    output_num = data.uc[0];
    if(output_num>0) done=true;
  }
  wait(3 * (num / 8 + 1) * CLOCK_PERIOD, SC_NS);
  initiator.read_from_socket(SEARCH_MM_BASE + SEARCH_RESULT_ADDR, mask, data.uc, 4);
  tar_ans = data.uc[0];
  is_find = data.uc[1];
  sc_stop();
}
