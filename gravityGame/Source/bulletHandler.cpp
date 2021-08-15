#include "bulletHandler.h"

BulletHandler::BulletHandler(){}

void BulletHandler::setReferencesPtr(std::vector<std::shared_ptr<Model>>* referencePtr) {
    this->referencePtr = referencePtr;
}

void BulletHandler::setGenericBullet(std::shared_ptr<Bullet> genericBullet) {
    this->genericBullet = genericBullet;
}

std::shared_ptr<Bullet> BulletHandler::createBullet(glm::vec2 coordinate, glm::vec2 direction) {
    float velocit[2] = { direction[0] * 12,direction[1] * 12 };
    float coor[2] = { 0,0 };
    std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(*genericBullet);
    bullet->moveWithPosition(coor);
    bullet->setVelocity(velocit);
    return bullet;
}

void BulletHandler::updateBullets(glm::vec2 newOffset) {
    for (int i = 0; i < 100; i++) {
        if (bullets[i] != nullptr) {
            bullets[i]->rotate(glm::vec2(0, 1));
            bullets[i]->moveWithVelocity(newOffset);
        }
    }

    float* newOffset2;
    for (int i = 0; i < 100; i++) {
        if (bullets[i] == nullptr)
            continue;
        else if (bullets[i]->checkToDestroy())
            bullets[i] = nullptr;
        else {
            newOffset2 = bullets[i]->calculateVelocity(*referencePtr);
            glm::vec2 newOffset2temp = glm::vec2{ newOffset2[0],newOffset2[1] };
            bullets[i]->moveWithVelocity(newOffset2temp);
        }
    }
}

void BulletHandler::renderBullets() {
    for (int i = 0; i < 100; i++) {
        if (bullets[i] != nullptr)
            bullets[i]->render();
    }
}

void BulletHandler::shoot(glm::vec2 cursorPos) {
    glm::vec2 temp = { 100,100 };
    glm::vec2 direction = glm::normalize(glm::vec2(-(800 / 2 - cursorPos.x), 800 / 2 - cursorPos.y));
    glm::vec2 directionToShoot = { direction[0],direction[1] };
    std::shared_ptr<Bullet> bullet = createBullet(temp, directionToShoot);
    for (int i = 0; i < 100; i++) {
        if (bullets[i] == nullptr) {
            bullets[i] = bullet;
            break;
        }
    }
}
