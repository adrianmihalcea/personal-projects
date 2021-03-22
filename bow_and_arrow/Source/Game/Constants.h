// Tema 1 EGC 2020 - Mihalcea Adrian

#pragma once

#include <string>
#include <vector>

const float kPI = 3.1415926f;

const int kStartingLives = 3;
const glm::vec3 kHeartColour(1, 0, 0);
const int kHeartY = 50, kHeartX[] = { 50, 150, 250 };
const float magicHeartConstant = 1.139f;

const std::vector<std::vector<int>> kNumberLines = {
	{0, 1, 2, 3, 4, 5},
	{1, 2},
	{0, 1, 6, 4, 3},
	{0, 1, 6, 2, 3},
	{5, 6, 1, 2},
	{0, 5, 6, 2, 3},
	{0, 5, 4, 3, 2, 6},
	{0, 1, 2},
	{0, 1, 2, 3, 4, 5, 6},
	{3, 2, 1, 0, 5, 6}
};
const std::vector<std::pair<int, int>> kNumberIndices = {
	{0, 1},
	{1, 2},
	{2, 3},
	{3, 4},
	{4, 5},
	{5, 0},
	{5, 2}
};
const std::string kNrStrings[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
const int kScoreY = 575, kScoreX1 = 50, kScoreX2 = 120;
const glm::vec3 kScoreColour(0.25f, 0.25f, 0.5f);
const int kNumberSize = 50;

const glm::vec3 kBalloonStringColour(0.5f, 0, 0.5f);
const int kBalloonRadius = 50;
const float kBalloonScaleY = 1.25f;
const glm::vec3 kGoodBalloonColour(1, 0, 0);
const glm::vec3 kBadBalloonColour(0.75f, 0.5f, 0);
const float kBalloonPopTime = 0.5f;

const glm::vec3 kArrowColour(1.0f, 1.0f, 1.0f);
const int kArrowWidth = 5, kArrowLength = 100, kTipSize = 20;
const float kArrowSpeedIncrement = 10;
const float kMinArrowSpeed = 250;
const float kMaxArrowSpeed = 2000;
const int kArrowDrawDistance = 50;
const int kPlayerX = 50;
const float kPlayerHitTimer = 1;

const glm::vec3 kPowerColour(0.75f, 0.25f, 0);
const glm::vec3 kPowerOutlineColour(0, 0, 1);
const int kPowerMaxLength = 150, kPowerHeight = 35;

const glm::vec3 kShurikenColour(0.5f, 0, 0);
const float kShurikenMaxSpeed = 800;
const int kSpinSpeed = 10;
const int kShurikenSize = 25;

const float kBowRadius = 75;
const int kMoveSpeed = 250;
enum CircleType { fullCircle, halfCircle };