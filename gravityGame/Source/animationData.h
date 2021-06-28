#pragma once
#include <iostream>
#include <vector>
class AnimationData{
	private:
		unsigned int* frames;
		std::string type;
		int framesSize;
		std::vector<int> order;
	public:	
		AnimationData();
		void setFramesSize(int framesSize);
		void setType(std::string type);
		void setFrame(int index, unsigned int data);
		void setOrder(int i);
		int getFramesSize();
		std::string getType();
		unsigned int getFrame(int index);
		int getOrder(int index);
};

