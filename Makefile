gcc_options = -std=c++17 -Wall -O2 --pedantic-errors

calc_nutation: calc_nutation.o nutation.o delta_t.o time.o file.o common.o
	g++92 $(gcc_options) -o $@ $^

calc_nutation.o : calc_nutation.cpp
	g++92 $(gcc_options) -c $<

nutation.o : nutation.cpp
	g++92 $(gcc_options) -c $<

delta_t.o : delta_t.cpp
	g++92 $(gcc_options) -c $<

time.o : time.cpp
	g++92 $(gcc_options) -c $<

file.o : file.cpp
	g++92 $(gcc_options) -c $<

common.o : common.cpp
	g++92 $(gcc_options) -c $<

run : calc_nutation
	./calc_nutation

clean :
	rm -f ./calc_nutation
	rm -f ./*.o

.PHONY : run clean

