#include "map.h"

Map::Map(const char* modelPath, int width,int height) {
    windowSize[0] = width;
    windowSize[1] = height;
}

std::shared_ptr<MovementEngine> Map::getMovementPtr() {
    return models.at(1)->getMovementPointer();
}

void Map::createModel(std::string modelPath, int x, int y, float v[2], float gravity, int locked) {
    float coor[2] = { x,y };
    std::shared_ptr<Model> model{ new Model };
    model->generateModel(modelPath.c_str(), windowSize, coor, v, gravity, locked);
    models.push_back(model);
}

void Map::createMap() {
    float velocit[2] = { 0,0 };
    float pos[2] = { 380,220 };
    float pos2[2] = { -200,0 };
    float pos3[2] = { 350,300 };
    createModel("models/sky.json", 0, 0, velocit, 1, 0);
    createModel("models/spaceman.json", pos[0], pos[1], velocit, 1, 0);
    createModel("models/spacewoman.json", pos3[0], pos3[1], velocit, 1, 0);
    createModel("models/planet1.json", pos2[0], pos2[0], velocit, 10000, 1); //optimize to see if res40 is too much
    adjustDownwardOnStart();
}

void Map::adjustDownwardOnStart() { //for only adjusting downward on stratup
    std::vector<std::shared_ptr<Model>> references;
    references.push_back(models.at(3)); // expand for other planets
    models.at(1)->calculateGravity(references);
    adjustDownward();
}

void Map::adjustDownward() { //adjusting downward for everything else
    glm::vec2 direction = models.at(1)->getGravityDirection();
    float angleDown = atan2(-1, 0);
    float angle = atan2(direction.y, direction.x); //gets the angle that everything needs to be rotated by
    float angleDifference = angle - angleDown;
    std::cout << angleDifference << std::endl;
    if (abs(angleDifference) < 0.1)
        return;
    std::shared_ptr<VertexData> player = models.at(1)->getVertexDataPointer(); //player vertex data
    for (int i = 2; i < models.size(); i++) { //loops through all models
        std::shared_ptr<VertexData> temp = models.at(i)->getVertexDataPointer(); //gets model vertex data
        float xDiff = temp->getAvgX() - player->getAvgX();  //gets difference of x
        float yDiff = temp->getAvgY() - player->getAvgY();  //gets difference of y
        float angle2 = atan2(yDiff, xDiff); //you need to extract the angle for a correct calculation
        float magnitude = sqrt(pow(yDiff, 2) + pow(xDiff, 2));  //magnitude is needed for calculating the new rotated position
        angle2 -= angleDifference;
        float newPos[2] = { (magnitude * cos(angle2)) + windowSize[0] / 2 - temp->getAvgXModel(),(magnitude * sin(angle2)) + windowSize[0] / 2 - temp->getAvgYModel() };
        models.at(i)->moveWithPosition(newPos);
    }
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

void Map::destroyMap() {
    for (int i = 0; i < models.size(); i++)
        models.at(i)->destroy();
}
