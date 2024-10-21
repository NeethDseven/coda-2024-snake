#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define SEGMENT_SIZE 20
#define MAX_LENGTH 50
#define BORDER_THICKNESS 10

static const SDL_Color BORDER_COLOR = {178, 34, 34, 255};
static const SDL_Color SNAKE_COLORS[] = {
    {255, 0, 0, 255},
    {0, 255, 0, 255},
    {0, 0, 255, 255},
    {255, 255, 0, 255},
    {255, 165, 0, 255},
    {128, 0, 128, 255}
};
static const SDL_Color FOOD_COLOR = {255, 0, 0, 255};
static const SDL_Color BACKGROUND_COLOR = {139, 69, 19, 255};

#define FOOD_IMAGE_PATH "apple.png"
#define BACKGROUND_IMAGE_PATH "plateau.jpg"

typedef struct {
    SDL_Color color;
    SDL_Texture* texture;
} SnakeSkin;

typedef struct {
    SDL_Color color;
    SDL_Texture* texture;
} FoodSkin;

typedef struct {
    SDL_Color color;
    SDL_Texture* texture;
} BackgroundSkin;

void init_skins(SDL_Renderer* renderer, FoodSkin* foodSkin, BackgroundSkin* backgroundSkin);

#endif