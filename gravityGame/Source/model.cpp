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
	glm::vec2 deltaVelocity = gravityEngine->getDeltaVelocity(pos, referencesRaw);
	velocity[0] += deltaVelocity[0];
	velocity[1] += deltaVelocity[1];
	pos[0] += velocity[0];
	pos[1] += velocity[1];
	vertexData->move(pos[0], pos[1]);
}

std::shared_ptr<VertexData> Model::getVertexDataPointer() {
	return vertexData;
}

void Model::render() {
	vertexData->render();
}

void Model::destroy() {
	vertexData->destroy();
}
