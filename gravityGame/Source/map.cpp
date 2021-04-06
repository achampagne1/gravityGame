#include "map.h"

Map::Map(const char* modelPath, int width,int height) {
    windowSize[0] = width;
    windowSize[1] = height;

    createModel("models/sky.json", 0, 0, 0, 1, 0);
    createModel("models/spaceman.json", 0, 0, 0, 1, 0);
    createModel("models/spacewoman.json", 0, 0, 0, 1, 0);
    createModel("models/planet1.json", 0, 0, 0, 10000, 1); //optimize to see if res40 is too much

}

void Map::createModel(std::string modelPath, int x, int y, float v[2], float gravity, int locked) {
    float coor[2] = { x,y };
    std::shared_ptr<Model> model{ new Model };
    model->generateModel(modelPath.c_str(), windowSize, coor, v, gravity, locked);
    models.push_back(model);
}

void Map::renderMap() {
    for (int i = 0; i < models.size(); i++)
        models.at(i)->render();
}

void Map::updateMap() {
    std::vector<std::shared_ptr<Model>> references;
    references.push_back(models.at(3));
    float* newOffset = models.at(1)->calculateVelocity(references); //janky way of doing it, but this is for transfering the data in that array to a new array so the original array does not get modified
    float newOffsetTemp[2] = { newOffset[0],newOffset[1] };
    newOffsetTemp[0] = -newOffset[0];
    newOffsetTemp[1] = -newOffset[1];


    for (int i = 2; i < models.size(); i++) {
        models.at(i)->rotate(glm::vec2(0, 1));
        models.at(i)->moveWithVelocity(newOffsetTemp);
        //things need to be rotated as well
    }

    newOffset = models.at(2)->calculateVelocity(references);
    models.at(2)->moveWithVelocity(newOffset);
}
