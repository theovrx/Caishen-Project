#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

struct Kanji
{
    int fall_speed;
    SDL_Texture *texture;
    SDL_Rect *rect;
    
};
