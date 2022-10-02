#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <string.h>
#include "../include/solver.h"

int init(SDL_Renderer **renderer, SDL_Window **window, int width, int height)
{
	SDL_Surface *icon = NULL;

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
	{
		fprintf(stderr, "\nError SDL_Init : %s\n\n", SDL_GetError());
		return -1;
	}
	*window = SDL_CreateWindow("Sudoku Solver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if(*window == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateWindow : %s\n\n", SDL_GetError());
		return -1;
	}
	icon = SDL_LoadBMP("images/icon.bmp");
	if(icon == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		return -1;
	}
	SDL_SetWindowIcon(*window, icon);
	SDL_FreeSurface(icon);
	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL)
	{
		*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_SOFTWARE);
		if(renderer == NULL)
		{
			fprintf(stderr, "\nError SDL_CreateRenderer : %s\n\n", SDL_GetError());
			return -1;
		}
	}
	return 0;
}

int init_Grid(SDL_Renderer *renderer)
{
	SDL_Color beige = {229, 211, 118, 255};
	SDL_Color black = {0, 0, 0, 255};
	SDL_Rect rect1 = {16, 16, 666, 666};
	SDL_Rect rect2 = {238, 16, 222, 666};
	SDL_Rect rect3 = {16, 238, 666, 222};
	int x1 = 16, y1 = 16, x2 = 682, y2 = 682, i; 
	
	if(SDL_SetRenderDrawColor(renderer, beige.r, beige.g, beige.b, beige.a) != 0)
	{
		fprintf(stderr, "\nError SDL_SetRenderDrawColor : %s\n\n", SDL_GetError());
		return -1;
	}
	if(SDL_RenderClear(renderer) != 0)
	{
		fprintf(stderr, "\nError SDL_RenderClear : %s\n\n", SDL_GetError());
		return -1;
	}
	if(SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a) != 0)
	{
		fprintf(stderr, "\nError SDL_SetRenderDrawColor : %s\n\n", SDL_GetError());
		return -1;
	}
	if(SDL_RenderDrawRect(renderer, &rect1) != 0)
	{
		fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
		return -1;
	}
	if(SDL_RenderDrawRect(renderer, &rect2) != 0)
	{
		fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
		return -1;
	}
	if(SDL_RenderDrawRect(renderer, &rect3) != 0)
	{
		fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
		return -1;
	}
	rect1.x = 15;
	rect1.y = 15;
	rect1.h = 668;
	rect1.w = 668;
	
	rect2.x = 237;
	rect2.y = 15;
	rect2.h = 668;
	rect2.w = 224;
	
	rect3.x = 15;
	rect3.y = 237;
	rect3.h = 224;
	rect3.w = 668;
	if(SDL_RenderDrawRect(renderer, &rect1) != 0)
	{
		fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
		return -1;
	}
	if(SDL_RenderDrawRect(renderer, &rect2) != 0)
	{
		fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
		return -1;
	}
	if(SDL_RenderDrawRect(renderer, &rect3) != 0)
	{
		fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
		return -1;
	}
	rect1.x = 17;
	rect1.y = 17;
	rect1.h = 664;
	rect1.w = 664;
	
	rect2.x = 239;
	rect2.y = 17;
	rect2.h = 664;
	rect2.w = 220;
	
	rect3.x = 17;
	rect3.y = 239;
	rect3.h = 220;
	rect3.w = 664;
	if(SDL_RenderDrawRect(renderer, &rect1) != 0)
	{
		fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
		return -1;
	}
	if(SDL_RenderDrawRect(renderer, &rect2) != 0)
	{
		fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
		return -1;
	}
	if(SDL_RenderDrawRect(renderer, &rect3) != 0)
	{
		fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
		return -1;
	}
	for(i=1; i<9; i++)
	{
		if(i%3==2)
		{
			if(SDL_RenderDrawLine(renderer, x1+74*i-1, y1, x1+74*i-1, y2) != 0)
			{
				fprintf(stderr, "\nError RenderDrawLine : %s\n\n", SDL_GetError());
			return -1;
			}
		}
		else{	
			if(SDL_RenderDrawLine(renderer, x1+74*i, y1, x1+74*i, y2) != 0)
			{
				fprintf(stderr, "\nError RenderDrawLine : %s\n\n", SDL_GetError());
			return -1;
			}
			
		}
	}
	for(i=1; i<9; i++)
	{
		if(i%3==2)
		{
			if(SDL_RenderDrawLine(renderer, x1, y1+74*i-1, x2, y1+74*i-1) != 0)
			{
				fprintf(stderr, "\nError RenderDrawLine : %s\n\n", SDL_GetError());
			return -1;
			}
		}
		else
		{
			if(SDL_RenderDrawLine(renderer, x1, y1+74*i, x2, y1+74*i) != 0)
			{
				fprintf(stderr, "\nError RenderDrawLine : %s\n\n", SDL_GetError());
			return -1;
			}
		}
	}
	return 0;
}

int print_grid(struct list_solutions* list, SDL_Renderer* renderer)
{
	//Fonction qui affiche la solution dans la fenetre
	
	int x, y;
	int statut = -1;
	SDL_Color black = {0, 0, 0, 255};
	SDL_Rect rect = {18, 18, 72, 72};
	SDL_Rect dst;
	SDL_Surface *tmp = NULL;
	SDL_Texture *button1 = NULL;
	SDL_Texture *button2 = NULL;
	SDL_Texture *button3 = NULL;
	SDL_Texture *button4 = NULL;
	SDL_Texture *button5 = NULL;
	SDL_Texture *button6 = NULL;
	SDL_Texture *button7 = NULL;
	SDL_Texture *button8 = NULL;
	SDL_Texture *button9 = NULL;
	SDL_Texture *void_ = NULL;
	
	fprintf(stderr, "\nLoading images... ");
	tmp = SDL_LoadBMP("images/non_selected/void_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	void_ = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(void_ == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	tmp = SDL_LoadBMP("images/non_selected/1_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button1 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button1 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	tmp = SDL_LoadBMP("images/non_selected/2_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button2 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button2 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	tmp = SDL_LoadBMP("images/non_selected/3_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button3 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button3 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	tmp = SDL_LoadBMP("images/non_selected/4_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button4 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button4 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	tmp = SDL_LoadBMP("images/non_selected/5_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button5 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button5 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	tmp = SDL_LoadBMP("images/non_selected/6_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button6 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button6 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	tmp = SDL_LoadBMP("images/non_selected/7_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button7 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button7 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	tmp = SDL_LoadBMP("images/non_selected/8_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button8 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button8 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	tmp = SDL_LoadBMP("images/non_selected/9_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button9 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button9 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	fprintf(stderr, "Done.");
				
	if(SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a) != 0)
	{
		fprintf(stderr, "\nError SDL_SetRenderDrawColor : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	dst.w = rect.w;
	dst.h = rect.h;
	dst.y=rect.y;
	
	fprintf(stderr, "\nPrinting content... ");
	for(x=0; x<9; x++)
	{
		dst.x = rect.x;
		for(y=0; y<9; y++)
		{	
			if((list->actual)[x][y] == 1)
			{
				if(SDL_RenderCopy(renderer, button1, NULL, &dst) != 0)
				{
					fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
					goto Quit;
				}
			}
			
			if((list->actual)[x][y] == 2)
			{
				if(SDL_RenderCopy(renderer, button2, NULL, &dst) != 0)
				{
					fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
					goto Quit;
				}
			}
			
			if((list->actual)[x][y] == 3)
			{
				if(SDL_RenderCopy(renderer, button3, NULL, &dst) != 0)
				{
					fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
					goto Quit;
				}
			}
			
			if((list->actual)[x][y] == 4)
			{
				if(SDL_RenderCopy(renderer, button4, NULL, &dst) != 0)
				{
					fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
					goto Quit;
				}
			}
			if((list->actual)[x][y] == 5)
			{
				if(SDL_RenderCopy(renderer, button5, NULL, &dst) != 0)
				{
					fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
					goto Quit;
				}
			}
			if((list->actual)[x][y] == 6)
			{
				if(SDL_RenderCopy(renderer, button6, NULL, &dst) != 0)
				{
					fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
					goto Quit;
				}
			}
			if((list->actual)[x][y] == 7)
			{
				if(SDL_RenderCopy(renderer, button7, NULL, &dst) != 0)
				{
					fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
					goto Quit;
				}
			}
			if((list->actual)[x][y] == 8)
			{
				if(SDL_RenderCopy(renderer, button8, NULL, &dst) != 0)
				{
					fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
					goto Quit;
				}
			}
			if((list->actual)[x][y] == 9)
			{
				if(SDL_RenderCopy(renderer, button9, NULL, &dst) != 0)
				{
					fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
					goto Quit;
				}
			}
			if((list->actual)[x][y] == 0)
			{
				if(SDL_RenderCopy(renderer, void_, NULL, &dst) != 0)
				{
					fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
					goto Quit;
				}
			}
			if(y % 3 == 2)
				dst.x = dst.x + 73 + 3;
			else
				dst.x = dst.x + 73;
		}
		if(x % 3 == 2)
			dst.y = dst.y + 73 + 3;
		else
			dst.y = dst.y + 73;	
		
	}
	fprintf(stderr, "Done.\n");
	statut = 0;

Quit:
	if(button1 != NULL)
		SDL_DestroyTexture(button1);
	if(button2 != NULL)
		SDL_DestroyTexture(button2);
	if(button3 != NULL)
		SDL_DestroyTexture(button3);
	if(button4 != NULL)
		SDL_DestroyTexture(button4);
	if(button5 != NULL)
		SDL_DestroyTexture(button5);
	if(button6 != NULL)
		SDL_DestroyTexture(button6);
	if(button7 != NULL)
		SDL_DestroyTexture(button7);
	if(button8 != NULL)
		SDL_DestroyTexture(button8);
	if(button9 != NULL)
		SDL_DestroyTexture(button9);
	if(void_ != NULL)
		SDL_DestroyTexture(void_);	
	return statut;
}



int init_Buttons1(SDL_Renderer *renderer)
{
	SDL_Color black = {0, 0, 0, 255};
	int i;
	SDL_Rect rect1 = {21, 700, 60, 60};
	SDL_Rect rect2 = {91, 708+rect1.h, 516, 72};
	SDL_Rect dst;
	SDL_Surface *tmp = NULL;
	SDL_Texture *button1 = NULL;
	SDL_Texture *button2 = NULL;
	SDL_Texture *button3 = NULL;
	SDL_Texture *button4 = NULL;
	SDL_Texture *button5 = NULL;
	SDL_Texture *button6 = NULL;
	SDL_Texture *button7 = NULL;
	SDL_Texture *button8 = NULL;
	SDL_Texture *button9 = NULL;
	SDL_Texture *eraser = NULL;
	SDL_Texture *confirmation = NULL;
	
	fprintf(stderr, "\nInitialising grid... ");
	if(init_Grid(renderer) != 0)
		return -1;
	else
		fprintf(stderr, "Done.\n");
	if(SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a) != 0)
	{
		fprintf(stderr, "Error SDL_SetRenderDrawColor : %s\n\n", SDL_GetError());
		return -1;
	}
	fprintf(stderr, "Initialising buttons... ");
	for(i=0; i<10; i++)
	{
		if(SDL_RenderDrawRect(renderer, &rect1) != 0)
		{
			fprintf(stderr, "Error RenderDrawRect : %s\n\n", SDL_GetError());
			return -1;
		}
		dst.x = rect1.x+1;
		dst.y = rect1.y+1;
		dst.w = rect1.w-2;
		dst.h = rect1.h-2;
		if(i == 0)
		{
			tmp = SDL_LoadBMP("images/non_selected/1_non_selected.bmp");
			if(tmp == NULL)
			{
				fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
				return -1;
			}
			button1 = SDL_CreateTextureFromSurface(renderer, tmp);
			SDL_FreeSurface(tmp);
			if(button1 == NULL)
			{
				fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
				return -1;
			}
			if(SDL_RenderCopy(renderer, button1, NULL, &dst) != 0)
			{
				fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
				return -1;
			}
			SDL_DestroyTexture(button1);
		}
		
		if(i == 1)
		{
			tmp = SDL_LoadBMP("images/non_selected/2_non_selected.bmp");
			if(tmp == NULL)
			{
				fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
				return -1;
			}
			button2 = SDL_CreateTextureFromSurface(renderer, tmp);
			SDL_FreeSurface(tmp);
			if(button2 == NULL)
			{
				fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
				return -1;
			}
			if(SDL_RenderCopy(renderer, button2, NULL, &dst) != 0)
			{
				fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
				return -1;
			}
			SDL_DestroyTexture(button2);
		}
		
		if(i == 2)
		{
			tmp = SDL_LoadBMP("images/non_selected/3_non_selected.bmp");
			if(tmp == NULL)
			{
				fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
				return -1;
			}
			button3 = SDL_CreateTextureFromSurface(renderer, tmp);
			SDL_FreeSurface(tmp);
			if(button3 == NULL)
			{
				fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
				return -1;
			}
			if(SDL_RenderCopy(renderer, button3, NULL, &dst) != 0)
			{
				fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
				return -1;
			}
			SDL_DestroyTexture(button3);
		}
		
		if(i == 3)
		{
			tmp = SDL_LoadBMP("images/non_selected/4_non_selected.bmp");
			if(tmp == NULL)
			{
				fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
				return -1;
			}
			button4 = SDL_CreateTextureFromSurface(renderer, tmp);
			SDL_FreeSurface(tmp);
			if(button4 == NULL)
			{
				fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
				return -1;
			}
			if(SDL_RenderCopy(renderer, button4, NULL, &dst) != 0)
			{
				fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
				return -1;
			}
			SDL_DestroyTexture(button4);
		}
		if(i == 4)
		{
			tmp = SDL_LoadBMP("images/non_selected/5_non_selected.bmp");
			if(tmp == NULL)
			{
				fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
				return -1;
			}
			button5 = SDL_CreateTextureFromSurface(renderer, tmp);
			SDL_FreeSurface(tmp);
			if(button5 == NULL)
			{
				fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
				return -1;
			}
			if(SDL_RenderCopy(renderer, button5, NULL, &dst) != 0)
			{
				fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
				return -1;
			}
			SDL_DestroyTexture(button5);
		}
		if(i == 5)
		{
			tmp = SDL_LoadBMP("images/non_selected/6_non_selected.bmp");
			if(tmp == NULL)
			{
				fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
				return -1;
			}
			button6 = SDL_CreateTextureFromSurface(renderer, tmp);
			SDL_FreeSurface(tmp);
			if(button6 == NULL)
			{
				fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
				return -1;
			}
			if(SDL_RenderCopy(renderer, button6, NULL, &dst) != 0)
			{
				fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
				return -1;
			}
			SDL_DestroyTexture(button6);
		}
		if(i == 6)
		{
			tmp = SDL_LoadBMP("images/non_selected/7_non_selected.bmp");
			if(tmp == NULL)
			{
				fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
				return -1;
			}
			button7 = SDL_CreateTextureFromSurface(renderer, tmp);
			SDL_FreeSurface(tmp);
			if(button7 == NULL)
			{
				fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
				return -1;
			}
			if(SDL_RenderCopy(renderer, button7, NULL, &dst) != 0)
			{
				fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
				return -1;
			}
			SDL_DestroyTexture(button7);
		}
		if(i == 7)
		{
			tmp = SDL_LoadBMP("images/non_selected/8_non_selected.bmp");
			if(tmp == NULL)
			{
				fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
				return -1;
			}
			button8 = SDL_CreateTextureFromSurface(renderer, tmp);
			SDL_FreeSurface(tmp);
			if(button8 == NULL)
			{
				fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
				return -1;
			}
			if(SDL_RenderCopy(renderer, button8, NULL, &dst) != 0)
			{
				fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
				return -1;
			}
			SDL_DestroyTexture(button8);
		}
		if(i == 8)
		{
			tmp = SDL_LoadBMP("images/non_selected/9_non_selected.bmp");
			if(tmp == NULL)
			{
				fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
				return -1;
			}
			button9 = SDL_CreateTextureFromSurface(renderer, tmp);
			SDL_FreeSurface(tmp);
			if(button9 == NULL)
			{
				fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
				return -1;
			}
			if(SDL_RenderCopy(renderer, button9, NULL, &dst) != 0)
			{
				fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
				return -1;
			}
			SDL_DestroyTexture(button9);
		}
		if(i == 9)
		{
			tmp = SDL_LoadBMP("images/non_selected/eraser_non_selected.bmp");
			if(tmp == NULL)
			{
				fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
				return -1;
			}
			eraser = SDL_CreateTextureFromSurface(renderer, tmp);
			SDL_FreeSurface(tmp);
			if(eraser == NULL)
			{
				fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
				return -1;
			}
			if(SDL_RenderCopy(renderer, eraser, NULL, &dst) != 0)
			{
				fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
				return -1;
			}
			SDL_DestroyTexture(eraser);
		}
		
		rect1.x = rect1.x-1;
		rect1.y = rect1.y-1;
		rect1.w = rect1.w+2;
		rect1.h = rect1.h+2;
		if(SDL_RenderDrawRect(renderer, &rect1) != 0)
		{
			fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
			return -1;
		}
		rect1.x = rect1.x+rect1.w+5;
		rect1.y = rect1.y+1;
		rect1.w = rect1.w-2;
		rect1.h = rect1.h-2;
	}
	if(SDL_RenderDrawRect(renderer, &rect2) != 0)
	{
		fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
		return -1;
	}
	dst.x = rect2.x+1;
	dst.y = rect2.y+1;
	dst.w = rect2.w-2;
	dst.h = rect2.h-2;
	tmp = SDL_LoadBMP("images/non_selected/confirmation_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		return -1;
	}
	confirmation = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(confirmation == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		return -1;
	}
	if(SDL_RenderCopy(renderer, confirmation, NULL, &dst) != 0)
	{
		fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
		return -1;
	}
	SDL_DestroyTexture(confirmation);
	rect2.x = rect2.x-1;
	rect2.y = rect2.y-1;
	rect2.w = rect2.w+2;
	rect2.h = rect2.h+2;
	if(SDL_RenderDrawRect(renderer, &rect2) != 0)
	{
		fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
		return -1;
	}
	fprintf(stderr, "Done.\n");
	return 0;
}

int init_Buttons2(struct list_solutions *list, int nmb, SDL_Renderer* renderer)
{
	SDL_Color black = {0, 0, 0, 255};
	int jump = 5;
	int unit_grid = list->nmb % 10;
	int tens_grid = (list->nmb - unit_grid) / 10;
	int unit_nmb = nmb % 10;
	int tens_nmb = (nmb - unit_nmb) / 10;
	int statut = -1;
	SDL_Texture *first = NULL;
	SDL_Texture *second = NULL;
	SDL_Texture *third =  NULL;
	SDL_Texture *fourth = NULL;
	SDL_Rect rect1 = {68, 700, 404, 74};
	SDL_Rect rect2;
	SDL_Rect rect3;
	SDL_Rect rect4;
	SDL_Surface *tmp = NULL;
	SDL_Texture *button1 = NULL;
	SDL_Texture *button2 = NULL;
	SDL_Texture *button3 = NULL;
	SDL_Texture *button4 = NULL;
	SDL_Texture *button5 = NULL;
	SDL_Texture *button6 = NULL;
	SDL_Texture *button7 = NULL;
	SDL_Texture *button8 = NULL;
	SDL_Texture *button9 = NULL;
	SDL_Texture *button0 = NULL;
	SDL_Texture *solution = NULL;
	SDL_Texture *chevron_l = NULL;
	SDL_Texture *chevron_r = NULL;
	SDL_Texture *slash = NULL;
	SDL_Texture *cancel = NULL;
	SDL_Texture *error = NULL;

	fprintf(stderr, "\nLoading images... ");
	tmp = SDL_LoadBMP("images/0_.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button0 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button0 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	tmp = SDL_LoadBMP("images/non_selected/1_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button1 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button1 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	tmp = SDL_LoadBMP("images/non_selected/2_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button2 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button2 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	tmp = SDL_LoadBMP("images/non_selected/3_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button3 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button3 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	tmp = SDL_LoadBMP("images/non_selected/4_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button4 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button4 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	tmp = SDL_LoadBMP("images/non_selected/5_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button5 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button5 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	tmp = SDL_LoadBMP("images/non_selected/6_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button6 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button6 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	tmp = SDL_LoadBMP("images/non_selected/7_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button7 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button7 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	tmp = SDL_LoadBMP("images/non_selected/8_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button8 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button8 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	tmp = SDL_LoadBMP("images/non_selected/9_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	button9 = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(button9 == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	tmp = SDL_LoadBMP("images/non_selected/cancel_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	cancel = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(cancel == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	tmp = SDL_LoadBMP("images/solution.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	solution = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(solution == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	tmp = SDL_LoadBMP("images/non_selected/chevron_left_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	chevron_l = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(chevron_l == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	tmp = SDL_LoadBMP("images/non_selected/chevron_right_non_selected.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	chevron_r = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(chevron_r == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	tmp = SDL_LoadBMP("images/slash.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	slash = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(slash == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	tmp = SDL_LoadBMP("images/error.bmp");
	if(tmp == NULL)
	{
		fprintf(stderr, "\nError SDL_LoadBMP : %s\n\n", SDL_GetError());
		goto Quit;
	}
	error = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if(error == NULL)
	{
		fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s\n\n", SDL_GetError());
		goto Quit;
	}
	fprintf(stderr, "Done.\n");
		
	if(tens_grid == 0)
		first = button0;
	if(tens_grid == 1)
		first = button1;
	if(tens_grid == 2)
		first = button2;
	if(tens_grid == 3)
		first = button3;
	if(tens_grid == 4)
		first = button4;
	if(tens_grid == 5)
		first = button5;
	if(tens_grid == 6)
		first = button6;
	if(tens_grid == 7)
		first = button7;
	if(tens_grid == 8)
		first = button8;
	if(tens_grid == 9)
		first = button9;
	if(unit_grid == 0)
		second = button0;
	if(unit_grid == 1)
		second = button1;
	if(unit_grid == 2)
		second = button2;
	if(unit_grid == 3)
		second = button3;
	if(unit_grid == 4)
		second = button4;
	if(unit_grid == 5)
		second = button5;
	if(unit_grid == 6)
		second = button6;
	if(unit_grid == 7)
		second = button7;
	if(unit_grid == 8)
		second = button8;
	if(unit_grid == 9)
		second = button9;
	if(tens_nmb == 0)
		third = button0;
	if(tens_nmb == 1)
		third = button1;
	if(tens_nmb == 2)
		third = button2;
	if(tens_nmb == 3)
		third = button3;
	if(tens_nmb == 4)
		third = button4;
	if(tens_nmb == 5)
		third = button5;
	if(tens_nmb == 6)
		third = button6;
	if(tens_nmb == 7)
		third = button7;
	if(tens_nmb == 8)
		third = button8;
	if(tens_nmb == 9)
		third = button9;
	if(unit_nmb == 0)
		fourth = button0;
	if(unit_nmb == 1)
		fourth = button1;
	if(unit_nmb == 2)
		fourth = button2;
	if(unit_nmb == 3)
		fourth = button3;
	if(unit_nmb == 4)
		fourth = button4;
	if(unit_nmb == 5)
		fourth = button5;
	if(unit_nmb == 6)
		fourth = button6;
	if(unit_nmb == 7)
		fourth = button7;
	if(unit_nmb == 8)
		fourth = button8;
	if(unit_nmb == 9)
		fourth = button9;
		
	fprintf(stderr, "Initialising grid... ");	
	if(init_Grid(renderer) != 0)
		goto Quit;
	fprintf(stderr, "Done.\n");
	fprintf(stderr, "Printing buttons... ");	
	if(SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a) != 0)
	{
		fprintf(stderr, "\nError SDL_SetRenderDrawColor : %s\n\n", SDL_GetError());
		goto Quit;
	}
	rect4.x = 270;
	rect4.y = 779;
	rect4.w = 158;
	rect4.h = 62;
	if(SDL_RenderDrawRect(renderer, &rect4) != 0)
	{
		fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
		goto Quit;
	}
	rect4.x = rect4.x-1;
	rect4.y = rect4.y-1;
	rect4.w = rect4.w+2;
	rect4.h = rect4.h+2;
	if(SDL_RenderDrawRect(renderer, &rect4) != 0)
	{
		fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
		goto Quit;
	}
	rect4.x = rect4.x+2;
	rect4.y = rect4.y+2;
	rect4.w = rect4.w-4;
	rect4.h = rect4.h-4;
	if(SDL_RenderCopy(renderer, cancel, NULL, &rect4) != 0)
	{
		fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
		goto Quit;
	}
	
	if(nmb == 0)
	{
		rect1.x = 89;
		rect1.w = 518;
		
		if(SDL_RenderDrawRect(renderer, &rect1) != 0)
		{
			fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
			goto Quit;
		}
		rect1.x = rect1.x-1;
		rect1.y = rect1.y-1;
		rect1.w = rect1.w+2;
		rect1.h = rect1.h+2;
		if(SDL_RenderDrawRect(renderer, &rect1) != 0)
		{
			fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
			goto Quit;
		}
		rect1.x = rect1.x+2;
		rect1.y = rect1.y+2;
		rect1.w = rect1.w-4;
		rect1.h = rect1.h-4;
		if(SDL_RenderCopy(renderer, error, NULL, &rect1) != 0)
		{
			fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
			goto Quit;
		}
	}
	else
	{
		if(nmb < 10)
		{
			if(list->previous == NULL)
				rect1.x = rect1.x + 74/2;
			if(list->next == NULL)
				rect1.x = rect1.x + 74/2;
			if(SDL_RenderDrawRect(renderer, &rect1) != 0)
			{
				fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
				goto Quit;
			}
			rect1.x = rect1.x-1;
			rect1.y = rect1.y-1;
			rect1.w = rect1.w+2;
			rect1.h = rect1.h+2;
			if(SDL_RenderDrawRect(renderer, &rect1) != 0)
			{
				fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
				goto Quit;
			}	
		}
		else if(nmb > 9)
		{
			rect1.x = 13;
			rect1.w = 518;
			if(list->previous == NULL)
				rect1.x = rect1.x + 74/2;
			if(list->next == NULL)
				rect1.x = rect1.x + 74/2;
			if(SDL_RenderDrawRect(renderer, &rect1) != 0)
			{
				fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
				goto Quit;
			}
			rect1.x = rect1.x-1;
			rect1.y = rect1.y-1;
			rect1.w = rect1.w+2;
			rect1.h = rect1.h+2;
			if(SDL_RenderDrawRect(renderer, &rect1) != 0)
			{
				fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
				goto Quit;
			}
		}
		
		if(list->previous != NULL)
		{	
			rect2.x = rect1.x+rect1.w+jump+1;
			rect2.y = rect1.y+1;
			rect2.w = 74;
			rect2.h = 74;
			if(list->next != NULL)
			{
				rect3.x = rect2.x+rect2.w+jump;
				rect3.y = rect2.y;
				rect3.w = rect2.w;
				rect3.h = rect2.h;
			}
			if(SDL_RenderDrawRect(renderer, &rect2) != 0)
			{
				fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
				goto Quit;
			}
			rect2.x = rect2.x-1;
			rect2.y = rect2.y-1;
			rect2.w = rect2.w+2;
			rect2.h = rect2.h+2;
			if(SDL_RenderDrawRect(renderer, &rect2) != 0)
			{
				fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
				goto Quit;
			}
			rect2.x = rect2.x+2;
			rect2.y = rect2.y+2;
			rect2.w = rect2.w-4;
			rect2.h = rect2.h-4;
			if(SDL_RenderCopy(renderer, chevron_l, NULL, &rect2) != 0)
			{
				fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
				goto Quit;
			}
		}
			
		if(list->next != NULL)
		{
			if(list->previous == NULL)
			{
				rect3.x = rect1.x+rect1.w+jump+1;
				rect3.y = rect1.y+1;
				rect3.w = 74;
				rect3.h = 74;
			}
			if(SDL_RenderDrawRect(renderer, &rect3) != 0)
			{
				fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
				goto Quit;
			}
			rect3.x = rect3.x-1;
			rect3.y = rect3.y-1;
			rect3.w = rect3.w+2;
			rect3.h = rect3.h+2;
			if(SDL_RenderDrawRect(renderer, &rect3) != 0)
			{
				fprintf(stderr, "\nError RenderDrawRect : %s\n\n", SDL_GetError());
				goto Quit;
			}
			rect3.x = rect3.x+2;
			rect3.y = rect3.y+2;
			rect3.w = rect3.w-4;
			rect3.h = rect3.h-4;
			if(SDL_RenderCopy(renderer, chevron_r, NULL, &rect3) != 0)
			{
				fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
				goto Quit;
			}
		}
		
		rect1.x = rect1.x+16;
		rect1.y = rect1.y+2;
		rect1.w = 226;
		rect1.h = 72;
		if(SDL_RenderCopy(renderer, solution, NULL, &rect1) != 0)
		{
			fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
			goto Quit;
		}
		rect1.x = rect1.x + rect1.w + 5;
		rect1.y = (rect1.h - 64) / 2 + rect1.y;
		rect1.w = 64;
		rect1.h = 64;
		if(nmb > 9)
		{
			if(SDL_RenderCopy(renderer, first, NULL, &rect1) != 0)
			{
				fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
				goto Quit;
			}
			rect1.x = rect1.x + 48;
		}
		if(SDL_RenderCopy(renderer, second, NULL, &rect1) != 0)
		{
			fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
			goto Quit;
		}
		rect1.x = rect1.x + 48;
		if(SDL_RenderCopy(renderer, slash, NULL, &rect1) != 0)
		{
			fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
			goto Quit;
		}
		rect1.x = rect1.x + 45;
		if(nmb > 9)
		{
			if(SDL_RenderCopy(renderer, third, NULL, &rect1) != 0)
			{
				fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
				goto Quit;
			}
			rect1.x = rect1.x + 48;
		}
		if(SDL_RenderCopy(renderer, fourth, NULL, &rect1) != 0)
		{
			fprintf(stderr, "\nError SDL_RenderCopy : %s\n\n", SDL_GetError());
			goto Quit;
		}
	}
	fprintf(stderr, "Done.\n");
	statut = 0;
	
Quit :
	if(button1 != NULL)
		SDL_DestroyTexture(button1);
	if(button1 != NULL)
	SDL_DestroyTexture(button2);
	if(button1 != NULL)
	SDL_DestroyTexture(button3);
	if(button1 != NULL)
	SDL_DestroyTexture(button4);
	if(button1 != NULL)
	SDL_DestroyTexture(button5);
	if(button1 != NULL)
	SDL_DestroyTexture(button6);
	if(button1 != NULL)
	SDL_DestroyTexture(button7);
	if(button1 != NULL)
	SDL_DestroyTexture(button8);
	if(button1 != NULL)
	SDL_DestroyTexture(button9);
	if(button1 != NULL)SDL_DestroyTexture(button0);
	SDL_DestroyTexture(solution);
	if(button1 != NULL)
	SDL_DestroyTexture(slash);
	if(button1 != NULL)
	SDL_DestroyTexture(chevron_l);
	if(button1 != NULL)
	SDL_DestroyTexture(chevron_r);
	if(button1 != NULL)
	SDL_DestroyTexture(cancel);
	
	return statut;
}

int deselecting(SDL_Renderer* renderer, struct list_solutions* list, int x, int y, int state)
{
	int m, n;
	SDL_Surface *tmp = NULL;
	SDL_Texture *button = NULL;
	SDL_Rect dst;
	SDL_Rect grid = {18, 18, 72, 72};
	SDL_Rect buttons = {22, 701, 58, 58};
	SDL_Rect confirmation = {92, 769, 514, 70};
	
	if(state == INITIALISATION)
	{
		if(x > 17 && x < 682 && y > 17 && y < 682)
		{
			m = 0;
			n = 0;
			dst.w = grid.w;
			dst.h = grid.h;
			dst.y = grid.y;
			while(m < 10)
			{
				dst.x = grid.x;
				while(n < 10)
				{
					if(x >= dst.x && x <= dst.x+dst.w && y >= dst.y && y <= dst.y+dst.h)
					{
						if((list->actual)[m][n] == 0)
							tmp = SDL_LoadBMP("images/non_selected/void_non_selected.bmp");
						if((list->actual)[m][n] == 1)
							tmp = SDL_LoadBMP("images/non_selected/1_non_selected.bmp");
						if((list->actual)[m][n] == 2)
							tmp = SDL_LoadBMP("images/non_selected/2_non_selected.bmp");
						if((list->actual)[m][n] == 3)
							tmp = SDL_LoadBMP("images/non_selected/3_non_selected.bmp");
						if((list->actual)[m][n] == 4)
							tmp = SDL_LoadBMP("images/non_selected/4_non_selected.bmp");
						if((list->actual)[m][n] == 5)
							tmp = SDL_LoadBMP("images/non_selected/5_non_selected.bmp");
						if((list->actual)[m][n] == 6)
							tmp = SDL_LoadBMP("images/non_selected/6_non_selected.bmp");
						if((list->actual)[m][n] == 7)
							tmp = SDL_LoadBMP("images/non_selected/7_non_selected.bmp");
						if((list->actual)[m][n] == 8)
							tmp = SDL_LoadBMP("images/non_selected/8_non_selected.bmp");
						if((list->actual)[m][n] == 9)
							tmp = SDL_LoadBMP("images/non_selected/9_non_selected.bmp");
						if(tmp == NULL)
						{
							fprintf(stderr, "Error SDL_LoadBMP in \"deselecting\" function : %s\n\n", SDL_GetError());
							return -1;
						}
						button = SDL_CreateTextureFromSurface(renderer, tmp);
						SDL_FreeSurface(tmp);
						if(button == NULL)
						{
							fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"deselecting\" function : %s\n\n", SDL_GetError());
							return -1;
						}
						if(m%8 == 2)
							dst.h = dst.h + 1;
						if(m%8 == 6)
						{
							dst.y = dst.y - 1;
							dst.h = dst.h + 1;
						}
						if(n%8 == 2)
							dst.w = dst.w + 1;
						if(n%8 == 6)
						{
							dst.x = dst.x - 1;
							dst.w = dst.w + 1;
						}
						if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
						{
							fprintf(stderr, "Error SDL_RenderCopy in \"deselecting\" function : %s\n\n", SDL_GetError());
							return -1;
						}
						SDL_DestroyTexture(button);
						return 1;
					}
					
					if(n % 3 == 2)
						dst.x = dst.x + 73 + 3;
					else
						dst.x = dst.x + 73;
					n++;
				}
				if(m % 3 == 2)
					dst.y = dst.y + 73 + 3;
				else
					dst.y = dst.y + 73;
				m++;
				n = 0;
			}
		}
		else if(x > 21 && x < 679 && y > 700 && y < 758)
		{
			m = 0;
			dst.w = buttons.w;
			dst.h = buttons.h;
			dst.y = buttons.y;
			dst.x = buttons.x;
			while(m < 10)
			{
				if(x >= dst.x && x <= dst.x+dst.w)
				{
					if(m == 0)
						tmp = SDL_LoadBMP("images/non_selected/1_non_selected.bmp");
					if(m == 1)
						tmp = SDL_LoadBMP("images/non_selected/2_non_selected.bmp");
					if(m == 2)
						tmp = SDL_LoadBMP("images/non_selected/3_non_selected.bmp");
					if(m == 3)
						tmp = SDL_LoadBMP("images/non_selected/4_non_selected.bmp");
					if(m == 4)
						tmp = SDL_LoadBMP("images/non_selected/5_non_selected.bmp");
					if(m == 5)
						tmp = SDL_LoadBMP("images/non_selected/6_non_selected.bmp");
					if(m == 6)
						tmp = SDL_LoadBMP("images/non_selected/7_non_selected.bmp");
					if(m == 7)
						tmp = SDL_LoadBMP("images/non_selected/8_non_selected.bmp");
					if(m == 8)
						tmp = SDL_LoadBMP("images/non_selected/9_non_selected.bmp");
					if(m == 9)
						tmp = SDL_LoadBMP("images/non_selected/eraser_non_selected.bmp");
					if(tmp == NULL)
					{
						fprintf(stderr, "Error SDL_LoadBMP in \"deselecting\" function : %s\n\n", SDL_GetError());
						return -1;
					}
					button = SDL_CreateTextureFromSurface(renderer, tmp);
					SDL_FreeSurface(tmp);
					if(button == NULL)
					{
						fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"deselecting\" function : %s\n\n", SDL_GetError());
						return -1;
					}
					if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
					{
						fprintf(stderr, "Error SDL_RenderCopy in \"deselecting\" function : %s\n\n", SDL_GetError());
						return -1;
					}	
					SDL_DestroyTexture(button);
					return 1;
				}
					
				dst.x = dst.x + dst.w + 8;
				m++;
			}
		}
		else if(x > 91 && x < 605 && y > 761 && y < 831)
		{
			dst.w = confirmation.w;
			dst.h = confirmation.h;
			dst.y = confirmation.y;
			dst.x = confirmation.x;
			tmp = SDL_LoadBMP("images/non_selected/confirmation_non_selected.bmp");
			if(tmp == NULL)
			{
				fprintf(stderr, "Error SDL_LoadBMP in \"deselecting\" function : %s\n\n", SDL_GetError());
				return -1;
			}
			button = SDL_CreateTextureFromSurface(renderer, tmp);
			SDL_FreeSurface(tmp);
			if(button == NULL)
			{
				fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"deselecting\" function : %s\n\n", SDL_GetError());
				return -1;
			}
			if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
			{
				fprintf(stderr, "Error SDL_RenderCopy in \"deselecting\" function : %s\n\n", SDL_GetError());
				return -1;
			}
			SDL_DestroyTexture(button);
			return 1;
		}	
	}
	else if(state == SOLUTION1)
	{
		if(list->next != NULL && list->previous != NULL)
		{
			if(x > 479 && x < 553 && y > 700 && y < 774)
			{
				dst.x = 480;
				dst.y = 701;
				dst.w = 72;
				dst.h = 72;
				tmp = SDL_LoadBMP("images/non_selected/chevron_left_non_selected.bmp");
				if(tmp == NULL)
				{
					fprintf(stderr, "Error SDL_LoadBMP in \"deselecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				button = SDL_CreateTextureFromSurface(renderer, tmp);
				SDL_FreeSurface(tmp);
				if(button == NULL)
				{
					fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"deselecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
				{
					fprintf(stderr, "Error SDL_RenderCopy in \"deselecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				SDL_DestroyTexture(button);
				return 1;
				
			}
			if(x > 558 && x < 632 && y > 700 && y < 774)
			{
				dst.x = 559;
				dst.y = 701;
				dst.w = 72;
				dst.h = 72;
				tmp = SDL_LoadBMP("images/non_selected/chevron_right_non_selected.bmp");
				if(tmp == NULL)
				{
					fprintf(stderr, "Error SDL_LoadBMP in \"deselecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				button = SDL_CreateTextureFromSurface(renderer, tmp);
				SDL_FreeSurface(tmp);
				if(button == NULL)
				{
					fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"deselecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
				{
					fprintf(stderr, "Error SDL_RenderCopy in \"deselecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				SDL_DestroyTexture(button);
				return 1;
				
			}
		}
		else if(list->next != NULL || list->previous != NULL)
		{
			if(x > 516 && x < 587 && y > 700 && y < 774)
			{
				dst.x = 517;
				dst.y = 701;
				dst.w = 72;
				dst.h = 72;
				if(list->next != NULL)
					tmp = SDL_LoadBMP("images/non_selected/chevron_right_non_selected.bmp");
				if(list->previous != NULL)
					tmp = SDL_LoadBMP("images/non_selected/chevron_left_non_selected.bmp");
				if(tmp == NULL)
				{
					fprintf(stderr, "Error SDL_LoadBMP in \"deselecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				button = SDL_CreateTextureFromSurface(renderer, tmp);
				SDL_FreeSurface(tmp);
				if(button == NULL)
				{
					fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"deselecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
				{
					fprintf(stderr, "Error SDL_RenderCopy in \"deselecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				SDL_DestroyTexture(button);
				return 1;
			}
		}
	}
	else if(state == SOLUTION2)
	{
		if(list->next != NULL && list->previous != NULL)
		{
			if(x > 538 && x < 612 && y > 700 && y < 774)
			{
				dst.x = 539;
				dst.y = 701;
				dst.w = 72;
				dst.h = 72;
				tmp = SDL_LoadBMP("images/non_selected/chevron_left_non_selected.bmp");
				if(tmp == NULL)
				{
					fprintf(stderr, "Error SDL_LoadBMP in \"deselecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				button = SDL_CreateTextureFromSurface(renderer, tmp);
				SDL_FreeSurface(tmp);
				if(button == NULL)
				{
					fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"deselecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
				{
					fprintf(stderr, "Error SDL_RenderCopy in \"deselecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				SDL_DestroyTexture(button);
				return 1;
				
			}
			if(x > 617 && x < 691 && y > 700 && y < 774)
			{
				dst.x = 618;
				dst.y = 701;
				dst.w = 72;
				dst.h = 72;
				tmp = SDL_LoadBMP("images/non_selected/chevron_right_non_selected.bmp");
				if(tmp == NULL)
				{
					fprintf(stderr, "Error SDL_LoadBMP in \"deselecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				button = SDL_CreateTextureFromSurface(renderer, tmp);
				SDL_FreeSurface(tmp);
				if(button == NULL)
				{
					fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"deselecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
				{
					fprintf(stderr, "Error SDL_RenderCopy in \"deselecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				SDL_DestroyTexture(button);
				return 1;
				
			}
		}
		else if(list->next != NULL || list->previous != NULL)
		{
			if(x > 575 && x < 649 && y > 700 && y < 774)
			{
				dst.x = 576;
				dst.y = 701;
				dst.w = 72;
				dst.h = 72;
				if(list->next != NULL)
					tmp = SDL_LoadBMP("images/non_selected/chevron_right_non_selected.bmp");
				if(list->previous != NULL)
					tmp = SDL_LoadBMP("images/non_selected/chevron_left_non_selected.bmp");
				if(tmp == NULL)
				{
					fprintf(stderr, "Error SDL_LoadBMP in \"deselecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				button = SDL_CreateTextureFromSurface(renderer, tmp);
				SDL_FreeSurface(tmp);
				if(button == NULL)
				{
					fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"deselecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
				{
					fprintf(stderr, "Error SDL_RenderCopy in \"deselecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				SDL_DestroyTexture(button);
				return 1;
			}
		}
	}
	if(state != INITIALISATION)
	{
		if(x < 428 && x > 270 && y < 841 && y > 779)
		{
			dst.x = 271;
			dst.y = 780;
			dst.w = 156;
			dst.h = 60;
			tmp = SDL_LoadBMP("images/non_selected/cancel_non_selected.bmp");
			if(tmp == NULL)
			{
				fprintf(stderr, "Error SDL_LoadBMP in \"deselecting\" function : %s\n\n", SDL_GetError());
				return -1;
			}
			button = SDL_CreateTextureFromSurface(renderer, tmp);
			SDL_FreeSurface(tmp);
			if(button == NULL)
			{
				fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"deselecting\" function : %s\n\n", SDL_GetError());
				return -1;
			}
			if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
			{
				fprintf(stderr, "Error SDL_RenderCopy in \"deselecting\" function : %s\n\n", SDL_GetError());
				return -1;
			}
			SDL_DestroyTexture(button);
			return 1;
		}	
	}
	
	return 0;
}

int selecting(SDL_Renderer* renderer, struct list_solutions* list, int x, int y, int* s, int* t, int* tmpx, int* tmpy, int state)
{
	int m, n;
	int* trash = malloc(sizeof(int));
	*trash = -1;
	SDL_Surface *tmp = NULL;
	SDL_Texture *button = NULL;
	SDL_Rect dst;
	SDL_Rect grid = {18, 18, 72, 72};
	SDL_Rect buttons = {22, 701, 58, 58};
	SDL_Rect confirmation = {92, 769, 514, 70};
	
	if(state == INITIALISATION)
	{
		if(x > 17 && x < 682 && y > 17 && y < 682)
		{
			m = 0;
			n = 0;
			dst.w = grid.w;
			dst.h = grid.h;
			dst.y = grid.y;
			while(m < 10)
			{
				dst.x = grid.x;
				while(n < 10)
				{
					if(x >= dst.x && x <= dst.x+dst.w && y >= dst.y && y <= dst.y+dst.h)
					{
						if(*s != -1 && *t != -1)
						{
							deselecting(renderer, list, *tmpx, *tmpy, state);
							if(*s == m && *t == n)
							{
								*s = -1;
								*t = -1;
								*tmpx = -1;
								*tmpy = -1;
							}
						}
						if((list->actual)[m][n] == 0)
							tmp = SDL_LoadBMP("images/selected/void_selected.bmp");
						if((list->actual)[m][n] == 1)
							tmp = SDL_LoadBMP("images/selected/1_selected.bmp");
						if((list->actual)[m][n] == 2)
							tmp = SDL_LoadBMP("images/selected/2_selected.bmp");
						if((list->actual)[m][n] == 3)
							tmp = SDL_LoadBMP("images/selected/3_selected.bmp");
						if((list->actual)[m][n] == 4)
							tmp = SDL_LoadBMP("images/selected/4_selected.bmp");
						if((list->actual)[m][n] == 5)
							tmp = SDL_LoadBMP("images/selected/5_selected.bmp");
						if((list->actual)[m][n] == 6)
							tmp = SDL_LoadBMP("images/selected/6_selected.bmp");
						if((list->actual)[m][n] == 7)
							tmp = SDL_LoadBMP("images/selected/7_selected.bmp");
						if((list->actual)[m][n] == 8)
							tmp = SDL_LoadBMP("images/selected/8_selected.bmp");
						if((list->actual)[m][n] == 9)
							tmp = SDL_LoadBMP("images/selected/9_selected.bmp");
						if(tmp == NULL)
						{
							fprintf(stderr, "Error SDL_LoadBMP in \"selecting\" function : %s\n\n", SDL_GetError());
							return -1;
						}
						button = SDL_CreateTextureFromSurface(renderer, tmp);
						SDL_FreeSurface(tmp);
						if(button == NULL)
						{
							fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"selecting\" function : %s\n\n", SDL_GetError());
							return -1;
						}
						if(m%8 == 2)
							dst.h = dst.h + 1;
						if(m%8 == 6)
						{
							dst.y = dst.y - 1;
							dst.h = dst.h + 1;
						}
						if(n%8 == 2)
							dst.w = dst.w + 1;
						if(n%8 == 6)
						{
							dst.x = dst.x - 1;
							dst.w = dst.w + 1;
						}
						if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
						{
							fprintf(stderr, "Error SDL_RenderCopy in \"selecting\" function : %s\n\n", SDL_GetError());
							return -1;
						}
						SDL_DestroyTexture(button);
						*s = m;
						*t = n;
						*tmpx = x;
						*tmpy = y;
						return 1;
					}
					
					if(n % 3 == 2)
						dst.x = dst.x + 73 + 3;
					else
						dst.x = dst.x + 73;
					n++;
				}
				if(m % 3 == 2)
					dst.y = dst.y + 73 + 3;
				else
					dst.y = dst.y + 73;
				m++;
				n = 0;
			}
		}
		else if(x > 21 && x < 679 && y > 700 && y < 758)
		{
			m = 0;
			dst.w = buttons.w;
			dst.h = buttons.h;
			dst.y = buttons.y;
			dst.x = buttons.x;
			while(m < 10)
			{
				if(x >= dst.x && x <= dst.x+dst.w)
				{
					if(m == 0)
						tmp = SDL_LoadBMP("images/selected/1_selected.bmp");
					if(m == 1)
						tmp = SDL_LoadBMP("images/selected/2_selected.bmp");
					if(m == 2)
						tmp = SDL_LoadBMP("images/selected/3_selected.bmp");
					if(m == 3)
						tmp = SDL_LoadBMP("images/selected/4_selected.bmp");
					if(m == 4)
						tmp = SDL_LoadBMP("images/selected/5_selected.bmp");
					if(m == 5)
						tmp = SDL_LoadBMP("images/selected/6_selected.bmp");
					if(m == 6)
						tmp = SDL_LoadBMP("images/selected/7_selected.bmp");
					if(m == 7)
						tmp = SDL_LoadBMP("images/selected/8_selected.bmp");
					if(m == 8)
						tmp = SDL_LoadBMP("images/selected/9_selected.bmp");
					if(m == 9)
						tmp = SDL_LoadBMP("images/selected/eraser_selected.bmp");
					if(tmp == NULL)
					{
						fprintf(stderr, "Error SDL_LoadBMP in \"selecting\" function : %s\n\n", SDL_GetError());
						return -1;
					}
					button = SDL_CreateTextureFromSurface(renderer, tmp);
					SDL_FreeSurface(tmp);
					if(button == NULL)
					{
						fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"selecting\" function : %s\n\n", SDL_GetError());
						return -1;
					}
					if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
					{
						fprintf(stderr, "Error SDL_RenderCopy in \"selecting\" function : %s\n\n", SDL_GetError());
						return -1;
					}	
					SDL_DestroyTexture(button);
					if(*s != -1 && *t != -1)
					{
						if(m < 9)
							(list->actual)[*s][*t] = m + 1;
						else
							(list->actual)[*s][*t] = 0;
						selecting(renderer, list, *tmpx, *tmpy, trash, trash, trash, trash, state);
					}
					return 1;
				}
					
				dst.x = dst.x + dst.w + 8;
				m++;
			}
		}
		else if(x > 91 && x < 605 && y > 761 && y < 831)
		{
			dst.w = confirmation.w;
			dst.h = confirmation.h;
			dst.y = confirmation.y;
			dst.x = confirmation.x;
			tmp = SDL_LoadBMP("images/selected/confirmation_selected.bmp");
			if(tmp == NULL)
			{
				fprintf(stderr, "Error SDL_LoadBMP in \"selecting\" function : %s\n\n", SDL_GetError());
				return -1;
			}
			button = SDL_CreateTextureFromSurface(renderer, tmp);
			SDL_FreeSurface(tmp);
			if(button == NULL)
			{
				fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"selecting\" function : %s\n\n", SDL_GetError());
				return -1;
			}
			if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
			{
				fprintf(stderr, "Error SDL_RenderCopy in \"selecting\" function : %s\n\n", SDL_GetError());
				return -1;
			}
			SDL_DestroyTexture(button);
			return 1;
		}	
	}
	else if(state == SOLUTION1)
	{
		if(list->next != NULL && list->previous != NULL)
		{
			if(x > 479 && x < 553 && y > 700 && y < 774)
			{
				dst.x = 480;
				dst.y = 701;
				dst.w = 72;
				dst.h = 72;
				tmp = SDL_LoadBMP("images/selected/chevron_left_selected.bmp");
				if(tmp == NULL)
				{
					fprintf(stderr, "Error SDL_LoadBMP in \"selecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				button = SDL_CreateTextureFromSurface(renderer, tmp);
				SDL_FreeSurface(tmp);
				if(button == NULL)
				{
					fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"selecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
				{
					fprintf(stderr, "Error SDL_RenderCopy in \"selecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				SDL_DestroyTexture(button);
				return 1;
				
			}
			if(x > 558 && x < 632 && y > 700 && y < 774)
			{
				dst.x = 559;
				dst.y = 701;
				dst.w = 72;
				dst.h = 72;
				tmp = SDL_LoadBMP("images/selected/chevron_right_selected.bmp");
				if(tmp == NULL)
				{
					fprintf(stderr, "Error SDL_LoadBMP in \"selecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				button = SDL_CreateTextureFromSurface(renderer, tmp);
				SDL_FreeSurface(tmp);
				if(button == NULL)
				{
					fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"selecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
				{
					fprintf(stderr, "Error SDL_RenderCopy in \"selecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				SDL_DestroyTexture(button);
				return 1;
				
			}
		}
		else if(list->next != NULL || list->previous != NULL)
		{
			if(x > 516 && x < 587 && y > 700 && y < 774)
			{
				dst.x = 517;
				dst.y = 701;
				dst.w = 72;
				dst.h = 72;
				if(list->next != NULL)
					tmp = SDL_LoadBMP("images/selected/chevron_right_selected.bmp");
				if(list->previous != NULL)
					tmp = SDL_LoadBMP("images/selected/chevron_left_selected.bmp");
				if(tmp == NULL)
				{
					fprintf(stderr, "Error SDL_LoadBMP in \"selecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				button = SDL_CreateTextureFromSurface(renderer, tmp);
				SDL_FreeSurface(tmp);
				if(button == NULL)
				{
					fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"selecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
				{
					fprintf(stderr, "Error SDL_RenderCopy in \"selecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				SDL_DestroyTexture(button);
				return 1;
			}
		}
	}
	else if(state == SOLUTION2)
	{
		if(list->next != NULL && list->previous != NULL)
		{
			if(x > 538 && x < 612 && y > 700 && y < 774)
			{
				dst.x = 539;
				dst.y = 701;
				dst.w = 72;
				dst.h = 72;
				tmp = SDL_LoadBMP("images/selected/chevron_left_selected.bmp");
				if(tmp == NULL)
				{
					fprintf(stderr, "Error SDL_LoadBMP in \"selecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				button = SDL_CreateTextureFromSurface(renderer, tmp);
				SDL_FreeSurface(tmp);
				if(button == NULL)
				{
					fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"selecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
				{
					fprintf(stderr, "Error SDL_RenderCopy in \"selecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				SDL_DestroyTexture(button);
				return 1;
				
			}
			if(x > 617 && x < 691 && y > 700 && y < 774)
			{
				dst.x = 618;
				dst.y = 701;
				dst.w = 72;
				dst.h = 72;
				tmp = SDL_LoadBMP("images/selected/chevron_right_selected.bmp");
				if(tmp == NULL)
				{
					fprintf(stderr, "Error SDL_LoadBMP in \"selecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				button = SDL_CreateTextureFromSurface(renderer, tmp);
				SDL_FreeSurface(tmp);
				if(button == NULL)
				{
					fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"selecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
				{
					fprintf(stderr, "Error SDL_RenderCopy in \"selecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				SDL_DestroyTexture(button);
				return 1;
				
			}
		}
		else if(list->next != NULL || list->previous != NULL)
		{
			if(x > 575 && x < 649 && y > 700 && y < 774)
			{
				dst.x = 576;
				dst.y = 701;
				dst.w = 72;
				dst.h = 72;
				if(list->next != NULL)
					tmp = SDL_LoadBMP("images/selected/chevron_right_selected.bmp");
				if(list->previous != NULL)
					tmp = SDL_LoadBMP("images/selected/chevron_left_selected.bmp");
				if(tmp == NULL)
				{
					fprintf(stderr, "Error SDL_LoadBMP in \"selecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				button = SDL_CreateTextureFromSurface(renderer, tmp);
				SDL_FreeSurface(tmp);
				if(button == NULL)
				{
					fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"selecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
				{
					fprintf(stderr, "Error SDL_RenderCopy in \"selecting\" function : %s\n\n", SDL_GetError());
					return -1;
				}
				SDL_DestroyTexture(button);
				return 1;
			}
		}
	}
	if(state != INITIALISATION)
	{
		if(x < 428 && x > 270 && y < 841 && y > 779)
		{
			dst.x = 271;
			dst.y = 780;
			dst.w = 156;
			dst.h = 60;
			tmp = SDL_LoadBMP("images/selected/cancel_selected.bmp");
			if(tmp == NULL)
			{
				fprintf(stderr, "Error SDL_LoadBMP in \"selecting\" function : %s\n\n", SDL_GetError());
				return -1;
			}
			button = SDL_CreateTextureFromSurface(renderer, tmp);
			SDL_FreeSurface(tmp);
			if(button == NULL)
			{
				fprintf(stderr, "Error SDL_CreateTextureFromSurface in \"selecting\" function : %s\n\n", SDL_GetError());
				return -1;
			}
			if(SDL_RenderCopy(renderer, button, NULL, &dst) != 0)
			{
				fprintf(stderr, "Error SDL_RenderCopy in \"selecting\" function : %s\n\n", SDL_GetError());
				return -1;
			}
			SDL_DestroyTexture(button);
			return 1;
		}	
	}
	return 0;
}
