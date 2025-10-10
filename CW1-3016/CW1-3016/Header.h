#pragma once

#include <iostream>
#include <string>
using namespace std;
#include <SDL3/SDL.h>

struct Grid {
	int Height = 24;
	int Width = 24;
	int SQUARE_SIZE = 40;
};

class Unit {
public:
	Unit(string Name, int Health);
	string GetName();
private:
	string Name;
	int Health;
};

class Square {
public:
	Square();
	Unit* GetContents();
	Unit* Contains;


};


class Map {
public:
	Map(int x, int y);
	Unit* GetContentsOfGrid(int X, int Y);
private:
	vector<vector<Square>>Grid;
};

class Game {
public:
	Game(string P1Name,string P2Name);
private:
	int Turn;
};
class Player {
public:
	Player(string Name);
private:
	string Name;

};

