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
    mapLoader->setWindowSize(windowSize);
    mapLoader->initMap(mapPath);

    player = std::dynamic_pointer_cast<Player>(mapLoader->getModels("player").at(0));
    models.push_back(player);

    std::vector<std::shared_ptr<Model>> tempPlanetVector = mapLoader->getModels("planet");
    for (int i = 0; i < tempPlanetVector.size(); i++) {
        models.push_back(tempPlanetVector.at(i));
        planets.push_back(std::dynamic_pointer_cast<Planet>(tempPlanetVector.at(0)));
    }

    std::vector<std::shared_ptr<Model>> tempNpcVector = mapLoader->getModels("npc");
    for (int i = 0; i < tempNpcVector.size(); i++) {
        models.push_back(tempNpcVector.at(i));
        npc.push_back(std::dynamic_pointer_cast<Npc>(tempNpcVector.at(i)));
    }

    background = std::dynamic_pointer_cast<Background>(mapLoader->getModels("background").at(0));

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
        float offsetOfModel[2] = { 0,0 };
        offsetOfModel[0] = offset[0] + temp->getAvgX() - temp->getAvgXModel() +playerData->getAvgXModel();
        offsetOfModel[1] = offset[1] + temp->getAvgY() - temp->getAvgXModel() +playerData->getAvgYModel();
        //NOTE: the avergae of the model must be subtracted out due to the move function adding it back in later
        models.at(i)->moveWithPosition(offsetOfModel);
    }
}

void Map::adjustDownwardOnStart() { //for only adjusting downward on stratup
    //NOTE: might want to adjust for bullets as well, if the level starts with bullets flying
    std::vector<std::shared_ptr<Model>> references;
    for (int i = 0; i < planets.size(); i++)
        references.push_back(planets.at(i));
    player->calculateGravity(references);
    glm::vec2 direction = player->getGravityDirection();
    for (int i = 1; i < models.size(); i++) {
        float* temp = adjustDownward(models.at(i)->getVertexDataPointer(),direction);
        float newPos[2] = { temp[0],temp[1] }; //is there a better way of doing this?
        models.at(i)->moveWithPosition(newPos);
    }
}

float* Map::adjustDownward(std::shared_ptr<VertexData> input, glm::vec2 direction) { //adjusting downward for everything else
    float angleDown = atan2(-1, 0);
    float angle = atan2(direction.y, direction.x); //gets the angle that everything needs to be rotated by
    float angleDifference = angle - angleDown;
    std::shared_ptr<VertexData> playerVertexData = player->getVertexDataPointer(); //player vertex data
    float xDiff = input->getAvgX() - playerVertexData->getAvgX();  //gets difference of x
    float yDiff = input->getAvgY() - playerVertexData->getAvgY();  //gets difference of y
    float angle2 = atan2(yDiff, xDiff); //you need to extract the angle for a correct calculation
    float magnitude = sqrt(pow(yDiff, 2) + pow(xDiff, 2));  //magnitude is needed for calculating the new rotated position
    angle2 -= angleDifference;
    float newPos[2] = { (magnitude * cos(angle2)) + windowSize[0] / 2 - input->getAvgXModel(),(magnitude * sin(angle2)) + windowSize[1] / 2 - input->getAvgYModel() };
    return newPos;
}

void Map::shoot() {
    std::shared_ptr<VertexData> playerVertexData = player->getVertexDataPointer();
    float temp[2] = { playerVertexData->getAvgX(),playerVertexData->getAvgY()};
    glm::vec2 direction = glm::normalize(glm::vec2(-(windowSize[0]/2-cursorPos[0]), windowSize[1] / 2 - cursorPos[1]));
    float directionToShoot[2] = { direction[0],direction[1]};
    std::shared_ptr<Bullet> bullet = mapLoader->createBullet(temp, directionToShoot);
    for (int i = 0; i < 100; i++) {
        if (bullets[i] == nullptr) {
            bullets[i] = bullet;
            break;
        }
    }
}

void Map::bulletStuff(std::vector<std::shared_ptr<Model>> references) {
    float* newOffset;
    for (int i = 0; i < 100; i++) {
        if (bullets[i] == nullptr)
            continue;
        else if (bullets[i]->checkToDestroy()) 
            bullets[i] = nullptr;
        else {
            newOffset = bullets[i]->calculateVelocity(references);
            bullets[i]->moveWithVelocity(newOffset);
        }
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

void Map::setCursorPos(double xPos, double yPos) {
    cursorPos[0] = xPos;
    cursorPos[1] = yPos;
}

void Map::setScreenSize(float width, float height){
    windowSize[0] = width;
    windowSize[1] = height;
}

void Map::renderMap() {
    background->render();
    for (int i = 0; i < 100; i++) {
        if (bullets[i] != nullptr)
            bullets[i]->render();
    }
    for (int i = 0; i < models.size(); i++)
        models.at(i)->render();
}

void Map::updateMap() {
    std::vector<std::shared_ptr<Model>> references;
    for (int i = 0; i < planets.size(); i++)
        references.push_back(planets.at(i));
    float* newOffset = player->calculateVelocity(references); //janky way of doing it, but this is for transfering the data in that array to a new array so the original array does not get modified
    glm::vec2 direction = player->getGravityDirection();
    float newOffsetTemp[2] = { newOffset[0],newOffset[1] };
    currentPlayerLocation[0] += newOffsetTemp[0];
    currentPlayerLocation[1] += newOffsetTemp[1];
    newOffsetTemp[0] *= -1; //reverses the direction
    newOffsetTemp[1] *= -1;


    for (int i = 1; i < models.size(); i++) {
        models.at(i)->rotate(glm::vec2(0, 1));
        models.at(i)->moveWithVelocity(newOffsetTemp);
        //float* temp = adjustDownward(models.at(1)->getVertexDataPointer(), direction); //messes up,but why
        //float newPos[2] = { temp[0],temp[1] }; //is there a better way of doing this?
        //models.at(i)->moveWithPosition(newPos);
        //things need to be rotated as well
    }
    for (int i = 0; i < 100; i++) {
        if (bullets[i] != nullptr) {
            bullets[i]->rotate(glm::vec2(0, 1));
            bullets[i]->moveWithVelocity(newOffsetTemp);
        }
    }

    //respawn();
    for (int i = 0; i < npc.size(); i++) {
        newOffset = npc.at(i)->calculateVelocity(references);
        npc.at(i)->moveWithVelocity(newOffset);
    }
    
    bulletStuff(references);
    //adjustDownward(); //adjusting downward works, but messes up because of collision makes you hop
}

Map::~Map() {}
  