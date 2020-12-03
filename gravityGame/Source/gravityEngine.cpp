#include "gravityEngine.h"

GravityEngine::GravityEngine() {

}

glm::vec2 GravityEngine::getDeltaVelocity(std::shared_ptr<VertexData> model, std::vector<std::shared_ptr<VertexData>> references) {
    return calculateDeltaVelocity(model,references);
}

glm::vec2 GravityEngine::calculateDirection(std::shared_ptr<VertexData> model, std::vector<std::shared_ptr<VertexData>> references) {
    float xComponant = 0;
    float yComponant = 0;
    for (int i = 0; i < references.size(); i++) {
        xComponant += (references.at(i)->getAvgX() - model->getAvgX()) * references.at(i)->getGravity();
        yComponant += (references.at(i)->getAvgY() - model->getAvgY()) * references.at(i)->getGravity();
    }
    xComponant /= references.size();
    yComponant /= references.size();
    if (xComponant == 0 && yComponant == 0)
        direction =  glm::vec2(xComponant, yComponant);
    else
        direction = glm::normalize(glm::vec2(xComponant, yComponant));
    return direction;
}

float GravityEngine::calculateGravity(std::shared_ptr<VertexData> model, std::vector<std::shared_ptr<VertexData>> references) {
    float gravity = 0;
    for (int i = 0; i < references.size(); i++) {
        gravity += references.at(i)->getGravity();
    }
    gravity /= references.size();
    return gravity;
}

glm::vec2 GravityEngine::calculateDeltaVelocity(std::shared_ptr<VertexData> model, std::vector<std::shared_ptr<VertexData>> references) {
    glm::vec2 directionOfAcceleration = calculateDirection(model, references);
    float gravity = calculateGravity(model, references);
    return glm::vec2(directionOfAcceleration[0] * gravity, directionOfAcceleration[1] * gravity);
}

glm::vec2 GravityEngine::getDirection() {
    return direction;
}