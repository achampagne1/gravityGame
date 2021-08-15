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
		glm::vec2 windowSize2 = glm::vec2{ windowSize[0],windowSize[1] };
		model->generateModel(path.c_str(), windowSize2, glm::vec2{ 0,0 }, glm::vec2{ 0,0 });
		modelsLoaded.push_back(model);
		loadedModels[type] = model;
	}
}

std::shared_ptr<Model> MapLoader::ifElseHell(std::string input) {
	if (input == "npc") {
		std::shared_ptr<Npc> npc{ new Npc };
		return npc;
	}
	else if (input == "player") {
		std::shared_ptr<Player> player{ new Player };
		return player;
	}
	else if (input == "planet") {
		std::shared_ptr<Planet> planet{ new Planet };
		return planet;
	}
	else if (input == "background") {
		std::shared_ptr<Background> background{ new Background };
		return background;
	}
	else if (input == "bullet") {
		std::shared_ptr<Bullet> bullet{ new Bullet };
		return bullet;
	}

}

std::vector<std::shared_ptr<Model>> MapLoader::getModels(std::string modelType) { //This function is for loading models from memory only
	std::vector<std::shared_ptr<Model>> returnVec;
	if (modelType == "player") {
		std::shared_ptr<Player> player{ new Player };
		player = std::make_shared<Player>(*std::dynamic_pointer_cast<Player>(loadedModels[modelType]));
		loadGenerics(modelType, player,0);
		//set player specific parameters here
		returnVec.push_back(player);
	}
	else if(modelType == "npc") {
		for (int i = 0; i < jf[modelType].size(); i++) {
			std::shared_ptr<Npc> npc{ new Npc };
			npc = std::make_shared<Npc>(*std::dynamic_pointer_cast<Npc>(loadedModels[modelType]));
			loadGenerics(modelType, npc, i);
			//set NPC specific parameters here
			returnVec.push_back(npc);
		}
	}

	else if (modelType == "planet") {
		for (int i = 0; i < jf[modelType].size(); i++) {
			std::shared_ptr<Planet> planet{ new Planet };
			planet = std::make_shared<Planet>(*std::dynamic_pointer_cast<Planet>(loadedModels[modelType]));
			loadGenerics(modelType, planet, i);
			//set NPC specific parameters here
			returnVec.push_back(planet);
		}
	}

	else if (modelType == "background") {
		for (int i = 0; i < jf[modelType].size(); i++) {
			std::shared_ptr<Background> background{ new Background };
			background = std::make_shared<Background>(*std::dynamic_pointer_cast<Background>(loadedModels[modelType]));
			loadGenerics(modelType, background, i);
			//set NPC specific parameters here
			returnVec.push_back(background);
		}
	}
	else if (modelType == "bullet") {
		std::shared_ptr<Bullet> bullet{ new Bullet };
		bullet = std::make_shared<Bullet>(*std::dynamic_pointer_cast<Bullet>(loadedModels[modelType]));
		returnVec.push_back(bullet);
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

std::shared_ptr<Bullet> MapLoader::createBullet(float coor[], float direction[]) {
	float velocit[2] = { direction[0] * 12,direction[1] * 12 };
	std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(*std::dynamic_pointer_cast<Bullet>(loadedModels["bullet"]));
	bullet->moveWithPosition(coor);
	bullet->setVelocity(velocit);
	return bullet;
}
