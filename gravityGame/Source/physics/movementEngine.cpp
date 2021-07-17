#include "movementEngine.h"

MovementEngine::MovementEngine() {

}
glm::vec2 MovementEngine::calculateMovement() {
	float speed = 3;
	glm::vec2 movementVec = glm::vec2(0, 0);
	if (grounded) {
		if (jumpVar) {
			movementVec += glm::vec2(5 * cos(glm::radians(90.0f) - angle), 5 * sin(glm::radians(-90.0f) - angle));
			jumpVar = false;
		}
		if (direction == 1) {
			movementVec += glm::vec2(speed * cos(angle), speed * sin(angle));
			return movementVec;
		}
		if (direction == 3) {
			movementVec += glm::vec2(-speed * cos(angle), -speed * sin(angle));
			return movementVec;
		}
		grounded = false;
	}
	movementVec += glm::vec2(0, 0);
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

void MovementEngine::setGravityForceVec(glm::vec2 direction) {
	glm::vec2 rotatedVec = glm::rotate(direction, glm::radians(-90.0f));
	angle = atan2(rotatedVec.y, rotatedVec.x);
}