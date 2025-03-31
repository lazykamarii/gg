#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

using namespace std;

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 300;

int main(int argc, char* argv[]) {
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
        cout << "Lỗi SDL: " << SDL_GetError() << endl;
        return -1;
    }

    // Khởi tạo cửa sổ SDL (chỉ để giữ chương trình chạy)
    SDL_Window* window = SDL_CreateWindow("Test Mix_PlayChannel", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cout << "Lỗi tạo cửa sổ: " << SDL_GetError() << endl;
        SDL_Quit();
        return -1;
    }

    // Khởi tạo SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "Lỗi khởi tạo SDL_mixer: " << Mix_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Load âm thanh
    Mix_Chunk* sound = Mix_LoadWAV("jump.wav");
    if (!sound) {
        cout << "Lỗi load âm thanh: " << Mix_GetError() << endl;
        Mix_CloseAudio();
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    cout << "Nhấn SPACE để phát âm thanh. Nhấn ESC để thoát." << endl;

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                } else if (event.key.keysym.sym == SDLK_SPACE) {
                    // Phát âm thanh
                    if (Mix_PlayChannel(-1, sound, 0) == -1) {
                        cout << "Lỗi phát âm thanh: " << Mix_GetError() << endl;
                    } else {
                        cout << "Đã phát âm thanh!" << endl;
                    }
                }
            }
        }
        SDL_Delay(10);
    }

    // Dọn dẹp tài nguyên
    Mix_FreeChunk(sound);
    Mix_CloseAudio();
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
