#pragma once
#ifndef quadTreeH
#define quadTreeH
#include <iostream>
#include <vector>
#include "VertexData.h"

class QuadTree {
	private:
		int maxObjects = 10;
		int maxLevels = 5;

		int level;
		int bounds[4];
		std::vector<std::shared_ptr<VertexData>> objects;
		std::vector<std::shared_ptr<QuadTree>> nodes;

	public:
		QuadTree(int level, int* bounds);
		void clear();
};
#endif