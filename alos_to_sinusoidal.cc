#include <iostream>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/mean.hpp>


#define XDIM 432000
#define YDIM 1200

namespace ba = boost::accumulators;

int main()
{

	ba::accumulator_set<int, ba::stats<ba::tag::mean> > acc_grid = new ba::accumulator_set<int, ba::stats<ba::tag::mean> >[XDIM][YDIM];


	return 0;
}

