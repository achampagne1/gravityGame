#ifndef vertextData
#define vertexData

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h> // include glad to get all the required OpenGL headers


class VertexData {
	private:
		unsigned int VAO,VBO,EBO;
		unsigned int indicesNum;
	public:
		VertexData(float vertex[], unsigned int vertexSize, unsigned int indices[], unsigned int indicesSize);
		void render();
		void destroy();
};
#endif