// CW1-3016.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;
#include <string>
#include <SDL3/SDL.h>
#include<SDL3_ttf/SDL_ttf.h>
#include "Header.h"

Game::Game(string P1Name, string P2Name) {
    turn = 0;
    P1 = new Player(P1Name); 
    P2 = new Player(P2Name);

}
Player::Player(string Name) {
    this->Name = Name;
}
Unit::Unit(string Name, int Health) {
    this->Name = Name;
    this->Health = Health;
}

struct SDL_App {
    SDL_Window* window{ nullptr };
    SDL_Renderer* renderer{ nullptr };
    SDL_Event event;
    bool IsRunning = true;
    bool GameStart = false;
};
struct Grid {
    int Height = 20;
    int Width = 16;
    int SQUARE_SIZE = 40;
};

TTF_Font* font; TTF_Font* TitleFont;
SDL_App App;
Grid MainGrid;
const int TITLE_WINDOW_HEIGHT = 800;
const int TITLE_WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 1200;
const int WINDOW_WIDTH = 1800;
int TITLEFONT_SIZE=80;
int FONT_SIZE = 30;

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
    SDL_FRect BackGround,GridSquare;
    BackGround.x = GridStartX; BackGround.y = GridStartY; BackGround.w = MainGrid.Width * MainGrid.SQUARE_SIZE; BackGround.h = MainGrid.SQUARE_SIZE * MainGrid.Height;
    SDL_RenderFillRect(App.renderer, &BackGround);

    GridSquare.h = MainGrid.SQUARE_SIZE; GridSquare.w = MainGrid.SQUARE_SIZE;
    SDL_SetRenderDrawColor(App.renderer, 223, 255, 0, 255);
    for (int y = 0; y < MainGrid.Height; y+=2) {
        GridSquare.y = GridStartY + (y * MainGrid.SQUARE_SIZE);
        for (int x = 0; x < MainGrid.Width; x+=2) {
            GridSquare.x = GridStartX + (x * MainGrid.SQUARE_SIZE);
            SDL_RenderFillRect(App.renderer, &GridSquare);
        }
    }
    for (int y = 1; y < MainGrid.Height; y += 2) {
        GridSquare.y = GridStartY + (y * MainGrid.SQUARE_SIZE);
        for (int x = 1; x < MainGrid.Width; x += 2) {
            GridSquare.x = GridStartX + (x * MainGrid.SQUARE_SIZE);
            SDL_RenderFillRect(App.renderer, &GridSquare);
        }
    }

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
    CreateGame();
    if (App.GameStart) {
        App.IsRunning = true;
        while (App.IsRunning)
        {
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
