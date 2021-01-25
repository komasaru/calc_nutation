#ifndef CALC_NUTATION_COMMON_HPP_
#define CALC_NUTATION_COMMON_HPP_

#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

namespace calc_nutation {

// -------------------------------------
//   Functions
// -------------------------------------
std::string gen_time_str(struct timespec);

}  // namespace calc_nutation

#endif

