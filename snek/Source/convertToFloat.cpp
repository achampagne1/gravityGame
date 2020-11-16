#include "convertToFloat.h"

ConvertToFloat::ConvertToFloat(int width, int height) {
	this->width = width;
	this->height = height;
}

void ConvertToFloat::convertToGlobal(float* input,int size) {
	for (int i = 0; i < size; i++) {
		if (i % 3 == 0)
			input[i] = ((input[i] * 2) / width) - 1;
		else
			input[i] = ((input[i] * 2) / height) - 1;
	}
}

void ConvertToFloat::convertFromRGB(float* input, const int size) {
	for (int i = 0; i < size; i++)
		input[i] = input[i] / 255;
}