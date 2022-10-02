#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <string.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "include/solver.h"

int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	int height = 850, width = 700;
	int statut = EXIT_FAILURE;
	int nmb, x, y, tmpx, tmpy, tmpbuttonx = -1, tmpbuttony = -1, m, n, state, tmpstate;
	int fich;
	struct list_solutions* sudoku = NULL;
	struct list_solutions* tmpsudoku = NULL;
	SDL_Event event;
	SDL_bool quit = SDL_FALSE;
	int grid[9][9];
	time_t t;
	char path[50] = "logs/log_file_sudoku_solver.txt";
	t = time(NULL);
	
	if(t == (time_t)-1)
	{
		perror("Time");
		return statut;
	}
	fich = open(path, O_CREAT | O_APPEND | O_RDONLY | O_WRONLY | O_TRUNC);
	if(fich == -1)
		fprintf(stderr, "Error during log file creation, now printing errors in terminal.\n\n");
	else
	{
		dup2(fich, STDERR_FILENO);
		close(fich);
		fprintf(stderr, " Log file on %s--------------------------------------\n\n", ctime(&t));
	}
	fprintf(stderr, "->Initialising window and renderer...\n");
	if(init(&renderer, &window, width, height) != 0)
		goto Quit;
	else
		fprintf(stderr, "->Done.\n\n");
	for(m = 0; m < 9; m++)
	{
		for(n = 0; n < 9; n++)
			grid[m][n] = 0;
	}
	tmpsudoku = add_solution(tmpsudoku, grid, nmb);
	if(tmpsudoku == NULL)
		goto Quit;
Init:
	state = INITIALISATION;
	nmb = 0;
	x = -1;
	y = -1;
	tmpx = -1;
	tmpy = -1;
	sudoku = NULL;
	fprintf(stderr, "->Initialising Buttons1...");
	if(init_Buttons1(renderer) != 0)
		goto Quit;
	else
		fprintf(stderr, "->Done.\n\n");
	fprintf(stderr, "->Printing grid...");
	if(print_grid(tmpsudoku, renderer) != 0)
		goto Quit;
	else
		fprintf(stderr, "->Done.\n\n");
	SDL_Delay(100);
	SDL_RenderPresent(renderer);	
	while(!quit)
	{
		if(SDL_WaitEvent(&event) == 0)
		{
			fprintf(stderr, "Error SDL_WaitEvent : %s\n\n", SDL_GetError());
			continue;
		}
		if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && ((event.key.keysym.mod == KMOD_ALT && event.key.keysym.sym == SDLK_F4) || event.key.keysym.sym == SDLK_ESCAPE)))
			quit = SDL_TRUE;
		else if(event.type == SDL_MOUSEBUTTONDOWN)
		{
			if(event.button.button == SDL_BUTTON_LEFT)
			{	
				if(state == INITIALISATION)
				{
					if(selecting(renderer, tmpsudoku, event.button.x, event.button.y, &x, &y, &tmpx, &tmpy, state) == 1)
					{
						tmpbuttonx = event.button.x;
						tmpbuttony = event.button.y;
					}
					else if(selecting(renderer, tmpsudoku, event.button.x, event.button.y, &x, &y, &tmpx, &tmpy, state) == 0)
					{
						tmpbuttonx = -1;
						tmpbuttony = -1;
					}
					else
						goto Quit;
				}
				else
				{
					if(selecting(renderer, sudoku, event.button.x, event.button.y, &x, &y, &tmpx, &tmpy, state) == 1)
					{
						tmpbuttonx = event.button.x;
						tmpbuttony = event.button.y;
					}
					else if(selecting(renderer, sudoku, event.button.x, event.button.y, &x, &y, &tmpx, &tmpy, state) == 0)
					{
						tmpbuttonx = -1;
						tmpbuttony = -1;
					}
					else
						goto Quit;
				}
				tmpstate = state;
				SDL_RenderPresent(renderer);
				
			}
		}
		else if(event.type == SDL_MOUSEBUTTONUP)
		{
			if(event.button.button == SDL_BUTTON_LEFT)
			{	
				if((tmpbuttonx < 18  || tmpbuttonx > 681 || tmpbuttony < 18 || tmpbuttony > 681) && tmpbuttonx != -1 && tmpbuttony != -1)
				{
					//selection des nombres et clic solution
					if(state == INITIALISATION)
					{
						if(deselecting(renderer, tmpsudoku, tmpbuttonx, tmpbuttony, state) == -1)
							goto Quit;
					}
					else
					{
						if(deselecting(renderer, sudoku, tmpbuttonx, tmpbuttony, state) == -1)
							goto Quit;
					}
					SDL_RenderPresent(renderer);
					if((tmpbuttonx < 605  && tmpbuttonx > 91 && tmpbuttony < 831 && tmpbuttony > 761) && state == INITIALISATION)
					{
					Solver:
						for(m=0; m<9; m++)
						{
							for(n=0; n<9; n++)
								grid[m][n]=(tmpsudoku->actual)[m][n];
						}
						if(valide_grid(grid) == 0)
						{
							fprintf(stderr, "->Solving sudoku grid.\n\n");
							solver(&sudoku, grid, &nmb);
						}
						if(nmb == 0)
							state = ERROR;
						else if(nmb < 10)
							state = SOLUTION1;
						else
							state = SOLUTION2;
						if(state == ERROR)
						{
							fprintf(stderr, "->Initialising Buttons2...");
							if(init_Buttons2(tmpsudoku, nmb, renderer) != 0)
								goto Quit;
							else
								fprintf(stderr, "Done.\n\n");
							fprintf(stderr, "->Printing grid...");
							if(print_grid(tmpsudoku, renderer) != 0)
								goto Quit;
							else
								fprintf(stderr, "->Done.\n\n");
						}
						else
						{
							fprintf(stderr, "->Initialising Buttons2...");
							if(init_Buttons2(sudoku, nmb, renderer) != 0)
								goto Quit;
							fprintf(stderr, "->Done.\n\n");
							fprintf(stderr, "->Printing grid... ");
							if(print_grid(sudoku, renderer) != 0)
								goto Quit;
							else
								fprintf(stderr, "->Done.\n\n");
						}
						SDL_Delay(100);
						SDL_RenderPresent(renderer);
					}
				}
				if(tmpbuttonx > 516 && tmpbuttonx < 587 && tmpbuttony > 700 && tmpbuttony < 774 && tmpstate == SOLUTION1 && (sudoku->next == NULL || sudoku->previous == NULL) && sudoku->next != sudoku->previous)
				{
					if(sudoku->next != NULL)
						sudoku = sudoku->next;
					else
						sudoku = sudoku->previous;
					fprintf(stderr, "->Initialising Buttons2...");
					if(init_Buttons2(sudoku, nmb, renderer) != 0)
						goto Quit;
					else
						fprintf(stderr, "->Done.\n\n");
					fprintf(stderr, "->Printing grid...");
					if(print_grid(sudoku, renderer) != 0)
						goto Quit;
					else
						fprintf(stderr, "->Done.\n\n");
					SDL_Delay(100);
					SDL_RenderPresent(renderer);
				}
				else if((tmpbuttonx > 479 && tmpbuttonx < 553 && tmpbuttony > 700 && tmpbuttony < 774) && tmpstate == SOLUTION1 && sudoku->next != NULL && sudoku->previous != NULL)
				{
					//chevron < si y a previous et next
					sudoku = sudoku->previous;
					fprintf(stderr, "->Initialising Buttons2...");
					if(init_Buttons2(sudoku, nmb, renderer) != 0)
						goto Quit;
					else
						fprintf(stderr, "->Done.\n\n");
					fprintf(stderr, "->Printing grid...");
					if(print_grid(sudoku, renderer) != 0)
						goto Quit;
					else
						fprintf(stderr, "->Done.\n\n");
					SDL_Delay(100);
					SDL_RenderPresent(renderer);
				}
				else if((tmpbuttonx > 558 && tmpbuttonx < 632 && tmpbuttony > 700 && tmpbuttony < 774) && tmpstate == SOLUTION1 && sudoku->next != NULL && sudoku->previous != NULL)
				{
					//chevron > si y a previous et next
					sudoku = sudoku->next;
					fprintf(stderr, "->Initialising Buttons2...");
					if(init_Buttons2(sudoku, nmb, renderer) != 0)
						goto Quit;
					else
						fprintf(stderr, "->Done.\n\n");
					fprintf(stderr, "->Printing grid...");
					if(print_grid(sudoku, renderer) != 0)
						goto Quit;
					else
						fprintf(stderr, "->Done.\n\n");
					SDL_Delay(100);
					SDL_RenderPresent(renderer);
				}
				if(tmpbuttonx > 575 && tmpbuttonx < 649 && tmpbuttony > 700 && tmpbuttony < 774 && tmpstate == SOLUTION2 && (sudoku->next == NULL || sudoku->previous == NULL) && sudoku->next != sudoku->previous)
				{
					//chevron < or > si y a pas de next ou pas de previous
					if(sudoku->next != NULL)
						sudoku = sudoku->next;
					else
						sudoku = sudoku->previous;
					fprintf(stderr, "->Initialising Buttons2...");
					if(init_Buttons2(sudoku, nmb, renderer) != 0)
						goto Quit;
					else
						fprintf(stderr, "->Done.\n\n");
					fprintf(stderr, "->Printing grid...");
					if(print_grid(sudoku, renderer) != 0)
						goto Quit;
					else
						fprintf(stderr, "->Done.\n\n");
					SDL_Delay(100);
					SDL_RenderPresent(renderer);
				}
				else if((tmpbuttonx > 538 && tmpbuttonx < 612 && tmpbuttony > 700 && tmpbuttony < 774) && tmpstate == SOLUTION2 && sudoku->next != NULL && sudoku->previous != NULL)
				{
					//chevron < si y a previous et next
					sudoku = sudoku->previous;
					fprintf(stderr, "->Initialising Buttons2...");
					if(init_Buttons2(sudoku, nmb, renderer) != 0)
						goto Quit;
					else
						fprintf(stderr, "->Done.\n\n");
					fprintf(stderr, "->Printing grid...");
					if(print_grid(sudoku, renderer) != 0)
						goto Quit;
					else
						fprintf(stderr, "->Done.\n\n");
					SDL_Delay(100);
					SDL_RenderPresent(renderer);
				}
				else if((tmpbuttonx > 617 && tmpbuttonx < 691 && tmpbuttony > 700 && tmpbuttony < 774) && tmpstate == SOLUTION2 && sudoku->next != NULL && sudoku->previous != NULL)
				{
					//chevron > si y a previous et next
					sudoku = sudoku->next;
					fprintf(stderr, "->Initialising Buttons2...");
					if(init_Buttons2(sudoku, nmb, renderer) != 0)
						goto Quit;
					else
						fprintf(stderr, "->Done.\n\n");
					fprintf(stderr, "->Printing grid...");
					if(print_grid(sudoku, renderer) != 0)
						goto Quit;
					else
						fprintf(stderr, "->Done.\n\n");
					SDL_Delay(100);
					SDL_RenderPresent(renderer);
				}
				
				else if(tmpbuttonx < 428 && tmpbuttonx > 270 && tmpbuttony < 841 && tmpbuttony > 779 && (tmpstate == ERROR || tmpstate == SOLUTION2 || tmpstate == SOLUTION1))
				{
					//tous les cancels
					free_list(sudoku);
					goto Init;
				}
			}
				
		}
		else if(event.type == SDL_KEYDOWN)
		{
			if(state == INITIALISATION && x != -1 && y != -1)
			{
				if(event.key.keysym.sym == SDLK_0 || event.key.keysym.sym == SDLK_BACKSPACE || event.key.keysym.sym == SDLK_DELETE || event.key.keysym.sym == SDLK_KP_0)
					(tmpsudoku->actual)[x][y] = 0;
				else if(event.key.keysym.sym == SDLK_1 || event.key.keysym.sym == SDLK_KP_1)
					(tmpsudoku->actual)[x][y] = 1;
				else if(event.key.keysym.sym == SDLK_2 || event.key.keysym.sym == SDLK_KP_2)
					(tmpsudoku->actual)[x][y] = 2;
				else if(event.key.keysym.sym == SDLK_3 || event.key.keysym.sym == SDLK_KP_3)
					(tmpsudoku->actual)[x][y] = 3;
				else if(event.key.keysym.sym == SDLK_4 || event.key.keysym.sym == SDLK_KP_4)
					(tmpsudoku->actual)[x][y] = 4;
				else if(event.key.keysym.sym == SDLK_5 || event.key.keysym.sym == SDLK_KP_5)
					(tmpsudoku->actual)[x][y] = 5;
				else if(event.key.keysym.sym == SDLK_6 || event.key.keysym.sym == SDLK_KP_6)
					(tmpsudoku->actual)[x][y] = 6;
				else if(event.key.keysym.sym == SDLK_7 || event.key.keysym.sym == SDLK_KP_7)
					(tmpsudoku->actual)[x][y] = 7;
				else if(event.key.keysym.sym == SDLK_8 || event.key.keysym.sym == SDLK_KP_8)
					(tmpsudoku->actual)[x][y] = 8;
				else if(event.key.keysym.sym == SDLK_9 || event.key.keysym.sym == SDLK_KP_9)
					(tmpsudoku->actual)[x][y] = 9;
				if(selecting(renderer, tmpsudoku, tmpx, tmpy, &x, &y, &tmpx, &tmpy, state) == -1)
					goto Quit;
				SDL_RenderPresent(renderer);
				
			}
			if((event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_RETURN2 || event.key.keysym.sym == SDLK_KP_ENTER) && state == INITIALISATION)
					goto Solver;
			if(state == SOLUTION1 || state == SOLUTION2)
			{
				if(sudoku->previous != NULL && event.key.keysym.sym == SDLK_LEFT)
				{
					sudoku = sudoku->previous;
					fprintf(stderr, "->Initialising Buttons2...");
					if(init_Buttons2(sudoku, nmb, renderer) != 0)
						goto Quit;
					else
						fprintf(stderr, "->Done.\n\n");
					fprintf(stderr, "->Printing grid...");
					if(print_grid(sudoku, renderer) != 0)
						goto Quit;
					else
						fprintf(stderr, "->Done.\n\n");
					SDL_Delay(100);
					SDL_RenderPresent(renderer);
				}
				else if(sudoku->next != NULL && event.key.keysym.sym == SDLK_RIGHT)
				{
					sudoku = sudoku->next;
					fprintf(stderr, "->Initialising Buttons2...");
					if(init_Buttons2(sudoku, nmb, renderer) != 0)
						goto Quit;
					else
						fprintf(stderr, "->Done.\n\n");
					fprintf(stderr, "->Printing grid...");
					if(print_grid(sudoku, renderer) != 0)
						goto Quit;
					else
						fprintf(stderr, "->Done.\n\n");
					SDL_Delay(100);
					SDL_RenderPresent(renderer);
				}
			}
			if(event.key.keysym.sym == SDLK_BACKSPACE && state != INITIALISATION)
			{
				free_list(sudoku);
				goto Init;
			}
				
		}
	}
	statut = EXIT_SUCCESS;
Quit:
	if(renderer != NULL)
		SDL_DestroyRenderer(renderer);
	if(window != NULL)
		SDL_DestroyWindow(window);
	SDL_Quit();
	if(sudoku != NULL)
		free_list(sudoku);
	if(tmpsudoku != NULL)
		free_list(tmpsudoku);
	fprintf(stderr, "\n->Exiting the program.");
	exit(statut);
}
