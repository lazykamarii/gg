#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sdl_image.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRAVITY = 1;
const int JUMP_STRENGTH = -15;
const int PIPE_WIDTH = 80;
const int PIPE_GAP = 200;
const int PIPE_SPEED = 5;
SDL_Texture * ppause = nullptr;
SDL_Texture * pmenu = nullptr;
SDL_Texture * pgameover = nullptr;
struct Pipe {
    int x, height;
    bool passed;
};

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Rect bird = {100, SCREEN_HEIGHT / 2, 40, 40};
int velocity = 0;
std::vector<Pipe> pipes;
bool ispaused=0;
bool ismenu=1;
bool gameover=0;
int score=0;

void Init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    std::srand(std::time(0));
    ppause = IMG_LoadTexture(renderer,"bird.img");
    pmenu = IMG_LoadTexture (renderer, "bird.img");
    pgameover = IMG_LoadTexture (renderer, "bird.img");
}
void gamerestart(){
    bird= {100, SCREEN_HEIGHT/2, 40, 40};
    pipes.clear();
    score=0;
    gameover=false;
    velocity=0;
    ismenu=0;
    ispaused=0;
    
}
void HandleEvents(bool &running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (ismenu){
          if(  event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_a) (ismenu=!ismenu);
          else return;
        }
        if (gameover) if (event.type == SDL_KEYDOWN && event.key.keysym.sym== SDLK_r) gamerestart();
        if (event.type == SDL_QUIT) {
            running = false;
        } else if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) || event.type == SDL_MOUSEBUTTONDOWN) {
            velocity = JUMP_STRENGTH;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) (ispaused=!ispaused);
    }
}

void Update() {
    if (ismenu) return;
    if (ispaused) return;
    velocity += GRAVITY;
    bird.y += velocity;
    
    if (bird.y < 0) bird.y = 0;
    if (bird.y > SCREEN_HEIGHT - bird.h) bird.y = SCREEN_HEIGHT - bird.h;
    gameover=true;

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
                
                gameover=true;
                break;
            }
        }
    }
}

void Render() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 250, 255);
    SDL_RenderClear(renderer);
    SDL_Rect rmenu ={0,0, SCREEN_WIDTH, SCREEN_HEIGHT};
    if (ismenu){
        SDL_RenderCopy(renderer, pmenu, NULL, &rmenu);
        SDL_RenderPresent(renderer);
        return;
    }
    SDL_Rect rgameover={0,0,0, 0};
    if (gameover){
        SDL_RenderCopy(renderer, pgameover, NULL, &rgameover);
        SDL_RenderPresent(renderer);

        return;
    }
    SDL_Rect rpause ={1,0, SCREEN_WIDTH, SCREEN_HEIGHT};
    if(ispaused){
    SDL_RenderCopy(renderer,ppause, NULL, &rpause);
    SDL_RenderPresent(renderer);
    return;
}
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
        Render();
        Update();
        
        SDL_Delay(16);
    }
    Cleanup();
    return 0;
}
