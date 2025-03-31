void HandleEvents(bool &running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            int mouseX = event.button.x;
            int mouseY = event.button.y;
            
            if (event.type == SDL_QUIT) {
                if ((ismenu != ismenu2) || ishighscore) SaveGame();
                running = false;
                return;
            }
            
            if (ismenu) {
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (mouseX >= 296 && mouseX <= 511 && mouseY >= 153 && mouseY <= 204) {
                        Mix_HaltMusic();
                        ismenu = false;
                        gamerestart();
                        Mix_PlayMusic(bgmusic, -1);
                    } else if (mouseX >= 319 && mouseX <= 486 && mouseY >= 312 && mouseY <= 363) {
                        running = false;
                    } else if (mouseX >= 207 && mouseX <= 595 && mouseY >= 236 && mouseY <= 285) {
                        ishighscore = true;
                        ismenu = false;
                    }
                }
                return;
            }
            if (ismenu2){

            }
            if (ishighscore) {
                if (event.type == SDL_MOUSEBUTTONDOWN && 
                    mouseX >= 17 && mouseX <= 58 && 
                    mouseY >= 28 && mouseY <= 62) {
                    ismenu = true;
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
                        ismenu = true;
                        Mix_PlayMusic( menumusic, -1);
                    }
                }
                return;
            }
            
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_SPACE) {
                    Mix_VolumeChunk(jump, MIX_MAX_VOLUME * 0.2);
                    Mix_PlayChannel(-1, jump, 0);
                    velocity = JUMP_STRENGTH;
                } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    ispaused = !ispaused;
                    

                }
            }
            if(ispaused){
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (mouseX >= 327 && mouseX <= 476 && mouseY >= 235 && mouseY <= 260) {
                    ismenu = true;
                    Mix_PlayMusic(menumusic, -1);
                } else if ((mouseX >= 288 && mouseX <= 535 && mouseY >= 192 && mouseY <= 216) || 
                           (mouseX >= 16 && mouseX <= 49 && mouseY >= 15 && mouseY <= 45)) {
                    ispaused = !ispaused;
                }
            }
        }
    }
}