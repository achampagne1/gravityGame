#pragma once

#include "vertexData.h"

class CollisionEngine{
	private:

	public:
		CollisionEngine();
		void calculateCollision(std::shared_ptr<VertexData> model, std::vector<std::shared_ptr<VertexData>> references);

};

