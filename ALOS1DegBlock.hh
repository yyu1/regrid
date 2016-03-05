//-------------include guard------------
#ifndef __ALOS1DEGBLOCK_HH_INCLUDED__
#define __ALOS1DEGBLOCK_HH_INCLUDED__
//--------------------------------------



#include <cstdint>
#include <iostream>
#include <fstream>




//CLASS Definitions
#define ALOS_BLOCK_XDIM 1620000
#define ALOS_TILE_XDIM 4500
#define ALOS_TILE_YDIM 4500
#define ALOS_TYPE uint16_t   //unsigned 16-bit integer
#define DATA_TOP_DIR /nobackupp6/nexprojects/CMS-ALOS/25m/2007/uncompressed


class ALOS1DegBlock
{
	private:
		ALOS_TYPE *value_grid;
		//std::string construct_tile_name(int longitude, int latitude);


	public:
		ALOS1DegBlock(int latitude); //create and read in 1 deg global block for given latitude
		//Latitude given for the constructor is the upper latitude of the tile (since ALOS tile coordinates are given as upper left corner of the tile
		~ALOS1DegBlock();
		std::string construct_tile_name(int longitude, int latitude);





};


#endif // __ALOS1DEGBLOCK_HH_INCLUDED__
