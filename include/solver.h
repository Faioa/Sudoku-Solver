#ifndef SOLVER_H

#define SOLVER_H
#define INITIALISATION 0
#define SOLUTION1 1
#define SOLUTION2 2
#define ERROR 3


struct list_solutions{
	struct list_solutions *previous;
	int **actual;
	struct list_solutions *next;
	int nmb;
};

struct list_solutions* add_solution(struct list_solutions *list, int tab[9][9], int nmb);

void free_list(struct list_solutions *list);

int init(SDL_Renderer **renderer, SDL_Window **window, int width, int height);

int init_Grid(SDL_Renderer *renderer);

int possible(int grid[9][9], int n, int m, int i);

int print_grid(struct list_solutions* list, SDL_Renderer* renderer);

int valide_grid(int grid[9][9]);

void solver(struct list_solutions** list, int grid[9][9], int *nmb);

int init_Buttons1(SDL_Renderer *renderer);

int init_Buttons2(struct list_solutions *list, int nmb, SDL_Renderer* renderer);

int deselecting(SDL_Renderer* renderer, struct list_solutions* list, int x, int y, int state);

int selecting(SDL_Renderer* renderer, struct list_solutions* list, int x, int y, int* s, int* t, int* tmpx, int* tmpy, int state);

#endif
