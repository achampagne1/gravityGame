#include "convertToFloat.h"

ConvertToFloat::ConvertToFloat(int width, int height) {
	this->width = width;
	this->height = height;
}

ConvertToFloat::ConvertToFloat() {

}

void ConvertToFloat::convertToGlobal(float* input) {
	input[0] = 2.0*input[0] / width;
	input[1] = 2.0*input[1] / height;
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