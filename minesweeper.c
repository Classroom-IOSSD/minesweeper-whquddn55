#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "conio.h"
#define MAX 10

// background color
#define KNRM  "\x1B[0m"
#define BRED  "\x1B[41m"
#define BGRN  "\x1B[42m"
#define BYEL  "\x1B[43m"
#define BBLU  "\x1B[44m"
#define BMAG  "\x1B[45m"
#define BCYN  "\x1B[46m"
#define BWHT  "\x1B[47m"
// text color
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

// function prototype
int uncover_blank_cell(int row, int col);
void print_table();
void new_game();
void flag_mode();
void check_mode();
void end_of_game();

// global variables
// game table
unsigned char table_array[MAX][MAX];
// location of cursor
int xCur=0, yCur=0;
// gamemode
enum MODE { INPUT, FLAG, CHECK };
enum MODE game_mode=INPUT;
// the number of the remaining mines
int num_mines = 0;

int main(int argc, char *argv[]) {
	char input;
	if (argc == 2) {
		num_mines = atoi(argv[1]);
	}
	new_game();
	while (num_mines != 0) {			// when num_mines becomes 0 you will win the game
		print_table();
		input = getch();
		switch (input) {
			// flag
		case 'f':
		case 'F':
			flag_mode();
			break;
			// check cell
		case 'c':
		case 'C':
			check_mode();
			break;
			// jump to a new game
		case 'n':
		case 'N':
			new_game();
			break;
			// exit
		case 'q':
		case 'Q':
			end_of_game();
			break;
		default:
			break;
		}
	}

	return 0;
}


/*This is a recursive function which uncovers blank cells while they are adjacent*/
int uncover_blank_cell(int row, int column) {
	int table_value, rows[8], columns[8], i;

	if (table_array[row][column] != 0)
		return 0; // error

	table_array[row][column] += 10; // uncover current cell

	// Get position of adjacent cells of current cell
	rows[0] = row - 1;
	columns[0] = column + 1;
	rows[1] = row;
	columns[1] = column + 1;
	rows[2] = row + 1;
	columns[2] = column + 1;
	rows[3] = row - 1;
	columns[3] = column;
	rows[4] = row + 1;
	columns[4] = column;
	rows[5] = row - 1;
	columns[5] = column - 1;
	rows[6] = row;
	columns[6] = column - 1;
	rows[7] = row + 1;
	columns[7] = column - 1;

	for (i = 0; i < 8; i++) {
		table_value = table_array[rows[i]][columns[i]];

		if ((rows[i] >= 0 && rows[i] < MAX) && (columns[i] >= 0 && columns[i] < MAX)) {	// to prevent negative index and out of bounds
			if (table_value > 0 && table_value <= 8)
				table_array[rows[i]][columns[i]] += 10;										// it is a cell with 1-8 number so we need to uncover
			else if (table_value == 0)
				uncover_blank_cell(rows[i], columns[i]);
		}

	}

	return 1; // success!
}

void print_table() {
	// clear screen
	system("clear");

	int i, j, table_value;
	for (i = 0; i < MAX; i++) {
		for (j = 0; j < MAX; j++) {
			if (xCur == j && yCur == i) {
				if (game_mode == FLAG) {
					printf("|%sF%s", BMAG, KNRM);
					continue;
				}
				else if (game_mode == CHECK) {
					printf("|%sC%s", BMAG, KNRM);
					continue;
				}

			}
			table_value = table_array[i][j];

			if ((table_value >= 0 && table_value <= 8) || table_value == 0 || table_value == 99)
				printf("|X");
			else if (table_value == 10) // clean area
				printf("|%s%d%s", KCYN, table_value - 10, KNRM);
			else if (table_value == 11) // the number of near mine is 1
				printf("|%s%d%s", KYEL, table_value - 10, KNRM);
			else if (table_value > 11 && table_value <= 18) // the number of near mine is greater than 1
				printf("|%s%d%s", KRED, table_value - 10, KNRM);
			else if ((table_value >= 20 && table_value <= 28) || table_value == 100)
				printf("|%sF%s", KGRN, KNRM);
			else
				printf("ERROR"); // test purposes

		}
		printf("|\n");
	}

	printf("cell values: 'X' unknown, '%s0%s' no mines close, '1-8' number of near mines, '%sF%s' flag in cell\n", KCYN, KNRM, KGRN, KNRM);
	if (game_mode == INPUT) {
		printf("f (put/remove Flag in cell), c (Check cell), n (New game), q (Exit game): ");
	}
	else if (game_mode == FLAG) {
		printf("Enter (select to put/remove Flag in cell), q (Exit selection): ");
	}
	else if (game_mode == CHECK) {
		printf("Enter (select to check cell), q (Exit selection): ");
	}
	else {}
}

void new_game() {
	int i, j, table_value;
	int  rows[8], columns[8];
	// the number of mines
	num_mines = 10;
	srand(time(NULL));	// random seed
	// setting cursor
	xCur = 0;
	yCur = 0;
	// set all cells to 0
	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
			table_array[i][j] = 0;

	int row, column; // row,column
	for (i = 0; i < num_mines; i++) {
		/* initialize random seed: */

		row = rand() % 10;					// it generates a integer in the range 0 to 9
		column = rand() % 10;

		// put mines
		if (table_array[row][column] != 99) {
			table_array[row][column] = 99;

			// Get position of adjacent cells of current cell
			rows[0] = row - 1;
			columns[0] = column + 1;
			rows[1] = row;
			columns[1] = column + 1;
			rows[2] = row + 1;
			columns[2] = column + 1;
			rows[3] = row - 1;
			columns[3] = column;
			rows[4] = row + 1;
			columns[4] = column;
			rows[5] = row - 1;
			columns[5] = column - 1;
			rows[6] = row;
			columns[6] = column - 1;
			rows[7] = row + 1;
			columns[7] = column - 1;

			for (j = 0; j < 8; j++) {
				table_value = table_array[rows[j]][columns[j]];
				if ((rows[j] >= 0 && rows[j] < MAX) && (columns[j] >= 0 && columns[j] < MAX)) {	// to prevent negative index and out of bounds
					if (table_value != 99)																// to prevent remove mines
						table_array[rows[j]][columns[j]] += 1;									// sums 1 to each adjacent cell
				}
			}

		}
		else {							// to make sure that there are the properly number of mines in table
			i--;
			continue;
		}
	}
}

void flag_mode() {
	// cursor direction
	char direction;
	int table_value;
	game_mode = FLAG;
	int mode_flag = 0; // 0 : goto flagmode 1 : goto checkmode 2: goto endofgame
	do {
		print_table();
		direction = getch();
		// arrow direction
		if (direction == '8') {
			// up
			yCur -= 1;
			yCur = (MAX + yCur) % MAX;
		}
		else if (direction == '2') {
			// down
			yCur += 1;
			yCur = yCur % MAX;
		}
		else if (direction == '4') {
			xCur -= 1;
			xCur = (MAX + xCur) % MAX;
		}
		else if (direction == '6') {
			xCur += 1;
			xCur = xCur % MAX;
		}
		else if (direction == 'c' || direction == 'C') {
			mode_flag = 1;
			break;
		}
		else if (direction == '\n') {
			table_value = table_array[yCur][xCur];

			if (table_value == 99) {				// mine case
				table_array[yCur][xCur] += 1;
				num_mines -= 1;				// mine found
			}
			else if (table_value >= 0 && table_value <= 8) {	// number of mines case (the next cell is a mine)
				table_array[yCur][xCur] += 20;
			}
			else if (table_value >= 20 && table_value <= 28) {
				table_array[yCur][xCur] -= 20;
			}
			if (num_mines == 0) {
				mode_flag = 2;
				break;
			}
		}
	} while (direction != 'q' && direction != 'Q');
	if (mode_flag == 1) {
		mode_flag = 0;
		check_mode();
	}
	else if (mode_flag == 2) {
		mode_flag = 0;
		end_of_game();
	} 
	else {}
	game_mode = INPUT;
}

void check_mode() {
	// cursor direction
	char direction;
	int table_value;
	game_mode = CHECK;
	int mode_flag = 1; // 0 : goto flagmode 1 : goto checkmode 2: goto endofgame
	do {
		print_table();
		direction = getch();
		// arrow direction
		if (direction == '8') {
			// up
			yCur -= 1;
			yCur = (MAX + yCur) % MAX;
		}
		else if (direction == '2') {
			// down
			yCur += 1;
			yCur = yCur % MAX;
		}
		else if (direction == '4') {
			xCur -= 1;
			xCur = (MAX + xCur) % MAX;
		}
		else if (direction == '6') {
			xCur += 1;
			xCur = xCur % MAX;
		}
		else if (direction == 'f' || direction == 'F') {
			mode_flag = 0;
			break;
		}
		else if (direction == '\n') {
			table_value = table_array[yCur][xCur];
			if (table_value == 0)						// blank case
				uncover_blank_cell(yCur, xCur);
			else if (table_value == 99) {				// mine case
				mode_flag = 2;
				break;
			}
			else if (table_value > 0 && table_value <= 8)	// number case (the next cell is a mine)
				table_array[yCur][xCur] += 10;
		}
	} while (direction != 'q' && direction != 'Q');
	if (mode_flag == 0) {
		mode_flag = 1;
		flag_mode();
	}
	else if (mode_flag == 2) {
		mode_flag = 1;
		end_of_game();
	}
	game_mode = INPUT;
}

void end_of_game() {
	char input;
	game_mode = INPUT;
	print_table();
	printf("\nGAME OVER\n");

	if (num_mines == 0)
		printf("you won!!!!\n");
	else
		printf("BOOM! you LOOSE!\n");
	do {
		printf("Are you sure to exit? (y or n)? ");
		input = getch();
		putchar('\n');
		if (input == 'y' || input == 'Y') {
			printf("See you next time!\n");
			exit(0);
		}
		else if (input == 'n' || input == 'N') {
			new_game();
			break;
		}
		printf("Please answer y or n\n");
	} while (1);

	fflush(stdin);
}
