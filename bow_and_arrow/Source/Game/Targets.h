// Tema 1 EGC 2020 - Mihalcea Adrian

#pragma once

#include <stdlib.h>

#include <iostream>

#include "Constants.h"

using namespace std;

// Class for balloon objects
class Balloon {
public:
	Balloon(int x, int y, bool bad, float speed, float maxY) {
		x_ = x;
		y_ = y;
		bad_ = bad;
		speed_ = speed;
		max_y_ = maxY;
	}

	// Returns false if balloon goes off-screen
	bool GoUp(float deltaTimeSeconds) {
		if (time_since_pop_ >= 0) {
			time_since_pop_ += 2 * deltaTimeSeconds;
			return false;
		}
		y_ += speed_ * deltaTimeSeconds;
		return y_ + kBalloonRadius < max_y_;
	}

	// Getter for balloon's position
	pair<float, float> GetPos() {
		return { x_, y_ };
	}

	// Returns balloon type (false = bad, true = good)
	bool IsBad() {
		return bad_;
	}

	// Start the balloon pop timer
	void StartPop() {
		time_since_pop_ = 0;
	}

	// Gets the time since balloon was popped
	float GetTime() {
		return time_since_pop_;
	}

private:
	float x_, y_;
	bool bad_;
	float speed_, max_y_;
	float time_since_pop_ = -1;
};

// Class for shuriken objects
class Shuriken {
public:
	// Shuriken default constructor
	Shuriken(float x, float y, float speed, float sceneHeight) {
		x_ = x;
		y_ = y;
		speed_ = speed;
		scene_height_ = sceneHeight;
	}

	// Shuriken random constructor
	Shuriken(float x, float sceneHeight) {
		x_ = x;
		y_ = rand() % int(sceneHeight);
		speed_ = kShurikenMaxSpeed - rand() % 500;
		scene_height_ = sceneHeight;
	}

	// Returns false if shuriken goes off-screen
	bool goLeft(float deltaTimeSeconds) {
		x_ -= deltaTimeSeconds * speed_;
		angle_ += deltaTimeSeconds * kSpinSpeed;
		return x_ > 0;
	}

	// Getter for shuriken position
	pair<float, float> getPos() {
		return { x_, y_ };
	}

	// Getter for shuriken speed
	float getSpeed() {
		return speed_;
	}

	// Getter for shuriken angle
	float genAngle() {
		return angle_;
	}

private:
	float x_, y_;
	float angle_ = 0;
	float speed_;
	float scene_height_;
};