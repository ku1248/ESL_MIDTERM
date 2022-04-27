#include <cmath>
#include <iomanip>

#include "B_search.h"

B_search::B_search(sc_module_name n)
    : sc_module(n), t_skt("t_skt") {
  SC_THREAD(do_b_search);

  t_skt.register_b_transport(this, &B_search::blocking_transport);
}

B_search::~B_search() = default;

void B_search::do_b_search() {
  while (true) {
    tar_read = 0;
    for (unsigned int i = 0; i < 8; ++i) {
      map[i] = 0;
    }    
    if (tar_read == 0) {
      tar_num = i_map1.read();
      n = i_map2.read();
      tar_read = 1;
    }
    find = 0;
    ans = 0;
    unsigned char n_ = n;
    if (n % 8 == 0) {
      n = n >> 3;
    }
    else {
      n = (n >> 3) + 1;
    }
    for (int i = 0; i < n; i++) {
			for (unsigned int j = 0; j<8; ++j) {
        map[j] = i_map1.read();
      }
      unsigned char mid, start = 0, end = 7;
      for (unsigned int k = 0; k < 4; k++) {
        mid = (start + end) >> 1;
        if(map[mid] == tar_num && ans < n_) {
		  		ans = 8 * i + mid;
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
    o_result.write(ans);
    o_result.write(find);
  }
}

void B_search::blocking_transport(tlm::tlm_generic_payload &payload,
                                     sc_core::sc_time &delay) {
  sc_dt::uint64 addr = payload.get_address();
  unsigned char *mask_ptr = payload.get_byte_enable_ptr();
  unsigned char *data_ptr = payload.get_data_ptr();
  word buffer;
  switch (payload.get_command()) {
  case tlm::TLM_READ_COMMAND:
    switch (addr) {
    case SEARCH_RESULT_ADDR:
      buffer.uc[0] = o_result.read();
      buffer.uc[1] = o_result.read();
      break;
    case SEARCH_CHECK_ADDR:
      buffer.uint = o_result.num_available();
    break;
    default:
      std::cerr << "Error! B_search::blocking_transport: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
    }
    data_ptr[0] = buffer.uc[0];
    data_ptr[1] = buffer.uc[1];
    data_ptr[2] = buffer.uc[2];
    data_ptr[3] = buffer.uc[3];
    break;
  case tlm::TLM_WRITE_COMMAND:
    switch (addr) {
    case SEARCH_R_ADDR:
      if (mask_ptr[0] == 0xff) {
        i_map1.write(data_ptr[0]);
      }
      if (mask_ptr[1] == 0xff) {
        i_map2.write(data_ptr[1]);
      } 
      break;
    default:
      std::cerr << "Error! B_search::blocking_transport: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
    }
    break;
  case tlm::TLM_IGNORE_COMMAND:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  default:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  }
  payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
}