#include "SinFNF1DegBlock.hh"


SinFNF1DegBlock::SinFNF1DegBlock() {

	forest_count_grid = new unsigned char[TARGET_XDIM*TARGET_YDIM];
	water_count_grid = new unsigned char[TARGET_XDIM*TARGET_YDIM];
	nonforest_count_grid = new unsigned char[TARGET_XDIM*TARGET_YDIM];

	this->reset();

}

SinFNF1DegBlock::~SinFNF1DegBlock() {
	delete[] forest_count_grid;
	delete[] water_count_grid;
	delete[] nonforest_count_grid;
}


void SinFNF1DegBlock::addFNFPixel(unsigned long x, unsigned long y, unsigned char pixel) {

	if ((x > TARGET_XDIM) || (y > TARGET_YDIM)) {
		throw; //out of bounds
	}

	unsigned long index = y*TARGET_XDIM+x;
	switch (pixel) {
		case 1:
			//forest
			forest_count_grid[index]++;
			break;
		case 2:
			//non-forest
			nonforest_count_grid[index]++;
			break;
		case 3:
			//water
			water_count_grid[index]++;
			break;
	}
	
}

void SinFNF1DegBlock::reset() {

	for (unsigned long i=0; i<TARGET_XDIM*TARGET_YDIM; ++i) {
		forest_count_grid[i] = 0;
		water_count_grid[i] = 0;
		nonforest_count_grid[i] = 0;
	}

}

void SinFNF1DegBlock::writeFNFOutput(std::ofstream *outFNFStream) {
	//check if stream is ok to be written to
	if (!outFNFStream->is_open()) { throw; }


	const unsigned long long block_length = TARGET_XDIM*TARGET_YDIM;

	//Create memory block to hold output fnf map
	unsigned char *outFNFGrid = new unsigned char[block_length];

	//calculate forest cover percent

	for (unsigned long long i=0; i<block_length; ++i) {
		if (forest_count_grid[i]+water_count_grid[i]+nonforest_count_grid[i] > 0) {
			outFNFGrid[i] = (unsigned char)((float)forest_count_grid[i] / ((float)nonforest_count_grid[i] + (float)water_count_grid[i])*100);
		} else {
			outFNFGrid[i] = 0;
		}

	}

	outFNFStream->write(reinterpret_cast<const char*>(outFNFGrid), block_length);


	delete[] outFNFGrid;
}
