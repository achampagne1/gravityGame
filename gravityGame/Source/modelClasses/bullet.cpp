#include "bullet.h"

Bullet::Bullet() {

}

Bullet::Bullet(const Bullet& bullet) :Model(bullet) {
	timer->startTimer(10000); //sets lifespan to 2 seconds
}

bool Bullet::checkToDestroy() {
	if (timer->checkTimer())
		return 1;
	else
		return 0;
}

