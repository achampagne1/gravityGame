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

void ConvertToFloat::format(float* input, const int size) {
	int step = 0;
	for (int i = 0; i < size; i++) {
		if (step < 3) {
			if (step == 1)
				input[i] = ((input[i] * 2) / height) - 1;
			else
				input[i] = ((input[i] * 2) / width) - 1;
		}
		else if (step < 6) {
			input[i] = input[i] / 255;
		}
		if (step == 7)
			step = 0;
		else
			step++;
	}
}