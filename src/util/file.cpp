
#include <fstream>
#include <stdexcept>
#include <string>
#include <sstream>

#include "file.hpp"

using namespace Sim::Util;

std::string File::read(const std::string& path)
{
	std::stringstream ss;
	std::ifstream file(path, std::ios::binary);
	char buff[1024];

	if(!file.is_open())
	{
		throw std::runtime_error("Failed to open file: " + path);
	}

	while(!file.eof())
	{
		file.read(buff, sizeof(buff));
		ss.write(buff, file.gcount());
	}

	return ss.str();
}

