// CW1-3016.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;
#include <string>
#include <vector>
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
Sword::Sword(string Name, int Range, int Strength):Weapon(Name,Range,Strength) {
    cout << "Making sword\n";
}


vector<vector<int>> SquaresInRange;
vector<Weapon> Weapons;
Map GameMap(24, 24);
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

}
int SquareSize = 40;
int Game::GetCurrentPlayer() {
    return CurrentPlayer;
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
Map::Map(int x,int y){
    CreateWeapons();
    vector<Square>Temp;
    for (int Y = 0; Y < y; Y++) {
        Temp.clear();
        for (int X = 0; X< x; X++) {
            Temp.emplace_back();
        }
        Grid.push_back(Temp);
    };
    Width = x;
    Height = y;

    cout << "Weapons[0] at map creation: " + Weapons[0].GetName()<<"\n";
    Unit* U1 =new Unit("Name", 10, 1, 2, "assets/RedSwordUnit.png");
    U1->UpdatePosition(3, 3); U1->SetWeapon(new Weapon("Iron Sword",1,5));
    Grid[3][3].SetContents(U1);
    cout<<"Equipped weapons:" << Grid[3][3].GetContents()->GetWeapon()->GetName()<<"\n";
   // Grid[3][3].SetContents(new Unit("Name", 10,1,2, "assets/RedSwordUnit.png"));
   // Grid[3][3].GetContents()->UpdatePosition(3, 3);
   // Grid[3][3].GetContents()->SetWeapon(&Weapons[0]);
    Grid[0][1].SetContents(new Unit("Name4", 10, 1,2, "assets/RedSwordUnit.png"));
    Grid[0][1].GetContents()->UpdatePosition(0, 1);
    Grid[0][1].GetContents()->SetWeapon(&Weapons[0]);
    Grid[10][10].SetContents(new Unit("Name2", 10, 0,2, "assets/BlueSwordUnit.png"));
    Grid[10][10].GetContents()->UpdatePosition(10, 10);
    Grid[10][10].GetContents()->SetWeapon(&Weapons[0]);
    Grid[5][10].SetContents(new Unit("Name3", 10, 0,2, "assets/BlueSwordUnit.png"));
    Grid[5][10].GetContents()->UpdatePosition(5, 10);
    Grid[5][10].GetContents()->SetWeapon(&Weapons[0]);
    Grid[10][2].SetContents(new Unit("Name5", 10, 0, 3, "assets/BlueSwordUnit.png"));
    Grid[10][2].GetContents()->UpdatePosition(10, 2);
    Grid[10][2].GetContents()->SetWeapon(&Weapons[0]);
    AddUnitToGrid(Grid[0][1].GetContents()); AddUnitToGrid(Grid[10][2].GetContents());
    AddUnitToGrid(Grid[3][3].GetContents()); AddUnitToGrid(Grid[10][10].GetContents()); AddUnitToGrid(Grid[5][10].GetContents());
//    cout << Grid[3][3].GetContents();
    if (Grid[3][3].GetContents() != nullptr) {
      //  SDL_Log("Grid set correctly\n");
       // cout << "XPOS:" << Grid[3][3].GetContents()->GetXPos() << "\nYPOS:" << Grid[3][3].GetContents()->GetYPos();
    }
    if (Grid[10][10].GetContents() != nullptr) {
   //     SDL_Log("\nGrid set correctly2\n");
     //   cout << "XPOS:" << Grid[10][10].GetContents()->GetXPos() << "\nYPOS:" << Grid[10][10].GetContents()->GetYPos();
    }
    if (Grid[5][10].GetContents() != nullptr) {
      //  SDL_Log("\nGrid set correctly3\n");
    //    cout << "XPOS:" << Grid[5][10].GetContents()->GetXPos() << "\nYPOS:" << Grid[5][10].GetContents()->GetYPos();
    }
    if (Grid[0][1].GetContents() != nullptr) {
      //  SDL_Log("\nGrid set correctly4\n");
       // cout << "XPOS:" << Grid[0][1].GetContents()->GetXPos() << "\nYPOS:" << Grid[0][1].GetContents()->GetYPos();
    }
}
void Map::MoveUnit(vector<int>NewPos,vector<int>OldPos,Unit* UnitToMove) {
    AdjustGridForMove(NewPos,OldPos,UnitToMove);
    AdjustUnits(NewPos, OldPos, UnitToMove);
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

Unit* Map::GetContentsOfGrid(int X, int Y) {
    return Grid[X][Y].GetContents();
}
Unit* Map::GetIfUnitClicked(int MouseX,int MouseY) {
//    cout << "Size:" << UnitsInGrid.size();
    int GridStartX = (WINDOW_WIDTH - (GameMap.GetWidth() * SquareSize)) / 2; int GridStartY = (WINDOW_HEIGHT - ((GameMap.GetHeight() * SquareSize))) / 2;
    for (int i = 0; i < UnitsInGrid.size(); i++) {
       // cout << i << ":" << UnitsInGrid[i].GetXPos()<< ","<<UnitsInGrid[i].GetYPos() << "\n";
      //  cout << " true XPOS: " << (UnitsInGrid[i].GetXPos() * SquareSize + GridStartX) << "\n"<<" True YPos: "<< (UnitsInGrid[i].GetYPos() * SquareSize+ GridStartY)<<"\n";
        if (UnitsInGrid[i]->GetXPos() * SquareSize + GridStartX<MouseX && (UnitsInGrid[i]->GetXPos() + 1) * SquareSize + GridStartX > MouseX ) {
         //   SDL_Log("Column contains Unit!");
            if (UnitsInGrid[i]->GetYPos()* SquareSize + GridStartY<MouseY && (UnitsInGrid[i]->GetYPos() + 1) * SquareSize + GridStartY > MouseY) {
           //     SDL_Log("Row containd unit");
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
bool Map::GetIfAllPlayersUnitsUsedThisTurn(int PlayerID) {
    for (int i = 0; i < UnitsInGrid.size(); i++) {
        if (UnitsInGrid[i]->GetTeam() == PlayerID && !UnitsInGrid[i]->GetIfUsedThisTurn()) {
            return false;
        }
    }
    return true;
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

Unit::Unit(string Name, int Health,int Team,int Speed, string Path) {
    this->Name = Name;
    this->Health = Health;
    this->Team = Team;
    this->Speed = Speed;
    UsedThisTurn = false;
    SpritePath= string(SDL_GetBasePath()) + Path;

}
void Unit::SetWeapon(Weapon* weapon) {
    EquippedWeapon = weapon;
    cout << "Weapon for " << Name << " Set too " << weapon->GetName() << "\n";
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
                if (GameMap.GetContentsOfGrid(x, y) == nullptr) {

                    CurrentAttacks.push_back({ x,y });
                    cout << "Adding:" << x << y << "\n";
                }
            }
        }
    }
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

struct OptionBoxes {
    int x; int y; int h; int w;
};
struct SDL_App {
    SDL_Window* window{ nullptr };
    SDL_Renderer* renderer{ nullptr };
    SDL_Event event;
    bool IsRunning = true;
    bool GameStart = false;
};

TTF_Font* font; TTF_Font* TitleFont;
SDL_App App;
const int TITLE_WINDOW_HEIGHT = 800;
const int TITLE_WINDOW_WIDTH = 1200;

int TITLEFONT_SIZE=80;
int FONT_SIZE = 30;
vector<OptionBoxes> OptionPos;

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
    SDL_SetRenderDrawColor(App.renderer, 0, 255, 255, 180);
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
    SDL_SetRenderDrawColor(App.renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(App.renderer);
    DrawTitleScreenText();
    RenderBorders(TITLE_WINDOW_HEIGHT,TITLE_WINDOW_WIDTH);
    SDL_RenderPresent(App.renderer);
}
void DrawGridWithMoveOptions() {
    int GridStartX = (WINDOW_WIDTH - (GameMap.GetWidth() * SquareSize)) / 2; int GridStartY = (WINDOW_HEIGHT - ((GameMap.GetHeight() * SquareSize))) / 2;
    SDL_SetRenderDrawColor(App.renderer, 120, 230, 180, 255);
    SDL_FRect BackGround, GridSquare;
    BackGround.x = GridStartX; BackGround.y = GridStartY; BackGround.w = GameMap.GetWidth() * SquareSize; BackGround.h = SquareSize * GameMap.GetHeight();
    SDL_RenderFillRect(App.renderer, &BackGround);

    vector<vector<int>>CurrentMoves = GameInProgress->GetCurrentlySelected()->GetCurrentMoves();

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
                        SDL_SetRenderDrawColor(App.renderer, 69, 129, 142, 255);
                    }
                    else {
                        SDL_SetRenderDrawColor(App.renderer, 0, 0, 255, 255);
                    }
                    
                }
                else {
                    SDL_SetRenderDrawColor(App.renderer, 255, 0, 0, 255);

                }
                SDL_RenderFillRect(App.renderer, &GridSquare);
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

    //SDL_Texture* SpriteTexture = IMG_LoadTexture(App.renderer, Path.c_str());
    //if (!SpriteTexture) {
      //  SDL_Log("Failed to load texture: %s \n", SDL_GetError());
    //}
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


   // if (GameMap.GetContentsOfGrid(x, y)->GetTeam() == 0) {
 //       if (GameMap.GetContentsOfGrid(x, y)->GetIfUsedThisTurn()) {
     //       SDL_SetRenderDrawColor(App.renderer, 69, 129, 142, 255);
       // }
  //      else {
    //        SpriteTexture = IMG_LoadTexture(App.renderer, (GameMap.GetContentsOfGrid(x, y)->GetSpritePath()).c_str());
      //      if (!SpriteTexture) {
        //        SDL_Log("Failed to load texture: %s \n", SDL_GetError());
          //  }
         //   SDL_RenderTexture(App.renderer, SpriteTexture, nullptr, &GridSquare);
            //  SDL_SetRenderDrawColor(App.renderer, 0, 0, 255, 255);
           // SpriteDrawn = true;
       // }
   // }
   // else {
   //     SDL_SetRenderDrawColor(App.renderer, 255, 0, 0, 255);

   // }

  //  GridSquare.h = SquareSize; GridSquare.w = SquareSize;
   // SDL_SetRenderDrawColor(App.renderer, 223, 255, 0, 255);
    //for (int y = 0; y < MainGrid.Height; y+=2) {
      //  GridSquare.y = GridStartY + (y *  SquareSize);
        //for (int x = 0; x < MainGrid.Width; x+=2) {
          //  GridSquare.x = GridStartX + (x * SquareSize);
           // SDL_RenderFillRect(App.renderer, &GridSquare);
            //cout<<GameMap.GetContentsOfGrid(x, y);
           // if (GameMap.GetContentsOfGrid(x, y) == NULL) {
                // SDL_Log("Square empty");
           // }
           // else {
             //   SDL_Log(" SQaure contains unit :");
            //}
            
      //  }
    //}
    //for (int y = 1; y < MainGrid.Height; y += 2) {
      //  GridSquare.y = GridStartY + (y * SquareSize);
        //for (int x = 1; x < MainGrid.Width; x += 2) {
          //  GridSquare.x = GridStartX + (x * SquareSize);
            //SDL_RenderFillRect(App.renderer, &GridSquare);
         //   if (GameMap.GetContentsOfGrid(x, y) == NULL) {
                // SDL_Log("Square empty");
          //  }
            //else {
             //   SDL_Log(" SQaure contains unit :");
               // SDL_SetRenderDrawColor(App.renderer, 255, 0, 0, 255);

                //SDL_RenderFillRect(App.renderer, &GridSquare);

                //SDL_SetRenderDrawColor(App.renderer, 223, 255, 0, 255);

//            }
  //      }
//    }

}
void DrawGridWithAttackOptions() {
    int GridStartX = (WINDOW_WIDTH - (GameMap.GetWidth() * SquareSize)) / 2; int GridStartY = (WINDOW_HEIGHT - ((GameMap.GetHeight() * SquareSize))) / 2;
    SDL_SetRenderDrawColor(App.renderer, 120, 230, 180, 255);
    SDL_FRect BackGround, GridSquare;
    BackGround.x = GridStartX; BackGround.y = GridStartY; BackGround.w = GameMap.GetWidth() * SquareSize; BackGround.h = SquareSize * GameMap.GetHeight();
    SDL_RenderFillRect(App.renderer, &BackGround);

    vector<vector<int>>CurrentAttacks = GameInProgress->GetCurrentlySelected()->GetCurrentAttacks();
    cout << "Number of possible attacls" << CurrentAttacks.size();

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

                }
            }
            if (GameMap.GetContentsOfGrid(x, y) != NULL) {
                GridSquare.x = GridStartX + (x * SquareSize);
                if (GameMap.GetContentsOfGrid(x, y)->GetTeam() == 0) {
                    if (GameMap.GetContentsOfGrid(x, y)->GetIfUsedThisTurn()) {
                        SDL_SetRenderDrawColor(App.renderer, 69, 129, 142, 255);
                    }
                    else {
                        SDL_SetRenderDrawColor(App.renderer, 0, 0, 255, 255);
                    }

                }
                else {
                    SDL_SetRenderDrawColor(App.renderer, 255, 0, 0, 255);

                }
                SDL_RenderFillRect(App.renderer, &GridSquare);
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
void DrawGameScreenTemp() {
    SDL_SetRenderDrawColor(App.renderer, 175, 225, 175, 180);
    SDL_RenderClear(App.renderer);
    if (GameInProgress->GetCurrentlySelected() == nullptr) {
        DrawGrid();
    }
    else {
        if (MoveDone) {
            DrawGridWithAttackOptions();
        }
        else {
            DrawGridWithMoveOptions();
        }
       
    }
    
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
         //   SDL_Log("Y clicked");
            if (GridStartX + CurrentMoves[i][0] * SquareSize<MouseX && GridStartX + (1 + CurrentMoves[i][0]) * SquareSize > MouseX) {
        //        SDL_Log("X clicked");
           //     cout << CurrentMoves[i][0] << " " << CurrentMoves[i][1] << " Clicked\n";
                return CurrentMoves[i];

            }
        }

    }
    return { -1,-1 };

//    cout << "Size:" << UnitsInGrid.size();
  //  
    //for (int i = 0; i < UnitsInGrid.size(); i++) {
        // cout << i << ":" << UnitsInGrid[i].GetXPos()<< ","<<UnitsInGrid[i].GetYPos() << "\n";
       //  cout << " true XPOS: " << (UnitsInGrid[i].GetXPos() * MainGrid.SQUARE_SIZE + GridStartX) << "\n"<<" True YPos: "<< (UnitsInGrid[i].GetYPos() * MainGrid.SQUARE_SIZE + GridStartY)<<"\n";
      //  if (UnitsInGrid[i].GetXPos() * MainGrid.SQUARE_SIZE + GridStartX<MouseX && (UnitsInGrid[i].GetXPos() + 1) * MainGrid.SQUARE_SIZE + GridStartX > MouseX) {
            //   SDL_Log("Column contains Unit!");
        //    if (UnitsInGrid[i].GetYPos() * MainGrid.SQUARE_SIZE + GridStartY<MouseY && (UnitsInGrid[i].GetYPos() + 1) * MainGrid.SQUARE_SIZE + GridStartY > MouseY) {
                //     SDL_Log("Row containd unit");
          //      cout << "Unit clicked:" << UnitsInGrid[i].GetName();
            //    return UnitsInGrid[i];
            //}

//        }
  //  }

}
bool CreateApp() {
    if ((App.window = SDL_CreateWindow("TitleScreen", TITLE_WINDOW_WIDTH, TITLE_WINDOW_HEIGHT, 0)) == nullptr) {
        SDL_Quit();
        return false;
    }
    if ((App.window = SDL_CreateWindow("Grid", TITLE_WINDOW_WIDTH, TITLE_WINDOW_HEIGHT, 0)) == nullptr) {
        SDL_Quit();
        return false;
    }
    if ((App.renderer = SDL_CreateRenderer(App.window, nullptr)) == nullptr) {
    }

    return true;
}

bool CreateGame() {
    if ((App.window = SDL_CreateWindow("GameScreen",WINDOW_WIDTH, WINDOW_HEIGHT, 0)) == nullptr) {
        SDL_Quit();
        return false;
    }
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

    SDL_RenderFillRect(App.renderer, &OptionSquare);
    for (int i = 1; i < NoOfOptions; i++) {
        SDL_SetRenderDrawColor(App.renderer, 0, 255, 0, 255);
        OptionSquare.x = EdgeGap+(OptionWidth*i)+(Gap*i);
        OptionSquare.y = Vertical_Gap; 
        OptionSquare.w =OptionWidth; 
        OptionSquare.h = TITLE_WINDOW_HEIGHT - (Vertical_Gap * 2);
        SDL_RenderFillRect(App.renderer, &OptionSquare);
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
void CheckIfOptionsClicked(int X,int Y) {
    for (int i = 0; i < OptionPos.size(); i++) {
        if (X > OptionPos[i].x && X < OptionPos[i].x + OptionPos[i].w && Y> OptionPos[i].y && Y < OptionPos[i].y + OptionPos[i].h) {
        //    SDL_Log("Option %d clicked", i);
            if (i == 0) {
                SDL_Log("Starting game");
                App.IsRunning = false;

            }
        }
    }

}



int main()
{
    SDL_Log("Using SDL version %d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
    SDL_Log("Using SDL_image version %d.%d.%d", SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_MICRO_VERSION);
    SDL_Log("Running");
   
    if (SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0) {
        return -1;
    }
    if (!CreateApp()) {
        SDL_Log("Game could not run");
        return 2;
    }
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    font = TTF_OpenFont("arial.ttf", FONT_SIZE);
    if (!font) {
        SDL_Log("Failed to load font: %s",SDL_GetError());
        return 1;
    }
    TitleFont = TTF_OpenFont("arial.ttf", TITLEFONT_SIZE);
    if (!font) {
        SDL_Log("Failed to load font: %s", SDL_GetError());
        return 1;
    }

    CreateWeapons();
    SDL_Color color = { 255, 255, 255 };
   // SDL_Surface* surface = TTF_RenderText_Solid(font, "Hello SDL_ttf!", 24,color);
  //  SDL_Texture* texture = SDL_CreateTextureFromSurface(App.renderer, surface);
   // OpenTitleScreen();
    while (App.IsRunning) 
    {
        while (SDL_PollEvent(&App.event)) {
            switch (App.event.type) {
            case SDL_EVENT_QUIT:
                App.IsRunning = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                switch (App.event.key.key)
                {
                case SDLK_ESCAPE:
                    App.IsRunning= false;
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
        DrawTitleScreen();
        SDL_RenderPresent(App.renderer);
        SDL_Delay(10);

    }
    SDL_RenderClear(App.renderer);
    float MouseX = 0; float MouseY = 0;

    SetOptionsPos();
    if (App.GameStart){
        App.IsRunning = true;
        while (App.IsRunning)
        {
            SDL_RenderClear(App.renderer);
            DrawSelectScreen();
            SDL_RenderPresent(App.renderer);
            SDL_Delay(10);
            while (SDL_PollEvent(&App.event)) {
                switch (App.event.type) {
                case SDL_EVENT_QUIT:
                    App.IsRunning = false;
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    MouseX = App.event.button.x; MouseY = App.event.button.y;
                 //   SDL_Log("Mouse clicked at %f %f",MouseX,MouseY);
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
    CreateGame();
    MoveDone = false;
    GameInProgress = new Game("Player1", "Player2");
    GameInProgress->SetCurrentlySelected(nullptr);
    Unit Temp=Unit("",0,0,0,"");
    if (App.GameStart) {
        App.IsRunning = true;
        while (App.IsRunning)
        {
            while (SDL_PollEvent(&App.event)) {
                switch (App.event.type) {
                case SDL_EVENT_QUIT:
                    App.IsRunning = false;
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    MouseX = App.event.button.x; MouseY = App.event.button.y;
                    SDL_Log("Mouse clicked at %f %f", MouseX, MouseY);
                    if (GameInProgress->GetCurrentlySelected() != nullptr) {
                        vector<int> Move = CheckIfMoveOptionClicked(MouseX, MouseY);
                        if (MoveDone) {


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
                                else{
                                    GameInProgress->SetCurrentlySelected(nullptr);
                                    UpdateNeeded = true;
                                    MoveDone = false;
                                }
                                
                                

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
                        if (Temp!=nullptr ) {
                            SDL_Log("Clicked unit!");
                            if (Temp->GetTeam() == GameInProgress->GetCurrentPlayer() && !Temp->GetIfUsedThisTurn() && !MoveDone) {
                                SDL_Log("Player Clicked their Unit");
                                Temp->CalculateCurrentMoves();
                                GameInProgress->SetCurrentlySelected(Temp);
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


    std::cout << "Game Shut down!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
