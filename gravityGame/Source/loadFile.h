#pragma once
#ifndef loadFileH
#define loadFileH

#include <fstream>
#include <sstream>
#include <iostream>

class LoadFile {
	private:
	public:
        LoadFile() {}
		std::stringstream load(const char* path) {
            std::ifstream file;
            std::stringstream stream;
            file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try {
                file.open(path);
                stream << file.rdbuf();
                // close file handlers
                file.close();
                return stream;
            }
            catch (std::ifstream::failure e)
            {
                std::cout << "ERROR::FILE_NOT_SUCCESFULLY_READ" << std::endl;
                return stream;
            }
		}
};
#endif