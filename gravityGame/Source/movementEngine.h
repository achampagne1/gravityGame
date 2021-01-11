#pragma once
#include <iostream>
#include "vertexData.h"

class MovementEngine
{
	private:
		double angle = 0;
		int direction = 0;
	public:
		MovementEngine();
		void setDirection(int direction);
		glm::vec2 calculateMovement();
		void setGravityForceVec(glm::vec2 direction);
};

