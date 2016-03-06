#include <iostream>

int main() {


	for(unsigned int i=0; i<30; ++i) {
		unsigned long temp = i * 1200;
		temp /= 4500;

		std::cout << temp << '\n';
	}

	return 0;
}
