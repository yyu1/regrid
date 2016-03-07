all: test

test: test.o ALOS1DegBlock.o Sin1DegBlock.o
	g++ -o test test.o ALOS1DegBlock.o Sin1DegBlock.o

ALOS1DegBlock.o: ALOS1DegBlock.cc ALOS1DegBlock.hh
	g++ -c ALOS1DegBlock.cc

Sin1DegBlock.o: Sin1DegBlock.cc Sin1DegBlock.hh
	g++ -c Sin1DegBlock.cc

clean:
	rm *.o test
