#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRAVITY = 1;
const int JUMP_STRENGTH = -15;
const int PIPE_WIDTH = 80;
const int PIPE_GAP = 200;
const int PIPE_SPEED = 5;

struct Pipe {
    int x, height;
    bool passed;
};

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Rect bird = {100, SCREEN_HEIGHT / 2, 40, 40};
int velocity = 0;
std::vector<Pipe> pipes;

void Init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    std::srand(std::time(0));
}

void HandleEvents(bool &running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
            velocity = JUMP_STRENGTH;
        }
    }
}

void Update() {
    velocity += GRAVITY;
    bird.y += velocity;
    
    if (bird.y < 0) bird.y = 0;
    if (bird.y > SCREEN_HEIGHT - bird.h) bird.y = SCREEN_HEIGHT - bird.h;

    for (auto &pipe : pipes) {
        pipe.x -= PIPE_SPEED;
    }
    
    if (pipes.empty() || pipes.back().x < SCREEN_WIDTH - 300) {
        pipes.push_back({SCREEN_WIDTH, rand() % (SCREEN_HEIGHT - PIPE_GAP - 100) + 50, false});
    }
    
    if (!pipes.empty() && pipes.front().x < -PIPE_WIDTH) {
        pipes.erase(pipes.begin());
    }

    for (auto &pipe : pipes) {
        if (bird.x + bird.w > pipe.x && bird.x < pipe.x + PIPE_WIDTH) {
            if (bird.y < pipe.height || bird.y + bird.h > pipe.height + PIPE_GAP) {
                bird.y = SCREEN_HEIGHT / 2;
                velocity = 0;
                pipes.clear();
                break;
            }
        }
    }
}

void Render() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 250, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &bird);
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (const auto &pipe : pipes) {
        SDL_Rect topPipe = {pipe.x, 0, PIPE_WIDTH, pipe.height};
        SDL_Rect bottomPipe = {pipe.x, pipe.height + PIPE_GAP, PIPE_WIDTH, SCREEN_HEIGHT - pipe.height - PIPE_GAP};
        SDL_RenderFillRect(renderer, &topPipe);
        SDL_RenderFillRect(renderer, &bottomPipe);
    }
    
    SDL_RenderPresent(renderer);
}

void Cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Init();
    bool running = true;
    while (running) {
        HandleEvents(running);
        Update();
        Render();
        SDL_Delay(16);
    }
    Cleanup();
    return 0;
}
