#pragma once

#include "../modelClasses/vertexData.h"

class CollisionEngine{
	private:
		bool collide;
	public:
		CollisionEngine();
		glm::vec2 calculateCollision(std::shared_ptr<VertexData> vertexData, std::vector<std::shared_ptr<VertexData>> references, glm::vec2 velocity);
		bool getCollision();
		bool pointInTri(float pt[2], float v1[2], float v2[2], float v3[2]);
		float calcTriArea(float v1[2], float v2[2], float v3[2]);

};

