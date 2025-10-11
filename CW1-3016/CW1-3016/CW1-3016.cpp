// CW1-3016.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;
#include <string>
#include <vector>
#include <SDL3/SDL.h>
#include<SDL3_ttf/SDL_ttf.h>
#include "Header.h"


Game::Game(string P1Name, string P2Name) {
    Turn = 0;
    Player* P1 = new Player(P1Name); 
    Player* P2 = new Player(P2Name);

}
const int WINDOW_HEIGHT = 1400;
const int WINDOW_WIDTH = 2200;
Grid MainGrid;
Square::Square() {
    Contains=nullptr;

}
void Square::SetContents(Unit* NewContents) {
    Contains = NewContents;
}

Unit* Square::GetContents() {
    return Contains;
}

Map::Map(int x,int y){
    vector<Square>Temp;
    for (int Y = 0; Y < y; Y++) {
        Temp.clear();
        for (int X = 0; X< x; X++) {
            Temp.emplace_back();
        }
        Grid.push_back(Temp);
    };
    Grid[3][3].SetContents(new Unit("Name", 10,1));
    Grid[3][3].GetContents()->UpdatePosition(3, 3);
    Grid[3][20].SetContents(new Unit("Name", 10, 0));
    Grid[3][20].GetContents()->UpdatePosition(3, 20);
    AddUnitToGrid(Grid[3][3].GetContents()); AddUnitToGrid(Grid[3][20].GetContents());
    cout << Grid[3][3].GetContents();
    if (Grid[3][3].GetContents() != nullptr) {
        SDL_Log("Grid set correctly");
    }
}
Unit* Map::GetContentsOfGrid(int X, int Y) {
    return Grid[Y][X].GetContents();
}
Unit Map::GetIfUnitClicked(int MouseX,int MouseY) {
    int GridStartX = (WINDOW_WIDTH - (MainGrid.Width * MainGrid.SQUARE_SIZE)) / 2; int GridStartY = (WINDOW_HEIGHT - ((MainGrid.Height * MainGrid.SQUARE_SIZE))) / 2;
    for (int i = 0; i < UnitsInGrid.size(); i++) {
        if (UnitsInGrid[i].GetXPos() * MainGrid.SQUARE_SIZE + GridStartX<MouseX && (UnitsInGrid[i].GetXPos() + 1) * MainGrid.SQUARE_SIZE + GridStartX > MouseX) {
            return UnitsInGrid[i];
        }
    }
    return Unit("EMPTY",0,0);
}
void Map::AddUnitToGrid(Unit* Unit) {
    UnitsInGrid.push_back(*Unit);
}
Player::Player(string Name) {
    this->Name = Name;
}
Unit::Unit(string Name, int Health,int Team) {
    this->Name = Name;
    this->Health = Health;
    this->Team = Team;
}
void Unit::UpdatePosition(int x, int y) {
    XPos = x; YPos = y;
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
Map GameMap(MainGrid.Width, MainGrid.Height);
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
void DrawGrid() {
    int GridStartX = (WINDOW_WIDTH - (MainGrid.Width * MainGrid.SQUARE_SIZE)) / 2; int GridStartY = (WINDOW_HEIGHT - ((MainGrid.Height * MainGrid.SQUARE_SIZE))) / 2;
    SDL_SetRenderDrawColor(App.renderer, 120, 230, 180, 255);
    SDL_FRect BackGround, GridSquare;
    BackGround.x = GridStartX; BackGround.y = GridStartY; BackGround.w = MainGrid.Width * MainGrid.SQUARE_SIZE; BackGround.h = MainGrid.SQUARE_SIZE * MainGrid.Height;
    SDL_RenderFillRect(App.renderer, &BackGround);

    bool LastWasSquare=false, LastRowStartedWithSquare=false;
    GridSquare.h = MainGrid.SQUARE_SIZE; GridSquare.w = MainGrid.SQUARE_SIZE;
    SDL_SetRenderDrawColor(App.renderer, 223, 255, 0, 255);
    for (int y = 0; y < MainGrid.Height; y++) {
        GridSquare.y = GridStartY + (y * MainGrid.SQUARE_SIZE);
        LastWasSquare = true;
        if (!LastRowStartedWithSquare) {
            LastWasSquare = false;
        }
        for (int x = 0; x < MainGrid.Width; x ++){
            if (!LastWasSquare) {
                GridSquare.x = GridStartX + (x * MainGrid.SQUARE_SIZE);
                SDL_RenderFillRect(App.renderer, &GridSquare);
                LastWasSquare = true;
            }
            else {
                LastWasSquare = false;
            }
            if (GameMap.GetContentsOfGrid(x,y)!=NULL ) {
                if (GameMap.GetContentsOfGrid(x, y)->GetTeam() == 0) {
                    SDL_SetRenderDrawColor(App.renderer, 0, 0, 255, 255);
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

  //  GridSquare.h = MainGrid.SQUARE_SIZE; GridSquare.w = MainGrid.SQUARE_SIZE;
   // SDL_SetRenderDrawColor(App.renderer, 223, 255, 0, 255);
    //for (int y = 0; y < MainGrid.Height; y+=2) {
      //  GridSquare.y = GridStartY + (y * MainGrid.SQUARE_SIZE);
        //for (int x = 0; x < MainGrid.Width; x+=2) {
          //  GridSquare.x = GridStartX + (x * MainGrid.SQUARE_SIZE);
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
      //  GridSquare.y = GridStartY + (y * MainGrid.SQUARE_SIZE);
        //for (int x = 1; x < MainGrid.Width; x += 2) {
          //  GridSquare.x = GridStartX + (x * MainGrid.SQUARE_SIZE);
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
void DrawGameScreenTemp() {
    SDL_SetRenderDrawColor(App.renderer, 175, 225, 175, 180);
    SDL_RenderClear(App.renderer);
    DrawGrid();
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
            SDL_Log("Option %d clicked", i);
            if (i == 0) {
                SDL_Log("Starting game");
                App.IsRunning = false;

            }
        }
    }

}

int main()
{
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
                    SDL_Log("Mouse clicked at %f %f",MouseX,MouseY);
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
    Unit Temp=Unit("",0,0);
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
                    Temp= GameMap.GetIfUnitClicked(MouseX, MouseY);
                    if (Temp.GetName() != "EMPTY") {
                        SDL_Log("Clicked unit!");
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
            DrawGameScreenTemp();
            SDL_RenderPresent(App.renderer);
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
