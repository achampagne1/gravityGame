#include "player.h"

Player::Player() {

}

/*Player::Player(const Player& player) {
	type = player.type;
	texturesSize = player.texturesSize;
	vertexData = std::make_shared<VertexData>(*player.vertexData);
	currentAnimationType = player.currentAnimationType;
}*/

/*void Player::render() {
	if (texturesSize > 1) {
		if (frameCounter == 2) { //number of frames to change sprite
			animationFrame++;
			frameCounter = 0;
		}
		else
			frameCounter++;

		if (animationFrame == texturesSize)
			animationFrame = 0;
		vertexData->render(currentAnimationType->getFrame(currentAnimationType->getOrder(animationFrame)));
	}
	else {
		vertexData->render(currentAnimationType->getFrame(0));
	}

	bulletHandler->renderBullets();
}*/

std::shared_ptr<BulletHandler> Player::getBulletHandler() {
	return bulletHandler;
}