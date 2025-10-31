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
protected:
	string Name;
	int Range;
	int Strength;

};

class Sword:virtual public Weapon {
public:
	Sword(string Name, int Range, int Strength);
};
class Bow :virtual public Weapon {
public:
	Bow(string Name, int Range, int Strength);
};
class Unit {
public:
	Unit(string Name,int Dexterity,int Defence, int Health,int Team,int Speed,string Path);
	string GetName();
	int GetDexterity();
	int GetDefence();
	int GetSpeed();
	int GetHealth();
	//int GetStrength();
	int GetTeam();
	void UpdatePosition(int x, int y);
	int GetXPos();
	int GetYPos();
	void CalculateCurrentMoves();
	void CalculatePossibleAttacks();
	vector<vector<int>> GetCurrentMoves();
	vector<vector<int>> GetCurrentAttacks();
	bool GetIfUsedThisTurn();
	void SetUsed(bool Status);
	void SetWeapon(Weapon* weapon);
	string GetSpritePath();
	Weapon* GetWeapon();
	void TakeDamage(int Damage);

private:
	int Dexterity;
	string SpritePath;
	Weapon* EquippedWeapon;
	bool UsedThisTurn;
	vector<vector<int>>CurrentAttacks;
	vector<vector<int>> CurrentMoves;
	string Name;
	int Health;
	int Speed;
	int Team;
	int XPos;
	int YPos;
	int Defence;
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
	Map(int x, int y,int Units,int DeploymentZone);
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
	Unit* GetUnitInPos(int x, int y);
	bool CheckForDefeated();
	bool CheckForEndOfGame();
	int GetVictor();
	vector<vector<int>>GetStartingPositions(int StartY, int EndY,int NoOfUnits);
	bool CheckIfPosTaken(int X, int Y, vector<vector<int>> Positions);

private:
	vector<vector<Square>>Grid;
	int Width;
	int Height;
	vector<Unit*> UnitsInGrid;

};
class Combat {
public:
	Combat(Unit* Attacker, Unit* Defender);
	string DoCombat();
	Unit* GetAttacker();
	Unit* GetDefender();
	bool GetIfHits(int Dex,int Avo);
	int GetDamage(int Str, int Def);
	void DrawSlash(int x,int y);
	int GetRange();
private:
	Unit* Attacker;
	Unit* Defender;
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
	int GetTurn();
	Unit* GetCurrentlySelected();
	void SetCurrentlySelected(Unit* Selected);
	Unit* GetCurrentlySelectedEnemy();
	void SetCurrentlySelectedEnemy(Unit* Selected);
	vector<int> GetCurrentlySelectedPos();
	bool GetIfAllUnitsActivatedThisTurn(vector<Unit*> Units);
	void UpdateTurn();
	void SwapPlayers();
	int GetFrameCount();
	void IncrementFrameCount();


private:
	Unit* CurrentlySelected;
	Unit* CurrentEnemy;
	int Turn;
	Player* P1;
	Player* P2;
	int CurrentPlayer;
	int CurrentlySelectedX;
	int CurrentlySelectedY;
	int FrameCount;
};


