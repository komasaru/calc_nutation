/***********************************************************
  章動の計算
  : IAU2000A 章動理論(MHB2000, IERS2003)による
    黄経における章動(Δψ), 黄道傾斜における章動(Δε) の計算

  * IAU SOFA(International Astronomical Union, Standards of Fundamental Astronomy)
    の提供する C ソースコード "nut00a.c" で実装されているアルゴリズムを使用する。
  * 係数データファイルの項目について
    - 日月章動(luni-solar nutation, "dat_ls.txt")
      (左から) L L' F D Om PS PST PC EC ECT ES
    - 惑星章動(planetary nutation, "dat_pl.txt)
      (左から) L L' F D Om Lm Lv Le LM Lj Ls Lu Ln Pa PS PC ES EC
  * 参考サイト
    - [SOFA Library Issue 2012-03-01 for ANSI C: Complete List](http://www.iausofa.org/2012_0301_C/sofa/)
    - [USNO Circular 179](http://aa.usno.navy.mil/publications/docs/Circular_179.php)
    - [IERS Conventions Center](http://62.161.69.131/iers/conv2003/conv2003_c5.html)

    DATE          AUTHOR          VERSION
    2020.11.10    mk-mode.com     1.00 新規作成

  Copyright(C) 2020 mk-mode.com All Rights Reserved.
------------------------------------------------------------
  引数 : TT（地球時）
           書式：最大23桁の数字
                 （先頭から、西暦年(4), 月(2), 日(2), 時(2), 分(2), 秒(2),
                             1秒未満(9)（小数点以下9桁（ナノ秒）まで））
                 無指定なら現在(システム日時)と判断。
***********************************************************/
#include "common.hpp"
#include "file.hpp"
#include "nutation.hpp"
#include "time.hpp"

#include <cstdlib>   // for EXIT_XXXX
#include <ctime>
#include <iomanip>   // for setprecision
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


int main(int argc, char* argv[]) {
  constexpr double kR2D = 57.29577951308232087679815;   // Radians to degrees
  constexpr double kD2S = 3600.0;                       // Degrees to seconds
  namespace ns = calc_nutation;
  std::string tm_str;  // time string
  unsigned int s_tm;   // size of time string
  int s_nsec;          // size of nsec string
  int ret;             // return of functions
  struct timespec tt;  // TT
  struct tm t = {};    // for work
  double jcn;          // Julian Century Number
  double dpsi;         // delta-psi(rad)
  double deps;         // delta-eps(rad)
  double dpsi_d;       // delta-psi(deg)
  double deps_d;       // delta-eps(deg)
  double dpsi_s;       // delta-psi(sec)
  double deps_s;       // delta-eps(sec)
  std::vector<std::vector<std::string>> l_ls;   // List of Leap Second
  std::vector<std::vector<std::string>> l_dut;  // List of DUT1

  try {
    // 地球時取得
    if (argc > 1) {
      // コマンドライン引数より取得
      tm_str = argv[1];
      s_tm = tm_str.size();
      if (s_tm > 23) {
        std::cout << "[ERROR] Over 23-digits!" << std::endl;
        return EXIT_FAILURE;
      }
      s_nsec = s_tm - 14;
      std::istringstream is(tm_str);
      is >> std::get_time(&t, "%Y%m%d%H%M%S");
      tt.tv_sec  = mktime(&t);
      tt.tv_nsec = 0;
      if (s_tm > 14) {
        tt.tv_nsec = std::stod(
            tm_str.substr(14, s_nsec) + std::string(9 - s_nsec, '0'));
      }
    } else {
      // 現在日時の取得
      ret = std::timespec_get(&tt, TIME_UTC);
      if (ret != 1) {
        std::cout << "[ERROR] Could not get now time!" << std::endl;
        return EXIT_FAILURE;
      }
    }

    // うるう秒, DUT1 一覧取得
    ns::File o_f;
    if (!o_f.get_leap_sec_list(l_ls)) throw;
    if (!o_f.get_dut1_list(l_dut)) throw;

    // Calculation
    ns::Time o_tm(tt, l_ls, l_dut);  // Object of TT
    jcn = o_tm.calc_t();             // ユリウス世紀数(for TT)
    ns::Nutation o_n(jcn);           // Object of Nutation
    if (!o_n.calc_nutation(dpsi, deps)) {
      std::cout << "[ERROR] Could not calculate delta-psi, "
                << "delta-epsilon!" << std::endl;
      return EXIT_FAILURE;
    }
    dpsi_d = dpsi   * kR2D;
    deps_d = deps   * kR2D;
    dpsi_s = dpsi_d * kD2S;
    deps_s = deps_d * kD2S;

    // Display
    std::cout << "         TT: "
              << ns::gen_time_str(tt) << std::endl
              << "   T(of TT): "
              << std::fixed << std::setprecision(10) << jcn
              << " century (= Julian Century Number)" << std::endl
              << std::setprecision(20)
              << "  DeltaPsi = "
              << std::setw(24) << dpsi   << " rad" << std::endl
              << "           = "
              << std::setw(24) << dpsi_d << " °"  << std::endl
              << "           = "
              << std::setw(24) << dpsi_s << " ″"  << std::endl
              << "  DeltaEps = "
              << std::setw(24) << deps   << " rad" << std::endl
              << "           = "
              << std::setw(24) << deps_d << " °"  << std::endl
              << "           = "
              << std::setw(24) << deps_s << " ″"  << std::endl;
  } catch (...) {
      std::cerr << "EXCEPTION!" << std::endl;
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

