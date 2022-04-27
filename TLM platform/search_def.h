#ifndef FILTER_DEF_H_
#define FILTER_DEF_H_

// FLOYD Filter inner transport addresses
// Used between blocking_transport() & do_filter()
const int SEARCH_R_ADDR = 0x00000000;
const int SEARCH_RESULT_ADDR = 0x00000004;
const int SEARCH_CHECK_ADDR = 0x00000008;

union word {
  int sint;
  unsigned int uint;
  unsigned char uc[4];
};

#endif
