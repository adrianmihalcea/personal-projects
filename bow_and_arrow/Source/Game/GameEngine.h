// Tema 1 EGC 2020 - Mihalcea Adrian

#pragma once

#include <stdlib.h>

#include <list>

#include "Constants.h"
#include "Targets.h"

using namespace std;

// Class for game logic
class GameEngine {
public:
	// Constructor for game engine, setting game starting difficulty (default 1)2
	GameEngine(float scene_width, float scene_height, int difficulty = 1) {
		scene_width_ = scene_width;
		difficulty_ = difficulty;
		scene_height_ = scene_height;
	}

	// Spawns 0-2 balloons every second (increases with difficulty)
	void SpawnBalloons() {
		int nr = rand() % 3;
		for (int i = 0; i < nr; ++i) {
			int x = rand() % int(scene_width_ - 200) + 200;
			int bad_chance = (difficulty_ < 3) ? (5 - difficulty_) : 2;
			int isBad = rand() % bad_chance;
			balloons_.push_back(Balloon(x, 0, isBad == 0, 100, scene_height_));
		}
	}

	// Begins shooting the arrow, sets the speed and fixed angle
	void ShootArrow(float speed) {
		if (arrow_speed_ > 0) {
			return;
		}

		arrow_speed_ = (speed + kMinArrowSpeed > kMaxArrowSpeed) ? kMaxArrowSpeed
			: speed + kMinArrowSpeed;
		arrow_angle_ = bow_angle_;
	}

	// Updates the bow angle, and the arrow angle if it's not already moving
	void UpdateBowAngle(float bow_angle) {
		bow_angle_ = bow_angle;
		if (arrow_speed_ == 0) {
			arrow_angle_ = bow_angle;
		}
	}

	// Updates arrow position (on the bow or flying with the current angle)
	// Stops flying when out of bounds
	void UpdateArrowPos(float deltaTimeSeconds) {
		if (arrow_speed_ > 0) {
			arrow_x_ += deltaTimeSeconds * arrow_speed_ * glm::cos(arrow_angle_);
			arrow_y_ += deltaTimeSeconds * arrow_speed_ * glm::sin(arrow_angle_);
			if (ArrowOutOfBounds()) {
				arrow_speed_ = 0;
			}
		}
		else {
			arrow_x_ = kPlayerX;
			arrow_y_ = player_y_;
		}
		if (hit_timer_ > 0) {
			hit_timer_ -= deltaTimeSeconds;
		}
	}

	// Starts the hit timer
	void StartHitTimer() {
		hit_timer_ = kPlayerHitTimer;
	}

	// Spawns new shuriken, based on difficulty
	void SpawnShuriken() {
		int new_shuriken = rand() % (2 + difficulty_);
		for (int i = shurikens_.size(); i < new_shuriken; ++i) {
			shurikens_.push_back(Shuriken(scene_width_, scene_height_));
		}
	}

	// Returns iterator to balloon hit by arrow
	list<Balloon>::iterator HitBalloon() {
		if (arrow_speed_ == 0) {
			return balloons_.end();
		}
		list<Balloon>::iterator it = balloons_.begin();
		while (it != balloons_.end()) {
			if (it->GetTime() == -1) {
				float x, y;
				tie(x, y) = GetTipCoordinates();
				float dist =
					Distance(it->GetPos().first, it->GetPos().second * kBalloonScaleY,
						x, y);
				// Checks whether the arrow tip is within the circle (hitbox) surrounding the balloon
				if (dist < kBalloonRadius * kBalloonScaleY) {
					return it;
				}
			}
			++it;
		}
		return balloons_.end();
	}

	// Returns iterator to shuriken object that collided with the player
	list<Shuriken>::iterator HitByShuriken() {
		list<Shuriken>::iterator it = shurikens_.begin();
		while (it != shurikens_.end()) {
			float dist = Distance(kPlayerX, player_y_, it->getPos().first, it->getPos().second);
			// Checks whether the 2 hitboxes (circles) intersect
			if (dist <= kBowRadius + kShurikenSize) {
				break;
			}
			++it;
		}
		return it;
	}

	// Returns iterator to shuriken object that collided with arrow
	list<Shuriken>::iterator HitShuriken() {
		if (arrow_speed_ == 0) {
			return shurikens_.end();
		}
		list<Shuriken>::iterator it = shurikens_.begin();
		while (it != shurikens_.end()) {
			float dist =
				Distance(GetTipCoordinates().first, GetTipCoordinates().second,
					it->getPos().first, it->getPos().second);
			// Checks whether arrow tip is within the hitbox (circle) surrounding shuriken
			if (dist <= kShurikenSize) {
				break;
			}
			++it;
		}
		return it;
	}

	// Increases difficulty
	void IncreaseDifficulty() {
		++difficulty_;
	}

	// Setter for player y
	void SetPlayerY(float y) {
		player_y_ = y;
	}

	// Getter for player y coordinate
	float GetPlayerY() {
		return player_y_;
	}

	// Setter for arrow position
	void SetArrowPos() {
		arrow_x_ = kPlayerX;
		arrow_y_ = player_y_;
	}

	// Getter for arrow angle
	float GetArrowAngle() {
		return arrow_angle_;
	}

	// getter for arrow x coordinate
	float GetArrowX() {
		return arrow_x_;
	}

	// getter for arrow y coordinate
	float GetArrowY() {
		return arrow_y_;
	}

	float GetArrowSpeed() {
		return arrow_speed_;
	}

	// Getter for pointer to balloons list
	list<Balloon>* GetBalloons() {
		return &balloons_;
	}

	// Getter for pointer to shurikens list
	list<Shuriken>* GetShurikens() {
		return &shurikens_;
	}

	// Getter for the hit timer
	float GetHitTimer() {
		return hit_timer_;
	}

private:
	int difficulty_ = 1;
	float scene_width_, scene_height_;
	float arrow_speed_ = 0;
	float bow_angle_, arrow_angle_;
	float arrow_x_, arrow_y_;
	float player_y_;
	list<Balloon> balloons_;
	list<Shuriken> shurikens_;
	float hit_timer_ = 0;

	// Checks whether arrow went off-screen
	bool ArrowOutOfBounds() {
		return (arrow_x_ + kArrowLength + kTipSize >= scene_width_) ||
			(arrow_x_ <= 0) || (arrow_y_ <= 0) ||
			(arrow_y_ >= scene_height_);
	}

	// Returns distance between 2 points
	float Distance(float x1, float y1, float x2, float y2) {
		return glm::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	}

	// Computes coordinates of arrow tip
	pair<float, float> GetTipCoordinates() {
		float x = arrow_x_, y = arrow_y_, length = kArrowLength + kTipSize;

		x += glm::cos(arrow_angle_) * length + glm::sin(arrow_angle_) * kArrowWidth;
		y += glm::sin(arrow_angle_) * length + glm::cos(arrow_angle_) * kArrowWidth;

		return { x, y };
	}
};