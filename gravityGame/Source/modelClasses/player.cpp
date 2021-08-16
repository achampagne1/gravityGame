#include "player.h"

Player::Player() {

}

std::shared_ptr<BulletHandler> Player::getBulletHandler() {
	return bulletHandler;
}