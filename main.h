#if !defined(MAIN_H)
#define MAIN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <limits.h>
#include "hanzi_struct.h"

/**
 * @brief Resizes a text rectangle based on font size and resolution scaling.
 * @param rect Pointer to SDL_Rect to store the calculated dimensions
 * @param text The text string to measure
 * @param fontSize The desired font size
 * @param resolution The resolution used for scaling calculations
 * @param font Pointer to the TTF_Font to use for text measurement
 * @return 0 on success, -1 if TTF_SizeText fails
 */
int resizeText(SDL_Rect* rect, char text[], int fontSize, int resolution, TTF_Font* font);

/**
 * @brief Resizes a Hanzi character rectangle based on font size.
 * @param hanzi The Hanzi structure containing character and rectangle information
 * @param fontSize The desired font size for scaling
 * @param font Pointer to the TTF_Font to use for UTF8 text measurement
 * @return 0 on success, -1 if TTF_SizeUTF8 fails
 */
int resizeHanzi(Hanzi hanzi, int fontSize, TTF_Font* font);

/**
 * @brief Creates and initializes a new Hanzi character structure.
 * @param character The UTF8 character string (up to 5 bytes)
 * @param fontPath Path to the font file to load
 * @param fontSize The desired font size
 * @param resolution The resolution for scaling calculations
 * @param color The SDL_Color for rendering the character
 * @param x Initial x-coordinate position
 * @param y Initial y-coordinate position
 * @param fallSpeed The speed at which the character falls
 * @return Pointer to newly allocated Hanzi structure, or NULL if allocation fails
 */
Hanzi* createHanzi(char character[5], char fontPath[], int fontSize, int resolution, SDL_Color color, int x, int y, int fallSpeed);

/**
 * @brief Renders a Hanzi character to the SDL renderer.
 * @param hanzi The Hanzi structure containing character and rendering information
 * @param renderer Pointer to the SDL_Renderer to draw to
 * @return 0 on success, 1 if rendering fails
 */
int drawHanzi(Hanzi hanzi, SDL_Renderer* renderer);

/**
 * @brief Frees all resources associated with a Hanzi structure.
 * @param hanzi Pointer to the Hanzi structure to free
 * @return 0 on success
 */
int freeHanzi(Hanzi* hanzi);

#endif