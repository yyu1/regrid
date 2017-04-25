//-------include guard-------
#ifndef __SINFNF1DEGBLOCK_HH_INCLUDED__
#define __SINFNF1DEGBLOCK_HH_INCLUDED__
//---------------------------

#include <cstdint>
#include <iostream>
#include <fstream>


//CLASS Definitions
#define TARGET_XDIM 432000
#define TARGET_YDIM 1200
#define MISSING_VALUE 0

class SinFNF1DegBlock
{
	private:
		unsigned char *forest_count_grid;
		unsigned char *water_count_grid;
		unsigned char *nonforest_count_grid;

	public:
		SinFNF1DegBlock();
		~SinFNF1DegBlock();
		void reset();
		void addFNFPixel(unsigned long x, unsigned long y, unsigned char pixel);
		void writeFNFOutput(std::ofstream *outFNFStream);
};
#endif //__SINFNF1DEGBLOCK_HH_INCLUDED__

