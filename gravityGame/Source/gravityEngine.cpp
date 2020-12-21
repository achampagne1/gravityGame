#include "gravityEngine.h"

GravityEngine::GravityEngine() {

}

glm::vec2 GravityEngine::getDeltaVelocity(std::shared_ptr<VertexData> model, std::vector<std::shared_ptr<VertexData>> references) {
    return calculateDeltaVelocity(model,references);
}

glm::vec2 GravityEngine::calculateDeltaVelocity(std::shared_ptr<VertexData> model, std::vector<std::shared_ptr<VertexData>> references) {
    float bigG = 3;
    glm::vec2 forceVec = glm::vec2(0,0);
    for (int i = 0; i < references.size(); i++) {
        float seperationX = references.at(i)->getAvgX() - model->getAvgX();
        float seperationY = references.at(i)->getAvgY() - model->getAvgY();
        float seperation = sqrt(pow(seperationX, 2) + pow(seperationY, 2));
        float forceLocal = (bigG * references.at(i)->getGravity()) / (seperation * seperation);
        glm::vec2 coor = glm::normalize(glm::vec2(1/seperationX, 1/seperationY));
        coor[0] = (coor[0] * forceLocal);
        coor[1] = (coor[1] * forceLocal);
        forceVec += coor;
    }
    return forceVec;
}

glm::vec2 GravityEngine::getDirection() {
    return glm::vec2(0, 0);
}