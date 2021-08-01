#pragma once


#include "..\loadFile.h"
#include "..\convertToFloat.h"
#include "..\shaderLoader.h"
#include "..\animationData.h"
#include "json.hpp" 
#include "stb_image.h"



class VertexData {
private:
	unsigned int VAO = 0, VBO = 0, EBO = 0;
	std::shared_ptr<AnimationData> currentAnimationType;

	int width = 0;
	int height = 0;
	float aspectRatioX = 0;
	float aspectRatioY = 0;
	float x = 0;
	float y = 0;
	float rotation = 0;	
	glm::vec2 avg;
	glm::vec2 avgModel;
	float gravity = 0;
	bool mirrorVar = false;

	std::unique_ptr <Shader> shader{ new Shader("Shaders/3.3.shader.vs", "Shaders/3.3.shader.fs") }; //add shader path to constructor
	glm::mat4 trans = glm::mat4(1.0f);
public:
	VertexData();
	VertexData(const VertexData& vertexData);
	~VertexData();
	std::vector<std::shared_ptr<AnimationData>> generateObject(const char* modelPath, int width, int height);
	void render(int animationFrame);
	void move(float x, float y);
	void move(glm::vec2 input);
	glm::vec2 getAvg();
	glm::vec2 getAvgModel();
	float getGravity();
	int getTexturesSize();
	std::shared_ptr<AnimationData> getCurrentAnimation();
	void rotate(glm::vec2 direction);
	void computeAverage(std::vector<float>input);
	void moveVertices(float x, float y);
	void mirrorSprite();

	int indicesSizeCollision = 0;
	int verticesSizeCollision= 0;

	std::vector<float> textureVertices;
	std::vector<int> textureIndices;
	std::vector<float> collisionVertices;
	std::vector<float> collisionVerticesUpdated;
	std::vector<int> collisionIndices;
};