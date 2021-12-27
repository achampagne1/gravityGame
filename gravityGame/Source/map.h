#pragma once

#include "mapLoader.h"
#include "bulletHandler.h"


class Map{
	private:
		std::vector<std::shared_ptr<Model>> models;
		std::vector<std::shared_ptr<Model>> references;
		std::shared_ptr<MapLoader> mapLoader{ new MapLoader };
		std::shared_ptr<BulletHandler> bulletHandlerPtr;
		std::shared_ptr<Player> player;
		std::vector<std::shared_ptr<Npc>> npc;
		std::vector<std::shared_ptr<Planet>> planets;
		std::vector<std::shared_ptr<Platform>> platforms;
		std::shared_ptr<Background> background;
		int windowSize[2];
		int windowSizeOnStart[2];
		int mapBounds[4] = { 0,0,0,0 };
		glm::vec2 cursorPos = { 0,0 };
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
		void respawn();
		void setCursorPos(double xPos, double yPos);
		void setScreenSize(float width, float height);
		void setScreenSizeOnStart(float width, float height);
		void updateMap();
};

