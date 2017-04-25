//-------------include guard------------
#ifndef __ALOSFNF1DEGBLOCK_HH_INCLUDED__
#define __ALOSFNF1DEGBLOCK_HH_INCLUDED__
//--------------------------------------



#include <cstdint>
#include <iostream>
#include <fstream>
#include <cmath>
#include "SinFNF1DegBlock.hh"



//CLASS Definitions
#define ALOS_BLOCK_XDIM 1620000
#define ALOS_TILE_XDIM 4500
#define ALOS_TILE_YDIM 4500
#define ALOS_TYPE uint16_t   //unsigned 16-bit integer
#define ALOS_MISSING_VALUE 0  //missing value to replace for where there are no tiles (water)
#define ALOS_PIXEL_SIZE_DEGREES 0.00022222222222222222222222222222222222222222222222  //alos 0.8 sec in units of degrees

class ALOSFNF1DegBlock
{
	private:
		unsigned char *fnf_grid;
		int myLatitude;
		std::string data_top_dir;
		std::string construct_tile_name(int longitude, int latitude);
		unsigned long long blockNPixelOffset(int tileLongitude, unsigned long long tileY);
		void readTile8(std::ifstream *inFile, unsigned char *valueBlock, int longitude);
		void loadData(int latitude);

	public:
		static unsigned long vertMap(unsigned int tileY);  //maps tileY to output regridded y
		static unsigned long horzMap(unsigned long xPix, double latitude); //map to target block x
		ALOSFNF1DegBlock(int latitude); //create and read in 1 deg global block for given latitude
		//Latitude given for the constructor is the upper latitude of the tile (since ALOS tile coordinates are given as upper left corner of the tile
		~ALOSFNF1DegBlock();
		int currentLatitude();
		void reload(int latitude); // reload data from another latitude so we don't need to reallocate memory

		//This function does the main work of putting ALOS looks into output Sin1DegBlocks.
		void regrid(SinFNF1DegBlock *outBlock);

};


#endif // __ALOSFNF1DEGBLOCK_HH_INCLUDED__

