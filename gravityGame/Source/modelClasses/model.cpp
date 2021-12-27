#include "model.h"

Model::Model() {

}

Model::Model(const Model& model) {
	type = model.type;
	texturesSize = model.texturesSize;
	vertexData = std::make_shared<VertexData>(*model.vertexData);
	currentAnimationType = model.currentAnimationType;
}

void Model::setType(std::string type) {
	this->type = type;
}

std::string Model::getType() {
	return type;
}

void Model::generateModel(const char* modelPath, glm::vec2 windowSize, glm::vec2 position, glm::vec2 velocity) {
	animationDataVec = vertexData->generateObject(modelPath, windowSize[0], windowSize[1]);
	this->position = position;
	this->velocity = velocity;
	vertexData->move(position);
	setAnimationType("generic");
	texturesSize = currentAnimationType->getFramesSize();
}

void Model::calculateCollision(std::vector<std::shared_ptr<Model>> references) {
	std::vector<std::shared_ptr<VertexData>> referencesRaw = toVertexData(references);
	float velocity2[2] = {velocity.x,velocity.y};
	collisionEngine->calculateCollision(vertexData, referencesRaw, velocity2);
	if (collisionEngine->getCollision()) {
		velocity.x = 0;
		velocity.y = 0;
		deltaVelocity[0] = 0;
		deltaVelocity[1] = 0;
		movementEngine->setGrounded(true);
	}
}

void Model::calculateMovement() { //what moves you around
	movementVec = movementEngine->calculateMovement();
	velocity += movementVec;
	if (movementEngine->directionChange())
		vertexData->mirrorSprite();

}

glm::vec2 Model::calculateVelocity(std::vector<std::shared_ptr<Model>> references) { //responsible for calculating the velocity of the object
	//this velocity can be used to move either the object, or offset everything else.
	deltaVelocity = glm::vec2(0, -gravity); //for gravity
	velocity.x += deltaVelocity[0];
	velocity.y += deltaVelocity[1];
	calculateCollision(references);
	calculateMovement();
	return velocity;
}

void Model::moveWithVelocity(glm::vec2 newVelocity) { //I do not know if set velocity and move with Velocity act as the same
	position += newVelocity;
	vertexData->move(position);
}

void Model::setVelocity(glm::vec2 velocity) {
	this->velocity = velocity;
}

void Model::moveWithPosition(glm::vec2 newPos) {
	position = newPos;
	vertexData->move(position);
}

void Model::respawn() {
	//moveWithPosition(respawnPoint);
}

std::vector<std::shared_ptr<VertexData>> Model::toVertexData(std::vector<std::shared_ptr<Model>> input) {
	std::vector<std::shared_ptr<VertexData>> referencesRaw;
	for (int i = 0; i < input.size(); i++) {
		referencesRaw.push_back(input.at(i)->getVertexDataPointer());
	}
	return referencesRaw;
}

std::shared_ptr<VertexData> Model::getVertexDataPointer() {
	return vertexData;
}

std::shared_ptr<MovementEngine> Model::getMovementPointer() {
	return movementEngine;
}

glm::vec2 Model::getGravityDirection() {
	return gravityDirection;
}

void Model::render() {
	if (texturesSize > 1) {
		if (frameCounter == 2) { //number of frames to change sprite
			animationFrame++;
			frameCounter = 0;
		}
		else
			frameCounter++;

		if (animationFrame == texturesSize)
			animationFrame = 0;
		vertexData->render(currentAnimationType->getFrame(currentAnimationType->getOrder(animationFrame)));
	}
	else {
		vertexData->render(currentAnimationType->getFrame(0));
	}
}

void Model::setAnimationType(std::string type) {
	for (int i = 0; i < animationDataVec.size(); i++) { //animation data vec needs to be sorted by letter so binary search can be used.
		if (animationDataVec.at(i)->getType() == type) {
			currentAnimationType = animationDataVec.at(i);
			return;
		}
	}
}

Model::~Model() {};