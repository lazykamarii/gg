#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sdl_image.h>
#include <sdl_ttf.h>
#include <string>
#include <iostream>
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float GRAVITY = 0.8;
const int JUMP_STRENGTH = -13;
const int PIPE_WIDTH = 80;
const int PIPE_GAP = 200;
const int PIPE_SPEED = 5;
SDL_Texture * ppause = nullptr;
SDL_Texture * pmenu = nullptr;
SDL_Texture * pgameover = nullptr;
struct Pipe {
    int x, height;
    bool passed=0;
};
TTF_Font* font = nullptr;
SDL_Color textColor={255,255,255};

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Rect bird = {100, SCREEN_HEIGHT / 2, 40, 40};
float velocity = 0;
std::vector<Pipe> pipes;
bool ispaused=0;
bool ismenu=1;
bool gameover=0;
int score=0;

void Init() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    std::srand(std::time(0));
    font= TTF_OpenFont("font.otf", 32);
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
          if(  event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_a) (ismenu=false);
            return;
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
    if (bird.y > SCREEN_HEIGHT - bird.h) {
    bird.y = SCREEN_HEIGHT - bird.h;
    gameover = true; 
}
    for (auto &pipe : pipes) {
        pipe.x -= PIPE_SPEED;
    }
    for (auto &pipe : pipes) {
        if (!pipe.passed && bird.x > pipe.x + PIPE_WIDTH) {
            pipe.passed = true;
            score++;
        }
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
void RenderScore() {
    if (!font) return;
    
    string scoreText = to_string(score);

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (!textSurface) return;

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect textRect = {SCREEN_WIDTH / 2 - textWidth / 2, 20, textWidth, textHeight};

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture); 
}

void Render() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 250, 255);
    SDL_RenderClear(renderer);
    SDL_Rect rmenu ={0,0, SCREEN_WIDTH, SCREEN_HEIGHT};
    if (ismenu){
        SDL_RenderCopy(renderer, pmenu, NULL, &rmenu);
        SDL_RenderPresent(renderer);
    }else{
        
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
        int bottomPipeY = pipe.height + PIPE_GAP;
        int bottomPipeHeight = SCREEN_HEIGHT - bottomPipeY;
        SDL_Rect bottomPipe = {pipe.x, bottomPipeY, PIPE_WIDTH, bottomPipeHeight};

        SDL_RenderFillRect(renderer, &topPipe);
        SDL_RenderFillRect(renderer, &bottomPipe);
    }
    
    RenderScore();
    
    SDL_RenderPresent(renderer);
    }
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
