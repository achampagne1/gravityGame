#pragma once
#include "modelClasses\bullet.h"

class BulletHandler {
	private:
		std::shared_ptr<Bullet> bullets[100];
		std::vector<std::shared_ptr<Model>>* referencePtr;
		std::shared_ptr<Bullet> genericBullet;
	public:
		BulletHandler();
		void updateBullets(glm::vec2 newOffset);
		void setReferencesPtr(std::vector<std::shared_ptr<Model>>* referencePtr);
		void setGenericBullet(std::shared_ptr<Bullet> genericBullet);
		std::shared_ptr<Bullet> createBullet(glm::vec2 coordinate, glm::vec2 direction);
		void renderBullets();
		void shoot(glm::vec2 cursorPos);

};