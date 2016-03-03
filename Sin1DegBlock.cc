#include "Sin1DegBlock.hh"


Sin1DegBlock::Sin1DegBlock() {

	sum_grid = new float[TARGET_XDIM*TARGET_YDIM];
	count_grid = new char[TARGET_XDIM*TARGET_YDIM];

	for (unsigned long i=0; i<TARGET_XDIM*TARGET_YDIM; ++i) {
		sum_grid[i] = 0;
		count_grid[i] = 0;
	}

}

Sin1DegBlock::~Sin1DegBlock() {
	delete[] sum_grid;
	delete[] count_grid;
}



float Sin1DegBlock::getMean(unsigned long x, unsigned long y) {
	if ((x > TARGET_XDIM) || (y > TARGET_YDIM)) {
		throw;  //out of bounds
	}

	unsigned long index = y*TARGET_XDIM + x;

	if (count_grid[index] == 0) {
		return MISSING_VALUE;
	} else {
		return sum_grid[index] / (float)count_grid[index];
	}

}

void Sin1DegBlock::addValue(float val, unsigned long x, unsigned long y) {

	if ((x > TARGET_XDIM) || (y > TARGET_YDIM)) {
		throw; //out of bounds
	}

	unsigned long index = y*TARGET_XDIM + x;
	sum_grid[index] += val;
	++count_grid[index];
}

void Sin1DegBlock::reset() {

	for (unsigned long i=0; i<TARGET_XDIM*TARGET_YDIM; ++i) {
		sum_grid[i] = 0;
		count_grid[i] = 0;
	}

}
