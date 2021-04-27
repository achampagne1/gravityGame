#include "map.h"

Map::Map(const char* mapPath, int width,int height) {
    this->mapPath = mapPath;
    windowSize[0] = width;
    windowSize[1] = height;
}

std::shared_ptr<MovementEngine> Map::getMovementPtr() {
    return player->getMovementPointer();
}

void Map::createMap() {
    mapLoader->initMap(mapPath);
    mapLoader->setWindowSize(windowSize);
    player = std::dynamic_pointer_cast<Player>(mapLoader->loadModels("player").at(0));
    models.push_back(player);
    std::vector<std::shared_ptr<Model>> tempNpcVector = mapLoader->loadModels("npc");
    for (int i = 0; i < tempNpcVector.size(); i++) {
        models.push_back(tempNpcVector.at(i));
        npc.push_back(std::dynamic_pointer_cast<Npc>(tempNpcVector.at(i)));
    }

    std::vector<std::shared_ptr<Model>> tempPlanetVector = mapLoader->loadModels("planets");
    for (int i = 0; i < tempPlanetVector.size(); i++) {
        models.push_back(tempPlanetVector.at(i));
        planets.push_back(std::dynamic_pointer_cast<Planet>(tempPlanetVector.at(0)));
    }

    background = std::dynamic_pointer_cast<Background>(mapLoader->loadModels("background").at(0));
    centerMap();
    adjustDownwardOnStart();
}

void Map::centerMap() {
    std::shared_ptr<VertexData> playerData = player->getVertexDataPointer();
    float offset[2] = { 0,0 };
    offset[0] = 380-playerData->getAvgX();
    offset[1] = 220 - playerData->getAvgY();
    for (int i = 0; i < models.size(); i++) {
        std::shared_ptr<VertexData> temp= models.at(i)->getVertexDataPointer();
        std::cout << temp->getAvgX() << " " << temp->getAvgY() << std::endl;
        float offsetOfModel[2] = { 0,0 };
        offsetOfModel[0] = offset[0] + temp->getAvgX() - temp->getAvgXModel() +playerData->getAvgXModel();
        offsetOfModel[1] = offset[1] + temp->getAvgY() - temp->getAvgXModel() +playerData->getAvgYModel();
        //NOTE: the avergae of the model must be subtracted out due to the move function adding it back in later
        models.at(i)->moveWithPosition(offsetOfModel);
    }
}

void Map::adjustDownwardOnStart() { //for only adjusting downward on stratup
    std::vector<std::shared_ptr<Model>> references;
    for (int i = 0; i < planets.size(); i++)
        references.push_back(planets.at(i));
    player->calculateGravity(references);
    adjustDownward();
}

void Map::adjustDownward() { //adjusting downward for everything else
    glm::vec2 direction = player->getGravityDirection();
    float angleDown = atan2(-1, 0);
    float angle = atan2(direction.y, direction.x); //gets the angle that everything needs to be rotated by
    float angleDifference = angle - angleDown;
    if (abs(angleDifference) < 0.000001) //why does the guy jump
        return;
    std::shared_ptr<VertexData> playerVertexData = player->getVertexDataPointer(); //player vertex data
    for (int i = 1; i < models.size(); i++) { //loops through all models
        std::shared_ptr<VertexData> temp = models.at(i)->getVertexDataPointer(); //gets model vertex data
        float xDiff = temp->getAvgX() - playerVertexData->getAvgX();  //gets difference of x
        float yDiff = temp->getAvgY() - playerVertexData->getAvgY();  //gets difference of y
        float angle2 = atan2(yDiff, xDiff); //you need to extract the angle for a correct calculation
        float magnitude = sqrt(pow(yDiff, 2) + pow(xDiff, 2));  //magnitude is needed for calculating the new rotated position
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

void Map::shoot() {
    float temp[2] = { 0,0 };
    float temp2[2] = { 1,0 };
    std::shared_ptr<Bullet> bullet = mapLoader->createBullet(temp, temp2);
    models.push_back(bullet);
    bullets.push_back(bullet);
}

void Map::updateMap() {
    std::vector<std::shared_ptr<Model>> references;
    for (int i = 0; i < planets.size(); i++)
        references.push_back(planets.at(i));
    float* newOffset = player->calculateVelocity(references); //janky way of doing it, but this is for transfering the data in that array to a new array so the original array does not get modified
    float newOffsetTemp[2] = { newOffset[0],newOffset[1] };
    currentPlayerLocation[0] += newOffsetTemp[0];
    currentPlayerLocation[1] += newOffsetTemp[1];
    newOffsetTemp[0] *= -1; //reverses the direction
    newOffsetTemp[1] *= -1;


    for (int i = 1; i < models.size(); i++) {
        models.at(i)->rotate(glm::vec2(0, 1));
        models.at(i)->moveWithVelocity(newOffsetTemp);
        //things need to be rotated as well
    }

    //adjustDownward(); //adjusting downward works, but messes up because of collision makes you hop
    //respawn();
    for (int i = 0; i < npc.size(); i++) {
        newOffset = npc.at(i)->calculateVelocity(references);
        npc.at(i)->moveWithVelocity(newOffset);
    }

    for (int i = 0; i < bullets.size(); i++) {
        newOffset = bullets.at(i)->calculateVelocity(references);
        bullets.at(i)->moveWithVelocity(newOffset);
    }
}

void Map::renderMap() {
    background->render();
    for (int i = 0; i < models.size(); i++)
        models.at(i)->render();
}

void Map::destroyMap() {
    for (int i = 0; i < models.size(); i++)
        models.at(i)->destroy();
    background->destroy();
}
