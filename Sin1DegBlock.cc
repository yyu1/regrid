#include "Sin1DegBlock.hh"


Sin1DegBlock::Sin1DegBlock() {

	sum_grid = new float[TARGET_XDIM*TARGET_YDIM];
	count_grid = new unsigned char[TARGET_XDIM*TARGET_YDIM];
	landWater_grid = new signed char[TARGET_XDIM*TARGET_YDIM];

	for (unsigned long i=0; i<TARGET_XDIM*TARGET_YDIM; ++i) {
		sum_grid[i] = 0;
		count_grid[i] = 0;
	}

}

Sin1DegBlock::~Sin1DegBlock() {
	delete[] sum_grid;
	delete[] count_grid;
	delete[] landWater_grid;
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

void Sin1DegBlock::addLandWaterPixel(unsigned long x, unsigned long y, bool isLand) {

	if ((x > TARGET_XDIM) || (y > TARGET_YDIM)) {
		throw; //out of bounds
	}

	unsigned long index = y*TARGET_XDIM+x;
	isLand ? landWater_grid[index]++ : landWater_grid[index]--; //For final determination, negative is water, >=0 is land
}

void Sin1DegBlock::reset() {

	for (unsigned long i=0; i<TARGET_XDIM*TARGET_YDIM; ++i) {
		sum_grid[i] = 0;
		count_grid[i] = 0;
	}

}

void Sin1DegBlock::writeMeanAsFloat(std::ofstream *outValStream, std::ofstream *outMaskStream) {
	const unsigned long long block_length = TARGET_XDIM*TARGET_YDIM;

	//Check if stream is ok to be written to
	if (!outstream->is_open()) { throw; }

	//Create memory block to hold mean values
	float *meanGrid = new float[block_length];

	//Create memory block to hold land mask
	char *landMaskGrid = new char[block_length];

	//calculate means

	for (unsigned long long i=0; i<block_length; ++i) {
		if (count_grid[i] > 0) {
			meanGrid[i] = sum_grid[i] / (float)count_grid[i];
		} else {
			meanGrid[i] = MISSING_VALUE;
		}

		//create landmask
		landMaskGrid[i] = landWater_grid[i] >= 0;

	}

	outValStream->write(reinterpret_cast<const char*>(meanGrid), std::streamsize(block_length*sizeof(float)));
	outMaskStream->write(landMaskGrid, block_length);

	delete[] meanGrid;
	delete[] landMaskGrid;
}

void Sin1DegBlock::writeMeanAsInt16(std::ofstream *outValStream, std::ofstream *outMaskStream) {
	const unsigned long long block_length = TARGET_XDIM*TARGET_YDIM;

	//check if stream is ok to be written to
	if (!outstream->is_open()) { throw; }

	//Create memory block to hold mean values
	uint16_t *meanGrid = new uint16_t[block_length];

	//Create memory block to hold land mask
	char *landMaskGrid = new char[block_length];

	//calculate means

	for (unsigned long long i=0; i<block_length; ++i) {
		if (count_grid[i] > 0) {
			meanGrid[i] = (uint16_t)((sum_grid[i] / (float)count_grid[i])*10000);
		} else {
			meanGrid[i] = MISSING_VALUE;
		}
		//create landmask
		landMaskGrid[i] = landWater_grid[i] >= 0;

	}

	outValStream->write(reinterpret_cast<const char*>(meanGrid), std::streamsize(block_length*sizeof(uint16_t)));
	outMaskStream->write(landMaskGrid, block_length);

	delete[] meanGrid;
	delete[] landMaskGrid;
}
