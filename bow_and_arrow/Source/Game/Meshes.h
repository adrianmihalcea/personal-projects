// Tema 1 EGC 2020 - Mihalcea Adrian

#pragma once

#include <string>
#include <vector>

#include "Constants.h"
#include "Targets.h"

using namespace std;

// Returns pointer to mesh for displaying numbers (digital clock)
Mesh* getNumberMesh(string name, int number) {
	vector<int> lines = kNumberLines[number];
	vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(0, 2 * kNumberSize, 0), kScoreColour),
		VertexFormat(glm::vec3(kNumberSize, 2 * kNumberSize, 0), kScoreColour),
		VertexFormat(glm::vec3(kNumberSize, kNumberSize, 0), kScoreColour),
		VertexFormat(glm::vec3(kNumberSize, 0, 0), kScoreColour),
		VertexFormat(glm::vec3(0, 0, 0), kScoreColour),
		VertexFormat(glm::vec3(0, kNumberSize, 0), kScoreColour)
	};
	vector<unsigned short> indices;
	for (int i = 0; i < lines.size(); ++i) {
		int x, y;
		tie(x, y) = kNumberIndices[lines[i]];
		indices.push_back(x);
		indices.push_back(y);
	}

	Mesh* ret = new Mesh(name);

	ret->InitFromData(vertices, indices);
	ret->SetDrawMode(GL_LINES);
	return ret;
}

// Returns pointer to mesh for the hearts for current lives
Mesh* getHeartMesh(string name) {
	vector<VertexFormat> vertices;
	vector<unsigned short> indices;

	int i = 0;
	for (float x = -magicHeartConstant; x <= 1.139; x += 0.001) {
		float delta = cbrt(x * x) * cbrt(x * x) - 4 * x * x + 4;
		float y1 = (cbrt(x * x) + sqrt(delta)) / 2;
		float y2 = (cbrt(x * x) - sqrt(delta)) / 2;
		vertices.push_back(VertexFormat(glm::vec3(x, y1, 0), kHeartColour));
		vertices.push_back(VertexFormat(glm::vec3(x, y2, 0), kHeartColour));
		indices.push_back(i++);
		indices.push_back(i++);
	}

	Mesh* ret = new Mesh(name);

	ret->InitFromData(vertices, indices);
	ret->SetDrawMode(GL_LINE_LOOP);
	return ret;
}

// Returns pointer to mesh for the power bar outline
Mesh* getPowerOutlineMesh(string name) {
	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(0, 0, 0), kPowerOutlineColour),
		VertexFormat(glm::vec3(kPowerMaxLength, 0, 0), kPowerOutlineColour),
		VertexFormat(glm::vec3(kPowerMaxLength, kPowerHeight, 0), kPowerOutlineColour),
		VertexFormat(glm::vec3(0, kPowerHeight, 0), kPowerOutlineColour)
	};

	std::vector<unsigned short> indices = {
		0, 1, 2, 3
	};

	Mesh* ret = new Mesh(name);

	ret->InitFromData(vertices, indices);
	ret->SetDrawMode(GL_LINE_LOOP);
	return ret;
}

// Returns pointer to mesh for the power bar
Mesh* getPowerMesh(string name) {
	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(2, 2, 0), kPowerColour),
		VertexFormat(glm::vec3(kPowerMaxLength - 2, 2, 0), kPowerColour),
		VertexFormat(glm::vec3(kPowerMaxLength - 2, kPowerHeight - 2, 0), kPowerColour),
		VertexFormat(glm::vec3(2, kPowerHeight - 2, 0), kPowerColour)
	};

	std::vector<unsigned short> indices = {
		0, 1, 2,
		0, 2, 3
	};

	Mesh* ret = new Mesh(name);

	ret->InitFromData(vertices, indices);
	return ret;
}

// Returns pointer to mesh for the arrow
Mesh* getArrowMesh(string name) {
	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(0, 0, 0), kArrowColour),
		VertexFormat(glm::vec3(kArrowLength, 0, 0), kArrowColour),
		VertexFormat(glm::vec3(0, kArrowWidth, 0), kArrowColour),
		VertexFormat(glm::vec3(kArrowLength, 0 + kArrowWidth, 0), kArrowColour),

		VertexFormat(glm::vec3(kArrowLength, kArrowWidth / 2 + kTipSize / 2, 0), kArrowColour),
		VertexFormat(glm::vec3(kArrowLength, kArrowWidth / 2 - kTipSize / 2, 0), kArrowColour),
		VertexFormat(glm::vec3(kArrowLength + kTipSize, kArrowWidth / 2, 0), kArrowColour)
	};

	std::vector<unsigned short> indices = {
		0, 1, 2,
		1, 2, 3,
		4, 5, 6
	};

	Mesh* ret = new Mesh(name);

	ret->InitFromData(vertices, indices);
	return ret;
};

// Returns pointer to mesh for the shurikens
Mesh* getShurikenMesh(string name, float side) {
	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(0, 0, 0), kShurikenColour),
		VertexFormat(glm::vec3(0, side, 0), kShurikenColour),
		VertexFormat(glm::vec3(-side, side, 0), kShurikenColour),
		VertexFormat(glm::vec3(-side, 0, 0), kShurikenColour),
		VertexFormat(glm::vec3(-side, -side, 0), kShurikenColour),
		VertexFormat(glm::vec3(0, -side, 0), kShurikenColour),
		VertexFormat(glm::vec3(side, -side, 0), kShurikenColour),
		VertexFormat(glm::vec3(side, 0, 0), kShurikenColour),
		VertexFormat(glm::vec3(side, side, 0), kShurikenColour)
	};

	std::vector<unsigned short> indices = {
		0, 1, 2,
		0, 3, 4,
		0, 5, 6,
		0, 7, 8
	};

	Mesh* ret = new Mesh(name);

	ret->InitFromData(vertices, indices);
	return ret;
}

// Returns pointer to mesh for circles (bow and balloons)
Mesh* getCircleMesh(string name, CircleType circleType, glm::vec3 colour, float radius) {
	float increment = 2 * kPI / 1000;
	float start_angle, end_angle;
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	int i;

	if (circleType == fullCircle) {
		start_angle = 0 * kPI;
		end_angle = 2 * kPI;
		i = 1;
		vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), colour));
	}
	else {
		start_angle = 1.5f * kPI;
		end_angle = 2.5f * kPI;
		i = 0;
	}

	for (float currAngle = start_angle; currAngle <= end_angle; currAngle += increment) {
		vertices.push_back(VertexFormat(glm::vec3(radius * cos(currAngle), radius * sin(currAngle), 0), colour));
		indices.push_back(i++);
		if (circleType == fullCircle && i % 2 == 1) {
			indices.push_back(0);
		}
	}

	Mesh* ret = new Mesh(name);

	ret->SetDrawMode(GL_LINE_LOOP);

	ret->InitFromData(vertices, indices);
	return ret;
}

// Returns pointer to mesh for the balloon string
Mesh* getBalloonStringMesh(string name, float delta) {
	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(0, 0, 0), kBalloonStringColour),
		VertexFormat(glm::vec3(-delta / 2, -delta, 0), kBalloonStringColour),
		VertexFormat(glm::vec3(delta / 2.0f, -2 * delta, 0), kBalloonStringColour),
		VertexFormat(glm::vec3(-delta / 2.0f, -3 * delta, 0), kBalloonStringColour),
		VertexFormat(glm::vec3(delta / 2.0f, -4 * delta, 0), kBalloonStringColour)
	};
	std::vector<unsigned short> indices = {
		0, 1, 2, 3, 4
	};

	Mesh* ret = new Mesh(name);
	ret->SetDrawMode(GL_LINE_STRIP);
	ret->InitFromData(vertices, indices);

	return ret;
}