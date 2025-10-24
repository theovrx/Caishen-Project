#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#if !defined(KANJI_STRUCT_C)
#define KANJI_STRUCT_C
struct Kanji
{
    int fall_speed;
    char character[5];
    SDL_Texture *texture;
    SDL_Rect *rect;
    
};
#endif
