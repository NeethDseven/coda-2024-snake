#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include "snake.h"
#include "config.h"

int compter_segments(Segment* tete) {
    int count = 0;
    Segment* courant = tete;
    while (courant != NULL) {
        count++;
        courant = courant->suivant;
    }
    return count;
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error SDL2 Init: %s\n", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        printf("Error initializing SDL_mixer: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("Error initializing SDL_ttf: %s\n", TTF_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!window || !renderer) {
        printf("Error creating window or renderer: %s\n", SDL_GetError());
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Texture* collisionTexture = IMG_LoadTexture(renderer, "wallCollision.jpg");
    SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, BACKGROUND_IMAGE_PATH);
    SDL_Texture* foodTexture = IMG_LoadTexture(renderer, FOOD_IMAGE_PATH);
    SDL_Texture* biteTexture = IMG_LoadTexture(renderer, "ownbite.jpg");
    SDL_Texture* pauseTexture = IMG_LoadTexture(renderer, "pause.png");
    SDL_Texture* victoryTexture = IMG_LoadTexture(renderer, "Win.png");

    if (!collisionTexture || !backgroundTexture || !foodTexture || !biteTexture || !pauseTexture || !victoryTexture) {
        printf("Error loading textures: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    Mix_Music* backgroundMusic = Mix_LoadMUS("MusiqueOfTheDoom.mid");
    Mix_Chunk* eatSound = Mix_LoadWAV("crockinappel.mp3");
    Mix_Chunk* collisionSound = Mix_LoadWAV("collision.mp3");
    Mix_Chunk* victorySound = Mix_LoadWAV("win_sound.mp3");

    if (!backgroundMusic || !eatSound || !collisionSound) {
        printf("Error loading sounds: %s\n", Mix_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    Mix_PlayMusic(backgroundMusic, -1);
    Mix_VolumeMusic(12);

    srand(time(NULL));
    Segment* tete = NULL;
    int initial_x = (WINDOW_WIDTH / 2) / SEGMENT_SIZE * SEGMENT_SIZE;
    int initial_y = (WINDOW_HEIGHT / 2) / SEGMENT_SIZE * SEGMENT_SIZE;
    ajouter_segment(&tete, initial_x, initial_y);
    ajouter_segment(&tete, initial_x - SEGMENT_SIZE, initial_y);
    ajouter_segment(&tete, initial_x - 2 * SEGMENT_SIZE, initial_y);

    int food_x, food_y;
    generer_nourriture(tete, &food_x, &food_y);

    int speed = SEGMENT_SIZE;
    int direction_x = speed;
    int direction_y = 0;
    int running = 1;
    int paused = 0;
    SDL_Event e;

    Uint32 startTime = SDL_GetTicks();

    TTF_Font* font = TTF_OpenFont("font.ttf", 22);
    if (!font) {
        printf("Error loading font: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.scancode == SDL_SCANCODE_P) {
                    paused = !paused;
                }
                if (!paused) {
                    if (e.key.keysym.scancode == SDL_SCANCODE_W && direction_y == 0) {
                        direction_x = 0; direction_y = -speed;
                    } else if (e.key.keysym.scancode == SDL_SCANCODE_S && direction_y == 0) {
                        direction_x = 0; direction_y = speed;
                    } else if (e.key.keysym.scancode == SDL_SCANCODE_A && direction_x == 0) {
                        direction_y = 0; direction_x = -speed;
                    } else if (e.key.keysym.scancode == SDL_SCANCODE_D && direction_x == 0) {
                        direction_y = 0; direction_x = speed;
                    }
                }
            }
        }
        if (paused) {
        SDL_RenderCopy(renderer, pauseTexture, NULL, NULL);
        }
        if (!paused) {
            int nouvelle_x = tete->x + direction_x;
            int nouvelle_y = tete->y + direction_y;

            if (nouvelle_x < 0 || nouvelle_x >= WINDOW_WIDTH || nouvelle_y < 0 || nouvelle_y >= WINDOW_HEIGHT) {
                Mix_PlayChannel(-1, collisionSound, 0);
                SDL_RenderCopy(renderer, collisionTexture, NULL, NULL);
                SDL_RenderPresent(renderer);
                Mix_HaltMusic();
                SDL_Delay(4000);
                break;
            }

            if (position_occupee(tete->suivant, nouvelle_x, nouvelle_y)) {
                Mix_PlayChannel(-1, collisionSound, 0);
                SDL_RenderCopy(renderer, biteTexture, NULL, NULL);
                SDL_RenderPresent(renderer);
                Mix_HaltMusic();
                SDL_Delay(4000);
                break;
            }

            if (manger_nourriture(tete, food_x, food_y)) {
                ajouter_segment(&tete, tete->x, tete->y);
                generer_nourriture(tete, &food_x, &food_y);
                Mix_PlayChannel(-1, eatSound, 0);
            } else {
                deplacer_segments(tete, nouvelle_x, nouvelle_y);
            }

            int segmentcompte = compter_segments(tete);
            if (segmentcompte >= 20) {
                Mix_PlayChannel(-1, victorySound, 0);
                SDL_RenderCopy(renderer, victoryTexture, NULL, NULL);
                SDL_RenderPresent(renderer);
                Mix_HaltMusic();
                SDL_Delay(8000);
                break;
            }
        } else {
            SDL_RenderCopy(renderer, pauseTexture, NULL, NULL);
        }

        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        Segment* courant = tete;
        while (courant != NULL) {
            int radius = SEGMENT_SIZE / 2;
            SDL_SetRenderDrawColor(renderer, rand() % 256, rand() % 256, rand() % 256, 255);
            drawCircle(renderer, courant->x + radius, courant->y + radius, radius);
            courant = courant->suivant;
        }

        SDL_Rect fruitRect = {food_x, food_y, SEGMENT_SIZE, SEGMENT_SIZE};
        SDL_RenderCopy(renderer, foodTexture, NULL, &fruitRect);

        int segmentcompte = compter_segments(tete);

        Uint32 elapsedTime = (SDL_GetTicks() - startTime) / 1000;
        char timerText[50];
        snprintf(timerText, sizeof(timerText), "Temps: %d s", elapsedTime);
        SDL_Color textColor = {255, 255, 255, 255};

        SDL_Surface* textSurface = TTF_RenderText_Solid(font, timerText, textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {WINDOW_WIDTH - textSurface->w - 10, 10, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        int nourritureRestante = 20 - segmentcompte;
        char nourritureText[50];
        snprintf(nourritureText, sizeof(nourritureText), "Nourriture restante: %d", nourritureRestante);
        SDL_Surface* nourritureSurface = TTF_RenderText_Solid(font, nourritureText, textColor);
        SDL_Texture* nourritureTexture = SDL_CreateTextureFromSurface(renderer, nourritureSurface);
        SDL_Rect nourritureRect = {10, 10, nourritureSurface->w, nourritureSurface->h};
        SDL_RenderCopy(renderer, nourritureTexture, NULL, &nourritureRect);
        SDL_FreeSurface(nourritureSurface);
        SDL_DestroyTexture(nourritureTexture);

        draw_border(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(120);
    }

    TTF_CloseFont(font);
    TTF_Quit();
    liberer_segments(tete);
    SDL_DestroyTexture(collisionTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(foodTexture);
    SDL_DestroyTexture(biteTexture);
    SDL_DestroyTexture(pauseTexture);
    SDL_DestroyTexture(victoryTexture);
    Mix_FreeChunk(victorySound);
    Mix_FreeChunk(collisionSound);
    Mix_FreeChunk(eatSound);
    Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}