#include "Sin1DegBlock.hh"


Sin1DegBlock::Sin1DegBlock() {

	hhSum_grid = new float[TARGET_XDIM*TARGET_YDIM];
	hvSum_grid = new float[TARGET_XDIM*TARGET_YDIM];
	count_grid = new unsigned char[TARGET_XDIM*TARGET_YDIM];
	landWater_grid = new signed char[TARGET_XDIM*TARGET_YDIM];

	this->reset();

}

Sin1DegBlock::~Sin1DegBlock() {
	delete[] hhSum_grid;
	delete[] hvSum_grid;
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
		return hvSum_grid[index] / (float)count_grid[index];
	}

}

void Sin1DegBlock::addValue(float hhVal, float hvVal, unsigned long x, unsigned long y) {

	if ((x > TARGET_XDIM) || (y > TARGET_YDIM)) {
		throw; //out of bounds
	}

	unsigned long index = y*TARGET_XDIM + x;
	hhSum_grid[index] += hhVal;
	hvSum_grid[index] += hvVal;
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
		hhSum_grid[i] = 0;
		hvSum_grid[i] = 0;
		count_grid[i] = 0;
		landWater_grid[i] = 0;
	}

}

void Sin1DegBlock::writeMeanAsInt16(std::ofstream *outHHValStream, std::ofstream *outHVValStream, std::ofstream *outMaskStream) {
	//check if stream is ok to be written to
	if (!(outHHValStream->is_open()  && outHVValStream->is_open())) { throw; }


	const unsigned long long block_length = TARGET_XDIM*TARGET_YDIM;

	//Create memory block to hold mean values
	uint16_t *meanGrid = new uint16_t[block_length];

	//Create memory block to hold land mask
	char *landMaskGrid = new char[block_length];

	//calculate means

	for (unsigned long long i=0; i<block_length; ++i) {
		if (count_grid[i] > 0) {
			meanGrid[i] = (uint16_t)((hhSum_grid[i] / (float)count_grid[i])*10000);
		} else {
			meanGrid[i] = MISSING_VALUE;
		}
		//create landmask
		landMaskGrid[i] = landWater_grid[i] >= 0;

	}

	outHHValStream->write(reinterpret_cast<const char*>(meanGrid), std::streamsize(block_length*sizeof(uint16_t)));
	outMaskStream->write(landMaskGrid, block_length);


	//Work on HV

	for (unsigned long long i=0; i<block_length; ++i) {
		if (landMaskGrid[i]) {
			if (count_grid[i] > 0) {
				meanGrid[i] = (uint16_t)((hvSum_grid[i] / (float)count_grid[i])*10000);
			} else {
				meanGrid[i] = MISSING_VALUE;
			}

		} else {
			meanGrid[i] = MISSING_VALUE;
		}
	}
	outHVValStream->write(reinterpret_cast<const char*>(meanGrid), std::streamsize(block_length*sizeof(uint16_t)));

	delete[] meanGrid;
	delete[] landMaskGrid;
}
