#include <stdio.h>
#include <stdlib.h>

int possible(int grille[9][9], int n, int m, int i){
	//Retourne 1 si il le chiffre i est une solution de la case de sudoku grille[n][m], -1 sinon.
	
	int x=0, y=0, x0, y0;
		
	//Vérification de la solution avec la colonne
	while(x<9){
		if(i==grille[x][m])
			return -1;
		x++;
	}
		
	//Vérification de la solution avec la ligne		
	while(y<9){
		if(i==grille[n][y])
			return -1;
		y++;
	}
	
	//Vérification de la solution avec le carré
	x0=(n/3)*3;
	y0=(m/3)*3;
	for(x=0; x<3; x++){
		for(y=0; y<3; y++){
			if(grille[x0+x][y0+y]==i)
				return -1;
		}
	}
	
	return 1;
}


void affiche_grille(int grille[9][9]){
	//Fonction qui affiche normalement la grille de sudoku dans le terminal
	
	int x, y;
	for(x=0; x<9; x++){
		for(y=0; y<9; y++)
			printf("%d ", grille[x][y]);
		printf("\n");
	}
	
	return;
}
	
	
int main(){
	int sudoku[9][9]={{0,0,7,0,0,0,4,0,3},
					  {0,0,0,4,8,0,0,7,9},
					  {2,0,0,0,0,3,0,0,0},
					  {7,0,0,3,2,0,1,0,0},
					  {0,0,4,0,0,0,0,9,0},
				  	  {1,0,8,0,5,4,0,0,0},
					  {0,5,2,0,0,0,0,8,0},
					  {0,3,6,0,4,7,9,2,1},
					  {0,7,1,2,6,8,5,0,4}};
	
	printf("%d \n", possible(sudoku, 6, 4, 3));
	affiche_grille(sudoku);
	
	return 0;
}