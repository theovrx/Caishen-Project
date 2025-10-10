#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init: %s\n", SDL_GetError());
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
        SDL_Quit();
        return 1;
    }

    int quit = 0;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1; 
            }
        }
        SDL_Delay(16); 
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
 