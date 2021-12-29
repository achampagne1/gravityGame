#include "collisionEngine.h"

CollisionEngine::CollisionEngine() {

}

glm::vec2 CollisionEngine::calculateCollision(std::shared_ptr<VertexData> vertexData, std::vector<std::shared_ptr<VertexData>> references, glm::vec2 velocity) {
	//find objects that will most likely collide with model will most likely use quad map (jsut going to use all objects for now, but is not efficient)
	//will the new velocity cause the model to intersect a reference
	//will it be better to move the original model, check collision, then move it back. Or should I create a copy, move it, and discard it
	std::shared_ptr<VertexData> temp = std::make_shared<VertexData>(*std::dynamic_pointer_cast<VertexData>(vertexData));
	//temp->move(velocity+vertexData->getAvg());
	for (int i = 0; i < references.size(); i++) {
		std::shared_ptr<VertexData> chosenPoint = references.at(i); //need to calculate this // need to convert to quad map
		collide = false;
		glm::vec2 direction;
		float pt[2], v1[2], v2[2], v3[2];
		for (int i = 0; i < temp->collisionVertices.size(); i += 8) {
			pt[0] = temp->collisionVerticesUpdated[i]; //gives point that we are checking if is inside of a shape
			pt[1] = temp->collisionVerticesUpdated[i + 1];
			for (int j = 0; j < chosenPoint->collisionIndices.size(); j += 3) {
				v1[0] = chosenPoint->collisionVerticesUpdated[chosenPoint->collisionIndices[j] * 8];
				v2[0] = chosenPoint->collisionVerticesUpdated[chosenPoint->collisionIndices[j + 1] * 8];
				v3[0] = chosenPoint->collisionVerticesUpdated[chosenPoint->collisionIndices[j + 2] * 8];
				v1[1] = chosenPoint->collisionVerticesUpdated[chosenPoint->collisionIndices[j] * 8 + 1];
				v2[1] = chosenPoint->collisionVerticesUpdated[chosenPoint->collisionIndices[j + 1] * 8 + 1];
				v3[1] = chosenPoint->collisionVerticesUpdated[chosenPoint->collisionIndices[j + 2] * 8 + 1];
				if (pointInTri(pt, v1, v2, v3)) {
					glm::vec2 dir = glm::vec2(v2[0] - v3[0], v2[1] - v3[1]);
					direction = glm::normalize(glm::rotate(dir, glm::radians(-90.0f)));
					float disX = temp->getAvg().x - chosenPoint->getAvg().x;
					float disY = temp->getAvg().y - chosenPoint->getAvg().y;
					float magV = sqrt(pow(velocity[0], 2) + pow(velocity[1], 2));
					glm::vec2 normalVec = glm::normalize(glm::vec2(disX, disY));
					normalVec *= magV;
					normalVec = glm::vec2(0, -velocity.y);
					collide = true;
					return normalVec;
				}
				else
					direction = glm::vec2(0, 0);
			}
		}
	}
	collide = false;
	return glm::vec2(0, 0);
}

bool CollisionEngine::getCollision() {
	return collide;
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
