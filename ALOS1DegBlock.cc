#include "ALOS1DegBlock.hh"


ALOS1DegBlock::ALOS1DegBlock(int latitude) {
	if (latitude > 90 || latitude < -89)  {throw;}

	//allocate heap space for the data block
	const unsigned long long block_pixels = ALOS_BLOCK_XDIM * ALOS_TILE_YDIM
	value_grid = new ALOS_TYPE[block_npixels]

	//Read data from files into new grid
	for (int i=-180, i<180, ++i) {



	}

}


ALOS1DegBlock::~ALOS1DegBlock() {




}


std::string ALOS1DegBlock::construct_tile_name(int longitude, int latitude) {
	if(longitude > 179 || longitude < -180 || latitude > 90 || latitude < -89) {throw;}



}
