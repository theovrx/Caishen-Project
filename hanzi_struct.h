#if !defined(HANZI_STRUCT_C)
#define HANZI_STRUCT_C

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
typedef struct Hanzi{
    char character[5];
    TTF_Font* font;
    int fontSize;
    int resolution;
    SDL_Color color;
    SDL_Texture *texture;
    SDL_Rect *rect;
    int fallSpeed;
    
}Hanzi;
#endif
