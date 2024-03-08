#include "Config.h"
#include <istream>
#include <fstream>

Config::Config() {
	std::ifstream file(fileName);
	std::string str;
	file >> blocksMaxQuantity;
	std::getline(file, str);
	blockFileNames.resize(blocksMaxQuantity);
	for (size_t i = 0; i < blocksMaxQuantity; i++) {
		std::getline(file, blockFileNames[i]);
	}
	file.close();
}