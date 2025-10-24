#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    struct my_struct
    {
        int a;
        float b;
    };
    
    
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

    TTF_Font* customFont = TTF_OpenFont("./Fonts/SimSun.ttf", 4);
    int resolution = 4;
    int timer = 0;
    if (!customFont) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Color White = {255, 255, 255};
    SDL_Rect Message_rect = {0, 0, 600, 150};
    int quit = 0;
    SDL_Event event;

    while (!quit) {
        SDL_Surface* surfaceMessage = TTF_RenderUTF8_Solid(customFont, "你好世界他她它", White); 
        if (!surfaceMessage) {
            printf("Erreur TTF_RenderUTF8_Solid: %s\n", TTF_GetError());
            break;
        }

        SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        SDL_FreeSurface(surfaceMessage); 

        if (!Message) {
            printf("Erreur SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
            break;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(Message); 

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1; 
            }
        }
        if (resolution < 512){
            if(timer == 6){
                resolution++;
                customFont = TTF_OpenFont("./Fonts/SimSun.ttf", resolution);
                timer = 0;
                Message_rect.y += 5;
            }
            else{
                timer++;
            }
        }
        SDL_Delay(16);
    }

    // Cleanup
    TTF_CloseFont(customFont);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}