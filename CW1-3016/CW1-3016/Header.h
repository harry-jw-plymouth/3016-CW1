#pragma once

using namespace std;
#include <SDL3/SDL.h>

class Game {
public:
	Game(string P1Name,string P2Name);
private:
	int turn;
	Player* P1;
	Player* P2;
};
class Player {
public:
	Player(string Name);
private:
	string Name;

};
class Unit {
public:
	Unit(string Name, int Health);
private:
	string Name;
	int Health;
};
