#include <SDL2/SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TANK_SIZE = 50;
const int TANK_SPEED = 5;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Rect tank = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, TANK_SIZE, TANK_SIZE };

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("Tank Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

void handleEvents(bool& running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }
    }
}

void update() {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP]) tank.y -= TANK_SPEED;
    if (state[SDL_SCANCODE_DOWN]) tank.y += TANK_SPEED;
    if (state[SDL_SCANCODE_LEFT]) tank.x -= TANK_SPEED;
    if (state[SDL_SCANCODE_RIGHT]) tank.x += TANK_SPEED;
    
    // Keep tank within bounds
    if (tank.x < 0) tank.x = 0;
    if (tank.y < 0) tank.y = 0;
    if (tank.x > SCREEN_WIDTH - TANK_SIZE) tank.x = SCREEN_WIDTH - TANK_SIZE;
    if (tank.y > SCREEN_HEIGHT - TANK_SIZE) tank.y = SCREEN_HEIGHT - TANK_SIZE;
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &tank);
    SDL_RenderPresent(renderer);
}

void cleanUp() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc,  char* argd[]) {
    if (!init()) return -1;
    bool running = true;
    while (running) {
        handleEvents(running);
        update();
        render();
        SDL_Delay(16); // Approx 60 FPS
    }
    cleanUp();
    return 0;
}