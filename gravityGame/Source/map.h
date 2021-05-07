#pragma once

#include "mapLoader.h"


class Map{
	private:
		std::vector<std::shared_ptr<Model>> models;
		std::vector<std::shared_ptr<Bullet>> bullets;
		std::shared_ptr<Player> player;
		std::vector<std::shared_ptr<Npc>> npc;
		std::vector<std::shared_ptr<Planet>> planets;
		std::shared_ptr<Background> background;
		std::unique_ptr<MapLoader> mapLoader{ new MapLoader };
		int windowSize[2];
		int mapBounds[4] = { 0,0,0,0 };
		double cursorPos[2] = { 0,0 };
		const char* mapPath = "";
		float currentPlayerLocation[2] = { 0,0 };

	public:
		Map(const char* mapPath, int width,int height);
		~Map();
		std::shared_ptr<MovementEngine> getMovementPtr();
		void renderMap();
		void shoot();
		void createMap();
		void centerMap();
		void adjustDownwardOnStart();
		void adjustDownward();
		void respawn();
		void setCursorPos(double xPos, double yPos);
		void setScreenSize(float width, float height);
		void updateMap();
		void bulletStuff(std::vector<std::shared_ptr<Model>> references);
};

