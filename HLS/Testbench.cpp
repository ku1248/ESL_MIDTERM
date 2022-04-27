#include <cstdio>
#include <cstdlib>
using namespace std;

#include "Testbench.h"

Testbench::Testbench(sc_module_name n) : sc_module(n) {
  SC_THREAD(feed_map);
  sensitive << i_clk.pos();
  dont_initialize();
  SC_THREAD(fetch_result);
  sensitive << i_clk.pos();
  dont_initialize();
}

Testbench::~Testbench() {
	//cout<< "Max txn time = " << max_txn_time << endl;
	//cout<< "Min txn time = " << min_txn_time << endl;
	//cout<< "Avg txn time = " << total_txn_time/n_txn << endl;
  cout << "Total latency = " << total_latency << endl;
	cout << "Total run time = " << total_run_time << endl;
}

int Testbench::read_map(string infile_name) {
  FILE *fp_s = NULL; // source file handler
  fp_s = fopen(infile_name.c_str(), "rb");
  if (fp_s == NULL) {
    printf("fopen %s error\n", infile_name.c_str());
    return -1;
  }
  //fscanf(fp_s, "%d", &node);
  fscanf(fp_s, "%d", &num);
  source_map = (unsigned char *)malloc((size_t)num);
  target_map = (unsigned char *)malloc((size_t)num);
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

void Testbench::feed_map() {
  unsigned int x, y, i, j, v, u; // for loop counter
  unsigned char R, G, B;      // color of R, G, B
  int adjustX, adjustY, xBound, yBound;
	n_txn = 0;
	max_txn_time = SC_ZERO_TIME;
	min_txn_time = SC_ZERO_TIME;
	total_txn_time = SC_ZERO_TIME;

#ifndef NATIVE_SYSTEMC
	o_map.reset();
#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);
	total_start_time = sc_time_stamp();

  o_map.put(tar);
  o_map.put(num);
  // if (num % 8 == 0)
  //   num = num / 8;
  // else
  //   num = num / 8 + 1;
  for (j = 0; j != num / 8; j++) {
    for (i = 0; i != 8; ++i) {
#ifndef NATIVE_SYSTEMC
		  o_map.put(*(source_map + j * 8 + i));
#else
		  o_map.write(*(source_map + j * 8 + i));
#endif
    }
  }
  for (i = 0; i != 8; ++i) {
    if (i < num % 8) {
#ifndef NATIVE_SYSTEMC
		  o_map.put(*(source_map + j * 8 + i));
#else
		  o_map.write(*(source_map + j * 8 + i));
#endif
    }
    else {
#ifndef NATIVE_SYSTEMC
		  o_map.put(255);
#else
		  o_map.write(255);
#endif
    }
  }
}

int Testbench::clock_cycle( sc_time time )
{
    sc_clock * clk_p = DCAST < sc_clock * >( i_clk.get_interface() );
    sc_time clock_period = clk_p->period(); // get period from the sc_clock object.
    return ( int )( time / clock_period );

}

void Testbench::fetch_result() {
  //static std::queue<sc_time> time_queue;
  unsigned int x, y, i; // for loop counter
  int total;
  //time_queue.push(sc_time_stamp());
#ifndef NATIVE_SYSTEMC
	i_result.reset();
#endif
	wait(5);
	wait(1);
#ifndef NATIVE_SYSTEMC
  is_find = i_result.get();
#else
  is_find = i_result.read();
#endif
#ifndef NATIVE_SYSTEMC
  tar_ans = i_result.get();
#else
  tar_ans = i_result.read();
#endif
	total_run_time = sc_time_stamp() - total_start_time;
  total_latency = clock_cycle(total_run_time);
  sc_stop();
}
