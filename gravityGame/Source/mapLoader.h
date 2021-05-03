#pragma once
#include "modelClasses\player.h"
#include "modelClasses\npc.h"
#include "modelClasses\planet.h"
#include "modelClasses\background.h"
#include "modelClasses\bullet.h"

class MapLoader
{
	private:
		const char* mapPath;
		using json = nlohmann::json;
		std::unique_ptr<LoadFile> fileLoader{ new LoadFile };
		std::vector<std::shared_ptr<Model>> loadedModels;
		std::vector<std::shared_ptr<Model>> modelsLoaded;
		json jf;
		int windowSize[2] = { 0,0 };
	public:
		MapLoader();
		void initMap(const char* mapPath);
		void setWindowSize(int windowSize[2]);
		std::vector<std::shared_ptr<Model>> getModels(std::string modelType);
		void loadModelsFromMemory();
		std::shared_ptr<Model> ifElseHell(std::string input);
		std::shared_ptr<Model> loadGenerics(std::string modelType, std::shared_ptr<Model> model, int locationInArray);
		std::shared_ptr<Bullet> createBullet(float coor[], float direction[]);
};

