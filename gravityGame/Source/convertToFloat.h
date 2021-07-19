#ifndef convertToFloat
#define convertToFloat
#include <vector>

class ConvertToFloat {
	public:
		ConvertToFloat(int width, int height);
		ConvertToFloat();
		void convertToGlobal(float* input);
		void convertFromRGB(float* input, const int size);
		void format(float* input, const int size);
		std::vector<float> format(std::vector<float> input);
	private:
		int width = 0;
		int height = 0;
};
#endif

