#include "collisionEngine.h"

CollisionEngine::CollisionEngine() {

}

glm::vec2 CollisionEngine::calculateCollision(std::shared_ptr<VertexData> model, std::vector<std::shared_ptr<VertexData>> references, float velocity[2]) {
	//find objects that will most likely collide with model will most likely use quad map (jsut going to use all objects for now, but is not efficient)
	std::shared_ptr<VertexData> chosenPoint = references.at(0); //need to calculate this
	bool collide = false;
	glm::vec2 direction;
	float pt[2];
	float v1[2];
	float v2[2];
	float v3[2];
	for (int i = 0; i < model->verticesSize; i+=8) {
		pt[0] = model->verticesUpdated[i]; //gives point that we are checking if is inside of a shape
		pt[1] = model->verticesUpdated[i + 1];
		for (int j = 0; j < chosenPoint->indicesSize; j+=3) {
			v1[0] = chosenPoint->verticesUpdated[chosenPoint->indices[j]*8];
			v2[0] = chosenPoint->verticesUpdated[chosenPoint->indices[j+1]*8];
			v3[0] = chosenPoint->verticesUpdated[chosenPoint->indices[j+2]*8];
			v1[1] = chosenPoint->verticesUpdated[chosenPoint->indices[j]*8 + 1];
			v2[1] = chosenPoint->verticesUpdated[chosenPoint->indices[j + 1]*8 + 1];
			v3[1] = chosenPoint->verticesUpdated[chosenPoint->indices[j + 2]*8 + 1];
			collide = pointInTri(pt, v1, v2, v3);
			if (collide) {
				std::cout << "bam!" << std::endl;
				glm::vec2 dir = glm::vec2(v2[0] - v3[0], v2[1] - v3[1]);
				direction = glm::normalize(glm::rotate(dir, glm::radians(-90.0f)));
				collide = false;
				goto breakOut;
			}
			else
				direction = glm::vec2(0, 0);
		}
	}
	breakOut:

	if (direction != glm::vec2(0, 0)) {
		double angle = atan2(direction.y, direction.x);
		double angleV = atan2(-velocity[1], -velocity[0]);
		double magV = sqrt(pow(velocity[0], 2) + pow(velocity[1], 2));
		std::cout << angle << std::endl;
		if (abs(angle) / abs(angleV) <= 1.1 && abs(angle) / abs(angleV) >= .9)
			angle = angleV;
		std::cout << angle << std::endl;
		std::cout << atan2(-velocity[1], -velocity[0]) << std::endl;
		return glm::vec2(magV * cos(angle), magV * sin(angle));
		//it bounces, why. The velocity is not fully counteracted by the normal force causing the bounce.
		//comne back to
	}

	return glm::vec2(0, 0);
	//NOTE: might need to check the other way
}

bool CollisionEngine::pointInTri(float pt[2], float v1[2], float v2[2], float v3[2]) {
	float TotalArea = calcTriArea(v1, v2, v3);
	float Area1 = calcTriArea(pt, v2, v3);
	float Area2 = calcTriArea(pt, v1, v3);
	float Area3 = calcTriArea(pt, v1, v2);

	if ((Area1 + Area2 + Area3) > (TotalArea))
		return false;
	else
		return true;
}


float CollisionEngine::calcTriArea(float v1[2], float v2[2], float v3[2]){
	float det = 0.0f;
	det = abs(.5 * (v1[0] * (v2[1] - v3[1]) + v2[0] * (v3[1] - v1[1]) + v3[0] * (v1[1] - v2[1])));
	return det;
}
