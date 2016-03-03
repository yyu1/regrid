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

void Sin1DegBlock::writeMeanAsFloat(std::ofstream *outstream) {
	const unsigned long long block_length = TARGET_XDIM*TARGET_YDIM;

	//Check if stream is ok to be written to
	if (!outstream->is_open()) { throw; }

	//Create memory block to hold mean values
	float *meanGrid = new float[block_length];

	//calculate means

	for (unsigned long long i=0; i<block_length; ++i) {
		if (count_grid[i] > 0) {
			meanGrid[i] = sum_grid[i] / (float)count_grid[i];
		} else {
			meanGrid[i] = MISSING_VALUE;
		}

	}

	outstream->write(reinterpret_cast<const char*>(meanGrid), std::streamsize(block_length*sizeof(float)));

	delete[] meanGrid;
}

void Sin1DegBlock::writeMeanAsInt16(std::ofstream *outstream) {
	const unsigned long long block_length = TARGET_XDIM*TARGET_YDIM;

	//check if stream is ok to be written to
	if (!outstream->is_open()) { throw; }

	//Create memory block to hold mean values
	uint16_t *meanGrid = new uint16_t[block_length];


	//calculate means

	for (unsigned long long i=0; i<block_length; ++i) {
		if (count_grid[i] > 0) {
			meanGrid[i] = (uint16_t)((sum_grid[i] / (float)count_grid[i])*10000);
		} else {
			meanGrid[i] = MISSING_VALUE;
		}

	}

	outstream->write(reinterpret_cast<const char*>(meanGrid), std::streamsize(block_length*sizeof(uint16_t)));

	delete[] meanGrid;
}
