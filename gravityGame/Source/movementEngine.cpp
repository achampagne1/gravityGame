#include "movementEngine.h"

MovementEngine::MovementEngine() {

}
glm::vec2 MovementEngine:: calculateMovement() {
	float speed =3;
	if (direction == 1)
		return glm::vec2(speed * cos(angle), speed * sin(angle));
	if (direction == 3)
		return glm::vec2(-speed * cos(angle), -speed * sin(angle));
	else
		return glm::vec2(0, 0);
}
void MovementEngine::setDirection(int direction) {
	this->direction = direction;
}
void MovementEngine::setGravityForceVec(glm::vec2 direction) {
	std::cout << glm::to_string(direction) << std::endl;
	glm::vec2 rotatedVec = glm::rotate(direction, glm::radians(-90.0f));
	angle = atan2(rotatedVec.y, rotatedVec.x);
}