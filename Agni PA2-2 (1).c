#include <stdio.h>
#include <stdlib.h>

#define N 9

// A utility function to convert dash to 0.
void reset_sudoku(int arr[N][N], int val)
{
	for (int i=0; i<N; i++)
	{
		for (int j = 0; j < N; j++) {
		        arr[i][j] = val;
		}
	}
}

// A utility function to convert dash to 0.
void tackle_dash(int arr[N][N])
{
	for (int i=0; i<N; i++)
	{
		for (int j = 0; j < N; j++) {
		    if (arr[i][j] < 1 || arr[i][j] > 9)
		        arr[i][j] = 0;
		}
	}
}

// A utility function to print sudoku
void print_sudoku(int arr[N][N])
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++) {
		    if (j>0) printf("\t");
			printf("%d",arr[i][j]);
		}
		printf("\n");
	}
}

int validate_grid(int grid[N][N], int startrow, int startcol)
{
	int cnt;
	for (int num=1; num<=9; num++)
	{
		cnt = 0;
		for (int row=startrow; row<startrow+3; row++)
		{
			for (int col=startcol; col<startcol+3; col++)
			{
				if (grid[row][col] == num)
					cnt++;
				if (cnt>1) return 0;
			}
		}
	}
	return 1;
}

// Validate input numbers
int validate_input(int arr[N][N])
{
	int cnt=0;

	// If duplicate number exists in same ROW
	for (int num=1; num<=9; num++)
	{
		for (int row=0; row<9; row++)
		{
			cnt = 0;
			for (int col=0; col<9; col++) {
				if (arr[row][col] == num)
					cnt++;
				if (cnt>1) return 0;
			}
		}
	}

	// If duplicate number exists in same COL
	for (int num=1; num<=9; num++)
	{
		for (int col=0; col<9; col++)
		{
			cnt = 0;
			for (int row=0; row<9; row++) {
				if (arr[row][col] == num)
					cnt++;
				if (cnt>1) return 0;
			}
		}
	}

	// If duplicate number exists in same SUB-GRID
	int grid_ok;
	for (int row=0; row<8; row+=3) {
		for (int col=0; col<8; col+=3) {
			grid_ok = validate_grid(arr, row, col);
			if (grid_ok==0) return 0;
		}
	}

	return 1;
}

// Checking if OK to assign num to row/col combination
int ok_to_put(int sudoku[N][N], int row, int col, int num)
{
	int startRow = row - (row%3);
	int startCol = col - (col%3);

	// If num exists in subgrid
	for (int i=0; i<3; i++) {
		for (int j=0; j<3; j++) {
			if (sudoku[startRow+i][startCol+j] == num) {
				return 0;
			}
		}
	}

	// If num exists in same row
	for (int i=0; i<9; i++) {
		if (sudoku[row][i] == num) {
			return 0;
		}
	}

	// If num exists in same column
	for (int i=0; i<9; i++) {
		if (sudoku[i][col] == num) {
			return 0;
		}
	}

	return 1;
}

int solve_puzzle(int sudoku[N][N], int row, int col)
{
	// if we reached bottom right corner (row=8,col=9)
	if (row==(N-1) && col==N) {
		return 1;
	}

	// if col=9 means goto next row
	if (col==N)  {
		row++;
		col = 0;
	}

	// if val exists at row/col then goto next column
	if (sudoku[row][col] > 0) {
		return solve_puzzle(sudoku, row, col+1);
	}

	for (int num=1; num<=N; num++)
	{
		if (ok_to_put(sudoku, row, col, num)==1)
		{
			sudoku[row][col] = num;

			if (solve_puzzle(sudoku, row, col + 1)==1)
				return 1;
		}

		// not-ok. So clear that row/col
		sudoku[row][col] = 0;
	}
	return 0;
}

int main(int argc, char *argv[])
{
    int row, column;
    int sudoku[9][9];
	int input_ok;

	//initialize with all 0
    reset_sudoku(sudoku, 0);

	FILE* in_file = fopen(argv[1], "r");				// read only mode
	//FILE* in_file  = fopen("input_file.dat", "r"); 		// Debug mode only

	if (in_file == NULL)  {	//Error reading File
		return 1;
	}

	while (!feof(in_file))	// Read input data
	{
		for (row = 0; row < 9; row++) {
			for (column = 0; column < 9; column++){
				fscanf(in_file, "%d ", &sudoku[row][column]);
			}
		}
	}
	fclose(in_file);

    tackle_dash(sudoku);

	input_ok = validate_input(sudoku);
	if (input_ok==0)
		printf("no-solution");

	if (solve_puzzle(sudoku, 0, 0)==1)
		print_sudoku(sudoku);
	else
		printf("no-solution");

	return 0;
}