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