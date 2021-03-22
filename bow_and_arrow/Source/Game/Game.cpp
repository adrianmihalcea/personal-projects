// Tema 1 EGC 2020 - Mihalcea Adrian

#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <list>
#include <vector>

#include <Core/Engine.h>
#include "Constants.h"
#include "Meshes.h"
#include "Object2D.h"
#include "Score.h"
#include "Targets.h"
#include "Game.h"
#include "Transform2D.h"

using namespace std;

// Initializations
void Game::Init() {
	score_ = new Score(kStartingLives);
	srand(time(NULL));
	glm::ivec2 resolution = window->GetResolution();
	engine_ = new GameEngine(resolution.x, resolution.y);
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);
	engine_->SetPlayerY(resolution.y / 2);
	engine_->SetArrowPos();

	// Creates meshes
	{
		Mesh* bow = getCircleMesh("bow", halfCircle, glm::vec3(0, 0, 1), kBowRadius);
		AddMeshToList(bow);

		Mesh* goodBalloon = getCircleMesh("goodBalloon", fullCircle, kGoodBalloonColour, kBalloonRadius);
		AddMeshToList(goodBalloon);
		Mesh* badBalloon = getCircleMesh("badBalloon", fullCircle, kBadBalloonColour, kBalloonRadius);
		AddMeshToList(badBalloon);

		Mesh* shuriken = getShurikenMesh("shuriken", kShurikenSize);
		AddMeshToList(shuriken);

		Mesh* balloonString = getBalloonStringMesh("balloonString", 25);
		AddMeshToList(balloonString);

		Mesh* arrow = getArrowMesh("arrow");
		AddMeshToList(arrow);

		Mesh* power = getPowerMesh("power");
		AddMeshToList(power);

		Mesh* powerOutline = getPowerOutlineMesh("powerOutline");
		AddMeshToList(powerOutline);

		Mesh* heart = getHeartMesh("heart");
		AddMeshToList(heart);

		for (int i = 0; i < 10; ++i) {
			Mesh* nr = getNumberMesh(kNrStrings[i], i);
			AddMeshToList(nr);
		}
	}
}

void Game::FrameStart() {
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Game::Update(float deltaTimeSeconds) {
	// Render score and highscore
	{
		int digits = score_->GetScore(), tens = digits / 10, units = digits % 10;
		glLineWidth(5);

		model_matrix_ = glm::mat3(1)
			* Transform2D::Translate(kScoreX1, kScoreY);
		RenderMesh2D(meshes[kNrStrings[tens]], shaders["VertexColor"], model_matrix_);

		model_matrix_ = glm::mat3(1)
			* Transform2D::Translate(kScoreX2, kScoreY);
		RenderMesh2D(meshes[kNrStrings[units]], shaders["VertexColor"], model_matrix_);

		digits = score_->GetHighScore();
		tens = digits / 10;
		units = digits % 10;

		model_matrix_ = glm::mat3(1)
			* Transform2D::Translate(kScoreX1 + 200, kScoreY)
			* Transform2D::Scale(0.5f, 0.5f);
		RenderMesh2D(meshes[kNrStrings[tens]], shaders["VertexColor"], model_matrix_);

		model_matrix_ = glm::mat3(1)
			* Transform2D::Translate(kScoreX2 + 175, kScoreY)
			* Transform2D::Scale(0.5f, 0.5f);
		RenderMesh2D(meshes[kNrStrings[units]], shaders["VertexColor"], model_matrix_);

		glLineWidth(1);
	}

	// Render lives
	{
		for (int i = 0; i < score_->GetLives(); ++i) {
			model_matrix_ = glm::mat3(1)
				* Transform2D::Translate(kHeartX[i], kHeartY)
				* Transform2D::Scale(30, 30);
			RenderMesh2D(meshes["heart"], shaders["VertexColor"], model_matrix_);
		}
	}

	// Render the bow
	{
		model_matrix_ = glm::mat3(1)
			* Transform2D::Translate(kPlayerX, engine_->GetPlayerY())
			* Transform2D::Rotate(mouse_angle_);
		engine_->UpdateBowAngle(mouse_angle_);
		float hit_timer = engine_->GetHitTimer();
		if (hit_timer > 0) {
			model_matrix_ *= Transform2D::Scale(1 - hit_timer, 1 - hit_timer);
		}
		RenderMesh2D(meshes["bow"], shaders["VertexColor"], model_matrix_);
	}

	// Render the arrow and power bar
	{
		model_matrix_ = glm::mat3(1);
		engine_->UpdateArrowPos(deltaTimeSeconds);
		float power_scale = (arrow_speed_ + kMinArrowSpeed) / kMaxArrowSpeed;
		model_matrix_ *= Transform2D::Translate(engine_->GetArrowX(), engine_->GetArrowY());
		model_matrix_ *= Transform2D::Rotate(engine_->GetArrowAngle());
		if (engine_->GetArrowSpeed() == 0) {
			model_matrix_ *= Transform2D::Translate(-power_scale * kArrowDrawDistance, 0);
		}
		RenderMesh2D(meshes["arrow"], shaders["VertexColor"], model_matrix_);

		if (arrow_speed_ == 0) {
			power_scale = engine_->GetArrowSpeed() / kMaxArrowSpeed;
		}
		model_matrix_ = glm::mat3(1)
			* Transform2D::Translate(kPlayerX - kPowerMaxLength / 2.0f + 50, engine_->GetPlayerY() - kBowRadius - 50)
			* Transform2D::Scale(power_scale, 1);
		RenderMesh2D(meshes["power"], shaders["VertexColor"], model_matrix_);
		model_matrix_ = glm::mat3(1)
			* Transform2D::Translate(kPlayerX - kPowerMaxLength / 2.0f + 50, engine_->GetPlayerY() - kBowRadius - 50);
		glLineWidth(5);
		RenderMesh2D(meshes["powerOutline"], shaders["VertexColor"], model_matrix_);
		glLineWidth(1);
	}

	// Spawns new balloons every second
	time_since_last_spawn_ += deltaTimeSeconds;
	if (time_since_last_spawn_ > 1) {
		time_since_last_spawn_ = 0;
		engine_->SpawnBalloons();
	}

	// Balloon logic
	{
		list<Balloon>* balloons = engine_->GetBalloons();
		list<Balloon>::iterator it = balloons->begin();
		while (it != balloons->end()) {
			if (it->GoUp(deltaTimeSeconds)) {
				model_matrix_ = glm::mat3(1)
					* Transform2D::Scale(1, kBalloonScaleY)
					* Transform2D::Translate(it->GetPos().first, it->GetPos().second);
				if (it->IsBad()) {
					RenderMesh2D(meshes["badBalloon"], shaders["VertexColor"], model_matrix_);
				}
				else {
					RenderMesh2D(meshes["goodBalloon"], shaders["VertexColor"], model_matrix_);
				}
				model_matrix_ = glm::mat3(1)
					* Transform2D::Translate(it->GetPos().first,
						it->GetPos().second * kBalloonScaleY - kBalloonRadius);
				RenderMesh2D(meshes["balloonString"], shaders["VertexColor"], model_matrix_);
				++it;
			}
			// Current balloon doesn't go up
			else {
				float time = it->GetTime();

				// The balloon is out of bounds or completely popped
				if (time > kBalloonPopTime) {
					it = balloons->erase(it);
				}
				// The balloon is popping
				else {
					model_matrix_ = glm::mat3(1)
						* Transform2D::Translate(it->GetPos().first, it->GetPos().second * kBalloonScaleY)
						* Transform2D::Scale(1 - time, (1 - time) * kBalloonScaleY);

					if (it->IsBad()) {
						RenderMesh2D(meshes["badBalloon"], shaders["VertexColor"], model_matrix_);
					}
					else {
						RenderMesh2D(meshes["goodBalloon"], shaders["VertexColor"], model_matrix_);
					}

					++it;
				}
			}
		}
	}

	// Shuriken logic
	{
		engine_->SpawnShuriken();
		list<Shuriken>* shurikens = engine_->GetShurikens();

		list<Shuriken>::iterator it = shurikens->begin();
		while (it != shurikens->end()) {
			if (it->goLeft(deltaTimeSeconds)) {
				model_matrix_ = glm::mat3(1)
					* Transform2D::Translate(it->getPos().first, it->getPos().second)
					* Transform2D::Rotate(it->genAngle());
				RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], model_matrix_);
				++it;
			}
			else {
				it = shurikens->erase(it);
			}
		}
	}

	// Collisions
	{
		// Balloon collisions
		{
			list<Balloon>* balloons = engine_->GetBalloons();
			list<Balloon>::iterator hit = engine_->HitBalloon();
			if (hit != balloons->end()) {
				hit->StartPop();
				if (hit->IsBad()) {
					score_->DecreaseScore();
				}
				else {
					score_->IncreaseScore();
					if (score_->GetScore() % 10 == 0 && score_->GetScore() != 0) {
						engine_->IncreaseDifficulty();
					}
				}
			}
		}

		// Shuriken collisions
		{
			list<Shuriken>* shurikens = engine_->GetShurikens();
			list<Shuriken>::iterator it = engine_->HitByShuriken();

			if (it != shurikens->end()) {
				shurikens->erase(it);
				engine_->StartHitTimer();
				score_->LoseLife();
			}

			it = engine_->HitShuriken();
			
			if (it != shurikens->end()) {
				shurikens->erase(it);
				score_->IncreaseScore();
				if (score_->GetScore() % 10 == 0 && score_->GetScore() != 0) {
					engine_->IncreaseDifficulty();
				}
			}
		}
	}
}

// Moves player with "W" and "S" and draws arrow with left click
void Game::OnInputUpdate(float deltaTime, int mods) {
	if (window->KeyHold(GLFW_KEY_W) &&
		engine_->GetPlayerY() + kBowRadius < window->GetResolution().y - 175) {
		engine_->SetPlayerY(engine_->GetPlayerY() + deltaTime * kMoveSpeed);
	}

	if (window->KeyHold(GLFW_KEY_S) &&
		engine_->GetPlayerY() - kBowRadius > 150) {
		engine_->SetPlayerY(engine_->GetPlayerY() - deltaTime * kMoveSpeed);
	}

	if (window->MouseHold(GLFW_MOUSE_BUTTON_1) &&
		arrow_speed_ + kMinArrowSpeed < kMaxArrowSpeed) {
		arrow_speed_ += kArrowSpeedIncrement;
	}
}

// On pressing 'X' the game exits (after recording the score)
void Game::OnKeyPress(int key, int mods) {
	if (key == GLFW_KEY_X) {
		score_->EndGame();
	}
}

// Gets the angle of the mouse pointer
void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
	glm::ivec2 resolution = window->GetResolution();
	mouse_angle_ = glm::atan(resolution.y - 1.0f * mouseY - engine_->GetPlayerY(), 1.0f * mouseX - kPlayerX);
}

// Releases arrow on left click release
void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
	if (button == GLFW_MOUSE_BUTTON_2) {
		engine_->ShootArrow(arrow_speed_);
		arrow_speed_ = 0;
	}
}
