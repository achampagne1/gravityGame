#pragma once
#include "modelClasses\player.h"
#include "modelClasses\npc.h"
#include "modelClasses\planet.h"
#include "modelClasses\bullet.h"
#include "modelClasses\background.h"

class MapLoader
{
	private:
		const char* mapPath;
		using json = nlohmann::json;
		std::shared_ptr<LoadFile> fileLoader{ new LoadFile };
		json jf;
		int windowSize[2] = { 0,0 };
	public:
		MapLoader();
		void initMap(const char* mapPath);
		void setWindowSize(int windowSize[2]);
		void loadModelDataFromMemory();
		std::vector<std::shared_ptr<Model>> loadModels(std::string modelType);
		std::shared_ptr<Model> loadGenerics(std::string modelType, std::shared_ptr<Model> model, int locationInArray);
		std::shared_ptr<Bullet> createBullet(float coor[], float direction[]);
};

