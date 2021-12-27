#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "../modelClasses/vertexData.h"

class GravityEngine {
	private:
		glm::vec2 calculateDeltaVelocity(float avgCoor[2], std::vector<std::shared_ptr<VertexData>> references);
		glm::vec2 forceVec = glm::vec2(0, 0);
	public:
		GravityEngine();
		glm::vec2 getDeltaVelocity(float avgCoor[2], std::vector<std::shared_ptr<VertexData>> references);
};
