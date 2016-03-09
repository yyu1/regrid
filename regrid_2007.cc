#include <iostream>
#include <fstream>
#include "ALOS1DegBlock.hh"
#include "Sin1DegBlock.hh"

int main() {

	ALOS1DegBlock alosblock(90);
	Sin1DegBlock outBlock;

	alosblock.regrid(&outBlock);

	std::string hhFile = "/nobackupp6/nexprojects/CMS-ALOS/3sec/alos_2007_global_3sec_hh.int";
	std::string hvFile = "/nobackupp6/nexprojects/CMS-ALOS/3sec/alos_2007_global_3sec_hv.int";
	std::string maskFile = "/nobackupp6/nexprojects/CMS-ALOS/3sec/alos_2007_global_3sec_mask.byt";

	std::ofstream hhOutFStream, hvOutFStream, maskOutFStream;

	hhOutFStream.open(hhFile, std::ios::binary | std::ios::out);
	hvOutFStream.open(hvFile, std::ios::binary | std::ios::out);
	maskOutFStream.open(maskFile, std::ios::binary | std::ios::out);

	outBlock.writeMeanAsInt16(&hhOutFStream, &hvOutFStream, &maskOutFStream);

	
	for (int j=89; j>-90; --j) {
		alosblock.reload(j);
		outBlock.reset();
		alosblock.regrid(&outBlock);
		outBlock.writeMeanAsInt16(&hhOutFStream, &hvOutFStream, &maskOutFStream);
	}


	hhOutFStream.close();
	hvOutFStream.close();
	maskOutFStream.close();

	return 0;
}
