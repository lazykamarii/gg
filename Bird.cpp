#include "Bird.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Bird::Bird(int x, int y, SDL_Texture* texture1, SDL_Texture* texture2, bool alive) 
    : x(x), y(y), velocity(0), texture1(texture1), texture2(texture2), alive(alive) {}

void Bird::Update() {
    velocity += 0.5;
    y += velocity;
    if (y < 0) y = 0;
        if (y > SCREEN_HEIGHT - 40) {
        y = SCREEN_HEIGHT - 40;
    }
}
int Bird::GetX()  {
    return x;
}
int Bird::GetY()  {
    return y;
}
void Bird::Jump() {
    velocity = -9;
}
void Bird::GameOVer(){
    alive=0;
}
float Bird::GetVeclocity(){
    return velocity;
}
bool Bird:: GetAlive(){
    return alive;
}
void Bird::VecIn(float vec){
    velocity=vec;
}
void Bird::ReGame(){
    alive = true;
}
const SDL_Rect& Bird::GetRect(){
    return rect;
}

void Bird::Render(SDL_Renderer* renderer) {
    SDL_Rect dst = { x, y, 50, 50 };
    if (alive)
    SDL_RenderCopy(renderer, texture1, nullptr, &dst);
    else SDL_RenderCopy (renderer, texture2, nullptr, &dst);
}