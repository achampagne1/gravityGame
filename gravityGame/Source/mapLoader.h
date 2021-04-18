#pragma once
#include "player.h"
#include "npc.h"
#include "planet.h"

class MapLoader
{
	private:
		const char* mapPath;
		using json = nlohmann::json;
		std::unique_ptr<LoadFile> fileLoader{ new LoadFile };
		json jf;
		int windowSize[2] = { 0,0 };
	public:
		MapLoader();
		void setMapPath(const char* mapPath);
		void setWindowSize(int windowSize[2]);
		std::vector<std::shared_ptr<Model>> loadModels(std::string modelType);
		std::shared_ptr<Model> loadGenerics(std::string modelType, std::shared_ptr<Model> model, int locationInArray);
};

