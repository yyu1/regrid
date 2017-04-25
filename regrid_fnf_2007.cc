#include <iostream>
#include <fstream>
#include "ALOSFNF1DegBlock.hh"
#include "SinFNF1DegBlock.hh"

int main() {

	ALOSFNF1DegBlock alosblock(76);
	SinFNF1DegBlock outBlock;

	alosblock.regrid(&outBlock);

	std::string fnfFile = "/nobackupp6/nexprojects/CMS-ALOS/3sec/alos_2007_global_3sec_fnf.byt";

	std::ofstream fnfOutFStream;

	fnfOutFStream.open(fnfFile, std::ios::binary | std::ios::out);

	outBlock.writeFNFOutput(&fnfOutFStream);

	
	for (int j=75; j>-90; --j) {
		std::cout << "Working on latitude " << j << "...\n";
		alosblock.reload(j);
		outBlock.reset();
		alosblock.regrid(&outBlock);
		outBlock.writeFNFOutput(&fnfOutFStream);
	}


	fnfOutFStream.close();

	return 0;
}
