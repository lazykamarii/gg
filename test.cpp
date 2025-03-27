#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <SDL_image.h>
#include <fstream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;
SDL_Texture * pgameover = nullptr;
    

void Init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    
    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow("SDL2 + TTF", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    font = TTF_OpenFont("font.otf", 72);  // Đảm bảo có file "arial.ttf" trong thư mục chạy
    if (!font) {
        std::cout << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        exit(1);
    }
    pgameover = IMG_LoadTexture (renderer, "gameover.png");
}

SDL_Texture* RenderText(const std::string &message, SDL_Color color) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, message.c_str(), color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    return textTexture;
}
void SaveClickPosition(int x, int y) {
    std::ofstream file("backup.txt", std::ios::app);  // Ghi thêm vào file
    if (file.is_open()) {
        file << "Click Position: " << x << ", " << y << std::endl;
        file.close();
        printf("Đã lưu vị trí click: %d, %d\n", x, y);
    } else {
        printf("Lỗi: Không thể mở file backup.txt\n");
    }
}

void Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Màu nền đen
    SDL_RenderClear(renderer);

    SDL_Color textColor = {255, 255, 255};  // Màu trắng
    SDL_Texture* textTexture = RenderText("ABC", textColor);
    
    int textW, textH;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textW, &textH);
    SDL_Rect renderQuad = { (SCREEN_WIDTH - textW) / 2, (SCREEN_HEIGHT - textH) / 2, textW, textH };

    SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(textTexture);
    
    SDL_Rect rgameover = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3};
    SDL_RenderCopy(renderer, pgameover, NULL, &rgameover);
    SDL_RenderPresent (renderer);
}

void Cleanup() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Init();
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                SaveClickPosition(mouseX, mouseY);
            }
        }

        Render();
        SDL_Delay(100);
    }

    Cleanup();
    return 0;
}
