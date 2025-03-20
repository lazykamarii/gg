#include <SDL.h>
#include <vector>
#include <algorithm>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TANK_SIZE = 40;
const int BULLET_SIZE = 10;
const int SPEED = 5;
const int BULLET_SPEED = 7;

struct Bullet {
    int x, y;
    int dirX, dirY;
};

struct Tank {
    int x, y;
    int dirX, dirY;
    std::vector<Bullet> bullets;
};

void moveTank(Tank &tank, const Uint8 *keystates) {
    tank.dirX = tank.dirY = 0;
    if (keystates[SDL_SCANCODE_W]) tank.dirY = -SPEED;
    if (keystates[SDL_SCANCODE_S]) tank.dirY = SPEED;
    if (keystates[SDL_SCANCODE_A]) tank.dirX = -SPEED;
    if (keystates[SDL_SCANCODE_D]) tank.dirX = SPEED;
    
    tank.x += tank.dirX;
    tank.y += tank.dirY;
}

void shoot(Tank &tank) {
    Bullet b = {tank.x + TANK_SIZE / 2, tank.y + TANK_SIZE / 2, tank.dirX, tank.dirY};
    if (b.dirX != 0 || b.dirY != 0) {
        tank.bullets.push_back(b);
    }
}

void updateBullets(std::vector<Bullet> &bullets) {
    for (auto &b : bullets) {
        b.x += b.dirX * BULLET_SPEED;
        b.y += b.dirY * BULLET_SPEED;
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet &b) {
        return b.x < 0 || b.x > SCREEN_WIDTH || b.y < 0 || b.y > SCREEN_HEIGHT;
    }), bullets.end());
}

void render(SDL_Renderer *renderer, Tank &player) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect tankRect = {player.x, player.y, TANK_SIZE, TANK_SIZE};
    SDL_RenderFillRect(renderer, &tankRect);
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (auto &b : player.bullets) {
        SDL_Rect bulletRect = {b.x, b.y, BULLET_SIZE, BULLET_SIZE};
        SDL_RenderFillRect(renderer, &bulletRect);
    }
    
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Tank Battle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    Tank player = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, 0};
    
    bool running = true;
    SDL_Event event;
    while (running) {
        const Uint8 *keystates = SDL_GetKeyboardState(NULL);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) shoot(player);
        }
        
        moveTank(player, keystates);
        updateBullets(player.bullets);
        render(renderer, player);
        SDL_Delay(16);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
