#include "model.h"

Model::Model() {

}

Model::Model(const char* modelPath, int windowSize[2], float pos[2], float velocity[2], float gravity, int locked) {
	vertexData->generateObject(modelPath, windowSize[0], windowSize[1], gravity, locked);
	this->pos[0] = pos[0];
	this->pos[1] = pos[1];
	this->velocity[0] = velocity[0];
	this->velocity[1] = velocity[1];
	vertexData->move(pos[0], pos[1]);
}

void Model::calculateGravity(std::vector<std::shared_ptr<Model>> references) {
	std::vector<std::shared_ptr<VertexData>> referencesRaw;
	for (int i = 0; i < references.size(); i++) {
		referencesRaw.push_back(references.at(i)->getVertexDataPointer());
	}
	float avgCoor[2] = { vertexData->getAvgX(),vertexData->getAvgY() };
	deltaVelocity+= gravityEngine->getDeltaVelocity(avgCoor, referencesRaw);
	movementEngine->setGravityForceVec(deltaVelocity);
}

void Model::calculateCollision(std::vector<std::shared_ptr<Model>> references) {
	std::vector<std::shared_ptr<VertexData>> referencesRaw;
	for (int i = 0; i < references.size(); i++) {
		referencesRaw.push_back(references.at(i)->getVertexDataPointer());
	}
	glm::vec2 deltaVelocityTemp = collisionEngine->calculateCollision(vertexData, referencesRaw, velocity);
	if (collisionEngine->getCollision()) {
		deltaVelocity = deltaVelocityTemp;
	}
}

void Model::calculateMovement() {
	//deltaVelocity -= movementVec;
	deltaVelocity = movementEngine->calculateMovement();
	//deltaVelocity += movementVec;
}

float* Model::calculateLocation(std::vector<std::shared_ptr<Model>> references) { //responsible for calculating the new location of the object 
	calculateGravity(references);
	//calculateMovement(); //revamp movement
	calculateCollision(references);
	velocity[0] += deltaVelocity[0];
	velocity[1] += deltaVelocity[1];
	if (abs(velocity[0]) < .001) //threshold to 0 out
		velocity[0] = 0;
	if (abs(velocity[1]) < .001)
		velocity[1] = 0;
	//pos[0] -= velocity[0];
	//pos[1] -= velocity[1];
	return velocity;
}

void Model::move(float v[2]) {
	pos[0] -= v[0];
	pos[1] -= v[1];
	vertexData->move(pos[0], pos[1]);
}

std::shared_ptr<VertexData> Model::getVertexDataPointer() {
	return vertexData;
}

std::shared_ptr<MovementEngine> Model::getMovementPointer() {
	return movementEngine;
}

void Model::render() {
	vertexData->render();
}

void Model::destroy() {
	vertexData->destroy();
}
