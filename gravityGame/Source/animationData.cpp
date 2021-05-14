#include "animationData.h"

void AnimationData::setFramesSize(int framesSize) {
	this->framesSize = framesSize;
	frames = new unsigned int[framesSize * 4];
}
void AnimationData::setType(std::string type) {
	this->type = type;
}
void AnimationData::setFrame(int index, unsigned int data) {
	frames[index] = data;
}
void AnimationData::setOrder(int i) {
	order.push_back(i);
}
int AnimationData::getFramesSize() {
	return order.size();
}
std::string AnimationData::getType() {
	return type;
}
unsigned int AnimationData::getFrame(int index) {
	return frames[index];
}
int AnimationData::getOrder(int index) {
	return order.at(index);
}
