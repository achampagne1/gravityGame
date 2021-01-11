#pragma once

#include "vertexData.h"

class CollisionEngine{
	private:

	public:
		CollisionEngine();
		glm::vec2 calculateCollision(std::shared_ptr<VertexData> model, std::vector<std::shared_ptr<VertexData>> references, float velocity[2]);
		bool pointInTri(float pt[2], float v1[2], float v2[2], float v3[2]);
		float calcTriArea(float v1[2], float v2[2], float v3[2]);

};

