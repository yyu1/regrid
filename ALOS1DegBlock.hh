//-------------include guard------------
#ifndef __ALOS1DEGBLOCK_HH_INCLUDED__
#define __ALOS1DEGBLOCK_HH_INCLUDED__
//--------------------------------------



#include <cstdint>
#include <iostream>
#include <fstream>
#include "Sin1DegBlock.hh"



//CLASS Definitions
#define ALOS_BLOCK_XDIM 1620000
#define ALOS_TILE_XDIM 4500
#define ALOS_TILE_YDIM 4500
#define ALOS_TYPE uint16_t   //unsigned 16-bit integer
#define ALOS_MISSING_VALUE 0  //missing value to replace for where there are no tiles (water)

class ALOS1DegBlock
{
	private:
		ALOS_TYPE *hh_grid;
		ALOS_TYPE *hv_grid;
		unsigned char *mask_grid;
		int myLatitude;
		std::string data_top_dir;
		std::string construct_tile_name(int longitude, int latitude);
		unsigned long long blockNPixelOffset(int tileLongitude, unsigned long long tileY);
		void readTile16(std::ifstream *inFile, ALOS_TYPE *valueBlock, int longitude);
		void readTile8(std::ifstream *inFile, unsigned char *valueBlock, int longitude);
		unsigned long vertMap(unsigned int tileY);  //maps tileY to output regridded y

	public:
		ALOS1DegBlock(int latitude); //create and read in 1 deg global block for given latitude
		//Latitude given for the constructor is the upper latitude of the tile (since ALOS tile coordinates are given as upper left corner of the tile
		~ALOS1DegBlock();
		int currentLatitude();
		void reload(int latitude); // reload data from another latitude so we don't need to reallocate memory

		//This function does the main work of putting ALOS looks into output Sin1DegBlocks.
		void regrid(Sin1DegBlock *OutBlock);


};


#endif // __ALOS1DEGBLOCK_HH_INCLUDED__

