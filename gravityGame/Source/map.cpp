#include "map.h"

Map::Map(const char* mapPath, int width, int height) {
    this->mapPath = mapPath;
    windowSize[0] = width;
    windowSize[1] = height;
}

std::shared_ptr<MovementEngine> Map::getMovementPtr() {
    return player->getMovementPointer();
}

void Map::createMap() {
    mapLoader->setWindowSize(windowSize);
    mapLoader->initMap(mapPath);

    player = std::dynamic_pointer_cast<Player>(mapLoader->getModels("player").at(0));
    bulletHandlerPtr = player->getBulletHandler();
    models.push_back(player);

    std::vector<std::shared_ptr<Model>> tempPlanetVector = mapLoader->getModels("planet");
    for (int i = 0; i < tempPlanetVector.size(); i++) {
        models.push_back(tempPlanetVector.at(i));
        planets.push_back(std::dynamic_pointer_cast<Planet>(tempPlanetVector.at(i)));
    }

    std::vector<std::shared_ptr<Model>> tempNpcVector = mapLoader->getModels("npc");
    for (int i = 0; i < tempNpcVector.size(); i++) {
        models.push_back(tempNpcVector.at(i));
        npc.push_back(std::dynamic_pointer_cast<Npc>(tempNpcVector.at(i)));
    }

    background = std::dynamic_pointer_cast<Background>(mapLoader->getModels("background").at(0));

    for (int i = 0; i < planets.size(); i++)
        references.push_back(planets.at(i));

    bulletHandlerPtr->setGenericBullet(std::dynamic_pointer_cast<Bullet>(mapLoader->getModels("bullet").at(0)));
    bulletHandlerPtr->setReferencesPtr(&references);
    centerMap();
    adjustDownwardOnStart();
}

void Map::centerMap() {
    glm::vec2 playerAvgModel = player->getVertexDataPointer()->getAvgModel();
    float offset[2] = { windowSize[0] / 2 - playerAvgModel.x,windowSize[1] / 2 - playerAvgModel.y };
    for (int i = 0; i < models.size(); i++) {
        glm::vec2 modelAvgModel = models.at(i)->getVertexDataPointer()->getAvgModel();
        glm::vec2 modelAvgGlobal = models.at(i)->getVertexDataPointer()->getAvg();
        float offsetOfModel[2] = { 0,0 };
        offsetOfModel[0] = offset[0] + modelAvgGlobal.x - modelAvgModel.x;
        offsetOfModel[1] = offset[1] + modelAvgGlobal.y - modelAvgModel.y;
        //NOTE: the avergae of the model must be subtracted out due to the move function adding it back in later
        models.at(i)->moveWithPosition(offsetOfModel);
    }
}

void Map::adjustDownwardOnStart() { //for only adjusting downward on stratup
    //NOTE: might want to adjust for bullets as well, if the level starts with bullets flying
    player->calculateGravity(references);
    glm::vec2 direction = player->getGravityDirection(); 
    for (int i = 1; i < models.size(); i++) {
        float* temp = adjustDownward(models.at(i)->getVertexDataPointer(), direction);
        float newPos[2] = { temp[0],temp[1] }; //is there a better way of doing this?
        models.at(i)->moveWithPosition(newPos);
    }
}

float* Map::adjustDownward(std::shared_ptr<VertexData> input, glm::vec2 direction) { //adjusting downward for everything else
    float angleDown = atan2(-1, 0);
    float angle = atan2(direction.y, direction.x); //gets the angle that everything needs to be rotated by
    float angleDifference = angle - angleDown;
    glm::vec2 playerAvg = player->getVertexDataPointer()->getAvg();
    glm::vec2 inputAvgGlobal = input->getAvg();
    glm::vec2 inputAvgModel = input->getAvgModel();
    float xDiff = inputAvgGlobal.x - playerAvg.x;  //gets difference of x
    float yDiff = inputAvgGlobal.y - playerAvg.y;  //gets difference of y
    float angle2 = atan2(yDiff, xDiff); //you need to extract the angle for a correct calculation
    float magnitude = sqrt(pow(yDiff, 2) + pow(xDiff, 2));  //magnitude is needed for calculating the new rotated position
    angle2 -= angleDifference;
    if (abs(angleDown - angle2) < .0001)
        angle2 = angleDown;
    float newPos[2] = { (magnitude * cos(angle2)) + windowSizeOnStart[0] / 2 - inputAvgModel.x,(magnitude * sin(angle2)) + windowSizeOnStart[1] / 2 - inputAvgModel.y };
    return newPos;
}

void Map::shoot() {
    bulletHandlerPtr->shoot(cursorPos);
}

void Map::respawn() {
    /*if (currentPlayerLocation[0]<mapBounds[0] || currentPlayerLocation[0] > mapBounds[1] || currentPlayerLocation[1]<mapBounds[2] || currentPlayerLocation[1] > mapBounds[3]) {
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
    }*/
}

void Map::setCursorPos(double xPos, double yPos) {
    cursorPos[0] = xPos;
    cursorPos[1] = yPos;
}

void Map::setScreenSize(float width, float height) {
    windowSize[0] = width;
    windowSize[1] = height;
}

void Map::setScreenSizeOnStart(float width, float height) {
    windowSizeOnStart[0] = width;
    windowSizeOnStart[1] = height;
}

void Map::renderMap() {
    background->render();
    bulletHandlerPtr->renderBullets();
    for (int i = 0; i < models.size(); i++)
        models.at(i)->render();
}

void Map::updateMap() {
    float* newOffset = player->calculateVelocity(references); //janky way of doing it, but this is for transfering the data in that array to a new array so the original array does not get modified
    float newOffsetTemp[2] = { newOffset[0],newOffset[1] };
    currentPlayerLocation[0] += newOffsetTemp[0];
    currentPlayerLocation[1] += newOffsetTemp[1];
    newOffsetTemp[0] *= -1; //reverses the direction
    newOffsetTemp[1] *= -1;
    glm::vec2 newOffsetTemp2 = glm::vec2{ newOffsetTemp[0],newOffsetTemp[1] };
    glm::vec2 direction = player->getGravityDirection();


    for (int i = 1; i < models.size(); i++) {
        models.at(i)->moveWithVelocity(newOffsetTemp2); //uncomment whenever centering is fixed
        float* temp = adjustDownward(models.at(i)->getVertexDataPointer(), direction);
        float newPos[2] = { temp[0],temp[1] }; //is there a better way of doing this?
        models.at(i)->moveWithPosition(newPos);
        //things need to be rotated as well
    }

    bulletHandlerPtr->updateBullets(newOffsetTemp2);


    for (int i = 0; i < npc.size(); i++) {
        newOffset = npc.at(i)->calculateVelocity(references);
        glm::vec2 newOffset2 = glm::vec2{ newOffset[0],newOffset[1] };
        glm::vec2 directionNpc = npc.at(i)->getGravityDirection();
        npc.at(i)->moveWithVelocity(newOffset2);
        npc.at(i)->rotate(directionNpc);
    }
}

Map::~Map() {}