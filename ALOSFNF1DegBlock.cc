#include "ALOSFNF1DegBlock.hh"

ALOSFNF1DegBlock::ALOSFNF1DegBlock(int latitude) {
	if (latitude > 90 || latitude < -89)  {throw;}

	data_top_dir = "/nobackupp6/nexprojects/CMS-ALOS/25m/2007/uncompressed/";


	//allocate heap space for the data block
	const unsigned long long block_xdim = ALOS_BLOCK_XDIM;
	const unsigned long long tile_ydim = ALOS_TILE_YDIM;
	const unsigned long long block_npixels = block_xdim * tile_ydim;
	fnf_grid = new unsigned char[block_npixels];

	loadData(latitude);
}

void ALOSFNF1DegBlock::loadData(int latitude) {
	if (latitude > 90 || latitude < -89)  {throw;}
	myLatitude = latitude;

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
		
		std::string fnfTileName = tileFilePrefix;
		fnfTileName.append("_C");
	

		//Read FNF
		inFile.open(fnfTileName, std::ios::binary | std::ios::in);
		readTile8(&inFile, fnf_grid, i);
		inFile.close();
	}


}


ALOSFNF1DegBlock::~ALOSFNF1DegBlock() {
	delete[] fnf_grid;
}

int ALOSFNF1DegBlock::currentLatitude() {
	return myLatitude;
}

void ALOSFNF1DegBlock::reload(int latitude) {
	//reload tiles from given latitude
	if (latitude == myLatitude) {return;}  //No change in latitude, we don't need to do anything
	
	loadData(latitude);
}

//This returns the pixel offset (array index for the alos1deg source block) for a given tile and row within tile
unsigned long long ALOSFNF1DegBlock::blockNPixelOffset(int tileLongitude, unsigned long long tileY) {

	if (tileLongitude < -180 || tileLongitude > 179 || tileY > ALOS_TILE_YDIM-1) {throw;}

	unsigned long long offset = tileY * ALOS_BLOCK_XDIM + (tileLongitude + 180)*ALOS_TILE_XDIM;

	return offset;

}

std::string ALOSFNF1DegBlock::construct_tile_name(int longitude, int latitude) {
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


void ALOSFNF1DegBlock::readTile8(std::ifstream *inFile, unsigned char *valueBlock, int longitude) {
	if (longitude < -180 || longitude > 179) {throw;}
	if (inFile->good()) {
		for (unsigned long j=0; j<ALOS_TILE_YDIM; ++j) {
			inFile->read((char*)(valueBlock+blockNPixelOffset(longitude, j)), std::streamsize(ALOS_TILE_XDIM));
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

unsigned long ALOSFNF1DegBlock::vertMap(unsigned int tileY) {
	//we are mapping from 0.8sec to 3 sec.
	unsigned long temp1 = tileY * TARGET_YDIM;
	temp1 /= ALOS_TILE_YDIM;
	return temp1;
}

unsigned long ALOSFNF1DegBlock::horzMap(unsigned long xPix, double latitude) {
	if (xPix >= ALOS_BLOCK_XDIM || latitude < -90 || latitude > 90) {throw;}

	//Convert Latitude to Radians
	latitude = latitude * M_PI / 180;
	
	//scaling ratio
	const double scalingRatio = (double)TARGET_XDIM/(double)ALOS_BLOCK_XDIM;

	//convert xPix to a unit that is signed
	double xValue = (double)xPix*scalingRatio - (TARGET_XDIM/2) + 0.5;
	xValue = xValue * cos(latitude);

	return (unsigned long)(xValue + (TARGET_XDIM/2));

}

void ALOSFNF1DegBlock::regrid(SinFNF1DegBlock* outBlock) {
	unsigned long long index;
	double latitude;
	unsigned long targetX, targetY;

	for (unsigned int j=0; j<ALOS_TILE_YDIM; ++j) {
		latitude = (double)myLatitude - ((double)j+0.5)*ALOS_PIXEL_SIZE_DEGREES;
		//std::cout << j << " " << latitude << '\n';
		targetY = vertMap(j);
		for (unsigned long i=0; i<ALOS_BLOCK_XDIM; ++i) {
			index = ((unsigned long long)j)*((unsigned long long)ALOS_BLOCK_XDIM) + i;

			if (fnf_grid[index] == 0) {continue;} //Alos pixel has no data

			targetX = horzMap(i, latitude);
			outBlock->addFNFPixel(targetX, targetY, fnf_grid[index]);
					
		}
	}

}
