#ifndef BIRD_H
#define BIRD_H

#include <SDL.h>

class Bird {
public:
    Bird(int x, int y, SDL_Texture* texture1, SDL_Texture* texture2, bool alive);
    void Update();
    void Render(SDL_Renderer* renderer);
    void Jump();
    void GameOVer();
    int GetX();
    int GetY();
    float GetVeclocity();
    bool GetAlive();
    void VecIn(float vec);
    void ReGame();
    const SDL_Rect& GetRect();
private:
    int x, y;
    float velocity;
    SDL_Texture* texture1;
    SDL_Texture* texture2;
    bool alive;
    SDL_Rect rect = {x, y, 40, 40 };
};

#endif