#ifndef vertextDataH
#define vertexDataH

#define GLFW_INCLUDE_NONE
#include "loadFile.h"
#include "convertToFloat.h"
#include "shaderLoader.h"
#include <GLFW/glfw3.h>
#include <glad/gl.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"
#include "json.hpp"


class VertexData {
	private:
		unsigned int VAO,VBO,EBO;
		int width = 0;
		int height = 0;
		int x = 0;
		int y = 0;
		float rotation = 0;
		float xAvgModel = 0;
		float yAvgModel = 0;
		float xAvgGlobal = 0;
		float yAvgGlobal = 0;
		float gravity = 0;
		int indicesSize = 0;

		std::unique_ptr <Shader> shader{ new Shader("Shaders/3.3.shader.vs", "Shaders/3.3.shader.fs") }; //add shader path to constructor
		glm::mat4 trans = glm::mat4(1.0f);
	public:
		VertexData();
		VertexData(const char* modelPath,int width,int height, float gravity, int locked);
		VertexData(const VertexData& data);
		void render();
		void move(int x, int y);
		int getX();
		int getY();
		float getAvgX();
		float getAvgY();
		float getGravity();
		void rotate(glm::vec2 direction);
		void computeAverage(float model[], int size);
		void destroy();
};
#endif