#pragma once

#include "model.h"

class Map{
	private:
		std::vector<std::shared_ptr<Model>> models;
		int windowSize[2];
		int mapBounds[4] = { 0,0,0,0 };
		const char* mapPath = "";
		float currentPlayerLocation[2] = { 0,0 };

	public:
		Map(const char* mapPath, int width,int height);
		std::shared_ptr<MovementEngine> getMovementPtr();
		void renderMap();
		void createModel(std::string modelPath, int x, int y, float v[2], float gravity, int locked);
		void createMap();
		void adjustDownwardOnStart();
		void adjustDownward();
		void respawn();
		void updateMap();
		void destroyMap();
};

