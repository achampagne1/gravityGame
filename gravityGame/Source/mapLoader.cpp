#include "mapLoader.h"

MapLoader::MapLoader() {

}

void MapLoader::initMap(const char* mapPath) {
	this->mapPath = mapPath;
	loadModelsFromMemory();
}

void MapLoader::setWindowSize(int windowSize[2]) {
	this->windowSize[0] = windowSize[0];
	this->windowSize[1] = windowSize[1];
}

void MapLoader::loadModelsFromMemory() { //responsible for loading all models from memory
	jf = json::parse(fileLoader->load(mapPath).str());
	for (int i = 0; i < jf["modelType"].size(); i++) {
		std::string type = jf["modelType"][i]["type"];
		std::shared_ptr<Model> model = ifElseHell(type);
		model->setType(type);
		std::string path = jf["modelType"][i]["path"];
		float coor[2] = { 0,0 };
		float velocity[2] = { 0,0 };
		model->generateModel(path.c_str(), windowSize, coor, velocity);
		modelsLoaded.push_back(model);
	}
}

std::shared_ptr<Model> MapLoader::ifElseHell(std::string input) {
	if (input == "npc") {
		std::shared_ptr<Npc> npc{ new Npc };
		return npc;
	}
	if (input == "player") {
		std::shared_ptr<Player> player{ new Player };
		return player;
	}
	if (input == "planet") {
		std::shared_ptr<Planet> planet{ new Planet };
		return planet;
	}
	if (input == "background") {
		std::shared_ptr<Background> background{ new Background };
		return background;
	}
}

std::vector<std::shared_ptr<Model>> MapLoader::getModels(std::string modelType) { //This function is for loading models from memory only
	std::vector<std::shared_ptr<Model>> returnVec;
	if (modelType == "player") {
		std::shared_ptr<Player> player{ new Player };
		for (int i = 0; i < modelsLoaded.size(); i++) {
			if (modelsLoaded.at(i)->getType() == modelType) {
				player = std::dynamic_pointer_cast<Player>(modelsLoaded.at(i));
			}
		}
		loadGenerics(modelType, player,0);
		//set player specific parameters here
		returnVec.push_back(player);
	}
	else if(modelType == "npc") {
		for (int i = 0; i < jf[modelType].size(); i++) {
			std::shared_ptr<Npc> npc{ new Npc };
			for (int i = 0; i < modelsLoaded.size(); i++) {
				if (modelsLoaded.at(i)->getType() == modelType) {
					npc = std::dynamic_pointer_cast<Npc>(modelsLoaded.at(i));
				}
			}
			loadGenerics(modelType, npc, i);
			//set NPC specific parameters here
			returnVec.push_back(npc);
		}
	}

	else if (modelType == "planet") {
		for (int i = 0; i < jf[modelType].size(); i++) {
			std::shared_ptr<Planet> planet{ new Planet };
			for (int i = 0; i < modelsLoaded.size(); i++) {
				if (modelsLoaded.at(i)->getType() == modelType) 
					planet = std::dynamic_pointer_cast<Planet>(modelsLoaded.at(i));
			}
			loadGenerics(modelType, planet, i);
			//set NPC specific parameters here
			returnVec.push_back(planet);
		}
	}

	else if (modelType == "background") {
		for (int i = 0; i < jf[modelType].size(); i++) {
			std::shared_ptr<Background> background{ new Background };
			for (int i = 0; i < modelsLoaded.size(); i++) {
				if (modelsLoaded.at(i)->getType() == modelType)
					background = std::dynamic_pointer_cast<Background>(modelsLoaded.at(i));
			}
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
	model->moveWithPosition(coor);
	model->setVelocity(velocit);
	return model;
}
