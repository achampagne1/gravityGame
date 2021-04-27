#include "mapLoader.h"

MapLoader::MapLoader() {

}

void MapLoader::initMap(const char* mapPath) {
	this->mapPath = mapPath;
	jf = json::parse(fileLoader->loadStraight(mapPath).str());
	loadModelDataFromMemory();
}

void MapLoader::setWindowSize(int windowSize[2]) {
	this->windowSize[0] = windowSize[0];
	this->windowSize[1] = windowSize[1];
}

void MapLoader::loadModelDataFromMemory() {
	for (int i = 0; i < jf["modelPaths"].size(); i++) {
		std::string path = jf["modelPaths"][i]["path"];
		std::string type = jf["modelPaths"][i]["type"];
		fileLoader->loadFromMemory(path.c_str(), type.c_str());
	}
}

std::vector<std::shared_ptr<Model>> MapLoader::loadModels(std::string modelType) { //This function is for loading models from memory only
	std::vector<std::shared_ptr<Model>> returnVec;
	if (modelType == "player") {
		std::shared_ptr<Player> player{ new Player };
		loadGenerics(modelType, player,0);
		//set player specific parameters here
		returnVec.push_back(player);
	}

	else if(modelType == "npc") {
		for (int i = 0; i < jf[modelType].size(); i++) {
			std::shared_ptr<Npc> npc{ new Npc };
			loadGenerics(modelType, npc, i);
			//set NPC specific parameters here
			returnVec.push_back(npc);
		}
	}

	else if (modelType == "planets") {
		for (int i = 0; i < jf[modelType].size(); i++) {
			std::shared_ptr<Planet> planet{ new Planet };
			loadGenerics(modelType, planet, i);
			//set NPC specific parameters here
			returnVec.push_back(planet);
		}
	}

	else if (modelType == "background") {
		for (int i = 0; i < jf[modelType].size(); i++) {
			std::shared_ptr<Background> background{ new Background };
			loadGenerics(modelType, background, i);
			//set NPC specific parameters here
			returnVec.push_back(background);
		}
	}
	return returnVec;
}

std::shared_ptr<Model> MapLoader::loadGenerics(std::string modelType, std::shared_ptr<Model> model, int locationInArray) {
	float velocit[2] = { 0,0 };
	float coor[2] = { jf[modelType][locationInArray]["coordinates"][0] ,jf[modelType][locationInArray]["coordinates"][1] };
	std::string path = jf[modelType][locationInArray]["type"];
	velocit[0] = jf[modelType][locationInArray]["velocity"][0];
	velocit[1] = jf[modelType][locationInArray]["velocity"][1];
	model->generateModel(path.c_str(), windowSize, coor, velocit, fileLoader);
	return model;
}

std::shared_ptr<Bullet> MapLoader::createBullet(float coor[], float direction[]) {
	float velocit[2] = { direction[0] * 15,direction[1] * 15 };
	std::shared_ptr<Bullet> bullet{ new Bullet };
	std::string path = "models/bullet.json";
	bullet->generateModel(path.c_str(), windowSize,coor,velocit, fileLoader);
	return bullet;
}
