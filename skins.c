#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "config.h"

void init_skins(SDL_Renderer* renderer, FoodSkin* foodSkin, BackgroundSkin* backgroundSkin) {
    SDL_Surface* surface = IMG_Load(FOOD_IMAGE_PATH);
    if (surface) {
        foodSkin->texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    } else {
        printf("Erreur lors du chargement de l'image de la nourriture: %s\n", IMG_GetError());
    }

    surface = IMG_Load(BACKGROUND_IMAGE_PATH);
    if (surface) {
        backgroundSkin->texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    } else {
        printf("Erreur lors du chargement de l'image de l'arrière-plan: %s\n", IMG_GetError());
    }
}

void draw_border(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, BORDER_COLOR.r, BORDER_COLOR.g, BORDER_COLOR.b, BORDER_COLOR.a);

    // Dessiner la bordure supérieure
    SDL_Rect top_border = {0, 0, WINDOW_WIDTH, BORDER_THICKNESS};
    SDL_RenderFillRect(renderer, &top_border);

    // Dessiner la bordure inférieure
    SDL_Rect bottom_border = {0, WINDOW_HEIGHT - BORDER_THICKNESS, WINDOW_WIDTH, BORDER_THICKNESS};
    SDL_RenderFillRect(renderer, &bottom_border);

    // Dessiner la bordure gauche
    SDL_Rect left_border = {0, 0, BORDER_THICKNESS, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &left_border);

    // Dessiner la bordure droite
    SDL_Rect right_border = {WINDOW_WIDTH - BORDER_THICKNESS, 0, BORDER_THICKNESS, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &right_border);
}