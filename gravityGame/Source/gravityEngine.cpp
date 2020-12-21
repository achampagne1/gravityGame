#include "gravityEngine.h"

GravityEngine::GravityEngine() {

}

glm::vec2 GravityEngine::getDeltaVelocity(std::shared_ptr<VertexData> model, std::vector<std::shared_ptr<VertexData>> references) {
    return calculateDeltaVelocity(model,references);
}

glm::vec2 GravityEngine::calculateDeltaVelocity(std::shared_ptr<VertexData> model, std::vector<std::shared_ptr<VertexData>> references) {
    float bigG = 2;
    float forceX = 0;
    float forceY = 0;
    float force = 0;
    for (int i = 0; i < references.size(); i++) {
        float seperationX = references.at(i)->getAvgX() - model->getAvgX();
        float seperationY = references.at(i)->getAvgY() - model->getAvgY();
        float seperation = sqrt(pow(seperationX, 2) + pow(seperationY, 2));
        std::cout << seperation << " " << seperationX << " " << seperationY << std::endl;
        if (seperationX > 0.0)
            forceX += (bigG * references.at(i)->getGravity()) / (seperationX * seperationX);
        if(seperationX<0.0)
            forceX -= (bigG * references.at(i)->getGravity()) / (seperationX * seperationX);

        if (seperationY > 0.0)
            forceY += (bigG * references.at(i)->getGravity()) / (seperationY * seperationY);
        if (seperationY < 0.0)
            forceY -= (bigG * references.at(i)->getGravity()) / (seperationY * seperationY);

        force += (bigG * references.at(i)->getGravity()) / (seperation * seperation);
    }
    forceX /= references.size();
    forceY /= references.size();
    glm::vec2 directionOfForce;
    if(forceX == 0)
        directionOfForce = glm::normalize(glm::vec2(forceX, 1/forceY));
    if(forceY == 0)
        directionOfForce = glm::normalize(glm::vec2(1/ forceX, forceY));
    else 
        directionOfForce = glm::normalize(glm::vec2(1/forceX, 1 / forceY));
    return directionOfForce*force;
}

glm::vec2 GravityEngine::getDirection() {
    return glm::vec2(0, 0);
}