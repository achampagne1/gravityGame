#pragma once
#include "model.h"
#include "../bulletHandler.h"

class Player : public Model{
private:
	std::shared_ptr<BulletHandler> bulletHandler{ new BulletHandler };
	std::shared_ptr<VertexData> vertexData{ new VertexData };
public:
	Player();
	std::shared_ptr<BulletHandler> getBulletHandler();
};

