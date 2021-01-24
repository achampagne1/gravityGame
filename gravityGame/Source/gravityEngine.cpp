#include "gravityEngine.h"

GravityEngine::GravityEngine() {

}

glm::vec2 GravityEngine::getDeltaVelocity(float avgCoor[2], std::vector<std::shared_ptr<VertexData>> references) {
    return calculateDeltaVelocity(avgCoor, references);
}

glm::vec2 GravityEngine::calculateDeltaVelocity(float avgCoor[2], std::vector<std::shared_ptr<VertexData>> references) {
    float bigG = .3; //for some reason, gravity got much stronger, so I had to back down bigG a lot
    glm::vec2 forceVec = glm::vec2(0, 0);
    for (int i = 0; i < references.size(); i++) {
        float forceLocal;
        float seperationX = references.at(i)->getAvgX() - avgCoor[0];
        float seperationY = references.at(i)->getAvgY() - avgCoor[1];
        float seperation = sqrt(pow(seperationX, 2) + pow(seperationY, 2));
        glm::vec2 coor;
        if (seperation != 0)
            forceLocal = (bigG * references.at(i)->getGravity()) / (seperation * seperation);
        else
            forceLocal = (bigG * references.at(i)->getGravity());

        coor = glm::normalize(glm::vec2(seperationX, seperationY));

        coor[0] = (coor[0] * forceLocal);
        coor[1] = (coor[1] * forceLocal);
        forceVec += coor;
    }
    return forceVec;
}

glm::vec2 GravityEngine::getDirection() {
    return glm::vec2(0, 0);
}