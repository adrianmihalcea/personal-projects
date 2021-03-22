// Tema 1 EGC 2020 - Mihalcea Adrian
#pragma once

#include <ctime>

#include <iostream>
#include <fstream>

#include "Constants.h"

using namespace std;

// Class for keeping track of current score and lives, score log and highscore
class Score {
public:
	Score(int lives) {
		lives_ = lives;

		SetHighScore();
	}

	// Ends the game by displaying final score in console and updating the score log
	void EndGame() {
		cout << "Final Score: " << score_ << '\n';
		time_t now = time(0);
		char* dt = ctime(&now);

		ofstream fout;
		fout.open("./Source/Laboratoare/Tema1/scores.txt", ofstream::out | ofstream::app);
		fout << score_ << "    ;" << dt << '\n';
		exit(0);
	}

	// Increases score
	void IncreaseScore() {
		++score_;
	}

	// Decreases score
	void DecreaseScore() {
		if (score_ == 0) {
			return;
		}
		--score_;
	}

	// Decreases lives and call end game
	void LoseLife() {
		--lives_;
		if (lives_ == 0) {
			EndGame();
		}
	}

	// Getter for highscore
	int GetHighScore() {
		return high_score_;
	}

	// Getter for lives
	int GetLives() {
		return lives_;
	}

	// Getter for score
	int GetScore() {
		return score_;
	}

private:
	int lives_;
	int score_ = 0;
	int high_score_ = 0;

	// Reads highscore from file
	void SetHighScore() {
		ifstream fin("./Source/Laboratoare/Tema1/scores.txt");
		char temp[1000];
		fin.getline(temp, 1000);
		int nr;
		while (fin >> nr) {
			if (nr > high_score_) {
				high_score_ = nr;
			}
			fin.getline(temp, 1000);
		}
	}
};