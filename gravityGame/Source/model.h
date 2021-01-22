#pragma once

#include "gravityEngine.h"
#include "collisionEngine.h"
#include "movementEngine.h"

class Model{
	private:
		std::shared_ptr<VertexData> vertexData{new VertexData};
		std::unique_ptr<GravityEngine> gravityEngine{ new GravityEngine };
		std::unique_ptr<CollisionEngine> collisionEngine{ new CollisionEngine };
		std::shared_ptr<MovementEngine> movementEngine{ new MovementEngine };

		float pos[2] = { 0,0 };
		float velocity[2] = { 0,0 };
		glm::vec2 deltaVelocity;
		glm::vec2 movementVec;

		std::shared_ptr<VertexData> getVertexDataPointer();
	public: 
		Model();
		Model(const char* modelPath,int windowSize[2], float pos[2], float velocity[2], float gravity, int locked);
		void calculateGravity(std::vector<std::shared_ptr<Model>> references);
		void calculateCollision(std::vector<std::shared_ptr<Model>> references);
		std::shared_ptr<MovementEngine> getMovementPointer();
		void calculateMovement();
		void calculateLocation(std::vector<std::shared_ptr<Model>> references);
		void render();
		void destroy();
};

