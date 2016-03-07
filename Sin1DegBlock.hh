//-------include guard-------
#ifndef __SIN1DEGBLOCK_HH_INCLUDED__
#define __SIN1DEGBLOCK_HH_INCLUDED__
//---------------------------

#include <cstdint>
#include <iostream>
#include <fstream>


//CLASS Definitions
#define TARGET_XDIM 432000
#define TARGET_YDIM 1200
#define MISSING_VALUE 0

class Sin1DegBlock
{
	private:
		float *hhSum_grid;
		float *hvSum_grid;
		unsigned char *count_grid;
		signed char *landWater_grid;

	public:
		Sin1DegBlock();
		~Sin1DegBlock();
		float getMean(unsigned long x, unsigned long y);  //return hv for now
		void addValue(float hhVal, float hvVal, unsigned long x, unsigned long y);
		void reset();
		void writeMeanAsInt16(std::ofstream *outHHValStream, std::ofstream *outHVValStream, std::ofstream *outMaskStream);
};
#endif //__SIN1DEGBLOCK_HH_INCLUDED__

