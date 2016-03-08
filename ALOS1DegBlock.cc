#include "ALOS1DegBlock.hh"

ALOS1DegBlock::ALOS1DegBlock(int latitude) {
	if (latitude > 90 || latitude < -89)  {throw;}

	data_top_dir = "/nobackupp6/nexprojects/CMS-ALOS/25m/2007/uncompressed/";
	myLatitude = latitude;


	//allocate heap space for the data block
	const unsigned long long block_xdim = ALOS_BLOCK_XDIM;
	const unsigned long long tile_ydim = ALOS_TILE_YDIM;
	const unsigned long long block_npixels = block_xdim * tile_ydim;
	hh_grid = new ALOS_TYPE[block_npixels];
	hv_grid = new ALOS_TYPE[block_npixels];
	mask_grid = new unsigned char[block_npixels];

	std::string blockFilePrefix = data_top_dir;
	if (myLatitude >= 0) {
		blockFilePrefix.append("north/");
	} else {
		blockFilePrefix.append("south/");
	}

	//Read data from files into new grid
	std::ifstream inFile;
	std::cout << "Reading tiles for latitude " << latitude << '\n';
	for (int i=-180; i < 180; ++i) {
		std::string tileFilePrefix = blockFilePrefix;
		tileFilePrefix.append(this->construct_tile_name(i, myLatitude));
		tileFilePrefix.append("_07_");
		
		std::string hhTileName = tileFilePrefix;
		hhTileName.append("sl_HH");
		std::string hvTileName = tileFilePrefix;
		hvTileName.append("sl_HV");
		std::string maskTileName = tileFilePrefix;
		maskTileName.append("mask");
	

		//Read HH
		std::cout << "Reading " << hhTileName << '\n';
		inFile.open(hhTileName, std::ios::binary | std::ios::in);
		readTile16(&inFile, hh_grid, i);
		inFile.close();

		//Read HV
		std::cout << "Reading " << hvTileName << '\n';
		inFile.open(hvTileName, std::ios::binary | std::ios::in);
		readTile16(&inFile, hv_grid, i);
		inFile.close();

		//Read mask
		std::cout << "Reading " << maskTileName << '\n';
		inFile.open(maskTileName, std::ios::binary | std::ios::in);
		readTile8(&inFile, mask_grid, i);
		inFile.close();

	}
}


ALOS1DegBlock::~ALOS1DegBlock() {
	delete[] hh_grid;
	delete[] hv_grid;
	delete[] mask_grid;
}

int ALOS1DegBlock::currentLatitude() {
	return myLatitude;
}

//This returns the pixel offset (array index for the alos1deg source block) for a given tile and row within tile
unsigned long long ALOS1DegBlock::blockNPixelOffset(int tileLongitude, unsigned long long tileY) {

	if (tileLongitude < -180 || tileLongitude > 179 || tileY > ALOS_TILE_YDIM-1) {throw;}

	unsigned long long offset = tileY * ALOS_BLOCK_XDIM + (tileLongitude + 180)*ALOS_TILE_XDIM;

	return offset;

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


void ALOS1DegBlock::readTile16(std::ifstream *inFile, ALOS_TYPE *valueBlock, int longitude) {
	if (longitude < -180 || longitude > 179) {throw;}
	if (inFile->good()) {
		for (unsigned long j=0; j<ALOS_TILE_YDIM; ++j) {
			ALOS_TYPE *curMemPointer = valueBlock+blockNPixelOffset(longitude, j);
			inFile->read(reinterpret_cast<char*>(curMemPointer), std::streamsize(ALOS_TILE_XDIM*sizeof(ALOS_TYPE)));
		}
	} else {
		//Fill with padding of missing value
		for (unsigned long j=0; j<ALOS_TILE_YDIM; ++j) {
			unsigned long long curOffSet = blockNPixelOffset(longitude, j);
			for (unsigned long i=0; i<ALOS_TILE_XDIM; ++i) {
				valueBlock[curOffSet+i] = ALOS_MISSING_VALUE;
			}
		}
	}
}

void ALOS1DegBlock::readTile8(std::ifstream *inFile, unsigned char *valueBlock, int longitude) {
	if (longitude < -180 || longitude > 179) {throw;}
	if (inFile->good()) {
		for (unsigned long j=0; j<ALOS_TILE_YDIM; ++j) {
			inFile->read((char*)(valueBlock+blockNPixelOffset(longitude, j)), std::streamsize(ALOS_TILE_XDIM*sizeof(ALOS_TYPE)));
		}
	} else {
		//Fill with padding of missing value
		for (unsigned long j=0; j<ALOS_TILE_YDIM; ++j) {
			unsigned long long curOffSet = blockNPixelOffset(longitude, j);
			for (unsigned long i=0; i<ALOS_TILE_XDIM; ++i) {
				valueBlock[curOffSet+i] = ALOS_MISSING_VALUE;
			}
		}
	}

}

unsigned long ALOS1DegBlock::vertMap(unsigned int tileY) {
	//we are mapping from 0.8sec to 3 sec.
	unsigned long temp1 = tileY * TARGET_YDIM;
	temp1 /= ALOS_TILE_YDIM;
	return temp1;
}

unsigned long ALOS1DegBlock::horzMap(unsigned long xPix, double latitude) {
	if (xPix >= ALOS_BLOCK_XDIM || latitude < -90 || latitude > 90) {throw;}

	//Convert Latitude to Radians
	latitude = latitude * M_PI / 180;

	//convert xPix to a unit that is signed
	double xValue = (double)xPix - (ALOS_BLOCK_XDIM/2) + 0.5;
	xValue = xValue * cos(latitude);

	return (unsigned long)(xValue + (ALOS_BLOCK_XDIM/2));

}

void ALOS1DegBlock::regrid(Sin1DegBlock* outBlock) {
	unsigned long long index;
	double latitude;
	unsigned long targetX, targetY;

	for (unsigned int j=0; j<ALOS_TILE_YDIM; ++j) {
		latitude = (double)myLatitude - ((double)j+0.5)*ALOS_PIXEL_SIZE_DEGREES;
		targetY = vertMap(j);
		for (unsigned long i=0; i<ALOS_BLOCK_XDIM; ++i) {
			index = (unsigned long long)j*ALOS_BLOCK_XDIM + i;

			if (mask_grid[index] == 0) {continue;} //Alos pixel has no data

			targetX = horzMap(i, latitude);
			switch(mask_grid[index]) {
				case 50:
					//water
					outBlock->addLandWaterPixel(targetX, targetY, false);
				case 100:
					//layover
					outBlock->addLandWaterPixel(targetX, targetY, true);
				case 150:
					//shadow
					outBlock->addLandWaterPixel(targetX, targetY, true);
				case 255:
					//land
					outBlock->addValue(hh_grid[index], hv_grid[index], targetX, targetY);
					outBlock->addLandWaterPixel(targetX, targetY, true);
			}
					


			
		}
	}

}
