#pragma once


#include "loadFile.h"
#include "convertToFloat.h"
#include "shaderLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"
#include "json.hpp" 
#include "stb_image.h"



class VertexData {
private:
	unsigned int VAO=0, VBO=0, EBO=0;
	unsigned int texture = 0;

	int width = 0;
	int height = 0;
	float x = 0;
	float y = 0;
	float rotation = 0;
	float xAvgModel = 0;
	float yAvgModel = 0;
	float xAvgGlobal = 0;
	float yAvgGlobal = 0;
	float gravity = 0;

	std::unique_ptr <Shader> shader{ new Shader("Shaders/3.3.shader.vs", "Shaders/3.3.shader.fs") }; //add shader path to constructor
	glm::mat4 trans = glm::mat4(1.0f);
public:
	VertexData();
	VertexData(const VertexData& data);
	void generateObject(const char* modelPath, int width, int height, float gravity, int locked);
	void render();
	void move(float x, float y);
	int getX();
	int getY();
	float getAvgX();
	float getAvgY();
	float getGravity();
	void rotate(glm::vec2 direction);
	void computeAverage(float model[], int size);
	void moveVertices(float x, float y);
	void destroy();

	int indicesSize = 0;
	int verticesSize = 0;
	float* vertices;
	float* verticesUpdated;
	int* indices;
};