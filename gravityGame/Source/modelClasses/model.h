#pragma once

#include "..\physics\collisionEngine.h"
#include "..\physics\movementEngine.h"

class Model{
	private:
		std::shared_ptr<VertexData> vertexData{new VertexData};
		std::unique_ptr<CollisionEngine> collisionEngine{ new CollisionEngine };
		std::shared_ptr<MovementEngine> movementEngine{ new MovementEngine };
		std::vector<std::shared_ptr<AnimationData>> animationDataVec;
		std::shared_ptr<AnimationData> currentAnimationType;
		glm::vec2 position = glm::vec2{ 0,0 };
		float respawnPoint[2] = { 0,0 };
		glm::vec2 velocity = glm::vec2{ 0,0 };
		int texturesSize = 0;
		int animationFrame = 0;
		int frameCounter = 0;
		glm::vec2 deltaVelocity;
		glm::vec2 movementVec = glm::vec2(0, 0);
		glm::vec2 gravityDirection = glm::vec2(0, 0);
		std::string type = "";
		float gravity = .25;

	public: 
		Model();
		Model(const Model& model);
		~Model();
		void setType(std::string type);
		std::string getType();
		void generateModel(const char* modelPath, glm::vec2 windowSize, glm::vec2 position, glm::vec2 velocity);
		void calculateCollision(std::vector<std::shared_ptr<Model>> references);
		std::shared_ptr<MovementEngine> getMovementPointer();
		glm::vec2 getGravityDirection();
		void calculateMovement();
		glm::vec2 calculateVelocity(std::vector<std::shared_ptr<Model>> references);
		std::vector<std::shared_ptr<VertexData>> toVertexData(std::vector<std::shared_ptr<Model>> input);
		std::shared_ptr<VertexData> getVertexDataPointer();
		void moveWithVelocity(glm::vec2 newVelocity);
		void setVelocity(glm::vec2 velocity);
		void moveWithPosition(glm::vec2 newPos);
		void respawn();
		void setAnimationType(std::string type);
		virtual void render();
};

