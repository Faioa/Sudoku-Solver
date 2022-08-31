#include "./headers/solver.h"


int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	int height = 850, width = 700;
	int statut = EXIT_FAILURE;
	int nmb;
	struct list_solutions* list = NULL;
	int sudoku[9][9]={{0,0,9,0,0,0,1,0,0},
			{0,6,0,0,0,0,0,8,0},
			{5,0,0,0,0,0,0,4,6},
			{0,0,0,0,0,0,0,0,0},
			{6,8,7,0,0,3,0,0,0},
			{0,0,0,0,0,5,4,0,2},
			{0,4,0,0,0,9,0,1,0},
			{0,0,6,0,0,0,3,0,0},
			{8,9,0,0,7,4,0,0,0}};

	if(init(&renderer, &window, width, height) != 0)
		goto Quit;
	if(init_Buttons1(renderer, window) != 0)
		goto Quit;
	print_grid(sudoku, renderer);
	SDL_RenderPresent(renderer);
	SDL_Delay(2000);


	solver(sudoku, &list, &nmb);
	if(nmb == 0)
		list = add_solution(list, sudoku, nmb);
	//list = list->next;
	init_Buttons2(list, nmb, renderer, window);
	print_list(list, renderer);
	SDL_RenderPresent(renderer);
	SDL_Delay(15000);
	statut = EXIT_SUCCESS;

Quit:
	if(renderer != NULL)
		SDL_DestroyRenderer(renderer);
	if(window != NULL)
		SDL_DestroyWindow(window);
	SDL_Quit();
	free_list(list);
	return statut;
}
