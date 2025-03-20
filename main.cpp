#include <iostream>
#include <SDL.h>
#include <vector>
using namespace std;
const int TILE_SIZE = 40;
const int MAP_ROWS = 15;
const int MAP_COLS = 20;
const int SCR_W = TILE_SIZE * MAP_COLS;
const int SCR_H = TILE_SIZE * MAP_ROWS;
const int SPEED = 5;
const int BULLET_SPEED = 6;

int gameMap[MAP_ROWS][MAP_COLS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};
class Game {
public:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Rect Tank = {60, 60, 20, 20};
    int vx = 0, vy = 0;
    struct bullet{
        SDL_Rect rect;
        int vx, vy;
    };
    vector <bullet> bullets;
    void Shoot() {
        bullet bullet;
        bullet.rect = {Tank.x + Tank.w / 2 - 5, Tank.y + Tank.h / 2 - 5, 10, 10};
        bullet.vx = vx == 0 ? (vy != 0 ? 0 : BULLET_SPEED) : (vx / abs(vx)) * BULLET_SPEED;
        bullet.vy = vy == 0 ? (vx != 0 ? 0 : BULLET_SPEED) : (vy / abs(vy)) * BULLET_SPEED;
        bullets.push_back(bullet);
    }
    void Init() {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("Tank Battle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCR_W, SCR_H, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }

    bool CheckCollision(int newX, int newY) {
        int leftTile = newX / TILE_SIZE;
        int rightTile = (newX + Tank.w - 1) / TILE_SIZE;
        int topTile = newY / TILE_SIZE;
        int bottomTile = (newY + Tank.h - 1) / TILE_SIZE;

        if (gameMap[topTile][leftTile] == 1 || gameMap[topTile][rightTile] == 1 ||
            gameMap[bottomTile][leftTile] == 1 || gameMap[bottomTile][rightTile] == 1) {
            return true;
        }
        return false;
    }

    void HandleEvents(bool& running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } 
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_w: vy = -SPEED; break;
                    case SDLK_s: vy = SPEED; break;
                    case SDLK_a: vx = -SPEED; break;
                    case SDLK_d: vx = SPEED; break;
                    case SDLK_SPACE: 
                    Shoot();
                    break;
                }
            } 
            else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_w: case SDLK_s: vy = 0; break;
                    case SDLK_a: case SDLK_d: vx = 0; break;
                }
            }
            
        }
    }

    void Update() {
        int newX = Tank.x + vx;
        int newY = Tank.y + vy;
        if (!CheckCollision(newX, Tank.y)) {
            Tank.x = newX;
        }
        if (!CheckCollision(Tank.x, newY)) {
            Tank.y = newY;
        }
        for (size_t i = 0; i < bullets.size(); i++) {
            bullets[i].rect.x += bullets[i].vx;
            bullets[i].rect.y += bullets[i].vy;
            
            if (bullets[i].rect.x < 0 || bullets[i].rect.x > SCR_W ||
                bullets[i].rect.y < 0 || bullets[i].rect.y > SCR_H) {
                bullets.erase(bullets.begin() + i);
                i--;
            }
        }
    
    }

    void RenderMap() {
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
        for (int row = 0; row < MAP_ROWS; row++) {
            for (int col = 0; col < MAP_COLS; col++) {
                if (gameMap[row][col] == 1) {
                    SDL_Rect wallRect = {col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                    SDL_RenderFillRect(renderer, &wallRect);
                }
            }
        }
    }

    void Render() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        RenderMap();
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &Tank);
        SDL_RenderPresent(renderer);
    }

    void Destroy() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

int main(int argc, char* argv[]) {
    Game game;
    game.Init();
    bool running = true;
    while (running) {
        game.HandleEvents(running);
        game.Update();
        game.Render();
        SDL_Delay(16);
    }
    game.Destroy();
}
