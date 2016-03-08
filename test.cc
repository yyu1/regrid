#include <iostream>
#include <fstream>
#include "ALOS1DegBlock.hh"
#include "Sin1DegBlock.hh"

int main() {

	ALOS1DegBlock testblock(1);
	std::cout << "instantiating target memory blocks..." << '\n';
	Sin1DegBlock outBlock;
	std::cout << "done instantiating target memory blocks." << '\n';

	testblock.regrid(&outBlock);

	std::string hhFile = "/nobackupp6/nexprojects/CMS-ALOS/3sec/test_alos_hh.int";
	std::string hvFile = "/nobackupp6/nexprojects/CMS-ALOS/3sec/test_alos_hv.int";
	std::string maskFile = "/nobackupp6/nexprojects/CMS-ALOS/3sec/test_alos_mask.byt";

	std::ofstream hhOutFStream, hvOutFStream, maskOutFStream;

	hhOutFStream.open(hhFile, std::ios::binary | std::ios::out);
	hvOutFStream.open(hhFile, std::ios::binary | std::ios::out);
	maskOutFStream.open(hhFile, std::ios::binary | std::ios::out);

	outBlock.writeMeanAsInt16(&hhOutFStream, &hvOutFStream, &maskOutFStream);

	hhOutFStream.close();
	hvOutFStream.close();
	maskOutFStream.close();

}
