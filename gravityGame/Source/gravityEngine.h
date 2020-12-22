#pragma once
#ifndef GravityEngineH
#define GravityEngineH

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "vertexData.h"

class GravityEngine {
	private:
		glm::vec2 calculateDeltaVelocity(float avgCoor[2], std::vector<std::shared_ptr<VertexData>> references);

	public:
		GravityEngine();
		glm::vec2 getDeltaVelocity(float avgCoor[2], std::vector<std::shared_ptr<VertexData>> references);
		glm::vec2 getDirection();
};

#endif