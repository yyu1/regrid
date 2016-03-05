#include <iostream>
#include "ALOS1DegBlock.hh"

int main() {

	ALOS1DegBlock testblock(1);

	std::cout << testblock.construct_tile_name(50, 20) << "\n";
	std::cout << testblock.construct_tile_name(-100,20)<< "\n";
	std::cout << testblock.construct_tile_name(0, -20) << "\n" ;
	std::cout << testblock.construct_tile_name(0,0) << "\n" ;
	std::cout << testblock.construct_tile_name(-1,-1) << "\n" ;
	std::cout << testblock.construct_tile_name(-10,-1) << "\n" ;

}
