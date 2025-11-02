// CW1-3016.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;
#include <string>
#include <vector>
#include <random>
#include <SDL3/SDL.h>
#include<SDL3_ttf/SDL_ttf.h>
#include "Header.h"
#include <SDL_image.h>

Weapon::Weapon(string Name,int Range, int Strength) {
    this->Name = Name;
    this->Range = Range;
    this->Strength = Strength;
}
string Weapon::GetName() {
    return Name;
}
int Weapon::GetStrength() {
    return Strength;
}
int Weapon::GetRange() {
    return Range;
}
Spear:: Spear(string Name, int Range, int Strength) :Weapon(Name, Range, Strength) {
    cout << "Making spear\n";
}
Sword::Sword(string Name, int Range, int Strength):Weapon(Name,Range,Strength) {
    cout << "Making sword\n";
}
Bow::Bow(string Name, int Range, int Strength) :Weapon(Name, Range, Strength) {
    cout << "Making bow \n";
}


vector<vector<int>> SquaresInRange;
vector<Weapon> Weapons;
Map GameMap(24, 24,5,4);
bool MoveDone;

Player::Player(string Name,int PlayerId) {
    this->Name = Name;
    this->PlayerId = PlayerId;
}
int Player::GetPlayerId() {
    return PlayerId;
}
Game::Game(string P1Name, string P2Name) {
    Turn = 0;
    P1 = new Player(P1Name,0); 
    P2 = new Player(P2Name,1);
    int CurrentPlayer = P1->GetPlayerId();
    FrameCount = 0;
}
vector<string> Game::GetUpdates() {
    vector<string> Temp;
    for (int i = 0; i < Updates.size(); i++) {
        Temp.push_back(Updates[i]);
    }
    Updates.clear();
    return Temp;
}
void Game::AddUpdate(string New) {
    Updates.push_back(New);
}
void Game::IncrementFrameCount() {
    FrameCount++;
}
int Game::GetFrameCount() {
    return FrameCount;
}

int SquareSize = 40;
int Game::GetCurrentPlayer() {
    return CurrentPlayer;
}
int Game::GetTurn() {
    return Turn;
}
Unit* Game::GetCurrentlySelectedEnemy() {
    return CurrentEnemy;
}

void Game::SetCurrentlySelectedEnemy(Unit* Selected) {
    CurrentEnemy = Selected;
}
Unit* Game::GetCurrentlySelected() {
    return CurrentlySelected;
}

void Game::SetCurrentlySelected(Unit* Selected) {
    CurrentlySelected = Selected;
    if (Selected != nullptr) {
        CurrentlySelectedX = Selected->GetXPos();
        CurrentlySelectedY = Selected->GetYPos();
    }
}
vector<int> Game::GetCurrentlySelectedPos() {
    return{ CurrentlySelectedX,CurrentlySelectedY };
}
void Game::SwapPlayers() {
    if (CurrentPlayer == P1->GetPlayerId()) {
        CurrentPlayer = P2->GetPlayerId();
        SDL_Log("Current player is now player 2");
    }
    else {
        CurrentPlayer = P1->GetPlayerId();
        SDL_Log("Current player is now player 1");
    }
}
bool Game::GetIfAllUnitsActivatedThisTurn(vector<Unit*> Units) {
    for (int i = 0; i < Units.size(); i++) {
        if (!Units[i]->GetIfUsedThisTurn()) {
            return false;
        }
    }
    return true;
}
void Game::UpdateTurn() {
    Turn++;
    cout << "\n Turn " << Turn << "\n";
}
void CreateWeapons() {
    Sword* IRon = new Sword("Iron sword", 1, 5);
    Weapons.push_back(Sword("Iron Sword", 1, 5));
    //   Weapons.emplace_back(make_shared<Sword>("Iron Sword", 1, 5));
    //   Weapons.emplace_back(new Sword("Power sword", 1, 7));

}

const int WINDOW_HEIGHT = 1400;
const int WINDOW_WIDTH = 2200;
//Grid MainGrid;
Game* GameInProgress;
Square::Square() {
    Contains=nullptr;

}
void Square::SetContents(Unit* NewContents) {
    Contains = NewContents;
}
Unit* Square::GetContents() {
    return Contains;
} 

bool UpdateNeeded = true;
int Map::GetRandomStat(int Lower, int Upper) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> UnitStat(Lower, Upper);
    return UnitStat(gen);

}
Map::Map(int x,int y,int Units,int DeplomentZone){
    CreateWeapons();
    vector<Square>Temp;
    for (int Y = 0; Y < y; Y++) {
        Temp.clear();
        for (int X = 0; X< x; X++) {
            Temp.emplace_back();
        }
        Grid.push_back(Temp);
    };
   // Grid.resize(y, vector<Square>(x));
    Width = x;
    Height = y;


    //cout << "Setup Positions \n";
    vector<vector<int>> Team1Positions = GetStartingPositions(0, DeplomentZone, Units);
    vector<vector<int>> Team2Positions = GetStartingPositions(y - DeplomentZone, y-1, Units);
    //cout << "Setup units \n";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> UnitType(0, 2);
    int RandomSelection;
    Unit* BlueUnit;
    for (int i = 0; i < Team1Positions.size(); i++) {
        //setupTeam0
       // cout << "Setting up  blue unit at " <<  (Team1Positions[i][0]) << " , " << (Team1Positions[i][1]) <<" \n";
        RandomSelection = UnitType(gen);
        if (RandomSelection == 0) {
           // cout << "Setting up Duelist \n";
            BlueUnit = new Unit(("Blue Duelist " + to_string(i)), GetRandomStat(9,20), GetRandomStat(3,9), GetRandomStat(9,16), 0, GetRandomStat(9,25), 4, "assets/BlueSwordUnit.png", "assets/UsedBlueSwordUnit.png");
            BlueUnit->SetWeapon(new Weapon("Iron Sword", 1, 14));
            BlueUnit->UpdatePosition(Team1Positions[i][0], Team1Positions[i][1]);
            Grid[Team1Positions[i][0]][Team1Positions[i][1]].SetContents(BlueUnit);
            AddUnitToGrid(Grid[Team1Positions[i][0]][Team1Positions[i][1]].GetContents());
         //   cout << "Contents of square: " << GetContentsOfGrid(Team1Positions[i][0], Team1Positions[i][1])->GetName()<<"\n";

        }
        else if (RandomSelection == 1) {
           // cout << "Setting up archer \n";
            BlueUnit= new Unit(("Blue Archer" + to_string(i)), GetRandomStat(20, 50), GetRandomStat(3, 8), GetRandomStat(8, 14), 0, GetRandomStat(8, 15),3, "assets/BlueArcherUnit.png", "assets/UsedBlueArcherUnit.png");
            BlueUnit->SetWeapon(new Weapon("Iron Bow", 6, 10));
            BlueUnit->UpdatePosition(Team1Positions[i][0], Team1Positions[i][1]);
            Grid[Team1Positions[i][0]][Team1Positions[i][1]].SetContents(BlueUnit);
         //   cout << "Grid right after placing " << Grid[Team1Positions[i][0]][Team1Positions[i][1]].GetContents() << "\n";
            AddUnitToGrid(Grid[Team1Positions[i][0]][Team1Positions[i][1]].GetContents());
           // cout << "Contents of square: " << GetContentsOfGrid(Team1Positions[i][0], Team1Positions[i][1])->GetName()<<"\n";
        }
        else if (RandomSelection == 2) {
            BlueUnit = new Unit(("Blue Knight" + to_string(i)), GetRandomStat(10, 20), GetRandomStat(5, 10), GetRandomStat(10, 18), 0, GetRandomStat(6, 11), 8, "assets/BlueKnightUnit.png", "assets/UsedBlueKnightUnit.png");
            BlueUnit->SetWeapon(new Weapon("Iron Spear", 2, 11));
            BlueUnit->UpdatePosition(Team1Positions[i][0], Team1Positions[i][1]);
            Grid[Team1Positions[i][0]][Team1Positions[i][1]].SetContents(BlueUnit);
            AddUnitToGrid(Grid[Team1Positions[i][0]][Team1Positions[i][1]].GetContents());
        }
     }
    for (int i = 0; i < Team2Positions.size(); i++) {
        //setupTeam1
       // Unit* BlueUnit = new Unit("Red" + to_string( i), 10, 5, 10, 1, 4, "assets/RedSwordUnit.png");
     //   BlueUnit->SetWeapon(new Weapon("Iron Sword", 1, 10));
       // BlueUnit->UpdatePosition(Team2Positions[i][0], Team2Positions[i][1]);
    //    Grid[Team2Positions[i][0]][Team2Positions[i][1]].SetContents(BlueUnit);
    //    AddUnitToGrid(Grid[Team2Positions[i][0]][Team2Positions[i][1]].GetContents());
        RandomSelection = UnitType(gen);
        if (RandomSelection == 0) {
            // cout << "Setting up Duelist \n";
            BlueUnit = new Unit(("Red Duelist" + to_string(i)), GetRandomStat(9, 20), GetRandomStat(3,9), GetRandomStat(9, 16), 1, GetRandomStat(9, 25),4, "assets/RedSwordUnit.png", "assets/UsedRedSwordUnit.png");
            BlueUnit->SetWeapon(new Weapon("Iron Sword", 1, 14));
            BlueUnit->UpdatePosition(Team2Positions[i][0], Team2Positions[i][1]);
            Grid[Team2Positions[i][0]][Team2Positions[i][1]].SetContents(BlueUnit);
            AddUnitToGrid(Grid[Team2Positions[i][0]][Team2Positions[i][1]].GetContents());
            //   cout << "Contents of square: " << GetContentsOfGrid(Team1Positions[i][0], Team1Positions[i][1])->GetName()<<"\n";

        }
        else if (RandomSelection == 1) {
            // cout << "Setting up archer \n";
            BlueUnit = new Unit(("Red Archer" + to_string(i)), GetRandomStat(20, 50), GetRandomStat(3, 8), GetRandomStat(8, 14), 1, GetRandomStat(8, 15),3, "assets/RedArcherUnit.png", "assets/UsedRedArcherUnit.png");
            BlueUnit->SetWeapon(new Weapon("Iron Bow", 6, 10));
            BlueUnit->UpdatePosition(Team2Positions[i][0], Team2Positions[i][1]);
            Grid[Team2Positions[i][0]][Team2Positions[i][1]].SetContents(BlueUnit);
            AddUnitToGrid(Grid[Team2Positions[i][0]][Team2Positions[i][1]].GetContents());
        }
        else if (RandomSelection == 2) {
            BlueUnit = new Unit(("Red Knight" + to_string(i)), GetRandomStat(10, 20), GetRandomStat(5, 10), GetRandomStat(10, 18), 1, GetRandomStat(6, 11), 8, "assets/RedKnightUnit.png", "assets/UsedRedKnightUnit.png");
            BlueUnit->SetWeapon(new Weapon("Iron Spear", 2, 11));
            BlueUnit->UpdatePosition(Team2Positions[i][0], Team2Positions[i][1]);
            Grid[Team2Positions[i][0]][Team2Positions[i][1]].SetContents(BlueUnit);
            AddUnitToGrid(Grid[Team2Positions[i][0]][Team2Positions[i][1]].GetContents());
        }
    }
}
bool Map::CheckForEndOfGame() {
    int NoOfTeams = 0;
    vector<int>Teams;
    for (int i = 0; i < UnitsInGrid.size(); i++) {
        if (!(find(Teams.begin(), Teams.end(), UnitsInGrid[i]->GetTeam())!=Teams.end())) {
            Teams.push_back(UnitsInGrid[i]->GetTeam());
        }
    }
    if (Teams.size() > 1) {
        return false;
    }
    return true;
}
void Map::MoveUnit(vector<int>NewPos,vector<int>OldPos,Unit* UnitToMove) {
    AdjustGridForMove(NewPos,OldPos,UnitToMove);
    AdjustUnits(NewPos, OldPos, UnitToMove);
}
bool Map::CheckIfPosTaken(int X, int Y, vector<vector<int>> Positions) {
    for (int i = 0; i < Positions.size(); i++) {
        if (Positions[i][0] == X && Positions[i][1] == Y) {
            return true;
        }
    }
    return false;
}
vector<vector<int>> Map::GetStartingPositions(int StartY, int EndY,int NoOfUnits) {
    vector<vector<int>> StartingPos;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> PositionsY(StartY, EndY);
    uniform_int_distribution<> PositionsX(0, Width-1);
    for (int i = 0; i < NoOfUnits; i++) {
        int RandomX = PositionsX(gen); int RandomY = PositionsY(gen);
        while (CheckIfPosTaken(RandomX, RandomY, StartingPos)) {
            RandomX = PositionsX(gen); RandomY = PositionsY(gen);
        }
        StartingPos.push_back({ RandomX, RandomY });
        cout << "Pushing: " << RandomX<<"," << RandomY;
    }
    return StartingPos;

}

void Map::AdjustGridForMove(vector<int>NewPos, vector<int>OldPos, Unit* UnitToMove) {
    Grid[NewPos[0]][NewPos[1]].SetContents(UnitToMove);
    Grid[NewPos[0]][NewPos[1]].GetContents()->UpdatePosition(NewPos[0],NewPos[1]);
    Grid[NewPos[0]][NewPos[1]].GetContents()->SetUsed(true);
    Grid[OldPos[0]][OldPos[1]].SetContents(nullptr);
}
void Map::AdjustUnits(vector<int>NewPos, vector<int>OldPos, Unit* UnitToMove) {
    //for (int i = 0; i < UnitsInGrid.size(); i++) {
      //  if (UnitsInGrid[i].GetXPos() == OldPos[0] && UnitsInGrid[i].GetYPos() == OldPos[1]) {
        //    UnitsInGrid[i].SetUsed(true);
          //  UnitsInGrid[i].UpdatePosition(NewPos[0], NewPos[1]);
           // break;
        //}

    //}
    UnitToMove->UpdatePosition(NewPos[0], NewPos[1]);
}
int Map::GetHeight() {
    return Height;
}
int Map::GetWidth() {
    return Width;
}
Unit* Map::GetUnitInPos(int x, int y) {
    for (int i = 0; i < UnitsInGrid.size(); i++) {
        if (UnitsInGrid[i]->GetXPos() == x && UnitsInGrid[i]->GetYPos() == y) {
            return UnitsInGrid[i];
        }
    }
    return nullptr;
}


Unit* Map::GetContentsOfGrid(int X, int Y) {
    return Grid[X][Y].GetContents();
}
Unit* Map::GetIfUnitClicked(int MouseX,int MouseY) {
    int GridStartX = (WINDOW_WIDTH - (GameMap.GetWidth() * SquareSize)) / 2; int GridStartY = (WINDOW_HEIGHT - ((GameMap.GetHeight() * SquareSize))) / 2;
    for (int i = 0; i < UnitsInGrid.size(); i++) {

        if (UnitsInGrid[i]->GetXPos() * SquareSize + GridStartX<MouseX && (UnitsInGrid[i]->GetXPos() + 1) * SquareSize + GridStartX > MouseX ) {
            SDL_Log("Column contains Unit!");
            if (UnitsInGrid[i]->GetYPos()* SquareSize + GridStartY<MouseY && (UnitsInGrid[i]->GetYPos() + 1) * SquareSize + GridStartY > MouseY) {
                SDL_Log("Row containd unit");
           //     cout << "Unit clicked:" << UnitsInGrid[i].GetName();
                return UnitsInGrid[i];
            }
           
        }
    }
    return nullptr;
}
void Map::AddUnitToGrid(Unit* Unit) {
    UnitsInGrid.push_back(Unit);
}
bool Map::CheckForDefeated() {
    bool ItemRemoved = false;
    for(int i=0;i<UnitsInGrid.size();i++){
        if (UnitsInGrid[i]->GetHealth() < 1) {
            Grid[UnitsInGrid[i]->GetXPos()][UnitsInGrid[i]->GetYPos()].SetContents(nullptr);
            UnitsInGrid.erase(UnitsInGrid.begin()+i);
            ItemRemoved = true;
        }
    }
    return ItemRemoved;
}

bool Map::GetIfAllPlayersUnitsUsedThisTurn(int PlayerID) {
    for (int i = 0; i < UnitsInGrid.size(); i++) {
        if (UnitsInGrid[i]->GetTeam() == PlayerID && !UnitsInGrid[i]->GetIfUsedThisTurn()) {
            return false;
        }
    }
    return true;
}
int Map::GetVictor() {
    return UnitsInGrid[0]->GetTeam();
}
void Map::SetAllUnitsToUnactivated() {
    for (int i = 0; i < UnitsInGrid.size(); i++) {
        UnitsInGrid[i]->SetUsed(false);
    }
 //   for (int y = 0; y < Height; y++) {
   //     for (int x = 0; x < Width; x++) {
     //       if (Grid[x][y].GetContents() != NULL) {
       //         Grid[x][y].GetContents()->SetUsed(false);
         //   }
        //}
    //}
}

Unit::Unit(string Name,int Dexterity,int Defence, int Health,int Team,int Swiftness,int Speed, string Path,string UsedPath) {
    this->Name = Name;
    this->Health = Health;
    this->Team = Team;
    this->Speed = Speed;
    this->Dexterity = Dexterity;
    this->Swiftness = Swiftness;
    UsedThisTurn = false;
    this->Defence = Defence;
    SpritePath= string(SDL_GetBasePath()) + Path;
    UsedSpritePath = string(SDL_GetBasePath()) + UsedPath;

}
string Unit::GetUsedSpritePath() {
    return UsedSpritePath;
}
int Unit::GetSwiftness() {
    return Swiftness;
}
int Unit::GetHealth() {
    return Health;
}
int Unit::GetDefence() {
    return Defence;
}
int Unit::GetSpeed() {
    return Speed;
}
void Unit::SetWeapon(Weapon* weapon) {
    EquippedWeapon = weapon;
   // cout << "Weapon for " << Name << " Set too " << weapon->GetName() << "\n";
}
Weapon* Unit::GetWeapon() {
    return EquippedWeapon;
}
void Unit::UpdatePosition(int x, int y) {
    XPos = x; YPos = y;
}
bool Unit::GetIfUsedThisTurn() {
    return UsedThisTurn;
}
void Unit::SetUsed(bool Status) {
    this->UsedThisTurn = Status;
}
string Unit::GetName() {
    return Name;
}
int Unit::GetTeam() {
    return Team;
}
int Unit::GetYPos() {
    return YPos;
}
int Unit::GetXPos() {
    return XPos;
}
vector<vector<int>> Unit::GetCurrentMoves() {
    return CurrentMoves;
}
vector<vector<int>> Unit::GetCurrentAttacks() {
    return CurrentAttacks;
}
void Unit::CalculatePossibleAttacks() {
    int XDifferance, YDifferance;
    cout << "Calculating possible attacks" << "/n range of "+EquippedWeapon->GetName() + ":" << EquippedWeapon->GetRange() << "\n";

    CurrentAttacks.clear();
    for (int y = 0; y < GameMap.GetHeight(); y++) {
        for (int x = 0; x < GameMap.GetWidth(); x++) {
            if (XPos > x) {
                XDifferance = XPos - x;
            }
            else {
                XDifferance = x - XPos;
            }
            if (YPos > y) {
                YDifferance = YPos - y;
            }
            else {
                YDifferance = y - YPos;
            }
            // cout << "\nX diff:" << XDifferance;
          //   cout << ",  Y diff:" << YDifferance;
            if (!((XDifferance + YDifferance) > EquippedWeapon->GetRange())) {
                if (GameMap.GetContentsOfGrid(x, y) != nullptr) {
                    if(GameMap.GetContentsOfGrid(x,y)->GetTeam()!=Team){
                        CurrentAttacks.push_back({ x,y });
                        cout << "Adding:" << x << y << "\n";
                    }                    
                }
            }
        }
    }
}
int Unit::GetDexterity() {
    return Dexterity;
}
void Unit::CalculateCurrentMoves() {
    int XDifferance, YDifferance;
    CurrentMoves.clear();
    for (int y = 0; y < GameMap.GetHeight(); y++) {
        for (int x = 0; x < GameMap.GetWidth(); x++) {
            if (XPos > x) {
                XDifferance =  XPos-x;
            }
            else {
                XDifferance = x-XPos ;
            }
            if (YPos > y) {
                YDifferance =   YPos-y;
            }
            else {
                YDifferance = y-YPos;
            }
           // cout << "\nX diff:" << XDifferance;
         //   cout << ",  Y diff:" << YDifferance;
            if (!((XDifferance + YDifferance) > Speed  )) {
                if (GameMap.GetContentsOfGrid(x, y) == nullptr) {

                    CurrentMoves.push_back({ x,y });
               //     cout << "Adding:" << x << y << "\n";
                }
            }
        }
    }
  //  cout << "Potential moves:" << CurrentMoves.size();

}
string Unit::GetSpritePath() {
    return SpritePath;
}
void Unit::TakeDamage(int Damage) {
    Health -= Damage;
}

Combat::Combat(Unit* Attacker, Unit* Defender) {
    this->Attacker = Attacker;
    this->Defender = Defender;
}
Unit* Combat::GetAttacker() {
    return Attacker;
}
Unit* Combat::GetDefender() {
    return Defender;
}
bool Combat::GetIfHits(int Dex, int Avo) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> Result(0, 100);
    int HitChance = 75 + Dex - Avo;
    if (Result(gen) > HitChance) {
        return false;
    }
    return true;
}
struct SDL_App {
    SDL_Window* window{ nullptr };
    SDL_Renderer* renderer{ nullptr };
    SDL_Event event;
    bool IsRunning = true;
    bool GameStart = false;
};
SDL_App App;
void Combat::DrawSlash(int XPos,int YPos) {
    int GridStartX = (WINDOW_WIDTH - (GameMap.GetWidth() * SquareSize)) / 2; int GridStartY = (WINDOW_HEIGHT - ((GameMap.GetHeight() * SquareSize))) / 2;
    SDL_SetRenderDrawColor(App.renderer, 120, 230, 180, 255);
    SDL_FRect Slash;
    SDL_FRect BackGround, GridSquare;
    BackGround.x = GridStartX; BackGround.y = GridStartY; BackGround.w = GameMap.GetWidth() * SquareSize; BackGround.h = SquareSize * GameMap.GetHeight();
    SDL_RenderFillRect(App.renderer, &BackGround);

    // SDL_Surface* Sprites;

    string Path = std::string(SDL_GetBasePath()) + "assets/BlueSwordUnit.png";

    SDL_Texture* SpriteTexture;
    bool SpriteDrawn = false;
    // SDL_DestroySurface(Sprites);
    bool LastWasSquare = false, LastRowStartedWithSquare = false;
    GridSquare.h = SquareSize; GridSquare.w = SquareSize;
    SDL_SetRenderDrawColor(App.renderer, 223, 255, 0, 255);
    for (int y = 0; y < GameMap.GetHeight(); y++) {
        GridSquare.y = GridStartY + (y * SquareSize);
        LastWasSquare = true;
        if (!LastRowStartedWithSquare) {
            LastWasSquare = false;
        }   
        for (int x = 0; x < GameMap.GetWidth(); x++) {
            if (!LastWasSquare) {
                GridSquare.x = GridStartX + (x * SquareSize);
                SDL_RenderFillRect(App.renderer, &GridSquare);
                LastWasSquare = true;
            }
            else {
                LastWasSquare = false;
            }
            if (GameMap.GetContentsOfGrid(x, y) != NULL) {
                GridSquare.x = GridStartX + (x * SquareSize);
                if (GameMap.GetContentsOfGrid(x, y)->GetIfUsedThisTurn()) {
                    SDL_SetRenderDrawColor(App.renderer, 69, 129, 142, 255);
                }
                else {
                    SpriteTexture = IMG_LoadTexture(App.renderer, (GameMap.GetContentsOfGrid(x, y)->GetSpritePath()).c_str());
                    if (!SpriteTexture) {
                        SDL_Log("Failed to load texture: %s \n", SDL_GetError());
                    }
                    SDL_RenderTexture(App.renderer, SpriteTexture, nullptr, &GridSquare);
                    //  SDL_SetRenderDrawColor(App.renderer, 0, 0, 255, 255);
                    SpriteDrawn = true;
                }

                if (!SpriteDrawn) {
                    SDL_RenderFillRect(App.renderer, &GridSquare);
                }
                SpriteDrawn = false;
                SDL_SetRenderDrawColor(App.renderer, 223, 255, 0, 255);
                
            }
        }
        if (XPos != -1) {
            SDL_SetRenderDrawColor(App.renderer, 0, 0, 10, 150);
            Slash.h = 5; Slash.w = SquareSize - 10; Slash.x = GridStartX + XPos * SquareSize + 5; Slash.y = GridStartY + YPos * SquareSize + (SquareSize / 2);
            SDL_RenderFillRect(App.renderer, &Slash);
            SDL_RenderPresent(App.renderer);
            SDL_SetRenderDrawColor(App.renderer, 223, 255, 0, 255);
        }
        if (LastRowStartedWithSquare) {
            LastRowStartedWithSquare = false;
        }
        else {
            LastRowStartedWithSquare = true;
        }
    }
}
int Combat::GetRange() {
    int XDifference, YDifference;
    if (Defender->GetXPos() > Attacker->GetXPos()) {
        XDifference = Defender->GetXPos() - Attacker->GetXPos();
    }
    else {
        XDifference =Attacker->GetXPos() - Defender->GetXPos();
    }
    if (Defender->GetYPos() > Attacker->GetYPos()) {
        YDifference = Defender->GetYPos() - Attacker->GetYPos();
    }
    else {
        YDifference = Attacker->GetYPos() - Defender->GetYPos();
    }
    int Distance = XDifference + YDifference;
    cout << "Range diff:" << Distance;
    return XDifference + YDifference;
}
string Combat::DoCombat() {
    SDL_SetRenderDrawColor(App.renderer, 0, 0, 10, 150);
    int GridStartX = (WINDOW_WIDTH - (GameMap.GetWidth() * SquareSize)) / 2; int GridStartY = (WINDOW_HEIGHT - ((GameMap.GetHeight() * SquareSize))) / 2;
    SDL_FRect Slash;
    int AttackerSpeed = Attacker->GetSwiftness() + 6;
    string Result="";
    if (GetIfHits(Attacker->GetDexterity(),Defender->GetSwiftness())) {

        int Damage = GetDamage(Attacker->GetWeapon()->GetStrength(), Defender->GetDefence());
        Defender->TakeDamage(Damage);
        DrawSlash(Defender->GetXPos(), Defender->GetYPos());
        if (Defender->GetHealth() >= 1) {
            GameInProgress->AddUpdate(Defender->GetName() + " took " + to_string(Damage) + " damage from an attack from " + Attacker->GetName());
            Result += Defender->GetName() + " took " + to_string(Damage) + " damage from an attack from " + Attacker->GetName() + "\n";
        }
        else {
            GameInProgress->AddUpdate(Defender->GetName() + " took " + to_string(Damage) + " damage from an attack from " + Attacker->GetName()+" and has fallen in battle");
            Result += Defender->GetName() + " took " + to_string(Damage) + " damage from an attack from " + Attacker->GetName() + " and has fallen in battle";
        }
        SDL_Delay(50);
        DrawSlash(-1, -1);
        SDL_Delay(50);
    }
    else {
        GameInProgress->AddUpdate(Defender->GetName() + " dodges attack from " + Attacker->GetName());
        Result += Defender->GetName() + " dodges attack from " + Attacker->GetName()+"\n";
    }
    if (Defender->GetHealth() > 0 ) {
        if (Defender->GetWeapon()->GetRange() >= GetRange()) {
            if (GetIfHits(Defender->GetDexterity(), AttackerSpeed)) {

                int Damage = GetDamage(Defender->GetWeapon()->GetStrength(), Attacker->GetDefence());
                Attacker->TakeDamage(Damage);
                if (Attacker->GetHealth() < 1) {
                    GameInProgress->AddUpdate(Defender->GetName() + " Counters for " + to_string(Damage) + " damage, "+Attacker->GetName()+" has fallen in battle!");
                    Result += Defender->GetName() + " Counters for " + to_string(Damage) + " damage," + Attacker->GetName() + " has fallen in battle! \n";
                }
                else {
                    GameInProgress->AddUpdate(Defender->GetName() + " Counters for " + to_string(Damage) + " damage");
                    Result += Defender->GetName() + " Counters for " + to_string(Damage) + " damage\n";
                }
                DrawSlash(Attacker->GetXPos(), Attacker->GetYPos());
                SDL_Delay(50);
                DrawSlash(-1, -1);
                SDL_Delay(50);
            }
            else {
                GameInProgress->AddUpdate(Attacker->GetName() + " dodges attack from " + Defender->GetName());
                Result += Attacker->GetName() + " dodges attack from " + Defender->GetName() + "\n";
            }
        }
        else {
            GameInProgress->AddUpdate(Defender->GetName() + " cant counter due to being out of range ");
            Result += Defender->GetName() + " cant counter due to being out of range \n";
        }
        
    }
    if (Attacker->GetHealth() > 0 && AttackerSpeed>Defender->GetSwiftness()+5 && Defender->GetHealth()>0) {
        Result += Attacker->GetName() + " gets to attempt a vantage attack \n";
        if (GetIfHits(Attacker->GetDexterity(), Defender->GetSwiftness())) {

            int Damage = GetDamage(Attacker->GetWeapon()->GetStrength(), Defender->GetDefence());
            Defender->TakeDamage(Damage);
            if (Defender->GetHealth() < 1) {
                GameInProgress->AddUpdate(Defender->GetName() + " took " + to_string(Damage)+" from a vantage attack and has fallen in battle");
                Result += Defender->GetName() + " took " + to_string(Damage) + " damage from a advantage attack and has fallen in battle"+ "\n";
            }
            else {
                GameInProgress->AddUpdate(Defender->GetName() + " took " + to_string(Damage) + " damage from a vantage attack " );
                Result += Defender->GetName() + " took " + to_string(Damage) + " damage from an advantage attack from " + Attacker->GetName() ;
            }
            Result += Defender->GetName() + " took " + to_string(Damage) + "\n";
            DrawSlash(Defender->GetXPos(), Defender->GetYPos());
            SDL_Delay(50);
            DrawSlash(-1, -1);
            SDL_Delay(50);
        }
        else {
            GameInProgress->AddUpdate(Defender->GetName() +" has dodged the advantage attack");
            Result += Defender->GetName() + " dodges the attack " + "\n";

        }
        if (Attacker->GetHealth() < 0) {
            Result += Attacker->GetName() + " fell in battle from the counterattack \n ";
        }
        if (Defender->GetHealth() < 0) {
            Result += Defender->GetName() + " fell in battle from the attack\n";

        }
        return Result;
    }
}

int Combat::GetDamage(int Str, int Def) {
    int Damage = Str - Def;
    if (Damage < 1) {
        return 1;
    }
    return Damage;
    
}

struct OptionBoxes {
    int x; int y; int h; int w;
};


TTF_Font* font; TTF_Font* TitleFont;
const int TITLE_WINDOW_HEIGHT = 800;
const int TITLE_WINDOW_WIDTH = 1200;

int TITLEFONT_SIZE=80;
int FONT_SIZE = 30;
vector<OptionBoxes> OptionPos;
vector<string>Updates;

void DrawTitleScreenText() {
    const SDL_Rect* dstrect;
    SDL_Color color = { 255, 255, 255 };;

    string Title="GAME NAME";
    SDL_Surface* textSurface = TTF_RenderText_Solid(TitleFont, Title.c_str(), Title.length(), color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(App.renderer, textSurface);
    SDL_FRect dstRect = { (TITLE_WINDOW_WIDTH-Title.length()*TITLEFONT_SIZE)/2, 100, static_cast<float>(textSurface->w), static_cast<float>(textSurface->h)};
    SDL_RenderTexture(App.renderer, textTexture, nullptr, &dstRect);


    string Text = "Welcome, Press enter to begin";
    textSurface = TTF_RenderText_Solid(font, "WELCOME", Text.length(), color);
    textTexture = SDL_CreateTextureFromSurface(App.renderer, textSurface);
    SDL_FRect dstRect2 = { ((TITLE_WINDOW_WIDTH-Text.length()*FONT_SIZE)/2), (TITLE_WINDOW_HEIGHT/4*3), static_cast<float>(textSurface->w), static_cast<float>(textSurface->h) };
    SDL_RenderTexture(App.renderer, textTexture, nullptr, &dstRect2);
    //   SDL_RenderCopy(App.renderer, textTexture, NULL, dstrect);
}
void RenderBorders(int H, int W) {
    SDL_SetRenderDrawColor(App.renderer, 47, 79, 79, 180);
    SDL_FRect TopRect, BottomRect, LeftRect, RightRect;
    TopRect.x = 0; TopRect.y = 0; TopRect.w = W; TopRect.h = H / 20;
    SDL_RenderFillRect(App.renderer, &TopRect);
    BottomRect.x = 0; BottomRect.y = H - (H / 20); BottomRect.h = H / 20; BottomRect.w = W;
    SDL_RenderFillRect(App.renderer, &BottomRect);
    LeftRect.x = 0; LeftRect.y = 0; LeftRect.h = H; LeftRect.w = H / 20;
    SDL_RenderFillRect(App.renderer, &LeftRect);
    RightRect.x = W - (H / 20); RightRect.y = 0; RightRect.w = H / 20; RightRect.h = H;
    SDL_RenderFillRect(App.renderer, &RightRect);

}
void DrawTitleScreen() {
    SDL_FRect TitleImage;
    TitleImage.x = 0; TitleImage.y = 0; TitleImage.h = TITLE_WINDOW_HEIGHT; TitleImage.w = TITLE_WINDOW_WIDTH;
    SDL_SetRenderDrawColor(App.renderer, 0x00, 0x00, 0x00, 0xFF);
    string TitleScreenPath=  string(SDL_GetBasePath()) + "assets/TitleScreen.png";
    SDL_Texture* SpriteTexture = IMG_LoadTexture(App.renderer, TitleScreenPath.c_str());
    if (!SpriteTexture) {
        SDL_Log("Failed to load texture: %s \n", SDL_GetError());
    }
    SDL_RenderTexture(App.renderer, SpriteTexture, nullptr, &TitleImage);
    //SDL_RenderClear(App.renderer);
    //DrawTitleScreenText();
   // RenderBorders(TITLE_WINDOW_HEIGHT,TITLE_WINDOW_WIDTH);
    SDL_RenderPresent(App.renderer);
}
void DrawGridWithMoveOptions() {
    int GridStartX = (WINDOW_WIDTH - (GameMap.GetWidth() * SquareSize)) / 2; int GridStartY = (WINDOW_HEIGHT - ((GameMap.GetHeight() * SquareSize))) / 2;
    SDL_SetRenderDrawColor(App.renderer, 120, 230, 180, 255);
    SDL_FRect BackGround, GridSquare;
    BackGround.x = GridStartX; BackGround.y = GridStartY; BackGround.w = GameMap.GetWidth() * SquareSize; BackGround.h = SquareSize * GameMap.GetHeight();
    SDL_RenderFillRect(App.renderer, &BackGround);

    SDL_Texture* SpriteTexture;
    vector<vector<int>>CurrentMoves = GameInProgress->GetCurrentlySelected()->GetCurrentMoves();

    bool SpriteDrawn = false;
    bool LastWasSquare = false, LastRowStartedWithSquare = false;
    GridSquare.h = SquareSize; GridSquare.w = SquareSize;
    SDL_SetRenderDrawColor(App.renderer, 223, 255, 0, 255);
    for (int y = 0; y < GameMap.GetHeight(); y++) {
        GridSquare.y = GridStartY + (y * SquareSize);
        LastWasSquare = true;
        if (!LastRowStartedWithSquare) {
            LastWasSquare = false;
        }
        for (int x = 0; x < GameMap.GetWidth(); x++) {
            if (!LastWasSquare) {
                GridSquare.x = GridStartX + (x * SquareSize);
                SDL_RenderFillRect(App.renderer, &GridSquare);
                LastWasSquare = true;
            }
            else {
                LastWasSquare = false;
            }
            for (int i = 0; i < CurrentMoves.size(); i++) {
                if (CurrentMoves[i][0] == x && CurrentMoves[i][1] == y) {
                    GridSquare.x = GridStartX + (x * SquareSize);
                    SDL_SetRenderDrawColor(App.renderer, 123, 104, 238, 255);
                    SDL_RenderFillRect(App.renderer, &GridSquare);
                    SDL_SetRenderDrawColor(App.renderer, 223, 255, 0, 255);

                }
            }
            if (GameMap.GetContentsOfGrid(x, y) != NULL) {
                GridSquare.x = GridStartX + (x * SquareSize);
                if (GameMap.GetContentsOfGrid(x, y)->GetTeam() == 0) {
                    if (GameMap.GetContentsOfGrid(x, y)->GetIfUsedThisTurn()) {
                        SpriteTexture = IMG_LoadTexture(App.renderer, (GameMap.GetContentsOfGrid(x, y)->GetSpritePath()).c_str());
                        if (!SpriteTexture) {
                            SDL_Log("Failed to load texture: %s \n", SDL_GetError());
                        }
                        SDL_RenderTexture(App.renderer, SpriteTexture, nullptr, &GridSquare);
                        SpriteDrawn = true;
                     //   SDL_SetRenderDrawColor(App.renderer, 69, 129, 142, 255);
                    }
                    else {
                        SpriteTexture = IMG_LoadTexture(App.renderer, (GameMap.GetContentsOfGrid(x, y)->GetSpritePath()).c_str());
                        if (!SpriteTexture) {
                            SDL_Log("Failed to load texture: %s \n", SDL_GetError());
                        }
                        SDL_RenderTexture(App.renderer, SpriteTexture, nullptr, &GridSquare);
                        SpriteDrawn = true;
                     //   SDL_SetRenderDrawColor(App.renderer, 0, 0, 255, 255);
                    }
                    
                }
                else {
                    SpriteTexture = IMG_LoadTexture(App.renderer, (GameMap.GetContentsOfGrid(x, y)->GetSpritePath()).c_str());
                    if (!SpriteTexture) {
                        SDL_Log("Failed to load texture: %s \n", SDL_GetError());
                    }
                    SDL_RenderTexture(App.renderer, SpriteTexture, nullptr, &GridSquare);
                    SpriteDrawn = true;
                  //  SDL_SetRenderDrawColor(App.renderer, 255, 0, 0, 255);

                }
               // SDL_RenderFillRect(App.renderer, &GridSquare);
                SDL_SetRenderDrawColor(App.renderer, 223, 255, 0, 255);
                if (!SpriteDrawn) {
                    SDL_RenderFillRect(App.renderer, &GridSquare);
                }
                SpriteDrawn = false;
                SDL_SetRenderDrawColor(App.renderer, 223, 255, 0, 255);

            }
        }
        if (LastRowStartedWithSquare) {
            LastRowStartedWithSquare = false;
        }
        else {
            LastRowStartedWithSquare = true;
        }
    }

}
void DrawGrid() {
    int GridStartX = (WINDOW_WIDTH - (GameMap.GetWidth() * SquareSize)) / 2; int GridStartY = (WINDOW_HEIGHT - ((GameMap.GetHeight() *SquareSize))) / 2;
    SDL_SetRenderDrawColor(App.renderer, 120, 230, 180, 255);
    SDL_FRect BackGround, GridSquare;
    BackGround.x = GridStartX; BackGround.y = GridStartY; BackGround.w = GameMap.GetWidth() * SquareSize; BackGround.h = SquareSize * GameMap.GetHeight();
    SDL_RenderFillRect(App.renderer, &BackGround);

   // SDL_Surface* Sprites;

    string Path = std::string(SDL_GetBasePath()) + "assets/BlueSwordUnit.png";

    SDL_Texture* SpriteTexture;
    bool SpriteDrawn = false;
   // SDL_DestroySurface(Sprites);
    bool LastWasSquare=false, LastRowStartedWithSquare=false;
    GridSquare.h = SquareSize; GridSquare.w = SquareSize;
    SDL_SetRenderDrawColor(App.renderer, 223, 255, 0, 255);
    for (int y = 0; y < GameMap.GetHeight(); y++) {
        GridSquare.y = GridStartY + (y * SquareSize);
        LastWasSquare = true;
        if (!LastRowStartedWithSquare) {
            LastWasSquare = false;
        }
        for (int x = 0; x < GameMap.GetWidth(); x ++){
            if (!LastWasSquare) {
                GridSquare.x = GridStartX + (x * SquareSize);
                SDL_RenderFillRect(App.renderer, &GridSquare);
                LastWasSquare = true;
            }
            else {
                LastWasSquare = false;
            }
            if (GameMap.GetContentsOfGrid(x,y)!=NULL ) {
                GridSquare.x = GridStartX + (x * SquareSize);
                if (GameMap.GetContentsOfGrid(x, y)->GetIfUsedThisTurn()) {
                    SpriteTexture = IMG_LoadTexture(App.renderer, (GameMap.GetContentsOfGrid(x, y)->GetUsedSpritePath()).c_str());
                    if (!SpriteTexture) {
                        SDL_Log("Failed to load texture: %s \n", SDL_GetError());
                    }
                    SDL_RenderTexture(App.renderer, SpriteTexture, nullptr, &GridSquare);
                    SpriteDrawn = true;
                }
                else {
                    SpriteTexture = IMG_LoadTexture(App.renderer, (GameMap.GetContentsOfGrid(x, y)->GetSpritePath()).c_str());
                    if (!SpriteTexture) {
                        SDL_Log("Failed to load texture: %s \n", SDL_GetError());
                    }
                    SDL_RenderTexture(App.renderer, SpriteTexture, nullptr, &GridSquare);
                    SpriteDrawn = true;
                }

                if(!SpriteDrawn){
                    SDL_RenderFillRect(App.renderer, &GridSquare);
                }
                SpriteDrawn = false;
                SDL_SetRenderDrawColor(App.renderer, 223, 255, 0, 255);
            }
        }
        if (LastRowStartedWithSquare) {
            LastRowStartedWithSquare = false;
        }
        else {
            LastRowStartedWithSquare = true;
        }
    }
}
void DrawGridWithAttackOptions() {
    SDL_Texture* SpriteTexture;
    int GridStartX = (WINDOW_WIDTH - (GameMap.GetWidth() * SquareSize)) / 2; int GridStartY = (WINDOW_HEIGHT - ((GameMap.GetHeight() * SquareSize))) / 2;
    SDL_SetRenderDrawColor(App.renderer, 120, 230, 180, 255);
    SDL_FRect BackGround, GridSquare;
    BackGround.x = GridStartX; BackGround.y = GridStartY; BackGround.w = GameMap.GetWidth() * SquareSize; BackGround.h = SquareSize * GameMap.GetHeight();
    SDL_RenderFillRect(App.renderer, &BackGround);

    vector<vector<int>>CurrentAttacks = GameInProgress->GetCurrentlySelected()->GetCurrentAttacks();
    cout << "Number of possible attacls" << CurrentAttacks.size();

    bool Targeted = false,SpriteDrawn=false;
    bool LastWasSquare = false, LastRowStartedWithSquare = false;
    GridSquare.h = SquareSize; GridSquare.w = SquareSize;
    SDL_SetRenderDrawColor(App.renderer, 223, 255, 0, 255);
    for (int y = 0; y < GameMap.GetHeight(); y++) {
        GridSquare.y = GridStartY + (y * SquareSize);
        LastWasSquare = true;
        if (!LastRowStartedWithSquare) {
            LastWasSquare = false;
        }
        for (int x = 0; x < GameMap.GetWidth(); x++) {
            if (!LastWasSquare) {
                GridSquare.x = GridStartX + (x * SquareSize);
                SDL_RenderFillRect(App.renderer, &GridSquare);
                LastWasSquare = true;
            }
            else {
                LastWasSquare = false;
            }
            for (int i = 0; i < CurrentAttacks.size(); i++) {
                if (CurrentAttacks[i][0] == x && CurrentAttacks[i][1] == y) {
                    GridSquare.x = GridStartX + (x * SquareSize);
                    SDL_SetRenderDrawColor(App.renderer, 123, 104, 238, 255);
                    SDL_RenderFillRect(App.renderer, &GridSquare);
                    SDL_SetRenderDrawColor(App.renderer, 223, 255, 0, 255);
                    Targeted = true;
                    // Draw square to show valid attack

                }
            }
            if (GameMap.GetContentsOfGrid(x, y) != NULL) {
                GridSquare.x = GridStartX + (x * SquareSize);
                if (!Targeted) {
                    if (GameMap.GetContentsOfGrid(x, y)->GetTeam() == GameInProgress->GetCurrentPlayer()) {
                        if (GameMap.GetContentsOfGrid(x, y)->GetIfUsedThisTurn()) {
                            // current player, unit used
                            SpriteTexture = IMG_LoadTexture(App.renderer, (GameMap.GetContentsOfGrid(x, y)->GetUsedSpritePath()).c_str());
                            if (!SpriteTexture) {
                                SDL_Log("Failed to load texture: %s \n", SDL_GetError());
                            }
                            SDL_RenderTexture(App.renderer, SpriteTexture, nullptr, &GridSquare);
                            SpriteDrawn = true;
                            SDL_SetRenderDrawColor(App.renderer, 255, 0, 0, 255);

                           // SDL_SetRenderDrawColor(App.renderer, 69, 129, 142, 255);
                        }
                        else {
                            // current player, unit not used
                            SpriteTexture = IMG_LoadTexture(App.renderer, (GameMap.GetContentsOfGrid(x, y)->GetSpritePath()).c_str());
                            if (!SpriteTexture) {
                                SDL_Log("Failed to load texture: %s \n", SDL_GetError());
                            }
                            SDL_RenderTexture(App.renderer, SpriteTexture, nullptr, &GridSquare);
                            SpriteDrawn = true;
                            SDL_SetRenderDrawColor(App.renderer, 255, 0, 0, 255);
                        }

                    }
                    else {
                        SpriteTexture = IMG_LoadTexture(App.renderer, (GameMap.GetContentsOfGrid(x, y)->GetSpritePath()).c_str());
                        if (!SpriteTexture) {
                            SDL_Log("Failed to load texture: %s \n", SDL_GetError());
                        }
                        SDL_RenderTexture(App.renderer, SpriteTexture, nullptr, &GridSquare);
                        SpriteDrawn = true;
                        SDL_SetRenderDrawColor(App.renderer, 255, 0, 0, 255);


                    }
                    if (!SpriteDrawn) {
                        SDL_RenderFillRect(App.renderer, &GridSquare);
                    }
                    SpriteDrawn = false;
                    SDL_SetRenderDrawColor(App.renderer, 223, 255, 0, 255);
                }
                Targeted = false;
            }
        }
        if (LastRowStartedWithSquare) {
            LastRowStartedWithSquare = false;
        }
        else {
            LastRowStartedWithSquare = true;
        }
    }
}
void DrawEnemySelected() {
    int GridStartX = ((WINDOW_WIDTH - (GameMap.GetWidth() * SquareSize)) / 2)+(GameMap.GetWidth()*SquareSize)+10 ;
    SDL_FRect BackGround, TextBox, ImageBox;
    int GridStartY = (WINDOW_HEIGHT - ((GameMap.GetHeight() * SquareSize))) / 2;
    int DisplayHeight = SquareSize * GameMap.GetHeight(); int DisplayWidth = 500;
    SDL_SetRenderDrawColor(App.renderer, 255, 213, 130, 255);
    BackGround.x = GridStartX;; BackGround.y = GridStartY; BackGround.w = DisplayWidth; BackGround.h = DisplayHeight;
    SDL_RenderFillRect(App.renderer, &BackGround);

    if (GameInProgress->GetCurrentlySelectedEnemy() != nullptr) {

        SDL_SetRenderDrawColor(App.renderer, 120, 200, 255, 255);
        ImageBox.x = GridStartX + 50; ImageBox.y = GridStartY + 50; ImageBox.w = DisplayWidth - 100; ImageBox.h = DisplayWidth - 100;
        SDL_RenderFillRect(App.renderer, &ImageBox);

        SDL_SetRenderDrawColor(App.renderer, 130, 230, 255, 255);
        TextBox.y = GridStartY + 100 + (DisplayWidth - 100); TextBox.x = GridStartX + 50; TextBox.w = DisplayWidth - 100; TextBox.h = DisplayHeight - (150 + (DisplayWidth - 100));
        SDL_RenderFillRect(App.renderer, &TextBox);
        vector<string>InfoLines;
        InfoLines.push_back(GameInProgress->GetCurrentlySelectedEnemy()->GetName());
        InfoLines.push_back("HP:" + to_string(GameInProgress->GetCurrentlySelectedEnemy()->GetHealth()));
        InfoLines.push_back("Def:" + to_string(GameInProgress->GetCurrentlySelectedEnemy()->GetDefence()));
        InfoLines.push_back("Spd:" + to_string(GameInProgress->GetCurrentlySelectedEnemy()->GetSwiftness()));
        InfoLines.push_back("Dex:" + to_string(GameInProgress->GetCurrentlySelectedEnemy()->GetDexterity()));
        InfoLines.push_back("Weapon:" + GameInProgress->GetCurrentlySelectedEnemy()->GetWeapon()->GetName());
        InfoLines.push_back("Str:" + to_string(GameInProgress->GetCurrentlySelectedEnemy()->GetWeapon()->GetStrength()));
        SDL_Color TextColour = { 255,255,255 };
        for (int i = 0; i < InfoLines.size(); i++) {
            SDL_Surface* InfoSurface = TTF_RenderText_Blended(font, InfoLines[i].c_str(), InfoLines[i].size(), TextColour);
            SDL_Texture* InfoTexture = SDL_CreateTextureFromSurface(App.renderer, InfoSurface);
            SDL_FRect InfoRect;
            InfoRect.x = GridStartX + 55; InfoRect.y = GridStartY + 105 + (DisplayWidth - 100) + i * InfoSurface->h; InfoRect.h = InfoSurface->h; InfoRect.w = InfoSurface->w;
            SDL_RenderTexture(App.renderer, InfoTexture, nullptr, &InfoRect);
        }
    }
}
void DrawUpdateBox() {
    int GridStartX = ((WINDOW_WIDTH - (GameMap.GetWidth() * SquareSize)) / 2)-200; int GridStartY = ((WINDOW_HEIGHT - ((GameMap.GetHeight() * SquareSize))) / 2 + (SquareSize * GameMap.GetHeight())) + 5;
    SDL_SetRenderDrawColor(App.renderer, 120, 230, 180, 255);
    SDL_FRect BackGround, GridSquare;
    BackGround.x = GridStartX; BackGround.y = GridStartY; BackGround.w = GameMap.GetWidth() * SquareSize+400; BackGround.h = 155;
    SDL_RenderFillRect(App.renderer, &BackGround);
}
void DrawCurrentlySelected() {
    SDL_Texture* SpriteTexture;
    SDL_FRect BackGround,TextBox, ImageBox;
    int GridStartY = (WINDOW_HEIGHT - ((GameMap.GetHeight() * SquareSize))) / 2;
    int StartX = 100; int DisplayHeight = SquareSize * GameMap.GetHeight(); int DisplayWidth = 500;
    SDL_SetRenderDrawColor(App.renderer, 144, 213, 255, 255);
    BackGround.x = StartX;; BackGround.y = GridStartY; BackGround.w = DisplayWidth; BackGround.h = DisplayHeight;
    SDL_RenderFillRect(App.renderer, &BackGround);
    if (GameInProgress->GetCurrentlySelected() != nullptr) {
        //SpriteTexture = IMG_LoadTexture(App.renderer, (GameInProgress->GetCurrentlySelected()->GetSpritePath()).c_str());
       // if (!SpriteTexture) {
      //      SDL_Log("Failed to load texture: %s \n", SDL_GetError());
    //    }  

        SDL_SetRenderDrawColor(App.renderer, 120, 200, 255, 255);
        ImageBox.x = StartX + 50; ImageBox.y = GridStartY + 50; ImageBox.w = DisplayWidth - 100; ImageBox.h = DisplayWidth - 100;
        SDL_RenderFillRect(App.renderer, &ImageBox);
     //   SDL_RenderTexture(App.renderer, SpriteTexture, nullptr, &ImageBox);

        SDL_SetRenderDrawColor(App.renderer, 130, 230, 255, 255);
        TextBox.y = GridStartY + 100 + (DisplayWidth - 100); TextBox.x = StartX + 50; TextBox.w = DisplayWidth - 100; TextBox.h = DisplayHeight - (150 + (DisplayWidth - 100));
        SDL_RenderFillRect(App.renderer, &TextBox);
        vector<string>InfoLines;
        InfoLines.push_back(GameInProgress->GetCurrentlySelected()->GetName());
        InfoLines.push_back("HP:" + to_string(GameInProgress->GetCurrentlySelected()->GetHealth()));
        InfoLines.push_back("Def:" + to_string(GameInProgress->GetCurrentlySelected()->GetDefence()));
        InfoLines.push_back("Spd:" + to_string(GameInProgress->GetCurrentlySelected()->GetSwiftness()));
        InfoLines.push_back("Dex:" + to_string(GameInProgress->GetCurrentlySelected()->GetDexterity()));
        InfoLines.push_back("Weapon:" + GameInProgress->GetCurrentlySelected()->GetWeapon()->GetName());
        InfoLines.push_back("Str:" + to_string(GameInProgress->GetCurrentlySelected()->GetWeapon()->GetStrength()));
        SDL_Color TextColour = { 255,255,255 };
        for (int i = 0; i < InfoLines.size(); i++) {
            SDL_Surface* InfoSurface = TTF_RenderText_Blended(font, InfoLines[i].c_str(), InfoLines[i].size(), TextColour);
            SDL_Texture* InfoTexture = SDL_CreateTextureFromSurface(App.renderer, InfoSurface);
            SDL_FRect InfoRect;
            InfoRect.x = StartX + 55; InfoRect.y = GridStartY + 105 + (DisplayWidth - 100)+i*InfoSurface->h; InfoRect.h =InfoSurface->h; InfoRect.w = InfoSurface->w;
            SDL_RenderTexture(App.renderer, InfoTexture, nullptr, &InfoRect);
        }
    }      
}
void RenderText() {
    string StatusText = "Turn " + to_string(GameInProgress->GetTurn()) + "     player " + to_string(GameInProgress->GetCurrentPlayer()) + "'s activation";
    SDL_Color TextColour = { 255,255,255 };
    SDL_Surface* StatusSurface = TTF_RenderText_Blended(font, StatusText.c_str(), StatusText.size(), TextColour);
    SDL_Texture* StatusTexture = SDL_CreateTextureFromSurface(App.renderer,StatusSurface);
    SDL_FRect StatusRect;
    StatusRect.x = WINDOW_WIDTH/2-(StatusTexture->w/2); StatusRect.y = 100 ; StatusRect.h = StatusSurface->h; StatusRect.w = StatusSurface->w;
    SDL_RenderTexture(App.renderer, StatusTexture, nullptr, &StatusRect);
}
void RenderUpdatesTexts() {
    vector<string>Updates = GameInProgress->GetUpdates();
    int GridStartX = (WINDOW_WIDTH - (GameMap.GetWidth() * SquareSize)) / 2; int GridStartY = ((WINDOW_HEIGHT - ((GameMap.GetHeight() * SquareSize))) / 2 + (SquareSize * GameMap.GetHeight())) + 10;
    SDL_Color TextColour = { 255,255,255 };
    for (int i = 0; i < Updates.size(); i++) {
        SDL_Surface* InfoSurface = TTF_RenderText_Blended(font, Updates[i].c_str(), Updates[i].size(), TextColour);
        SDL_Texture* InfoTexture = SDL_CreateTextureFromSurface(App.renderer, InfoSurface);
        SDL_FRect InfoRect;
        InfoRect.x = GridStartX; InfoRect.y = GridStartY+(i*InfoSurface->h); InfoRect.h = InfoSurface->h; InfoRect.w = InfoSurface->w;
        SDL_RenderTexture(App.renderer, InfoTexture, nullptr, &InfoRect);
    }
}
void DrawGameScreenTemp() {
    SDL_SetRenderDrawColor(App.renderer, 119, 136, 153, 180);
    SDL_RenderClear(App.renderer);
    DrawCurrentlySelected();
    DrawEnemySelected();
    DrawUpdateBox();
    RenderUpdatesTexts();
    if (GameInProgress->GetCurrentlySelected() == nullptr) {
        DrawGrid();
    }
    else {
        if (MoveDone) {
           //draw possible attacks
            DrawGridWithAttackOptions();
        }
        else {
            //draw potential moves
            DrawGridWithMoveOptions();
        }
       
    }
    RenderText();
    RenderBorders(WINDOW_HEIGHT, WINDOW_WIDTH);
    SDL_RenderPresent(App.renderer);


}
void OpenTitleScreen() {
    SDL_SetRenderDrawColor(App.renderer, 0, 0, 0, 255);
    SDL_RenderClear(App.renderer);

    //SDL_SetRenderDrawColor(App.renderer, 255, 255, 255, 255);  // white for text
   // SDL_RenderDebugText(App.renderer, 10.0f, 10.0f, "Hello, SDL3!");
    //SDL_RenderPresent(App.renderer);

}
vector<int> CheckIfMoveOptionClicked(int MouseX,int MouseY) {
    vector<vector<int>>CurrentMoves = GameInProgress->GetCurrentlySelected()->GetCurrentMoves();
    int GridStartX = (WINDOW_WIDTH - (GameMap.GetWidth() * SquareSize)) / 2; int GridStartY = (WINDOW_HEIGHT - ((GameMap.GetHeight() * SquareSize))) / 2;
    for (int i = 0; i < CurrentMoves.size(); i++) {
        if (GridStartY + CurrentMoves[i][1] * SquareSize<MouseY && GridStartY + (1 + CurrentMoves[i][1]) * SquareSize > MouseY) {
            if (GridStartX + CurrentMoves[i][0] * SquareSize<MouseX && GridStartX + (1 + CurrentMoves[i][0]) * SquareSize > MouseX) {
           //     cout << CurrentMoves[i][0] << " " << CurrentMoves[i][1] << " Clicked\n";
                return CurrentMoves[i];

            }
        }

    }
    return { -1,-1 };
}
bool CreateApp() {
   // if ((App.window = SDL_CreateWindow("TitleScreen", TITLE_WINDOW_WIDTH, TITLE_WINDOW_HEIGHT, 0)) == nullptr) {
     //   SDL_Quit();
       // return false;
    //}
    if ((App.window = SDL_CreateWindow("TitleScreen", TITLE_WINDOW_WIDTH, TITLE_WINDOW_HEIGHT, 0)) == nullptr) {
        SDL_Quit();
        return false;
    }
    if ((App.renderer = SDL_CreateRenderer(App.window, nullptr)) == nullptr) {
    }

    return true;
}

bool CreateGame() {
   // if ((App.window = SDL_CreateWindow("GameScreen",WINDOW_WIDTH, WINDOW_HEIGHT, 0)) == nullptr) {
     //   SDL_Quit();
       // return false;
    //}
    if ((App.window = SDL_CreateWindow("Grid", WINDOW_WIDTH, WINDOW_HEIGHT, 0)) == nullptr) {
        SDL_Quit();
        return false;
    }
    if ((App.renderer = SDL_CreateRenderer(App.window, nullptr)) == nullptr) {
    }
    

    return true;
}
void DrawOptions() {
    int WidthAfterBorders = TITLE_WINDOW_WIDTH - (TITLE_WINDOW_HEIGHT / 20 * 2);
    SDL_FRect OptionSquare;
    int NoOfOptions = 3;
    int Vertical_Gap = 150;
    int Gap = 30;
    int EdgeGap = Gap/2 + (TITLE_WINDOW_HEIGHT / 20);
    OptionBoxes Temp;

    SDL_SetRenderDrawColor(App.renderer, 255, 0, 0, 255);
    int OptionWidth = (TITLE_WINDOW_WIDTH / NoOfOptions) - ((EdgeGap / NoOfOptions*2) + Gap / (NoOfOptions));
    OptionSquare.x = EdgeGap; OptionSquare.y =Vertical_Gap; OptionSquare.w = OptionWidth; OptionSquare.h =TITLE_WINDOW_HEIGHT-(Vertical_Gap*2);
    Temp.x = OptionSquare.x; Temp.y = OptionSquare.y; Temp.w = OptionSquare.w; Temp.h = OptionSquare.h;
  //  OptionPos.push_back(Temp);
    SDL_Texture* SpriteTexture;
    OptionSquare.x = EdgeGap + (OptionWidth * 1) + (Gap * 1);
    OptionSquare.y = Vertical_Gap;
    OptionSquare.w = OptionWidth;
    OptionSquare.h = TITLE_WINDOW_HEIGHT - (Vertical_Gap * 2);
    string BattlePath = string(SDL_GetBasePath()) + "assets/Battle.png";
    string WarPath = string(SDL_GetBasePath()) + "assets/War.png";
    string skirmishPath = string(SDL_GetBasePath()) + "assets/skirmish.png";

    string StatusText = "Select game size";
    SDL_Color TextColour = { 255,255,255 };
    SDL_Surface* StatusSurface = TTF_RenderText_Blended(font, StatusText.c_str(), StatusText.size(), TextColour);
    SDL_Texture* StatusTexture = SDL_CreateTextureFromSurface(App.renderer, StatusSurface);
    SDL_FRect StatusRect;
    StatusRect.x = 480; StatusRect.y = 100; StatusRect.h = StatusSurface->h; StatusRect.w = StatusSurface->w;
    SDL_RenderTexture(App.renderer, StatusTexture, nullptr, &StatusRect);


    SDL_RenderFillRect(App.renderer, &OptionSquare);
    for (int i = 0; i < NoOfOptions; i++) {
        SDL_SetRenderDrawColor(App.renderer, 0, 255, 0, 255);
        OptionSquare.x = EdgeGap+(OptionWidth*i)+(Gap*i);
        OptionSquare.y = Vertical_Gap; 
        OptionSquare.w =OptionWidth; 
        OptionSquare.h = TITLE_WINDOW_HEIGHT - (Vertical_Gap * 2);
        SDL_RenderFillRect(App.renderer, &OptionSquare);
        if (i == 0) {
            SpriteTexture = IMG_LoadTexture(App.renderer, skirmishPath.c_str());
        }
        else if (i == 1) {
            SpriteTexture = IMG_LoadTexture(App.renderer, BattlePath.c_str());
        }
        else {
            SpriteTexture = IMG_LoadTexture(App.renderer, WarPath.c_str());
        }
        if (!SpriteTexture) {
            SDL_Log("Failed to load texture: %s \n", SDL_GetError());
        }
        SDL_RenderTexture(App.renderer, SpriteTexture, nullptr, &OptionSquare);
        Temp.x = OptionSquare.x; Temp.y = OptionSquare.y; Temp.w = OptionSquare.w; Temp.h = OptionSquare.h;
  //      OptionPos.push_back(Temp);
    }
}
void SetOptionsPos() {
    int WidthAfterBorders = TITLE_WINDOW_WIDTH - (TITLE_WINDOW_HEIGHT / 20 * 2);
    SDL_FRect OptionSquare;
    int NoOfOptions = 3;
    int Vertical_Gap = 150;
    int Gap = 30;
    int EdgeGap = Gap / 2 + (TITLE_WINDOW_HEIGHT / 20);
    OptionBoxes Temp;

    int OptionWidth = (TITLE_WINDOW_WIDTH / NoOfOptions) - ((EdgeGap / NoOfOptions * 2) + Gap / (NoOfOptions));
    Temp.x = EdgeGap; Temp.y = Vertical_Gap; Temp.w = OptionWidth; Temp.h = TITLE_WINDOW_HEIGHT - (Vertical_Gap * 2);
    OptionPos.push_back(Temp);

    for (int i = 1; i < NoOfOptions; i++) {
        Temp.x = EdgeGap + (OptionWidth * i) + (Gap * i);
        Temp.y = Vertical_Gap;
        Temp.w = OptionWidth;
        Temp.h = TITLE_WINDOW_HEIGHT - (Vertical_Gap * 2);
        OptionPos.push_back(Temp);
    }

}
void DrawSelectScreen() {
    SDL_SetRenderDrawColor(App.renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(App.renderer);
    DrawOptions();
    RenderBorders(TITLE_WINDOW_HEIGHT, TITLE_WINDOW_WIDTH);
    SDL_RenderPresent(App.renderer);

}
int UnitsInPlay = 10;
void CheckIfOptionsClicked(int X,int Y) {
    for (int i = 0; i < OptionPos.size(); i++) {
        if (X > OptionPos[i].x && X < OptionPos[i].x + OptionPos[i].w && Y> OptionPos[i].y && Y < OptionPos[i].y + OptionPos[i].h) {
        //    SDL_Log("Option %d clicked", i);
            if (i == 0) {
                SDL_Log("Starting game");
                SDL_Log("Skirmish selected, 5 units a side");
                App.IsRunning = false;
                UnitsInPlay = 5;
            }
            else if (i == 2) {
                SDL_Log("War selected, 15 units a side");
                App.IsRunning = false;
                UnitsInPlay = 15;
            }
            else {
                SDL_Log("Battle selected, 10 units a side");
                App.IsRunning = false;
                UnitsInPlay = 10;
            }
        }
    }

}

vector<int> GetPosClicked(int MouseX,int MouseY) {
    vector<int> Pos;

    int GridStartX = (WINDOW_WIDTH - (GameMap.GetWidth() * SquareSize)) / 2; int GridStartY = (WINDOW_HEIGHT - ((GameMap.GetHeight() * SquareSize))) / 2;
    int Current = 0; int Next = 0;
    for (int i = 0; i < GameMap.GetWidth(); i++) {
        Current = GridStartX + i * SquareSize; Next = GridStartX + ((i + 1) * SquareSize);
        if (MouseX >= Current && MouseX < Next) {
            Pos.push_back(i);
            break;
        }
    }
    for (int i = 0; i < GameMap.GetHeight(); i++) {
        Current = GridStartY + i * SquareSize; Next = GridStartY + ((i + 1) * SquareSize);
        if (MouseY >= Current && MouseY < Next) {
            Pos.push_back(i);
            break;
        }
    }

    return Pos;

}
bool Playing = true;
void  PlayGame() {
    float MouseX = 0; float MouseY = 0;
   // CreateGame();
    GameMap = Map(24, 24, UnitsInPlay, 4);
    MoveDone = false;
    GameInProgress = new Game("Player1", "Player2");
    GameInProgress->SetCurrentlySelected(nullptr);
    Unit Temp = Unit("", 0, 0, 0, 0, 0, 0, "", "");
    if (App.GameStart) {
        App.IsRunning = true;
        while (App.IsRunning)
        {
            if (GameMap.GetIfAllPlayersUnitsUsedThisTurn(GameInProgress->GetCurrentPlayer())) {
                GameInProgress->SwapPlayers();
            }
            GameInProgress->IncrementFrameCount();
            while (SDL_PollEvent(&App.event)) {
                switch (App.event.type) {
                case SDL_EVENT_QUIT:
                    App.IsRunning = false;
                    Playing = false;
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    MouseX = App.event.button.x; MouseY = App.event.button.y;
                    SDL_Log("Mouse clicked at %f %f", MouseX, MouseY);
                    cout << "Move done: " << MoveDone << " \n";
                    cout << "Currently selected: " << GameInProgress->GetCurrentlySelected() << " \n";


                    if (GameInProgress->GetCurrentlySelected() != nullptr) {
                        vector<int> Move = CheckIfMoveOptionClicked(MouseX, MouseY);
                        if (MoveDone) {
                            //check for selection of attack
                            vector<int> PosClicked = GetPosClicked(MouseX, MouseY);
                            vector<vector<int>> Attacks = GameInProgress->GetCurrentlySelected()->GetCurrentAttacks();
                            for (int i = 0; i < Attacks.size(); i++) {
                                cout << " Attacks: " << Attacks[i].size();
                                cout << "  \nPosClicked: " << PosClicked.size() << " \n";
                                if (PosClicked[0] == Attacks[i][0] && PosClicked[1] == Attacks[i][1]) {
                                    cout << "Enemy clicked" << "\n ";
                                    cout << "Currently selected before combat : " << GameInProgress->GetCurrentlySelected()->GetHealth();
                                    Unit* Enemy = GameMap.GetUnitInPos(PosClicked[0], PosClicked[1]);
                                    cout << "Combat: " << GameInProgress->GetCurrentlySelected()->GetName() << " VS " << Enemy->GetName() << "\n";
                                    Combat CurrentCombat(GameInProgress->GetCurrentlySelected(), Enemy);
                                    string Combatresult = CurrentCombat.DoCombat();
                                    cout << Combatresult << "\n";

                                    cout << "Currently selected after combat : " << GameInProgress->GetCurrentlySelected()->GetHealth();

                                    GameInProgress->SetCurrentlySelected(nullptr);
                                    UpdateNeeded = true;
                                    MoveDone = false;
                                    GameMap.CheckForDefeated();
                                    bool Victory = GameMap.CheckForEndOfGame();
                                    if (Victory) {
                                        App.IsRunning = false;
                                    }

                                }
                            }

                        }
                        else {
                            if (Move[0] != -1) {
                                GameInProgress->GetCurrentlySelected()->UpdatePosition(Move[0], Move[1]);
                                GameInProgress->GetCurrentlySelected()->SetUsed(true);
                                GameMap.MoveUnit(Move, GameInProgress->GetCurrentlySelectedPos(), GameInProgress->GetCurrentlySelected());


                                //cout<<"Weapon" <<GameInProgress->GetCurrentlySelected()->GetWeapon()<<"\n";
                                GameInProgress->GetCurrentlySelected()->CalculatePossibleAttacks();
                                if (!GameInProgress->GetCurrentlySelected()->GetCurrentAttacks().size() == 0) {
                                    UpdateNeeded = true;
                                    MoveDone = true;
                                }
                                else {
                                    GameInProgress->SetCurrentlySelected(nullptr);
                                    UpdateNeeded = true;
                                    MoveDone = false;
                                }
                                GameInProgress->SetCurrentlySelectedEnemy(nullptr);
                                GameInProgress->SwapPlayers();
                                if (GameMap.GetIfAllPlayersUnitsUsedThisTurn(GameInProgress->GetCurrentPlayer())) {
                                    GameInProgress->SwapPlayers();
                                    if (GameMap.GetIfAllPlayersUnitsUsedThisTurn(GameInProgress->GetCurrentPlayer())) {
                                        GameInProgress->SwapPlayers();
                                        GameInProgress->UpdateTurn();
                                        GameMap.SetAllUnitsToUnactivated();
                                    }
                                }
                            }
                        }
                    }
                    else {
                        Unit* Temp = GameMap.GetIfUnitClicked(MouseX, MouseY);
                        cout << "Else statement \n";
                        cout << " Temp: " << Temp << " \n";
                        cout << "Current team:" << GameInProgress->GetCurrentPlayer() << "\n";
                        if (Temp != nullptr) {
                            SDL_Log("Clicked unit!");
                            cout << " Unit team: " << Temp->GetTeam() << " \n";
                            cout << " Unit used: " << Temp->GetIfUsedThisTurn() << " \n";
                            if (Temp->GetTeam() == GameInProgress->GetCurrentPlayer() && !Temp->GetIfUsedThisTurn() && !MoveDone) {
                                SDL_Log("Player Clicked their Unit");
                                Temp->CalculateCurrentMoves();
                                GameInProgress->SetCurrentlySelected(Temp);
                                UpdateNeeded = true;
                            }
                            else if ((Temp->GetTeam() != GameInProgress->GetCurrentPlayer())) {
                                SDL_Log("Enemy Unit clicked");
                                GameInProgress->SetCurrentlySelectedEnemy(Temp);
                                UpdateNeeded = true;
                            }
                        }
                    }
                    break;
                case SDL_EVENT_KEY_DOWN:
                    switch (App.event.key.key)
                    {
                    case SDLK_ESCAPE:
                        App.IsRunning = false;
                        break;
                    default:
                        break;
                    }
                }
            }
            SDL_RenderClear(App.renderer);
            if (UpdateNeeded) {
                DrawGameScreenTemp();
                SDL_RenderPresent(App.renderer);
                UpdateNeeded = false;
            }
            SDL_Delay(10);
        }
    }

}
void CreateEndScreen() {
    if ((App.window = SDL_CreateWindow("Results", TITLE_WINDOW_WIDTH, TITLE_WINDOW_HEIGHT, 0)) == nullptr) {
        SDL_Quit();
    }
    if ((App.renderer = SDL_CreateRenderer(App.window, nullptr)) == nullptr) {
    }
}
bool DoEndScreen() {
    CreateEndScreen();
    string VictoryPath = string(SDL_GetBasePath()) + "assets/BlueVictoryScreen.png";
    if (GameMap.GetVictor() == 0) {
        VictoryPath = string(SDL_GetBasePath()) + "assets/BlueVictoryScreen.png";
    }
    else {
        VictoryPath = string(SDL_GetBasePath()) + "assets/RedVictoryScreen.png";
    }
    SDL_RenderClear(App.renderer);
    
    SDL_FRect EndImage;
    EndImage.x = 0; EndImage.y = 0; EndImage.h = TITLE_WINDOW_HEIGHT; EndImage.w = TITLE_WINDOW_WIDTH;
    SDL_SetRenderDrawColor(App.renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_Texture* SpriteTexture = IMG_LoadTexture(App.renderer, VictoryPath.c_str());
    if (!SpriteTexture) {
        SDL_Log("Failed to load texture: %s \n", SDL_GetError());
    }
    SDL_RenderTexture(App.renderer, SpriteTexture, nullptr, &EndImage);
    bool PlayAgain = false;
    bool EndScreenActive = true;
    SDL_RenderPresent(App.renderer);
    while (EndScreenActive) {
        while (SDL_PollEvent(&App.event)) {
            if (App.event.type == SDL_EVENT_QUIT) {
                App.IsRunning = false;
            }
            switch (App.event.type) {
            case SDL_EVENT_QUIT:
                App.IsRunning = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                switch (App.event.key.key)
                {
                case SDLK_ESCAPE:
                    EndScreenActive = false;
                    break;
                case SDLK_RETURN:
                    PlayAgain = true;
                    EndScreenActive = false;
                    break;
                default:
                    break;
                }
            }
        }

    }
    return PlayAgain;
    

}
void DisplayOptionsScreen() {
    float MouseX = 0; float MouseY = 0;
    bool Drawn = false;
    //SetOptionsPos();
    if (App.GameStart) {
        App.IsRunning = true;
        while (App.IsRunning)
        {
            SDL_RenderClear(App.renderer);
            if (!Drawn) {
                DrawSelectScreen();
                SDL_RenderPresent(App.renderer);
                Drawn = true;
            }
            SDL_Delay(10);
            while (SDL_PollEvent(&App.event)) {
                switch (App.event.type) {
                case SDL_EVENT_QUIT:
                    App.IsRunning = false;
                    Playing = false;
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    MouseX = App.event.button.x; MouseY = App.event.button.y;
                    CheckIfOptionsClicked(MouseX, MouseY);
                    break;
                case SDL_EVENT_KEY_DOWN:
                    switch (App.event.key.key)
                    {
                    case SDLK_ESCAPE:
                        App.IsRunning = false;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }

}
void DoTitleScreen() {
    bool Drawn = false;
    bool Close = false;
    SDL_Color color = { 255, 255, 255 };
    while (App.IsRunning)
    {
        while (SDL_PollEvent(&App.event)) {
            switch (App.event.type) {
            case SDL_EVENT_QUIT:
                App.IsRunning = false;
                Playing = false;
                Close = true;
                break;
            case SDL_EVENT_KEY_DOWN:
                switch (App.event.key.key)
                {
                case SDLK_ESCAPE:
                    App.IsRunning = false;
                    break;
                case SDLK_RETURN:
                    App.GameStart = true;
                    App.IsRunning = false;
                    break;
                default:
                    break;
                }
            }
        }
        if (!Drawn) {
            DrawTitleScreen();
            SDL_RenderPresent(App.renderer);
            Drawn = true;
        }
        SDL_Delay(10);

    }
}
bool CreateFonts() {
    font = TTF_OpenFont("arial.ttf", FONT_SIZE);
    if (!font) {
        SDL_Log("Failed to load font: %s", SDL_GetError());
        return 1;
    }
    TitleFont = TTF_OpenFont("arial.ttf", TITLEFONT_SIZE);
    if (!font) {
        SDL_Log("Failed to load font: %s", SDL_GetError());
        return 1;
    }
}
int main()
{  
    if (SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0) {
        return -1;
    }
    if (!CreateApp()) {
        SDL_Log("Game could not run");
        return 2;
    }
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    CreateFonts();   
   
    bool Close = false;
    bool Drawn = false;

    SetOptionsPos();
    
    DoTitleScreen();
    SDL_RenderClear(App.renderer);

    while (Playing ) {
        DisplayOptionsScreen();
        SDL_DestroyWindow(App.window);
        if (Playing) {
            UpdateNeeded = true;
            CreateGame();
            PlayGame();
        }
        SDL_RenderClear(App.renderer);
        SDL_DestroyWindow(App.window);
        if (Playing) {
            Playing = DoEndScreen();
            SDL_DestroyWindow(App.window);
        }
        if (Playing) {
            CreateApp();
        }
    }
    cout << " Game over\n";
    cout << "Team " << GameMap.GetVictor()<< "won! \n";
    std::cout << "Game Shut down!\n";
}