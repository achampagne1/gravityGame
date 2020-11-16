#ifndef convertToFloat
#define convertToFloat

class ConvertToFloat {
	public:
		ConvertToFloat(int width, int height);
		void convertToGlobal(float* input,const int size);
		void convertFromRGB(float* input, const int size);
	private:
		int width = 0;
		int height = 0;
};
#endif

