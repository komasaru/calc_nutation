# calc_nutation

ビルド方法
==========

`make`

（やり直す場合は、 `make clean` をしてから）

準備
====

* うるう年ファイル `LEAP_SEC.txt`, DUT1 ファイル `DUT1.txt` は適宜最新のものに更新すること。
* 係数データ `NUT_LS.txt`, `NUT_PL.txt` については、「[こちら](https://www.mk-mode.com/blog/2016/06/22/ruby-calc-nutation-by-iau2000a "Ruby - 章動の計算（IAU2000A 理論）！")」を参照のこと。

実行方法
========

`./calc_nutation [YYYYMMDDHHMMSSMMMMMMMMM]`

* コマンドライン引数に指定するのは TT（地球時）である。
* TT（地球時）は「年・月・日・時・分・秒・ナノ秒」を最大23桁で指定する。
* TT（地球時）を指定しない場合は、システム日時を TT とみなす。
* TT（地球時）を先頭から部分的に指定した場合は、指定していない部分を 0 とみなす。

