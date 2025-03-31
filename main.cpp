    #include <SDL2/SDL.h>
    #include <vector>
    #include <cstdlib>
    #include <ctime>
    #include <sdl_image.h>
    #include <sdl_ttf.h>
    #include <string>
    #include <iostream>
    #include <fstream>
    #include <algorithm>
    #include <SDL_mixer.h>
    using namespace std;

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    const float GRAVITY = 0.5;
    const float JUMP_STRENGTH = -9;
    const int PIPE_WIDTH = 60;
    const int PIPE_GAP = 160;
    const int PIPE_SPEED = 4;
    SDL_Texture * ppause = nullptr;
    SDL_Texture * pmenu = nullptr;
    SDL_Texture * pgameover = nullptr;
    SDL_Texture * pbird = nullptr;
    SDL_Texture * pbackground = nullptr;
    SDL_Texture * ppausebutton = nullptr;
    SDL_Texture * punpausebutton = nullptr;
    SDL_Texture * pmenubu = nullptr;
    SDL_Texture * phighscore = nullptr;
    SDL_Texture * die = nullptr;
    SDL_Texture *pcot1 = nullptr;
    SDL_Texture *pcot2 = nullptr;
    SDL_Texture *menu2 = nullptr;
    SDL_Texture * musicon = nullptr;
    SDL_Texture * musicoff = nullptr;
    SDL_Texture * cloudd = nullptr;
    Mix_Music * bgmusic = nullptr;
    Mix_Music * menumusic = nullptr;
    Mix_Music * gover = nullptr;
    Mix_Chunk * jump = nullptr;
    Mix_Chunk * getscore = nullptr;


    struct Pipe {
        int x, height;
        bool passed = false;
        bool isMoving = (rand() % 10 == 0);
        int verticalSpeed = (isMoving ? (rand() % 3 + 1) * (rand() % 2 == 0 ? 1 : -1) : 0);
        
    };
    struct Cloud {
        int x, y;
        int alpha;
        int lifetime;
        int vx;
    
        Cloud(int x, int y) : x(x), y(y), alpha(255), lifetime(60), vx(-2) {}
    };
    TTF_Font* font = nullptr;
    SDL_Color textColor={255, 255, 255};

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Rect bird = {100, SCREEN_HEIGHT / 2, 40, 40};
    float velocity = 0;
    std::vector<Pipe> pipes;
    std::vector<Cloud> clouds;
    bool ispaused=0;
    bool ismenu;
    bool ismenu2;
    bool gameover=0;
    int score=0;
    bool ismusic=1;
    bool alive = 1;
    bool ishighscore =0;
    bool gameoverMusicPlayed = false;
    bool cd = 0;
    

    void MenuMusic(){
        if (ismenu || ishighscore) Mix_PlayMusic(menumusic, -1);
    
    }


    void SaveClickPosition(int x, int y) {
        std::ofstream file("backup.txt", std::ios::app);
        if (file.is_open()) {
            file << "Click Position: " << x << ", " << y << std::endl;
            file.close();
            printf("Đã lưu vị trí click: %d, %d\n", x, y);
        } else {
            printf("Lỗi: Không thể mở file backup.txt\n");
        }
    }

    void Init() {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        TTF_Init();
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
        
        window = SDL_CreateWindow("Jump and Drop", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        std::srand(std::time(0));
        font= TTF_OpenFont("font.otf", 64);
        ppause = IMG_LoadTexture(renderer,"img/pause.png");
        pmenu = IMG_LoadTexture (renderer, "img/menu.png");
        pgameover = IMG_LoadTexture (renderer, "img/gameover.png");
        pbird = IMG_LoadTexture (renderer, "img/main.png");
        pbackground = IMG_LoadTexture (renderer, "img/background.png");
        ppausebutton = IMG_LoadTexture (renderer, "img/pausebutton.png");
        punpausebutton = IMG_LoadTexture (renderer, "img/unpausebutton.png");
        phighscore = IMG_LoadTexture (renderer, "img/highscore.png");
        die = IMG_LoadTexture (renderer, "img/maindie.png");
        pcot1 = IMG_LoadTexture(renderer, "img/cot1.png");
        pcot2 = IMG_LoadTexture(renderer, "img/cot2.png");
        menu2 = IMG_LoadTexture (renderer, "img/menu2.png");
        musicon = IMG_LoadTexture (renderer, "img/musicon.png");
        musicoff = IMG_LoadTexture (renderer, "img/musicoff.png");
        cloudd = IMG_LoadTexture (renderer, "img/cloud.png");
        if (ismusic){
        menumusic = Mix_LoadMUS ("music/menumusic.mp3");
        bgmusic = Mix_LoadMUS ("music/bgmusic.wav");
        gover = Mix_LoadMUS ("music/gameover.mp3");
        jump = Mix_LoadWAV ("music/jump.wav");
        getscore = Mix_LoadWAV ("music/getscore.wav");
        }
        
    }

    void CheckMenu(){
        std::ifstream file("save.txt", std::ios::ate);
        if (file.tellg()==0){ 
            ismenu =1;
            ismenu2=0;
        }else {
            ismenu2 =1;
            ismenu=0;
    }
}
    
    void gamerestart(){
        bird= {100, SCREEN_HEIGHT/2, 40, 40};
        pipes.clear();
        score=0;
        gameover=false;
        velocity=0;
        ismenu=0;
        ispaused=0;
        alive = 1;
        gameoverMusicPlayed = 0;
        if (ismusic) Mix_PlayMusic (bgmusic, -1);
        
    }

    bool CheckHighScore(int score){
        ifstream file ("hscore.txt");
        vector <int> a;
        int c;
        while (file >> c){
            a.push_back(c);
        }
        file.close();
        if (score > a[0]) return true;
        return false;
    }

    void UpdateHighScore (int score){
        ifstream file ("hscore.txt");
        vector <int> a;
        int c;
        while (file >> c){
            a.push_back(c);
        }
        a.push_back(score);
        sort(a.begin(), a.end(), greater<int>());
        file.close();
        ofstream filee ("hscore.txt", ios::trunc);
        for (int i=0; i<3; i++){
            filee << a[i] << " ";
        }
    }

    void RenderHighScore() {
        if (!font) return;
    
        SDL_Color yellow = {255, 255, 0};
        ifstream file("hscore.txt");
        int scores[3];
    
        for (int i = 0; i < 3; i++) {
            if (!(file >> scores[i])) scores[i] = 0;
        }
        file.close();
    
        std::string scoreTexts[3] = {std::to_string(scores[0]), std::to_string(scores[1]), std::to_string(scores[2])};
        
        
        SDL_Point positions[3] = {{581, 96}, {568, 215}, {566, 335}};
    
        for (int i = 0; i < 3; i++) {
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreTexts[i].c_str(), yellow);
            if (!textSurface) continue;
    
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);
    
            int textWidth, textHeight;
            SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
            SDL_Rect textRect = {positions[i].x, positions[i].y, textWidth, textHeight};
    
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_DestroyTexture(textTexture);
        }
    }
    void SaveGame() {
        ofstream file("save.txt", ios::trunc);
        if (!file.is_open()) {
            return;
        }
        
        file << bird.x << " " << bird.y << " " << velocity << " " << score <<" " << gameover << " " << alive << endl;
        
        file << pipes.size() << endl;
        for (const auto& pipe : pipes) {
            file << pipe.x << " " << pipe.height << " " << pipe.passed << " "
                 << pipe.isMoving << " " << pipe.verticalSpeed << endl;
        }
        
        file << ismusic << endl;

        file.close();
    }

    void LoadGame() {
        ifstream file("save.txt");
        if (!file.is_open()) {
            return;
        }
        
        file >> bird.x >> bird.y >> velocity >> score >> gameover >> alive;
        
        int numPipes;
        file >> numPipes;
        pipes.clear();
        for (int i = 0; i < numPipes; ++i) {
            Pipe pipe;
            file >> pipe.x >> pipe.height >> pipe.passed >> pipe.isMoving >> pipe.verticalSpeed;
            pipes.push_back(pipe);
        }
        
        file.close();
        ofstream filee ("save.txt",ios::trunc);
        filee.close();

    }
    



    void HandleEvents(bool &running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            int mouseX = event.button.x;
            int mouseY = event.button.y;
    
            if (event.type == SDL_QUIT) {
                if ( !ismenu2 && !ismenu && !ishighscore && !gameover ) SaveGame();
                running = false;
                return;
            }
            if (event.type== SDL_MOUSEBUTTONDOWN ){
                if (mouseX >= SCREEN_WIDTH - 60 && mouseX <= SCREEN_WIDTH - 10 &&
                    mouseY >= 10 && mouseY <= 60) {
                    ismusic = !ismusic;
                    Mix_HaltMusic();
                    ofstream file ("music.txt", ios::trunc);
                    if (ismusic) file << 1;
                    else file << 0;
                
                    
                }
            }
    
            if (ismenu) {
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (mouseX >= 296 && mouseX <= 511 && mouseY >= 153 && mouseY <= 204) {
                        Mix_HaltMusic();
                        ismenu = false;
                        gamerestart();
                        if(ismusic) Mix_PlayMusic(bgmusic, -1);
                    } else if (mouseX >= 319 && mouseX <= 486 && mouseY >= 312 && mouseY <= 363) {
                        running = false;
                    } else if (mouseX >= 207 && mouseX <= 595 && mouseY >= 236 && mouseY <= 285) {
                        ishighscore = true;
                        ismenu = false;
                    }
                }
                return;
            }
    
            if (ismenu2) {
                if(event.type== SDL_MOUSEBUTTONDOWN){
                    if (mouseX >=273 && mouseX<=530 && mouseY >=149 && mouseY <=206){
                        LoadGame();
                        cd=1;
                        ismenu2 =false;
                        ispaused = false;
                        SDL_Delay(500);
                        if(ismusic) Mix_PlayMusic(bgmusic, -1);
                    }
                    if (mouseX >=237 && mouseX<=567 && mouseY >=235 && mouseY <=282) {
                        gamerestart();
                        ismenu2=false;
                    }
                    if (mouseX >=208 && mouseX<=593 && mouseY >=317 && mouseY <=364){
                        ismenu2 = false;
                        ishighscore = true;
                    }
                    if (mouseX >=327 && mouseX<=481 && mouseY >=399 && mouseY <=447){
                        running =false;
                    }
                    
                }
            }
    
            if (ishighscore) {
                if (event.type == SDL_MOUSEBUTTONDOWN && 
                    mouseX >= 17 && mouseX <= 58 && 
                    mouseY >= 28 && mouseY <= 62) {
                    CheckMenu();
                    ishighscore = false;
                }
                return;
            }
    
            if (gameover) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
                    gamerestart();
                }
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (mouseX >= 374 && mouseX <= 416 && mouseY >= 283 && mouseY <= 294) {
                        CheckMenu();
                        if(ismusic) Mix_PlayMusic(menumusic, -1);
                    }
                }
                return;
            }
    
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_SPACE) {
                    Mix_VolumeChunk(jump, MIX_MAX_VOLUME * 0.2);
                    if (ismusic) Mix_PlayChannel(-1, jump, 0);
                    velocity = JUMP_STRENGTH;
                    clouds.push_back(Cloud(bird.x, bird.y + bird.h));
                } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    ispaused = !ispaused;
                }
            }
            if (ispaused) {
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (mouseX >= 327 && mouseX <= 476 && mouseY >= 235 && mouseY <= 260) {
                        ismenu = true;
                        if (!gameover)
                        SaveGame();
                        CheckMenu();
                        if(ismusic) Mix_PlayMusic(menumusic, -1);
                    } else if ((mouseX >= 288 && mouseX <= 535 && mouseY >= 192 && mouseY <= 216) || 
                               (mouseX >= 16 && mouseX <= 49 && mouseY >= 15 && mouseY <= 45)) {
                        ispaused = false;
                        if(ismusic) Mix_PlayMusic(bgmusic, -1);
                        cd=1;
                    }
                }
                return;
            }
            if (ismenu2== ismenu && !ishighscore && !gameover ) {
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (mouseX >= 10 && mouseX <= 50 && mouseY >= 10 && mouseY <= 50) { 
                        ispaused = !ispaused;
            
                        if (ispaused) {
                            Mix_HaltMusic();
                        } else {
                           if(ismusic) Mix_PlayMusic(bgmusic, -1);
                        }
                    }
                }
                return;
            }
    }
}
    

    void Update() {
        if (ismenu) return;
        if (ismenu2) return;
        if (ispaused) return;
        if (gameover) return;
        if (ishighscore) return;
        velocity += GRAVITY;
        if(!gameover)
        bird.y += velocity;
        
        if (bird.y < 0) bird.y = 0;
        if (bird.y > SCREEN_HEIGHT - bird.h) {
        bird.y = SCREEN_HEIGHT - bird.h;
        Mix_HaltMusic();
                    
                    
        if (!gameoverMusicPlayed && !ishighscore) {
        if (gover) {
        if(ismusic) Mix_PlayMusic(gover, 1);
        } 
        gameoverMusicPlayed = true;
    }
    
        SDL_Delay(50);
        gameover = true; 
        alive = false;
    }
        if (!gameover) {
        for (auto &pipe : pipes) {
            pipe.x -= PIPE_SPEED;
            if (pipe.isMoving) {
                pipe.height += pipe.verticalSpeed;
                if (pipe.height < 50 || pipe.height > SCREEN_HEIGHT - PIPE_GAP - 50) {
                    pipe.verticalSpeed = -pipe.verticalSpeed;
                }
            }
        }
    }
        if (!gameover)
        for (auto &pipe : pipes) {
            if (!pipe.passed && bird.x > pipe.x + PIPE_WIDTH) {
                pipe.passed = true;
                score++;
                if(ismusic) Mix_PlayChannel(1, getscore, 0);
            }
        }
        for (auto it = clouds.begin(); it != clouds.end();) {
            it->alpha -= 4;
            it->x += it->vx;
            it->lifetime--;
        
            if (it->alpha <= 0 || it->lifetime <= 0) {
                it = clouds.erase(it);
            } else {
                ++it;
            }
        }
        
        if (pipes.empty() || pipes.back().x < SCREEN_WIDTH - 300) {
            pipes.push_back({SCREEN_WIDTH, rand() % (SCREEN_HEIGHT - PIPE_GAP - 100) + 50, false});
        }
        
        if (!pipes.empty() && pipes.front().x < -PIPE_WIDTH) {
            pipes.erase(pipes.begin());
        }
        
        for (auto &pipe : pipes) {
            SDL_Rect topPipe = {pipe.x, 0, PIPE_WIDTH, pipe.height};
            SDL_Rect bottomPipe = {pipe.x, pipe.height + PIPE_GAP, PIPE_WIDTH, SCREEN_HEIGHT - (pipe.height + PIPE_GAP)};

            if (SDL_HasIntersection(&bird, &topPipe) || SDL_HasIntersection(&bird, &bottomPipe)) {
            alive = false;
            if (CheckHighScore(score)) UpdateHighScore(score);
    
            Mix_HaltMusic();
    
            if (!gameoverMusicPlayed ) {
            if (gover) {
             if (ismusic) Mix_PlayMusic(gover, 1);
        } 
            gameoverMusicPlayed = true;
        }

        SDL_Delay(50);
        gameover = true;
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
        SDL_Rect textRect = {SCREEN_WIDTH / 2 - textWidth / 2, 16  , textWidth, textHeight};

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture); 
    }

    void Render() {

        
        SDL_Rect background = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, pbackground, NULL, &background);
        SDL_Rect music = {SCREEN_WIDTH - 60, 10 , 50, 50};
        if (ismenu) {
            SDL_Rect rmenu = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, pmenu, NULL, &rmenu);
            SDL_RenderPresent(renderer);
            if (ismusic){
                SDL_RenderCopy(renderer, musicon, NULL, &music);
                SDL_RenderPresent(renderer);
            }else{ 
                SDL_RenderCopy (renderer, musicoff, NULL, &music);
                SDL_RenderPresent(renderer);
            }
            return;
        }
        if (ismenu2){ 
            SDL_Rect rmenu2 = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy (renderer, menu2, NULL, &rmenu2);
            
            if (ismusic){
                SDL_RenderCopy(renderer, musicon, NULL, &music);
                
            }else{ 
                SDL_RenderCopy (renderer, musicoff, NULL, &music);
                
            }
            SDL_RenderPresent(renderer);
            return;
    }
    
        
        if (ishighscore) {
            SDL_Rect highscore = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, phighscore, NULL, &highscore);
            RenderHighScore();
            
            if (ismusic){
                SDL_RenderCopy(renderer, musicon, NULL, &music);
                
            }else{ 
                SDL_RenderCopy (renderer, musicoff, NULL, &music);
                
            }
            SDL_RenderPresent(renderer);
            return;
        }
    
        
        if (!gameover && !ispaused) {
            SDL_Rect pausebutton = {10, 10, 40, 40};
            SDL_RenderCopy(renderer, ppausebutton, NULL, &pausebutton);
        }
        
        
        if (alive) {
            SDL_RenderCopy(renderer, pbird, NULL, &bird);
        } else {
            SDL_RenderCopy(renderer, die, NULL, &bird);
        }
        for (const auto& cloud : clouds) {
            SDL_Rect cloudRect = { cloud.x, cloud.y, 50, 30 }; 
            SDL_SetTextureAlphaMod(cloudd, cloud.alpha);  
            SDL_RenderCopy(renderer, cloudd, NULL, &cloudRect);
        }
    
        
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (const auto& pipe : pipes) {
            SDL_Rect topPipe = {pipe.x, 0, PIPE_WIDTH, pipe.height};
            SDL_Rect bottomPipe = {pipe.x, pipe.height + PIPE_GAP, PIPE_WIDTH, SCREEN_HEIGHT - (pipe.height + PIPE_GAP)};
            SDL_RenderCopy(renderer, pcot2, NULL, &topPipe);
            SDL_RenderCopy(renderer, pcot1, NULL, &bottomPipe);
        }
    
        
        RenderScore();

    
        
        if (ispaused) {
            SDL_Rect rpause = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3};
            SDL_Rect unpausebutton = {10, 10, 40, 40};
            SDL_RenderCopy(renderer, ppause, NULL, &rpause);
            SDL_RenderCopy(renderer, punpausebutton, NULL, &unpausebutton);
            SDL_RenderPresent(renderer);
            return;
        }
        if (ismusic){
            SDL_RenderCopy(renderer, musicon, NULL, &music);
            
        }else{ 
            SDL_RenderCopy (renderer, musicoff, NULL, &music);
            
        }
    
        
        if (gameover) {
            SDL_Rect rgameover = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3};
            SDL_RenderCopy(renderer, pgameover, NULL, &rgameover);
        }
        
    
        
        SDL_RenderPresent(renderer);
    }


    void Cleanup() {
        SDL_DestroyTexture(ppause);
        SDL_DestroyTexture(pmenu);
        SDL_DestroyTexture(pgameover);
        SDL_DestroyTexture(pbird);
        SDL_DestroyTexture(pbackground);
        SDL_DestroyTexture(ppausebutton);
        SDL_DestroyTexture(punpausebutton);
        SDL_DestroyTexture(pmenubu);
        SDL_DestroyTexture(phighscore);
        SDL_DestroyTexture(die);
        SDL_DestroyTexture(pcot1);
        SDL_DestroyTexture(pcot2);
    
        if (font) {
            TTF_CloseFont(font);
            font = nullptr;
        }
    

        Mix_FreeMusic(bgmusic);
        Mix_FreeMusic(menumusic);
        Mix_FreeMusic(gover);
        Mix_FreeChunk(jump);
        Mix_FreeChunk(getscore);
    

        Mix_CloseAudio();
    

        TTF_Quit();
    

        if (renderer) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }
        if (window) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }
    

        SDL_Quit();
    }

    int main(int argc, char* argv[]) {
        Init();
        bool running = true;
        ifstream  file ("muisc.txt");
        int temp;
        file >> temp;
        ismusic =temp;
        if (ismusic) Mix_PlayMusic (menumusic, -1);
        CheckMenu();
        while (running) {
            HandleEvents(running);
            
            Update();
            Render();
            SDL_Delay(16);
        }
        Cleanup();
        return 0;
    }
