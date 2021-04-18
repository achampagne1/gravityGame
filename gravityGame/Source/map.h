#pragma once

#include "mapLoader.h"


class Map{
	private:
		std::vector<std::shared_ptr<Model>> models;
		std::shared_ptr<Player> player;
		std::vector<std::shared_ptr<Npc>> npc;
		std::vector<std::shared_ptr<Planet>> planets;
		std::unique_ptr<MapLoader> mapLoader{ new MapLoader };
		int windowSize[2];
		int mapBounds[4] = { 0,0,0,0 };
		const char* mapPath = "";
		float currentPlayerLocation[2] = { 0,0 };

	public:
		Map(const char* mapPath, int width,int height);
		std::shared_ptr<MovementEngine> getMovementPtr();
		void renderMap();
		void createModel(std::string modelPath, int x, int y, float v[2], float gravity, int locked);
		void loadPlayer(std::string modelPath, int x, int y, float v[2], float gravity, int locked);
		void createMap();
		void centerMap();
		void adjustDownwardOnStart();
		void adjustDownward();
		void respawn();
		void updateMap();
		void destroyMap();
};

