#pragma once

#include <iostream>
#include <string>
using namespace std;
#include <SDL_image.h>
#include <SDL3/SDL.h>

class Weapon {
public :
	Weapon(string Name,int Range, int Strength);
	string GetName();
	int GetRange();
	int GetStrength();
private:
	string Name;
	int Range;
	int Strength;

};

class Sword:Weapon {
public:
	Sword(string Name, int Range, int Strength) ;
};
class Unit {
public:
	Unit(string Name, int Health,int Team,int Speed,string Path);
	string GetName();
	int GetTeam();
	void UpdatePosition(int x, int y);
	int GetXPos();
	int GetYPos();
	void CalculateCurrentMoves();
	vector<vector<int>> GetCurrentMoves();
	bool GetIfUsedThisTurn();
	void SetUsed(bool Status);
	void SetWeapon(Weapon weapon);
	string GetSpritePath();
	Weapon* GetWeapon();
private:
	string SpritePath;
	Weapon* EquippedWeapon;
	bool UsedThisTurn;
	vector<vector<int>> CurrentMoves;
	string Name;
	int Health;
	int Speed;
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
	void MoveUnit(vector<int>NewPos, vector<int>OldPos, Unit* UnitToMove);
	Unit* GetIfUnitClicked(int MouseX,int MouseY);
	int GetHeight();
	int GetWidth();
	void AdjustGridForMove(vector<int>NewPos, vector<int>OldPos, Unit* UnitToMove);
	void AdjustUnits(vector<int>NewPos, vector<int>OldPos, Unit* UnitToMove);
	bool GetIfAllPlayersUnitsUsedThisTurn(int PlayerID);
	void SetAllUnitsToUnactivated();

private:
	vector<vector<Square>>Grid;
	int Width;
	int Height;
	vector<Unit*> UnitsInGrid;

};

class Player {
public:
	Player(string Name,int PlayerId);
	int GetPlayerId();
private:
	string Name;
	int PlayerId;

};

class Game {
public:
	Game(string P1Name,string P2Name);
	int GetCurrentPlayer();
	Unit* GetCurrentlySelected();
	void SetCurrentlySelected(Unit* Selected);
	vector<int> GetCurrentlySelectedPos();
	bool GetIfAllUnitsActivatedThisTurn(vector<Unit*> Units);
	void UpdateTurn();
	void SwapPlayers();
private:
	Unit* CurrentlySelected;
	int Turn;
	Player* P1;
	Player* P2;
	int CurrentPlayer;
	int CurrentlySelectedX;
	int CurrentlySelectedY;
};


