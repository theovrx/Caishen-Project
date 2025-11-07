#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "hanzi_struct.h"

#define WINWIDTH 640
#define WINHEIGHT 480

int main(int argc, char* argv[]) {
    // Initialize SDL

    int current_state = 0;
    float size = 500;
    float x = 0;
    float y = 0;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window *win = SDL_CreateWindow("TEST",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINWIDTH, WINHEIGHT,
                                       SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    IMG_Init(2);

    SDL_Surface * optsrf = NULL;
    SDL_Surface * leclerc = IMG_Load("Leclerc-About-02-Official-Pic-1.jpg");

    optsrf = SDL_ConvertSurface(leclerc, SDL_GetWindowSurface(win)->format, 0);

    SDL_Texture * text = SDL_CreateTextureFromSurface(renderer,
        optsrf);
    SDL_FreeSurface(leclerc);
    struct Kanji *on_screen_kanji[16];

    // Game loop
    int running = 1;
    SDL_Event e;
    while (running) {
        // Handle events
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); 
        SDL_RenderClear(renderer);

        SDL_Rect dstrect = {x, y, size, size};

        SDL_RenderCopy(renderer, text, NULL,&dstrect);
        
 
        // SDL_RenderCopy(renderer, texture, NULL, &dstrect);

        // if (size > grotle_size  && current_state == 0) {
        //     image = SDL_LoadBMP("grotle.bmp");
        //     texture = SDL_CreateTextureFromSurface(renderer,
        //         image);
        //     SDL_FreeSurface(image);
        //     current_state = 1;
        // };

        // if (size > torterra_size  && current_state == 1) {
        //     image = SDL_LoadBMP("torterra.bmp");
        //     texture = SDL_CreateTextureFromSurface(renderer,
        //         image);
        //     SDL_FreeSurface(image);
        //     current_state = 2;
        // };

        // size = size + 1;


        // Show what we rendered
        SDL_RenderPresent(renderer);

    }

    // Cleanup
    SDL_DestroyTexture(text);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
