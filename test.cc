#include <iostream>
#include "ALOS1DegBlock.hh"

int main() {

//	ALOS1DegBlock testblock(1);
	std::cout << ALOS1DegBlock::horzMap(0, 0) << '\n';
	std::cout << ALOS1DegBlock::horzMap(0,90) << '\n';
	std::cout << ALOS1DegBlock::horzMap(0,-90) << '\n';
	std::cout << ALOS1DegBlock::horzMap(1620000-1, 0) << '\n';
	std::cout << ALOS1DegBlock::horzMap(1620000-1,90) << '\n';
	std::cout << ALOS1DegBlock::horzMap(1620000-1,-90) << '\n';
	std::cout << ALOS1DegBlock::horzMap(0,-88) << '\n';
}
