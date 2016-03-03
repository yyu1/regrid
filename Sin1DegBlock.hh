//-------include guard-------
#ifndef __SIN1DEGBLOCK_HH_INCLUDED__
#define __SIN1DEGBLOCK_HH_INCLUDED__
//---------------------------

//CLASS Definitions
#define TARGET_XDIM 432000
#define TARGET_YDIM 1200
#define MISSING_VALUE 0

class Sin1DegBlock
{
	private:
		float *sum_grid;
		char *count_grid;

	public:
		Sin1DegBlock();
		~Sin1DegBlock();
		float getMean(unsigned long x, unsigned long y);
		void addValue(float val, unsigned long x, unsigned long y);

};
#endif //__SIN1DEGBLOCK_HH_INCLUDED__

