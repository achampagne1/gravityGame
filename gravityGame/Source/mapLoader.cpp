#include "mapLoader.h"

MapLoader::MapLoader() {

}

void MapLoader::setMapPath(const char* mapPath) {
	this->mapPath = mapPath;
}

void MapLoader::setWindowSize(int windowSize[2]) {
	this->windowSize[0] = windowSize[0];
	this->windowSize[1] = windowSize[1];
}

std::shared_ptr<Model> MapLoader::loadModel(std::string modelType) {
	jf = json::parse(fileLoader->load(mapPath).str());
	if (modelType == "player") {
		std::shared_ptr<Player> player{ new Player };
		loadGenerics(modelType, player);
		//set player specific parameters now
		return player;
	}
	return NULL;
}

std::shared_ptr<Model> MapLoader::loadGenerics(std::string modelType, std::shared_ptr<Model> model) {
	float velocit[2] = { 0,0 };
	float coor[2] = { jf[modelType]["coordinates"][0],jf[modelType]["coordinates"][1] };
	std::string path = jf[modelType]["path"];
	velocit[0] = jf[modelType]["velocity"][0];
	velocit[1] = jf[modelType]["velocity"][1];
	model->generateModel(path.c_str(), windowSize, coor, velocit, 1);
	return model;
}
