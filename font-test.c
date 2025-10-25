#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "hanzi_struct.h"

int resizeHanzi(Hanzi hanzi, int fontSize, TTF_Font* font) {
    if (TTF_SizeUTF8(font, hanzi.character, &hanzi.rect->w, &hanzi.rect->h)){
        printf("Erreur TTF_SizeUTF8: %s\n", TTF_GetError());
        return -1;
    }
    hanzi.rect->h *= fontSize;
    hanzi.rect->w *= fontSize;
    return 0;
}

Hanzi createHanzi(char character[5], char fontPath[], int fontSize, int resolution, SDL_Color color, int fallSpeed) {
    TTF_Font* font = TTF_OpenFont(fontPath, 4);
    Hanzi hanzi = {"", font, fontSize, resolution, color, NULL, NULL, fallSpeed};
    hanzi.rect = malloc(sizeof(SDL_Rect));
    strcpy(hanzi.character, character);
    resizeHanzi(hanzi, fontSize, font);
    TTF_OpenFont("./Fonts/SimSun.ttf", hanzi.resolution);
    return hanzi;
}

//TODO: Implement freeHanzi

int main(int argc, char* argv[]) {
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Caishen Project", 
        SDL_WINDOWPOS_CENTERED,       
        SDL_WINDOWPOS_CENTERED,
        800,                          
        600,                          
        SDL_WINDOW_SHOWN              
    );

    if (!window) {
        printf("Erreur SDL_CreateWindow: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    int timer = 0;
    SDL_Color White = {255, 255, 255};
    Hanzi hanzi = createHanzi("你", "./Fonts/SimSun.ttf", 80, 4, White, 10);
    int quit = 0;
    SDL_Event event;

    while (!quit) {
        SDL_Surface* surfaceMessage = TTF_RenderUTF8_Solid(hanzi.font, hanzi.character, hanzi.color); //TODO: Implement draw
        if (!surfaceMessage) {
            printf("Erreur TTF_RenderUTF8_Solid: %s\n", TTF_GetError());
            break;
        }

        hanzi.texture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        SDL_FreeSurface(surfaceMessage); 

        if (!hanzi.texture) {
            printf("Erreur SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
            break;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, hanzi.texture, NULL, hanzi.rect);
        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(hanzi.texture); 

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1; 
            }
        }
        if (hanzi.resolution < 512){
            if(timer == 6){
                hanzi.resolution += 10;
                hanzi.font = TTF_OpenFont("./Fonts/SimSun.ttf", hanzi.resolution);
                timer = 0;
            }
            else{
                timer++;
            }
        }
        hanzi.rect->y += hanzi.fallSpeed;
        SDL_Delay(16);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}