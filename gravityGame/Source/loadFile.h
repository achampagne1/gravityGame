#pragma once
#ifndef loadFileH
#define loadFileH

#include <fstream>
#include <sstream>
#include <iostream>
#include "ModelDataPacket.h"
#include <vector>

class LoadFile {
	private:
        std::vector<std::shared_ptr<ModelDataPacket>> data;
	public:
        LoadFile() {}
		std::string load(const char* type) {
            for (int i = 0; i < data.size(); i++){
                if (data.at(i)->type == type)
                    return data.at(i)->data;
            }
            return "NONE FOUND";
		}
        void loadFromMemory(const char* path,const char* type) {
            std::shared_ptr<ModelDataPacket> temp{ new ModelDataPacket };
            temp->type = type;
            std::ifstream file;
            std::stringstream stream;
            file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try {
                file.open(path);
                stream << file.rdbuf();
                // close file handlers
                file.close();
                temp->data = stream.str();
                data.push_back(temp);
            }
            catch (std::ifstream::failure e)
            {
                std::cout << "ERROR::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            }
        }
        std::stringstream loadStraight(const char* path) {
            std::shared_ptr<ModelDataPacket> temp{ new ModelDataPacket };
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