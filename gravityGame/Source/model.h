#pragma once
#ifndef ModelH
#define ModelH

#include "gravityEngine.h"
#include "collisionEngine.h"

class Model{
	private:
		std::shared_ptr<VertexData> vertexData{new VertexData};
		std::unique_ptr<GravityEngine> gravityEngine{ new GravityEngine };
		std::unique_ptr<CollisionEngine> collosionEngine{ new CollisionEngine };

		float pos[2] = { 0,0 };
		float velocity[2] = { 0,0 };

		std::shared_ptr<VertexData> getVertexDataPointer();
	public: 
		Model();
		Model(const char* modelPath,int windowSize[2], float pos[2], float velocity[2], float gravity, int locked);
		void calculateGravity(std::vector<std::shared_ptr<Model>> references);
		void render();
		void destroy();
};

#endif

