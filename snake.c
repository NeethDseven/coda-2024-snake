#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "snake.h"
#include "config.h"

Segment* creer_segment(int x, int y) {
    Segment* nouveau = (Segment*)malloc(sizeof(Segment));
    nouveau->x = x;
    nouveau->y = y;
    nouveau->suivant = NULL;
    return nouveau;
}

void ajouter_segment(Segment** tete, int x, int y) {
    Segment* nouveau = creer_segment(x, y);
    if (*tete == NULL) {
        *tete = nouveau;
    } else {
        Segment* temp = *tete;
        while (temp->suivant != NULL) {
            temp = temp->suivant;
        }
        temp->suivant = nouveau;
    }
}

void liberer_segments(Segment* tete) {
    Segment* temp;
    while (tete != NULL) {
        temp = tete;
        tete = tete->suivant;
        free(temp);
    }
}

void deplacer_segments(Segment* tete, int nouvelle_x, int nouvelle_y) {
    int ancien_x = tete->x;
    int ancien_y = tete->y;

    tete->x = nouvelle_x;
    tete->y = nouvelle_y;

    Segment* courant = tete->suivant;
    while (courant != NULL) {
        int temp_x = courant->x;
        int temp_y = courant->y;

        courant->x = ancien_x;
        courant->y = ancien_y;

        ancien_x = temp_x;
        ancien_y = temp_y;

        courant = courant->suivant;
    }
}

int position_occupee(Segment* tete, int x, int y) {
    Segment* courant = tete;
    while (courant != NULL) {
        if (courant->x == x && courant->y == y) {
            return 1;
        }
        courant = courant->suivant;
    }
    return 0;
}

void generer_nourriture(Segment* tete, int* food_x, int* food_y) {
    int valid_position = 0;
    while (!valid_position) {
        *food_x = (rand() % (WINDOW_WIDTH / SEGMENT_SIZE)) * SEGMENT_SIZE;
        *food_y = (rand() % (WINDOW_HEIGHT / SEGMENT_SIZE)) * SEGMENT_SIZE;

        valid_position = 1;
        Segment* courant = tete;
        while (courant != NULL) {
            if (courant->x == *food_x && courant->y == *food_y) {
                valid_position = 0;
                break;
            }
            courant = courant->suivant;
        }
    }
}

int manger_nourriture(Segment* tete, int food_x, int food_y) {
    if (tete->x == food_x && tete->y == food_y) {
        return 1;
    }
    return 0;
}

int longueur_serpent(Segment* tete) {
    int length = 0;
    Segment* courant = tete;
    while (courant != NULL) {
        length++;
        courant = courant->suivant;
    }
    return length;
}

void drawCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

int paused = 0;