#include "collisionEngine.h"

CollisionEngine::CollisionEngine() {

}

void CollisionEngine::calculateCollision(std::shared_ptr<VertexData> model, std::vector<std::shared_ptr<VertexData>> references) {
	//find objects that will most likely collide with model will most likely use quad map (jsut going to use all objects for now, but is not efficient)
	std::shared_ptr<VertexData> closest;
	float seperationMin = 1000000.0;
	float seperationXClosest = 0;
	float seperationYClosest = 0;
	for (int i = 0; i < references.size(); i++) {
		float seperationX = references.at(i)->getAvgX() - model->getAvgX();
		float seperationY = references.at(i)->getAvgY() - model->getAvgY();
		float seperation = sqrt(pow(seperationX, 2) + pow(seperationY, 2));
		if (seperation < seperationMin) {
			seperationMin = seperation;
			closest = references.at(i);
			seperationXClosest = seperationX;
			seperationYClosest = seperationY;
		}

	}
	glm::vec2 coor = glm::normalize(glm::vec2(seperationXClosest, seperationYClosest));
	for (int i = 0; i < model->verticesSize-8; i=i+8) {
		float seperationX;
		float seperationY;
		if (i == model->verticesSize - 9) {
			seperationX = model->vertices[i] - model->vertices[0];
			seperationY = model->vertices[i + 1] - model->vertices[1];
		}
		else {
			seperationX = model->vertices[i] - model->vertices[i + 8];
			seperationY = model->vertices[i + 1] - model->vertices[i + 9];
		}
		glm::vec2 localUnitVec = glm::normalize(glm::vec2(seperationX, seperationY));
		if (glm::dot(localUnitVec, coor) == 0) {
			//this is hard
		}
	}
	//find edges that will most likely collide
	//see what edges intersect with direction vector

	//detect if collided by calculating if model edge is closer to center of reference than the actual edge
}
