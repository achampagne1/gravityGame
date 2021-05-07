#pragma once
#include "model.h"
#include "../Source/timer.h" 

class Bullet :public Model{
	private:
		std::unique_ptr<Timer> timer{ new Timer };
	public:
		Bullet();
		Bullet(const Bullet& bullet);
		bool checkToDestroy();
};

