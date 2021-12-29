#include "movementEngine.h"

MovementEngine::MovementEngine() {

}
glm::vec2 MovementEngine::calculateMovement() {
	float speed = 10;
	glm::vec2 movementVec = glm::vec2(0, 0);
	if (grounded) {
		if (jumpVar) {
			movementVec = glm::vec2(0, speed);
			jumpVar = false;
		}
		if (direction == 1) {
			movementVec = glm::vec2(-speed, 0);
			direction = 0;
			return movementVec;
		}
		if (direction == 3) {
			movementVec = glm::vec2(speed, 0);
			direction = 0;
			return movementVec;
		}
		//the two if statements below are for canceling out the movement vec
		if (direction == 4) {
			movementVec = glm::vec2(speed, 0);
			direction = 0;
			return movementVec;
		}
		if (direction == 5) {
			movementVec = glm::vec2(-speed, 0);
			direction = 0;
			return movementVec;
		}
		grounded = false;
	}
	return movementVec;
}
void MovementEngine::setDirection(int direction) {
	this->direction = direction;
}

bool MovementEngine::directionChange() {
	if (prevDirection != direction&&direction!=4) {
		prevDirection = direction;
		return true;
	}
	return false;
}

void MovementEngine::setGrounded(bool grounded) {
	this->grounded = grounded;
}

void MovementEngine::jump() {
	jumpVar = true;
}

