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
	Unit(string Name, int Health,int Team);
	string GetName();
	int GetTeam();
	void UpdatePosition(int x, int y);
	int GetXPos();
	int GetYPos();
private:
	string Name;
	int Health;
	int Team;
	int XPos;
	int YPos;
};

class Square {
public:
	Square();
	Unit* GetContents();
	void SetContents(Unit* NewContents);
	Unit* Contains;


};


class Map {
public:
	Map(int x, int y);
	Unit* GetContentsOfGrid(int X, int Y);
	void AddUnitToGrid(Unit* Unit);
	Unit GetIfUnitClicked(int MouseX,int MouseY);

private:
	vector<vector<Square>>Grid;
	vector<Unit> UnitsInGrid;
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

