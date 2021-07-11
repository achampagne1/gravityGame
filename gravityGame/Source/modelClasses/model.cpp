#include "model.h"

Model::Model() {
	
}

Model::Model(const Model& model) {
	type = model.type;
	texturesSize = model.texturesSize;
	vertexData = std::make_shared<VertexData>(*model.vertexData);
	currentAnimationType = model.currentAnimationType;
}

void Model::setType(std::string type){
	this->type = type;
}

std::string Model::getType() {
	return type;
}

void Model::generateModel(const char* modelPath, int windowSize[2], float pos[2], float velocity[2]) {
	animationDataVec = vertexData->generateObject(modelPath, windowSize[0], windowSize[1]);
	this->pos[0] = pos[0];
	this->pos[1] = pos[1];
	respawnPoint[0] = pos[0];
	respawnPoint[1] = pos[1];
	this->velocity[0] = velocity[0];
	this->velocity[1] = velocity[1];
	vertexData->move(pos[0], pos[1]);
	setAnimationType("generic");
	texturesSize = currentAnimationType->getFramesSize();	
}

void Model::calculateGravity(std::vector<std::shared_ptr<Model>> references) {
	std::vector<std::shared_ptr<VertexData>> referencesRaw = toVertexData(references);
	float avgCoor[2] = { vertexData->getAvgX(),vertexData->getAvgY() };
	deltaVelocity= gravityEngine->getDeltaVelocity(avgCoor, referencesRaw);
	gravityDirection = gravityEngine->getDirection();
	movementEngine->setGravityForceVec(gravityDirection);
}

void Model::calculateCollision(std::vector<std::shared_ptr<Model>> references) {
	std::vector<std::shared_ptr<VertexData>> referencesRaw = toVertexData(references);
	collisionEngine->calculateCollision(vertexData, referencesRaw, velocity);
	if (collisionEngine->getCollision()) {
		velocity[0] = 0;
		velocity[1] = 0;
		deltaVelocity[0] = 0;
		deltaVelocity[1] = 0;
		movementEngine->setGrounded(true);
	}
}

void Model::calculateMovement() { //what moves you around
	movementVec = movementEngine->calculateMovement();
	velocity[0] += movementVec[0]; //adds new movement velocity
	velocity[1] += movementVec[1];
}

float* Model::calculateVelocity(std::vector<std::shared_ptr<Model>> references) { //responsible for calculating the velocity of the object
	//this velocity can be used to move either the object, or offset everything else.
	calculateGravity(references);
	velocity[0] += deltaVelocity[0];
	velocity[1] += deltaVelocity[1];
	calculateCollision(references);
	calculateMovement();
	return velocity;
}

void Model::moveWithVelocity(float newV[2]) { //I do not know if set velocity and move with Velocity act as the same
	pos[0] += newV[0];
	pos[1] += newV[1];
	vertexData->move(pos[0], pos[1]);
}

void Model::setVelocity(float velocity[2]) {
	this->velocity[0] = velocity[0];
	this->velocity[1] = velocity[1];
}

void Model::moveWithPosition(float newPos[2]) {
	pos[0] = newPos[0];
	pos[1] = newPos[1];
	vertexData->move(pos[0], pos[1]);
}

void Model::respawn() {
	moveWithPosition(respawnPoint);
}

void Model::rotate(glm::vec2 direction) {
	vertexData->rotate(direction);
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
