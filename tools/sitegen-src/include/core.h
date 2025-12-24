/*
	core.h
*/

#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

/*
	std::string readFile();

	PARAMETERS
	
		const std::string& path - The path to the file you want to read.

	DESCRIPTION
	
		Reads a file into memory.

	RETURNS

		A std::string buffer to the opened file.
*/
inline std::string readFile(const std::string& path)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::ostringstream contents;
    contents << file.rdbuf();
    return contents.str();
}

