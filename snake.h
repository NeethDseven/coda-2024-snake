#ifndef  __SNAKE_H__
#define __SNAKE_H__

typedef struct Segment {
    int x, y;                  
    struct Segment* suivant;   
} Segment;

Segment* creer_segment(int x, int y);

void ajouter_segment(Segment** tete, int x, int y);

void liberer_segments(Segment* tete);

void deplacer_segments(Segment* tete, int nouvelle_x, int nouvelle_y);

void generer_nourriture(Segment* tete, int* food_x, int* food_y);

int manger_nourriture(Segment* tete, int food_x, int food_y);

int position_occupee(Segment* tete, int x, int y);

int longueur_serpent(Segment* tete);

void drawCircle(SDL_Renderer* renderer, int x, int y, int radius);

void renderFruit(SDL_Renderer* renderer, SDL_Texture* texture, int fruitIndex, int x, int y);

SDL_Texture* loadTexture(const char* file, SDL_Renderer* renderer);


#endif