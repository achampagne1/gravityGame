#ifndef convertToFloat
#define convertToFloat

class ConvertToFloat {
	public:
		ConvertToFloat(int width, int height);
		ConvertToFloat();
		void convertToGlobal(float* input);
		void convertFromRGB(float* input, const int size);
		void format(float* input, const int size);
	private:
		int width = 0;
		int height = 0;
};
#endif

