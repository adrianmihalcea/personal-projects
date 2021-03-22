#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include "GameEngine.h"
#include "Score.h"

class Game : public SimpleScene
{
public:
	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;

protected:
	glm::mat3 model_matrix_;
	float mouse_angle_ = 0;
	float time_since_last_spawn_ = 0;
	GameEngine *engine_;
	float arrow_speed_ = 0;
	Score* score_;
};
