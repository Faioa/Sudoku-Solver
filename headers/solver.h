#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>


struct list_solutions{
	struct list_solutions *previous;
	int **actual;
	struct list_solutions *next;
	int nmb;
};


struct list_solutions* add_solution(struct list_solutions *list, int tab[9][9], int nmb);

void free_list(struct list_solutions *list);

int init(SDL_Renderer **renderer, SDL_Window **window, int width, int height);

int init_Grid(SDL_Renderer *renderer, SDL_Window *window);

int possible(int grille[9][9], int n, int m, int i);

int print_list(struct list_solutions* list, SDL_Renderer* renderer);

int print_grid(int grid[9][9], SDL_Renderer* renderer);

void solver(int grid[9][9], struct list_solutions** list, int *nmb);

int init_Buttons1(SDL_Renderer *renderer, SDL_Window* window);

int init_Buttons2(struct list_solutions *list, int nmb, SDL_Renderer* renderer, SDL_Window* window);

int selecting(struct list_solutions* list, int x, int y, int state, SDL_Renderer* renderer);
