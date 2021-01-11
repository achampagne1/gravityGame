#include "movementEngine.h"

MovementEngine::MovementEngine() {

}
glm::vec2 MovementEngine::calculateMovement() {
	std::cout << direction << std::endl;
	if (direction == 1)
		return glm::vec2(.1 * cos(angle), .1 * sin(angle));
	if (direction == 3)
		return glm::vec2(-.1 * cos(angle), -.1 * sin(angle));
	else
		return glm::vec2(0, 0);
}
void MovementEngine::setDirection(int direction) {
	this->direction = direction;
}
void MovementEngine::setGravityForceVec(glm::vec2 direction) {
	glm::vec2 rotatedVec = glm::rotate(direction, glm::radians(-90.0f));
	angle = atan2(rotatedVec.y, rotatedVec.x);
}