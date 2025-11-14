#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <time.h>
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

Hanzi* createHanzi(char character[5], char fontPath[], int fontSize, int resolution, SDL_Color color, int x, int y, int fallSpeed) {
    TTF_Font* font = TTF_OpenFont(fontPath, 4);
    Hanzi *hanzi = malloc(sizeof(Hanzi));
    strcpy(hanzi->character, character);
    hanzi->fontSize = fontSize;
    hanzi->resolution = resolution;
    hanzi->color = color;
    hanzi->fallSpeed = fallSpeed;
    hanzi->rect = malloc(sizeof(SDL_Rect));
    hanzi->rect->x = x;
    hanzi->rect->y = y;
    resizeHanzi(*hanzi, fontSize, font);
    TTF_CloseFont(hanzi->font);
    hanzi->font = TTF_OpenFont(fontPath, hanzi->resolution);
    return hanzi;
}

int drawHanzi(Hanzi hanzi, SDL_Renderer* renderer){
    SDL_Surface* surfaceMessage = TTF_RenderUTF8_Solid(hanzi.font, hanzi.character, hanzi.color); //TODO: Implement drawHanzi
    if (!surfaceMessage) {
        printf("Erreur TTF_RenderUTF8_Solid: %s\n", TTF_GetError());
        return 1;
    }

    hanzi.texture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_FreeSurface(surfaceMessage); 

    if (!hanzi.texture) {
        printf("Erreur SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
        return 1;
    }

    SDL_RenderCopy(renderer, hanzi.texture, NULL, hanzi.rect);

    SDL_DestroyTexture(hanzi.texture); 

    return 0;
}

int freeHanzi(Hanzi* hanzi){
    TTF_CloseFont(hanzi->font);
    free(hanzi->rect);
    free(hanzi);
    return 0;
}

int main(int argc, char* argv[]) {
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    int WINWIDTH = 800;
    int WINHEIGHT = 600;

    SDL_Window* window = SDL_CreateWindow(
        "Caishen Project", 
        SDL_WINDOWPOS_CENTERED,       
        SDL_WINDOWPOS_CENTERED,
        WINWIDTH,                          
        WINHEIGHT,                          
        SDL_WINDOW_SHOWN              
    );

    if (!window) {
        printf("Erreur SDL_CreateWindow: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    SDL_Surface* icon = IMG_Load("./Assets/temple.png"); //TODO: Find license-free icon
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    Mix_Chunk *gBell = Mix_LoadWAV("./Sounds/bell.wav");
    Mix_VolumeChunk(gBell, 32);
    Mix_Music *gMusic = Mix_LoadMUS("./Sounds/bgMusic.wav");
    if (gMusic == NULL) {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }
    
    Mix_PlayMusic( gMusic, -1 );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Surface *backgroundIMG = IMG_Load("./Assets/background2.jpg");
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundIMG);

    int timer = 0;
    SDL_Color White = {255, 255, 255};
    SDL_Color Grey = {200, 200, 200};
    SDL_Color Black = {0, 0, 0};
    SDL_Color Red = {255, 0, 0};

    Hanzi *hanzi_array[16] = {0};

    SDL_Color colors[16] = {
        {255, 255, 255}, // White
        {255, 0, 0},     // Red
        {0, 255, 0},     // Green
        {0, 0, 255},     // Blue
        {255, 255, 0},   // Yellow
        {0, 255, 255},   // Cyan
        {255, 0, 255},   // Magenta
        {255, 165, 0},   // Orange
        {128, 0, 128},   // Purple
        {255, 192, 203}, // Pink
        {139, 69, 19},   // Brown
        {128, 128, 128}, // Gray
        {173, 216, 230}, // LightBlue
        {50, 205, 50},   // Lime
        {255, 215, 0},   // Gold
        {238, 130, 238}  // Violet
    };

    char *characters[16] = {
        "语", "文", "学", "中", "国", "汉", "字", "龙",
        "凤", "天", "地", "人", "和", "爱", "梦", "海"
    };

    for (int i = 0; i < 16; i++){
        Hanzi *hanzi = createHanzi(characters[i], "./Fonts/SimSun.ttf", 10, i, colors[i], i*50, 0, (i+1));
        hanzi_array[i] = hanzi;
    }

    TTF_Font *latinFont = TTF_OpenFont("./Fonts/WorkSans-Black.ttf", 256);
    SDL_Rect textRect;
    TTF_SizeText(latinFont, "Caishen Project", &textRect.w, &textRect.h);
    textRect.x = (WINWIDTH - textRect.w) / 2;
    textRect.y = (WINHEIGHT - textRect.h) / 2 + 20;

    int quit = 0;
    SDL_Event event;

    while (!quit) {

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        SDL_Surface *text = TTF_RenderText_Solid(latinFont, "Caishen Project", colors[9]);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_FreeSurface(text);
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);

        for (int i = 0; i < 16; i++){

            if(hanzi_array[i] == NULL){
                continue;
            }
            
            if (drawHanzi(*hanzi_array[i], renderer) == 1){
                break;
            }
        }

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1; 
            }
        }
        for (int i = 0; i < 16; i++){
            if(hanzi_array[i] == NULL){
                continue;
            }
            if (i[hanzi_array]->resolution < 512){
                if(timer == 6){
                    i[hanzi_array]->resolution += 2;
                    TTF_CloseFont(i[hanzi_array]->font);
                    i[hanzi_array]->font = TTF_OpenFont("./Fonts/SimSun.ttf", i[hanzi_array]->resolution);
                    timer = 0;
                }
                else{
                    timer++;
                }
            }
            if (i[hanzi_array]->rect->y > 600 - i[hanzi_array]->rect->w || i[hanzi_array]->rect->y < 0){
                i[hanzi_array]->fallSpeed *= -1;
                if (Mix_Playing(0)) {
                    Mix_HaltChannel(0);
                }
                Mix_PlayChannel(-1, gBell, 0);
            }
            
            i[hanzi_array]->rect->y += i[hanzi_array]->fallSpeed*1;
        }
        if (textRect.h > 50){
            textRect.w *= 0.99;
            textRect.h *= 0.99;
            textRect.x = (WINWIDTH - textRect.w) / 2;
            textRect.y = (WINHEIGHT - textRect.h) / 2+20;
        }
        SDL_Delay(16);
    }

    // Cleanup
    for (int i = 0; i < 16; i++){
        if(hanzi_array[i] != NULL){
            freeHanzi(hanzi_array[i]);
        }
    } 
    Mix_FreeChunk(gBell);
    Mix_FreeMusic(gMusic);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}