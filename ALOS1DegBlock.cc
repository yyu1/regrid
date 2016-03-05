#include "ALOS1DegBlock.hh"


ALOS1DegBlock::ALOS1DegBlock(int latitude) {
	if (latitude > 90 || latitude < -89)  {throw;}

	//allocate heap space for the data block
	const unsigned long long block_xdim = ALOS_BLOCK_XDIM;
	const unsigned long long tile_ydim = ALOS_TILE_YDIM;
	const unsigned long long block_npixels = block_xdim * tile_ydim;
	value_grid = new ALOS_TYPE[block_npixels];

	//Read data from files into new grid
	for (int i=-180; i < 180; ++i) {



	}

}


ALOS1DegBlock::~ALOS1DegBlock() {
	delete[] value_grid;
}


std::string ALOS1DegBlock::construct_tile_name(int longitude, int latitude) {
	if(longitude > 179 || longitude < -180 || latitude > 90 || latitude < -89) {throw;}

	std::string tile_name;
	std::string longitude_string;
	std::string latitude_string;

	//Create latitute portion of the name
	if (latitude >= 0) {
		tile_name += 'N';
		latitude_string = std::to_string(latitude);
	} else {
		tile_name += 'S';
		latitude_string = std::to_string(-latitude);
	}
	if (latitude_string.length() == 1) {latitude_string.insert(0,"0");} //add 0 padding
	tile_name += latitude_string;

	//Create longitude portion of the name
	if (longitude >= 0) {
		tile_name += 'E';
		longitude_string = std::to_string(longitude);
	} else {
		tile_name += 'W';
		longitude_string = std::to_string(-longitude);
	}
	for (int i=longitude_string.length(); i<3; ++i) {
		longitude_string.insert(0,"0");
	} //Add 0 padding
	tile_name += longitude_string;

	return tile_name;
}
