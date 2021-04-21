#pragma once
#include <iostream>
#include "../modelClasses/vertexData.h"

class MovementEngine
{
	private:
		double angle = 0;
		int direction = 0;
		bool jumpVar = false;
		bool grounded = false;
	public:
		MovementEngine();
		void setDirection(int direction);
		void jump();
		glm::vec2 calculateMovement();
		void setGravityForceVec(glm::vec2 direction);
		void setGrounded(bool grounded);
};

