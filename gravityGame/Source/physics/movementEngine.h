#pragma once
#include <iostream>
#include "../modelClasses/vertexData.h"

class MovementEngine
{
	private:
		double angle = 0;
		int direction = 0;
		int prevDirection = 0;
		bool jumpVar = false;
		bool grounded = false;
	public:
		MovementEngine();
		void setDirection(int direction);
		bool directionChange();
		void jump();
		glm::vec2 calculateMovement();
		void setGrounded(bool grounded);
};

