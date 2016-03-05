test: test.o ALOS1DegBlock.o
	g++ -o test test.o ALOS1DegBlock.o

ALOS1DegBlock.o: ALOS1DegBlock.cc ALOS1DegBlock.hh
	g++ -c ALOS1DegBlock.cc
