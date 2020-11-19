#ifndef vertextData
#define vertexData

#define GLFW_INCLUDE_NONE
#include "loadFile.h"
#include "convertToFloat.h"
#include "shaderLoader.h"
#include <GLFW/glfw3.h>
#include <glad/gl.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class VertexData {
	private:
		unsigned int VAO,VBO,EBO;
		int width = 0;
		int height = 0;

		std::unique_ptr <Shader> shader{ new Shader("Shaders/3.3.shader.vs", "Shaders/3.3.shader.fs") }; //add shader path to constructor
		glm::mat4 trans = glm::mat4(1.0f);
	public:
		VertexData(const char* modelPath,int width,int height);
		void render();
		void move(int x, int y);
		void rotate(int deg);
		void destroy();
};
#endif