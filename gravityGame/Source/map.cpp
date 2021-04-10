#include "map.h"

Map::Map(const char* mapPath, int width,int height) {
    this->mapPath = mapPath;
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
    using json = nlohmann::json;
    std::unique_ptr<LoadFile> fileLoader{ new LoadFile };
    json jf = json::parse(fileLoader->load(mapPath).str());
    for (int i = 0; i < jf["models"].size(); i++) {
        std::string path = jf["models"][i]["path"];
        int x = jf["models"][i]["coordinates"][0];
        int y = jf["models"][i]["coordinates"][1];
        velocit[0] = jf["models"][i]["velocity"][0];
        velocit[1] = jf["models"][i]["velocity"][1];
        createModel(path, x, y, velocit, 1, 0);
    }
    for (int i = 0; i < jf["mapBounds"].size(); i++)
        mapBounds[i] = jf["mapBounds"][i];

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
    if (abs(angleDifference) < 0.000001) //why does the guy jump
        return;
    std::shared_ptr<VertexData> player = models.at(1)->getVertexDataPointer(); //player vertex data
    for (int i = 2; i < models.size(); i++) { //loops through all models
        std::shared_ptr<VertexData> temp = models.at(i)->getVertexDataPointer(); //gets model vertex data
        float xDiff = temp->getAvgX() - player->getAvgX();  //gets difference of x
        float yDiff = temp->getAvgY() - player->getAvgY();  //gets difference of y
        float angle2 = atan2(yDiff, xDiff); //you need to extract the angle for a correct calculation
        float magnitude = sqrt(pow(yDiff, 2) + pow(xDiff, 2));  //magnitude is needed for calculating the new rotated position
        std::cout << magnitude << std::endl;
        angle2 -= angleDifference;
        float newPos[2] = { (magnitude * cos(angle2)) + windowSize[0] / 2 - temp->getAvgXModel(),(magnitude * sin(angle2)) + windowSize[1] / 2 - temp->getAvgYModel() };
        models.at(i)->moveWithPosition(newPos);
    }
}

void Map::respawn() {
    if (currentPlayerLocation[0]<mapBounds[0] || currentPlayerLocation[0] > mapBounds[1] || currentPlayerLocation[1]<mapBounds[2] || currentPlayerLocation[1] > mapBounds[3]) {
        //I need to get a global coordinate system done before i do respawn
        float newOffsetTemp[2] = { 0,-96 };
        for (int i = 2; i < models.size(); i++) {
            std::shared_ptr<VertexData> data = models.at(i)->getVertexDataPointer();
            currentPlayerLocation[0] += data->getAvgX();
            currentPlayerLocation[1] += data->getAvgY();
            models.at(i)->rotate(glm::vec2(0, 1));
            models.at(i)->moveWithPosition(newOffsetTemp);
            //things need to be rotated as well
        }
        std::cout << "respawn" << std::endl;
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
    currentPlayerLocation[0] += newOffsetTemp[0];
    currentPlayerLocation[1] += newOffsetTemp[1];
    newOffsetTemp[0] *= -1; //reverses the direction
    newOffsetTemp[1] *= -1;
    std::cout << currentPlayerLocation[0] << " " << currentPlayerLocation[1] << std::endl;


    for (int i = 2; i < models.size(); i++) {
        models.at(i)->rotate(glm::vec2(0, 1));
        models.at(i)->moveWithVelocity(newOffsetTemp);
        //things need to be rotated as well
    }

    //adjustDownward(); //adjusting downward works, but messes up because of collision makes you hop
    respawn();
    newOffset = models.at(2)->calculateVelocity(references);
    models.at(2)->moveWithVelocity(newOffset);
}

void Map::destroyMap() {
    for (int i = 0; i < models.size(); i++)
        models.at(i)->destroy();
}
