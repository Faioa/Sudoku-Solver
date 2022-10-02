all: Sudoku-Solver

Sudoku-Solver: main.c src/graphics.c src/solver.c
	gcc -o Sudoku-Solver main.c src/graphics.c src/solver.c -lSDL2main -lSDL2