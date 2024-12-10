#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
using namespace std;


class Achievement {
	string achievements;

public:
	void trackAchievement() {

	}
	void updateProgress() {

	}


};

class Player {
	string name;
	int highScores;
	Achievement achievement;
	//GameState currentGameState;

public:
	void saveProgress() {}
	void loadProgress() {}




};

class LeaderBoard {
	int highScores[5];		// top 5 players
	Player topPlayers[5];
public:
	void updateScore(Player player, int score) {}
	void displayTopScores(){}

};