#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <string.h>
#include "../include/solver.h"

struct list_solutions* add_solution(struct list_solutions *list, int tab[9][9], int nmb)
{
	struct list_solutions *new_solution = malloc(sizeof(struct list_solutions));
	struct list_solutions *tmp = list;
	int x, y;
	if(tmp != NULL)
	{
		while(tmp->next != NULL)
			tmp = tmp -> next;
	}
	new_solution -> previous = tmp;
	new_solution -> next = NULL;
	new_solution -> nmb = nmb;
	new_solution -> actual = (int **)malloc(sizeof(int*)*9);
	for(x = 0; x<9; x++)
		(new_solution -> actual)[x] = malloc(sizeof(int)*9);
	for(x = 0; x<9; x++)
	{
		for(y = 0; y<9; y++)
			(new_solution -> actual)[x][y] = tab[x][y];
	}
	if(list == NULL)
		return new_solution;
	
	tmp->next = new_solution;
	return list;
}

void free_list(struct list_solutions *list)
{
	int x;
	struct list_solutions* tmp = NULL;
	if(list != NULL)
	{
		while(list->previous != NULL)
			list = list->previous;
	}
	tmp = list;
	while(list != NULL)
	{
		for(x = 0; x < 9; x++)
			free((tmp->actual)[x]);
		list = list->next;
		free(tmp->actual);
		tmp=list;
	}
	return;
}

int possible(int grid[9][9], int n, int m, int i)
{
	//Retourne 0 si il le chiffre i est une solution de la case de sudoku grille[n][m], -1 sinon.
	int x=0, y=0, x0, y0;	
	//Vérification de la solution avec la colonne
	if(i != 0)
	{
		while(x<9)
		{
			if(i == grid[x][m])
			{
				if(x==n)
				{
					x++;
					continue;
				}
				return -1;
			}
			x++;
		}	
		//Vérification de la solution avec la ligne		
		while(y<9)
		{
			if(i  == grid[n][y])
			{
				if(y==m)
				{
					y++;
					continue;
				}
				return -1;
			}
			y++;
		}
		//Vérification de la solution dans le carré
		x0=(n/3)*3;
		y0=(m/3)*3;
		for(x=0; x<3; x++)
		{
			for(y=0; y<3; y++)
			{
				if(grid[x0+x][y0+y] == i)
				{
					if((x0 + x) == n && (y0 + y) == m)
						continue;
					return -1;
				}
			}
		}
	}
	return 0;
}

int valide_grid(int grid[9][9])
{
	int x, y;
	for(x = 0; x<9; x++)
	{
		for(y = 0; y<9; y++)
		{
			if(possible(grid, x, y, grid[x][y]) != 0)
			{
				return -1;
			}
		}
	}
	return 0;
}

void solver(struct list_solutions** list, int grid[9][9], int *nmb)
{
	//Fonction qui trouve la solution à la grille de sudoku
	int x, y, i;
	if(*nmb > 98)
		return;
	for(x=0; x<9; x++)
	{
		for(y=0; y<9; y++)
		{
			if(grid[x][y]==0){
				for(i=1; i<10; i++)
				{
					if(possible(grid, x, y, i) == 0)
					{
						grid[x][y]=i;
						solver(list, grid, nmb);
						grid[x][y]=0;
					}
				}
				return;
			}
		}
	}
	(*nmb)++;
	*list = add_solution(*list, grid, *nmb);
	return;
}


