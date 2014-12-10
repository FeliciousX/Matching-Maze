/**********************************************************************************************************/
/*																										  */
/*	Author: Churchill Lee				Student ID: 7436904												  */
/* 																										  */
/* 	Date: 13/3/2012						Last Modified: 30/4/2012										  */
/* 																										  */
/*																										  */
/**********************************************************************************************************/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

/* ASCII code equivalent for Arrow Keys */
#define ArrowUp 30
#define ArrowDown 31
#define ArrowLeft 17
#define ArrowRight 16

typedef struct{ 	 /* Structure to store player's details	*/
	char name[7];  	   	   /* Player name 			   */
	int diff;	   	   	  /* Board Size 			  */
	char status[5];	 	 /* Game status (Win / Quit) */
	int steps;		    /* Total steps taken 		*/
}/* structure name is */ PlayerData;

typedef struct{	  	/* Structure to store the board's details    */
	char letters;	    /* Randomized Characters 		  	  */
	int numbers;	   /* Randomized numbers  				 */
	char marker;      /* Coordinate of current location     */
	int penalty;     /* Penalty that adds number of steps  */
}/* structure name is */ BoardData;

/* Function Prototypes in their order of calling */
void ScanPlayers_(int *playerID);
void PlayerArray_(int playerID, PlayerData **player);
void ScanFile_(PlayerData *player);
void ShowScores_(int playerID, PlayerData player[]);
void GetInfo_(int playerID, PlayerData player[]);
bool CheckDifficulty_(int playerID, PlayerData player[]);
void BoardArray_(int playerID, BoardData ***board, PlayerData player[]);
void populate_(int playerID, BoardData **board, PlayerData player[]);
void game_(int playerID, BoardData **board, PlayerData player[]);
void board_(int playerID, BoardData **board, PlayerData player[]);
bool win_(int playerID, BoardData **board, PlayerData player[]);
void direction_(int playerID, PlayerData player[]);
void marker_(int playerID, int *X, int *Y, BoardData **board, PlayerData player[]);
void process_(int *UserInput, int playerID, int OriX, int OriY, BoardData **board, PlayerData player[]);
void ArrangeScores_(int playerID, PlayerData player[]);
void BubbleSort_(int limit, PlayerData Scores[]);
void LoopScores_(int limit, PlayerData player[], PlayerData Stats[], int *j, char status[]);
void WriteToFile_(int p, int q, PlayerData Wins[], PlayerData Quits[]);
void FreeBoardArray_(int playerID, BoardData **board, PlayerData player[]);
void Free1DArray_(int playerID, PlayerData *player);


/**********************************************************************************************************/
int main(void)
/**********************************************************************************************************/
{
	PlayerData *player = NULL;	  /* Dynamic 1D Array of Structure for player */
	BoardData **board = NULL;	 /* Dynamic 2D Array of Structure for board  */
	int playerID = 0;			/* Variable to store number of players 		*/
	system("cls");	/* Clears screen */

	ScanPlayers_(&playerID);
	PlayerArray_(playerID, &player);

	/* Only scan file and print Scores if number of player > 0 */
	if (playerID > 0)
	{
		ScanFile_(player);
		ShowScores_(playerID, player);
	}
	GetInfo_(playerID, player);
	CheckDifficulty_(playerID, player);
	BoardArray_(playerID, &board, player);
	populate_(playerID, board, player);
	game_(playerID, board, player);
	ArrangeScores_(playerID, player);
	FreeBoardArray_(playerID, board, player);
	Free1DArray_(playerID, player);

	return 0;
} /* end main() */

/**********************************************************************************************************/
void ScanPlayers_(int *playerID) /* Scan text file for number of players */
/**********************************************************************************************************/
{
	FILE *FR;
	FR = fopen("ScoreSheet.bin", "rb");	/* open file */

	if /* file doesn't exist */ (FR == 0)
	{
		printf("You are our first player! ");
	}
	else /* get number of players from first 'line' of file */
	{	fread(&*playerID, sizeof(int), 1, FR);	}

	fclose(FR); /* closes file */

} /* end ScanPlayers_() */

/**********************************************************************************************************/
void PlayerArray_(int playerID, PlayerData **player) /* Allocating memory to array */
/**********************************************************************************************************/
{
	/* Allocate memory for player array and set everything to 0 */
	*player = (PlayerData *)calloc(playerID + 1, sizeof(PlayerData));

	if (player == NULL)
		printf("Error! calloc could not allocate memory for player array\n");

} /* end PlayerArray_() */

/**********************************************************************************************************/
void ScanFile_(PlayerData *player) /* Scan text file to populate player array */
/**********************************************************************************************************/
{
	int i = 0;
	int numPlayers;	/* to store number of players */

	FILE *FR;
	FR = fopen("ScoreSheet.bin", "rb");

	fread(&numPlayers, sizeof(int), 1, FR); /* Scans first 'line' of file */

	fread(player, sizeof(PlayerData), numPlayers, FR);  /* Scans for players scores */

	fclose(FR);
}

/**********************************************************************************************************/
void ShowScores_(int playerID, PlayerData player[]) /* Print scores of previous players */
/**********************************************************************************************************/
{
	int i = 0;
	int q = 0;
	system("cls");
	printf("-SCORE BOARD-\n\n");
	printf("Name\t\tDifficulty\tStatus\t\tTotal Steps Taken\n");
	printf("____\t\t__________\t______\t\t_________________\n\n");
	do
	{
		if (strcmp(player[i].status, "Quit") == 0 && q == 0 && i != 0)
		{
			printf("\n");
			q++;
		}
		printf("%s\t\t%i x %i\t\t%s\t\t%i\n", 	player[i].name,
												player[i].diff,
												player[i].diff,
												player[i].status,
												player[i].steps);

		i++;
	} while (i < playerID);

	do /*nothing until user presses enter or Esc */
	{
		printf("\nPress Enter to Quit Score Board\n");
	} while (getch() != 13);
	system("cls");

} /* end ShowScores_() */

/**********************************************************************************************************/
void GetInfo_(int playerID, PlayerData player[]) /* Ask for player's name */
/**********************************************************************************************************/
{
	printf("Welcome to the Matching Maze! :)\n\nPlease enter your name below. (Maximum 7 letters)\n>>> ");
	scanf("%7s", player[playerID].name);
	fflush(stdin);

	do
	{
		printf("Please enter board size.\nEnter a number between 5 to 10: ");
		scanf("%i", &player[playerID].diff);
		fflush(stdin);
		system("cls");
	} while (CheckDifficulty_(playerID, player) != true);
} /* end GetInfo_() */

/**********************************************************************************************************/
bool CheckDifficulty_(int playerID, PlayerData player[]) /* Making sure board size is 5 - 10 */
/**********************************************************************************************************/
{
	if (player[playerID].diff >= 5 && player[playerID].diff <= 10)
		return true;
	else
		return false;
} /* end CheckDifficulty_() */

/**********************************************************************************************************/
void BoardArray_(int playerID, BoardData ***board, PlayerData player[]) /* Allocate memory to board array */
/**********************************************************************************************************/
{
	int i = 0;

	BoardData **temp = calloc(player[playerID].diff , sizeof(BoardData *));

	for (i = 0; i < player[playerID].diff; i++)
  		temp[i] = (BoardData *)calloc(player[playerID].diff, sizeof(BoardData));

  	if (board == NULL)
		printf("Error! calloc could not allocate memory for board array\n");

	*board = temp;

} /* end BoardArray_ */

/**********************************************************************************************************/
void populate_(int playerID, BoardData **board, PlayerData player[]) /* Populating raw.bina for the board */
/**********************************************************************************************************/
{
	int i, j;
	srand((unsigned) time(NULL));	/* Seeding the random generator */

	for(i = 0; i < player[playerID].diff; i++)
	{
		for(j = 0; j < player[playerID].diff; j++)
		{
			board[i][j].marker = 'O';
			board[i][j].penalty = 1;
			board[i][j].letters = rand() % (player[playerID].diff - 1) + 65;
			board[i][j].numbers = rand() % (player[playerID].diff - 1) + 1;
		} /* end for (j) */
	}    /* end for (i) */

	board[0][0].marker = '0';  /* initialize game coordinate              */
	board[0][0].penalty = 1;  /* initialize penalty for first coordinate */

} /* end populate_() */

/**********************************************************************************************************/
void game_(int playerID, BoardData **board, PlayerData player[]) /* controls the overall of the game */
/**********************************************************************************************************/
{
	int UserInput;		/* Variable to store user input 	*/
	int X = 0, Y = 0;  /* Variable for player's coordinate */

	do
	{
		system("cls");	  	  	  	  	  	 /* Clears screen and then */
		printf("***Welcome to the matching maze %s!***\n\n", player[playerID].name);
		board_(playerID, board, player);	/* prints the board       */

		if /* player wins */ (win_(playerID, board, player))
		{
			printf("\nCongratulation %s! You have won the game!!!\n\n", player[playerID].name);
			printf("Total steps taken: %i\n\n", player[playerID].steps);
			printf("Press Enter to continue...");
			do
			{
				/* do nothing until user press enter */
			} while (getch() != 13);
			break;
		}
		else
		{
			direction_(playerID, player);						  /* Prints menu 					 	   */
			marker_(playerID, &X, &Y, board, player);			 /* Gets current location coordinate  	  */
			process_(&UserInput, playerID, X, Y, board, player);/* Changes the coordinates of the marker */
		}
	} while (UserInput != 27); /* Exit if ESC key is pressed */

} /* end of game_() */

/**********************************************************************************************************/
void board_(int playerID, BoardData **board, PlayerData player[]) /* Displaying board */
/**********************************************************************************************************/
{
	int i,j;

	for(i = 0; i < player[playerID].diff; i++)
	{
		for(j = 0; j < player[playerID].diff; j++)
		{
			if(board[i][j].marker == '0')
				printf("| [%c%i] ", board[i][j].letters, board[i][j].numbers);
			else if(board[i][j].marker == 'O')
				printf("|  %c%i  ", board[i][j].letters, board[i][j].numbers);

		} /* end for (j) */
		printf("|\n");

	} /* end for (i)  */
}    /* end board_() */

/**********************************************************************************************************/
bool win_(int playerID, BoardData **board, PlayerData player[]) /* Check if player has reached target */
/**********************************************************************************************************/
{
	if(board[player[playerID].diff - 1][player[playerID].diff - 1].marker == '0')
	{
		strcpy(player[playerID].status, "Win");
		return true;
	}
	else
	{
		strcpy(player[playerID].status, "Quit");
		return false;
	}
} /* end win_() */

/**********************************************************************************************************/
void direction_(int playerID, PlayerData player[]) /* Displaying menu for player */
/**********************************************************************************************************/
{

	printf("\nTotal steps taken: %i\n\n",player[playerID].steps);
	printf("      ***** MENU *****         %c\n", ArrowUp);
	printf("Use arrow key to move cursor %c   %c\n", ArrowLeft, ArrowRight);
	printf("Press ESC to quit              %c\n\n", ArrowDown);

} /* end direction_() */

/**********************************************************************************************************/
void marker_(	int playerID, 	int *X, 	int *Y,			/* Getting coordinate of current */
				BoardData **board, 	PlayerData player[])   /* position in X and Y 		    */
/**********************************************************************************************************/
{
	int i, j;

	for(i = 0; i < player[playerID].diff; i++)
	{
		for(j = 0; j < player[playerID].diff; j++)
		{
			if(board[i][j].marker == '0')
			{
				*X = i;  /* Passing by reference the coordinate   */
				*Y = j;	/*  of the player's current location	 */
				break;
			}
		} /* end for (j)   */
	}    /* end for (i)   */
}       /* end marker_() */

/**********************************************************************************************************/
void process_(int *UserInput, int playerID, int OriX, int OriY, BoardData **board, PlayerData player[])
/* Changes coordinate based on the user input 															  */
/**********************************************************************************************************/
{
	int i, j;

	*UserInput = getch();	 /* Arrow keys returns 2 inputs */
	if (*UserInput == 224)	/* first input is 224		   */
	{
		*UserInput = getch();
		if(*UserInput == 80) /* Down */
		{
			for (i = OriX+1; i < player[playerID].diff; i++)
			{
				if(board[OriX][OriY].letters == board[i][OriY].letters /* OR */
				|| board[OriX][OriY].numbers == board[i][OriY].numbers)
				{
					board[OriX][OriY].marker = 'O';
					board[OriX][OriY].penalty++;

					board[i][OriY].marker = '0';
					player[playerID].steps+=board[i][OriY].penalty;
					break;
				}
			} /* end for (i) */
		}
		else if(*UserInput == 75) /* Left */
		{
			for (i = OriY-1; i >= 0; i--)
			{
				if(board[OriX][OriY].letters == board[OriX][i].letters /* OR */
				|| board[OriX][OriY].numbers == board[OriX][i].numbers)
				{
					board[OriX][OriY].marker = 'O';
					board[OriX][OriY].penalty++;

					board[OriX][i].marker = '0';
					player[playerID].steps+=board[OriX][i].penalty;
					break;
				}
			} /* end for (i) */
		}
		else if(*UserInput == 77) /* Right */
		{
			for (i = OriY+1; i < player[playerID].diff; i++)
			{
				if(board[OriX][OriY].letters == board[OriX][i].letters  /* OR */
				|| board[OriX][OriY].numbers == board[OriX][i].numbers)
				{
					board[OriX][OriY].marker = 'O';
					board[OriX][OriY].penalty++;

					board[OriX][i].marker = '0';
					player[playerID].steps+=board[OriX][i].penalty;
					break;
				}
			} /* end for (i) */
		}
		else if(*UserInput == 72) /* Up */
		{
			for (i = OriX-1; i >= 0; i--)
			{
				if(board[OriX][OriY].letters == board[i][OriY].letters /* OR */
				|| board[OriX][OriY].numbers == board[i][OriY].numbers)
				{
					board[OriX][OriY].marker = 'O';
					board[OriX][OriY].penalty++;

					board[i][OriY].marker = '0';
					player[playerID].steps+=board[i][OriY].penalty;
					break;
				}
			}  /* end for (i)    */
		}
	} 		 /* end nested if  */
} 			/* end process_() */

/**********************************************************************************************************/
void ArrangeScores_(int playerID, PlayerData player[]) /* Arranges High scores */
/**********************************************************************************************************/
{
	PlayerData *Wins = NULL;
	PlayerData *Quits = NULL;
	int i, p = 0, q = 0;
	int lowsteps = player[0].steps;
	int highsteps = player[0].steps;

	PlayerArray_(playerID, &Wins);
	PlayerArray_(playerID, &Quits);

	for (i = 0; i <= playerID; i++)
	{
		if (player[i].steps < lowsteps)
		{
			lowsteps = player[i].steps ;
		}

		if (player[i].steps > highsteps)
		{
			highsteps = player[i].steps ;
		}
	} /* end for (i) */

	BubbleSort_(playerID + 1, player);
	LoopScores_(playerID + 1, player, Wins, &p, "Win");
	LoopScores_(playerID + 1, player, Quits, &q, "Quit");
	WriteToFile_(p, q, Wins, Quits);
	ScanFile_(player);
	ShowScores_(playerID + 1, player);
	Free1DArray_(playerID, Wins);
	Free1DArray_(playerID, Quits);

} /* end ArrangeScores_() */

/**********************************************************************************************************/
void BubbleSort_(int limit, PlayerData Scores[]) /* Sorting the player.bina using bubble sort algorithm */
/**********************************************************************************************************/
{
	int i, j;
	PlayerData temp;

	for (i = 0; i < limit - 1; i++)
	{
		for (j = 0; j < limit - 1 - i; j++)
		{
			if (Scores[j].steps > Scores[j+1].steps)
			{
				temp = Scores[j+1];
				Scores[j+1] = Scores[j];
				Scores[j] = temp;
			}
		} /* end for (j) */

	} /* end for (i) */

} /* end BubbleSort_() */

/**********************************************************************************************************/
void LoopScores_(int limit, PlayerData player[], PlayerData Stats[], int *j, char status[])
/*
/**********************************************************************************************************/
{
	int i;
	int upperlim = 10, lowerlim = 5;

	for (i = 0; i <= limit; i++ )
	{
		if (strcmp(player[i].status, status) == 0 &&
			player[i].diff == upperlim) /* from highest difficulty */
		{
			strcpy(Stats[*j].name, player[i].name);
			strcpy(Stats[*j].status, player[i].status);
			Stats[*j].diff = player[i].diff;
			Stats[*j].steps = player[i].steps;
			(*j)++;
		}

		if (upperlim < lowerlim) /* finishes looping */
		{
			break;		/* exits loop */
		}

		if (i == limit)
		{
			upperlim--;
			i = -1;
		}
	} /* end for (i) */

}

/**********************************************************************************************************/
void WriteToFile_(int p, int q, PlayerData Wins[], PlayerData Quits[]) /* Write Data to the .txt file */
/**********************************************************************************************************/
{
	int i = 0;
	int ttl;
	FILE *fp;
	ttl = p + q;
	fp = fopen("ScoreSheet.bin", "wb");

	if (fp == 0) /* File doesn't exist */
	{
		printf("Error cannot write to file \"ScoreSheet.bin\"");
	}
	else
	{
			fwrite(&ttl, sizeof(int), 1, fp); 		    /* Writes number of player first 			 */
			fwrite(Wins, sizeof(PlayerData), p, fp);   /* Writes in the scores of the winners first */
			fwrite(Quits, sizeof(PlayerData), q, fp); /* followed by the scores of the quiiters    */
	}
	fclose(fp);

} /* end WriteToFile() */

/**********************************************************************************************************/
void FreeBoardArray_(int playerID, BoardData **Ar2D, PlayerData player[]) /* Frees the memory allocated   */
																		  /*	for a 2D Array 			  */
/**********************************************************************************************************/
{
	int i;

	for (i = 0; i < player[playerID].diff; i++)
		free(Ar2D[i]); /* end for(i) */

	free(Ar2D);
} /* end Free2DArray_() */

/**********************************************************************************************************/
void Free1DArray_(int playerID, PlayerData Ar1D[]) /* Frees the memory allocated for a 1D Array */
/**********************************************************************************************************/
{
	free(Ar1D);
} /* end Free1D Array_() */

