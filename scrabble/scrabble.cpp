// scrabble.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define BOARD 15 //bord size
#define LEGEND_X_POS 1 //position x of legend
#define LEGEND_Y_POS 1 //position y of legend
#define BOARD_X_POS 50 //position x of board
#define BOARD_Y_POS 1 //position y of board
#define LETTER_ON_HAND 7 //number of letter on hand
#define BLANK_COUNT 2//count of blank letter
#define LETTERS_ON_ALPHABET 26//saves actual number of alphabet
#define SPECIAL_AREA_COUNT 61//number of special area
#define HORIZONTAL 0//for orientation check horizontal value
#define VERTICAL 1//for orientation check vertical value
#define PLAYER 0//player number in functions
#define COMPUTER 1//computer number in functions
#define DEFAULT_DICTIONARY "dictionary.txt"//name of default dictionary
#define ENTER 0x0d//enter code
#define ESC 0x1b//esc
#define BACKSPACE 0x08 //backspace
#define LEFT_ARROW 0x4b//left arrow key
#define RIGHT_ARROW 0x4d//left arrow key
#define UP_ARROW 0x50//left arrow key
#define DOWN_ARROW 0x48//left arrow key
#define DOUBLE_LETTER 2//special letter area multipler 
#define TRIPLE_LETTER 3//special letter bigger area multipler
#define DOUBLE_WORD 2//special word multipler
#define TRIPLE_WORD 3//special second word multipler

enum area//name of the special area
{
	normal,
	doubleLetter,
	tripleLetter,
	doubleWord,
	tripleWord
};


// count of each letter, a have count on index 0, b on 1 etc
const short int LETTER_COUNT[LETTERS_ON_ALPHABET] = { 9,2,2,4,12,2,3,2,9,1,1,4,2,6,8,2,1,6,4,6,4,2,2,1,2,1 };

// point of each letter, a have point on index 0, b on 1 etc
const short int LETTER_POINT[LETTERS_ON_ALPHABET] = { 1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10 };


struct point
{
	int x;
	int y;
	area special;
};


struct computer_Word
{
	char word[BOARD];
	int maks = 0;
};


const point special[SPECIAL_AREA_COUNT] =
{
	{0, 0,tripleWord},{0,3,doubleLetter},{0,7,tripleWord},{0,11,doubleLetter},{0,14,tripleWord},
	{1,1,doubleWord},{1,5,tripleLetter},{1,9,tripleLetter},{1,13,doubleWord},{2,2,doubleWord},
	{2,6,doubleLetter},{2,8,doubleLetter},{2,12,doubleWord},{3,0,doubleLetter},{3,3,doubleWord},
	{3,7,doubleLetter},{3,11,doubleWord},{3,14,doubleLetter},{4,4,doubleWord},{4,10,doubleWord},
	{5,1,tripleLetter},{5,5,tripleLetter},{5,9,tripleLetter},{5,13,tripleLetter},{6,2,doubleLetter},
	{6,6,doubleLetter},{6,8,doubleLetter},{6,12,doubleLetter},{7,0,tripleWord},{7,3,doubleLetter},
	{7,7,doubleWord},{7,11,doubleLetter},{7,14,tripleWord},{8,2,doubleLetter},{8,6,doubleLetter},
	{8,8,doubleLetter},{8,12,doubleLetter},{9,1,tripleLetter},{9,5,tripleLetter},{9,9,tripleLetter},
	{9,13,tripleLetter},{10,4,doubleWord},{10,10,doubleWord},{11,0,doubleLetter},{11,3,doubleWord},
	{11,7,doubleLetter},{11,11,doubleWord},{11,14,doubleLetter},{12,2,doubleWord},{12,6,doubleLetter},
	{12,8,doubleLetter},{12,12,doubleWord},{13,1,doubleWord},{13,5,tripleLetter},{13,9,tripleLetter},
	{13,13,doubleWord},{14, 0,tripleWord},{14,3,doubleLetter},{14,7,tripleWord},{14,11,doubleLetter},
	{14,14,tripleWord}
};//array of special area

//struct save info about board
struct board_status
{
	char letter;
	area special;
};

//struct save info about player letter
struct player_letter
{
	char playerLetter;
	bool onUse = false;
};

//this structare saves all information about a game
struct game_status//when you initialize this structure, you must call MakeTableEmpty()
{
	board_status boardStatus[BOARD][BOARD];//board status, each letter is saved at position on board
	int playerScore;
	int computerScore;
	player_letter playerLetter[LETTER_ON_HAND];
	player_letter computerLetter[LETTER_ON_HAND];
	int letterOnBag;
	char *bag = (char*)malloc(sizeof(char)*BOARD*BOARD);
};

//saves actual rursor position
struct cursor_position
{
	int x;
	int y;
};

//making table empty
void MakeTableEmpty(game_status &status) 
{
	for(int i=0;i<BOARD;i++)
	{
		for (int j = 0; j < BOARD; j++)
		{
			status.boardStatus[i][j].letter = ' ';
		}
	}
}

//count power
long long int Power(int n, int x)
{
	int answer = 1;
	for (int i = 0; i < x; i++)
	{
		answer *= n;
	}
	return answer;
}

//count strong
long long int Strong(int n)
{
	long long int strong = 1;
	for (int i = 2; i <= n; i++)
	{
		strong *= i;
	}
	return strong;
}

//printing letter row on a board
void LetterRowPrint()
{
	
	char letter[] = { 'A',' ',NULL };
	for (int i = 0; i < BOARD; i++)
	{
		cputs(letter);
		letter[0] += 1;
	}
}


//check all word created by letters we use
bool CheckAllNewWord(dict_tree_node *pRoot, game_status status, int wordxStart, int wordyStart, int wordxEnd, int wordyEnd,char word1[])
{
	if (word1[0] == NULL)
	{
		return false;
	}
	if (wordyEnd - wordyStart > 0)//first, we put word on a board
	{
		for (int i = wordyStart; i <= wordyEnd; i++)
		{
			status.boardStatus[wordxStart][i].letter = word1[i - wordyStart];
		}
	}
	else
	{
		for (int i = wordxStart; i <= wordxEnd; i++)
		{
			status.boardStatus[i][wordyStart].letter = word1[i - wordxStart];
		}
	}
	
	

	for (int i = 0; i <= wordyEnd - wordyStart; i++)//same for the second orientation
	{
		char word[BOARD];
		int count=0;
		
		if (wordxStart - 1 >= 0)//get all letters before word
		{
			if (status.boardStatus[wordxStart - 1][wordyStart + i].letter != ' ')
			{
				int j = -1;
				word[wordxStart] = status.boardStatus[wordxStart][wordyStart + i].letter;
				
				while (wordxStart + j >= 0)
				{
					word[wordxStart + j] = status.boardStatus[wordxStart + j][wordyStart + i].letter;
					j--;
					if (status.boardStatus[wordxStart + j][wordyStart + i].letter == ' ')
					{
						break;
					}
				}
				count = j * -1;
				
				for (int i = 0; i <= count; i++)
				{
					word[i] = word[wordxStart + j+1];
					j++;
				}
			}
		}
		
		if (wordxStart + 1 < BOARD)//get all letter after word
		{
			if (status.boardStatus[wordxStart + 1][wordyStart + i].letter != ' ')
			{
				if (word[count-1] != status.boardStatus[wordxStart][wordyStart + i].letter)
				{
					count = 0;
					word[count] = status.boardStatus[wordxStart][wordyStart + i].letter;
				}
				else
				{
					count--;
				}
				int j = 1;
				
				while (j + wordxStart < BOARD)
				{
					word[count + j] = status.boardStatus[wordxStart + j][wordyStart + i].letter;
					j++;
					if (status.boardStatus[wordxStart + j][wordyStart + i].letter == ' ')
					{
						break;
					}
				}
				count += j;
			}
		}
		word[count] = NULL;
		
		if (count > 0)//check the word in a dictionary
		{
			if (DictTreeFindWord(pRoot, word, count) == false)
			{
				return false;
			}
		}
	}
	
	return true;
}


void NumberPrint(int i) //print a number max 99
{
	char number[3];

	if (i < 10)
	{
		number[0] = ' ';
		number[1] = '0' + i;
		number[2] = NULL;
	}
	else
	{
		number[0] = '0' + i / 10;
		number[1] = '0' + i % 10;
		number[2] = NULL;
	}
	cputs(number);
}


void AnyNumberPrint(int number)//prints any number
{
	clreol();
	int numberForce = 2;
	int numberCopy = number / 10;
	
	while (numberCopy != 0)
	{
		numberForce += 1;
		numberCopy /= 10;
	}
	char*tab = new char[numberForce];

	for (int i = numberForce - 2; i >= 0; i--)
	{
		tab[i] = number % 10 +'0';
		number /= 10;
	}
	tab[numberForce - 1] = NULL;
	cputs(tab);
	delete[] tab;
}


void PrintPoint(game_status status)//printing players point
{
	textbackground(BLACK);
	textcolor(LIGHTGRAY);
	gotoxy(BOARD_X_POS, BOARD_Y_POS + BOARD + 7);
	cputs("Player:");
	gotoxy(BOARD_X_POS + 10, BOARD_Y_POS + BOARD + 7);
	AnyNumberPrint(status.playerScore);
	gotoxy(BOARD_X_POS, BOARD_Y_POS + BOARD + 8);
	cputs("Computer:");
	gotoxy(BOARD_X_POS + 10, BOARD_Y_POS + BOARD + 8);
	AnyNumberPrint(status.computerScore);
}


int CountPoint(game_status &status, char word[], int counter, int startxPos, int startyPos, int orientation, int who, bool all, bool checkOnly)//count number of poin and add it ot a proper player
{
	int sum = 0;
	int wordBonus = 1;
	
	if (orientation == HORIZONTAL)//count a number of point for one orientation
	{
		for (int i = 0; i < counter; i++)
		{
			switch (status.boardStatus[startxPos][startyPos+i].special)//check speciality of the area, if is special, apply proper bonus
			{
			case doubleLetter:
				sum +=  DOUBLE_LETTER* (LETTER_POINT[word[i] - 'a']);
				break;

			case tripleLetter:
				sum += TRIPLE_LETTER * (LETTER_POINT[word[i] - 'a']);
				break;

			case doubleWord:
				wordBonus *= DOUBLE_WORD;
				sum += (LETTER_POINT[word[i] - 'a']);
				break;

			case tripleWord:
				wordBonus *= TRIPLE_WORD;
				sum += (LETTER_POINT[word[i] - 'a']);
				break;

			default:
				sum += (LETTER_POINT[word[i] - 'a']);
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < counter; i++)//same for second
		{
			switch (status.boardStatus[startxPos+i][startyPos].special)
			{
			case doubleLetter:
				sum += 2 * (LETTER_POINT[word[i] - 'a']);
				break;

			case tripleLetter:
				sum += 3 * (LETTER_POINT[word[i] - 'a']);
				break;

			case doubleWord:
				wordBonus *= 2;
				sum += (LETTER_POINT[word[i] - 'a']);
				break;

			case tripleWord:
				wordBonus *= 3;
				sum += (LETTER_POINT[word[i] - 'a']);
				break;

			default:
				sum += (LETTER_POINT[word[i] - 'a']);
				break;
			}
		}
	}
	sum *= wordBonus;

	if (checkOnly)
	{
		return sum;
	}

	if (who == PLAYER)
	{
		status.playerScore += sum;
		if (all == true)
		{
			status.playerScore += 50;
		}
	}
	else
	{
		status.computerScore += sum;
		if (all == true)
		{
			status.computerScore += 50;
		}
	}

	return 0;
}


void BoardPrint(game_status &status)//function print actual status of board
{
	textbackground(BLACK);
	textcolor(WHITE);
	gotoxy(BOARD_X_POS + 3, BOARD_Y_POS);
	LetterRowPrint();
	int color=WHITE,counter=0;
	
	for (int i = 1; i <= BOARD; i++)
	{

		gotoxy(BOARD_X_POS, BOARD_Y_POS + i);
		textcolor(WHITE);
		textbackground(BLACK);
		NumberPrint(i);
		putch('|');
		
		for (int j = 1; j <= BOARD; j++)
		{
			if (special[counter].x == i-1 && special[counter].y == j-1)
			{
				
				if (special[counter].special == doubleLetter)
				{
					status.boardStatus[i - 1][j - 1].special = doubleLetter;
					color = LIGHTBLUE;
				}
				else if (special[counter].special == tripleLetter)
				{
					status.boardStatus[i - 1][j - 1].special = tripleLetter;
					color = LIGHTCYAN;
				}
				else if (special[counter].special == tripleWord)
				{
					status.boardStatus[i - 1][j - 1].special = tripleWord;
					color = YELLOW;
				}
				else if (special[counter].special == doubleWord)
				{
					status.boardStatus[i - 1][j - 1].special = doubleWord;
					color = LIGHTMAGENTA;
				}
				counter += 1;
			}
			else
			{
				color = WHITE;
			}
			textbackground(color);
			textcolor(BLACK);
			putch(status.boardStatus[i-1][j-1].letter);
			textbackground(WHITE);
			putch('|');
		}
		textcolor(WHITE);
		textbackground(BLACK);
		NumberPrint(i);
	}
	gotoxy(BOARD_X_POS+3 , BOARD_Y_POS + BOARD+1);
	LetterRowPrint();
}


void CursorPositioning(cursor_position &cursorPosition, int zn)//setup actual position of coursor if the arrows are click
{
	if (zn == DOWN_ARROW)  
	{
		cursorPosition.y--;
	}
	else if (zn == UP_ARROW)
	{
		cursorPosition.y++;
	}
	else if (zn == LEFT_ARROW)
	{
		cursorPosition.x -= 2;
	}
	else if (zn == RIGHT_ARROW )
	{
		cursorPosition.x += 2;
	}
	if (cursorPosition.y < BOARD_Y_POS + 1)
	{
		cursorPosition.y = BOARD_Y_POS + 1;
	}
	else if (cursorPosition.y > BOARD_Y_POS + BOARD)
	{
		cursorPosition.y = BOARD_Y_POS + BOARD;
	}
	else if (cursorPosition.x < BOARD_X_POS + 2)
	{
		cursorPosition.x = BOARD_X_POS + 2;
	}
	else if (cursorPosition.x > BOARD_X_POS + BOARD * 2)
	{
		cursorPosition.x = BOARD_X_POS + BOARD * 2;
	}
	
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 15);
	textcolor(LIGHTGRAY);
	textbackground(BLACK);
	cputs("pos x:");
	putch((cursorPosition.x - BOARD_X_POS)/2 - 1 + 'A');
	cputs(" pos y:");
	NumberPrint(cursorPosition.y - BOARD_Y_POS);
}


void LegendPrinting(cursor_position cursorPosition)//function print legend on position modified by define values
{
	textcolor(7);
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS);
	// we print out a text at a given cursor position
	// the cursor will move by the length of the text
	cputs("Dominik Kwiatkowski 175454");
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS+1);
	cputs("implemented points:a,b,c,d,e,f,g,h,i,j,k,l,m");
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS+2);
	cputs("n            =new game start");
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS+3);
	cputs("q            =exit");
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 4);
	cputs("arrows       =moving on board");
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 5);
	cputs("enter        =confirmation");
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 6);
	cputs("esc          =action stop");
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 7);
	cputs("i            =new word applaying");
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 8);
	cputs("backspace    =last letter deleting");
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 9);
	cputs("o            =orientation change");
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 10);
	cputs("w            =letter changing");
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 11);
	cputs("1,2,3,4,5,6,7=letter to change choosing");
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 12);
	cputs("s            =game save");
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 13);
	cputs("l            =game load");
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 14);
	cputs("t            =learning mode");
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 15);
	cputs("pos x:");
	putch((cursorPosition.x - BOARD_X_POS)/2 - 1 + 'A');
	cputs(" pos y:");
	NumberPrint(cursorPosition.y-BOARD_Y_POS);
}


void PrintWordToPut(char word[], game_status status, int counter)//printing actual word, which player want ot put
{
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS +18);
	textcolor(LIGHTGRAY);
	textbackground(BLACK);
	clreol();
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 18);

	for (int i = 0; i < counter; i++)
	{
		textbackground(RED);
		for (int j = 0; j < LETTER_ON_HAND; j++)
		{
			if (word[i] == status.playerLetter[j].playerLetter && status.playerLetter[j].onUse==false)
			{
				textbackground(GREEN);
				status.playerLetter[j].onUse = true;
				break;
			}
		}
		putch(word[i]);
	}

	for (int i = 0; i < LETTER_ON_HAND; i++)
	{
		if (status.playerLetter[i].playerLetter != '?')
		{
			status.playerLetter[i].onUse = false;
		}
	}
}


void PrintWord(game_status status, cursor_position cursorPosition, char word[], int counter, int orientation)//print word during putting on board process
{
	if (orientation == HORIZONTAL)
	{
		for (int i = 0; i < counter; i++)
		{
			if (word[i] == status.boardStatus[cursorPosition.y-BOARD_Y_POS-1][(cursorPosition.x -BOARD_X_POS)/2 - 1 + i].letter ||
				status.boardStatus[cursorPosition.y - BOARD_Y_POS - 1][(cursorPosition.x - BOARD_X_POS) / 2 - 1 + i].letter == ' ')
			{
				textbackground(GREEN);
			}
			else
			{
				textbackground(RED);
			}
			gotoxy(cursorPosition.x + 1 + i * 2, cursorPosition.y);
			if (cursorPosition.x + 1 + i * 2 < BOARD_X_POS + 2 + BOARD * 2)
			{
				putch(word[i]);
			}
		}
	}
	else
	{
		for (int i = 0; i < counter; i++)
		{
			if (word[i] == status.boardStatus[cursorPosition.y - BOARD_Y_POS - 1 + i][(cursorPosition.x - BOARD_X_POS) / 2 - 1].letter ||
				status.boardStatus[cursorPosition.y - BOARD_Y_POS - 1 + i][(cursorPosition.x - BOARD_X_POS) / 2 - 1].letter == ' ')
			{
				textbackground(GREEN);
			}
			else
			{
				textbackground(RED);
			}
			gotoxy(cursorPosition.x+1 , cursorPosition.y + i);
			if (cursorPosition.y + i < BOARD_Y_POS + 1 + BOARD)
			{
				putch(word[i]);
			}
		}
	}
}


bool AnyLetterUse(game_status status)
{
	for (int i = 0; i < LETTER_ON_HAND; i++)
	{
		if (status.playerLetter[i].onUse == true && status.playerLetter[i].playerLetter != '?')
		{
			return true;
		}
	}
	return false;
}


void LetterGet2(player_letter playerLetter[LETTER_ON_HAND], game_status &status)
{
	for (int i = 0; i < LETTER_ON_HAND; i++)
	{
		if (status.letterOnBag > 0)
		{
			if (playerLetter[i].onUse)
			{
				playerLetter[i].onUse = false;
				playerLetter[i].playerLetter = status.bag[status.letterOnBag];
				status.letterOnBag--;
			}
		}
		else
		{
			if (playerLetter[i].onUse)
			{
				playerLetter[i].playerLetter = '?';
			}
		}
	}
}


void LetterGet(game_status &status,int who)//drawing letter to player hand
{
	if (who == PLAYER)
	{
		LetterGet2(status.playerLetter, status);
	}
	else
	{
		LetterGet2(status.computerLetter, status);
	}
}


bool VerticalConnectionCheck(game_status status, int counter, int wordxStart, int wordyStart)//check o=connected of word in position
{
	bool positionConnected = false;

	if (wordyStart == BOARD / 2 &&
		wordxStart <= BOARD / 2 &&
		wordxStart + counter - 1>= BOARD / 2)
	{
		positionConnected = true;
	}
	if (wordxStart + counter < BOARD && status.boardStatus[wordxStart + counter][wordyStart].letter != ' ')
	{
		positionConnected = true;
	}
	if (wordxStart - 1 > 0 && status.boardStatus[wordxStart - 1][wordyStart].letter != ' ' )
	{
		positionConnected = true;
	}
	if (positionConnected == false)
	{
		for (int i = 0; i < counter; i++)
		{
			if (status.boardStatus[wordxStart + i][wordyStart].letter != ' ')
			{
				positionConnected = true;
			}			
		}
	}
	return positionConnected;
}


bool HorizontalConnectionCheck(game_status status, int counter, int wordxStart, int wordyStart)//check connection of word in position
{
	bool positionConnected = false;

	if (wordxStart == BOARD / 2 &&
		wordyStart <= BOARD / 2 &&
		wordyStart + counter-1 >= BOARD / 2)//chech that word cross the center of board, in this case word is always connected
	{
		positionConnected = true;
	}
	else if (wordyStart + counter < BOARD && status.boardStatus[wordxStart][wordyStart + counter].letter != ' ')//check boundary position
	{
		positionConnected = true;
	}
	if (wordyStart - 1 > 0 && status.boardStatus[wordxStart][wordyStart - 1].letter != ' ')
	{
		positionConnected = true;
	}
	if (positionConnected == false)//check position, which word will be boarding and be on it
	{

		for (int i = 0; i < counter; i++)
		{
			if (status.boardStatus[wordxStart][wordyStart + i].letter != ' ')
			{
				positionConnected = true;
			}
		}
	}
	return positionConnected;
}


void GetBackPlayerLetter(int who, game_status &status, player_letter playerletter[])//get player letter back on right position
{
	if (who == COMPUTER)
	{
		for (int i = 0; i < LETTER_ON_HAND; i++)
		{
			status.playerLetter[i] = playerletter[i];
		}
	}
}


void MakeLetterFalse(game_status &status)//make all letter false
{
	for (int k = 0; k < LETTER_ON_HAND; k++)
	{
		if (status.playerLetter[k].playerLetter != '?')
		{
			status.playerLetter[k].onUse = false;
		}
	}
}


void AllCheck(game_status status, bool &all)
{
	for (int i = 0; i < LETTER_ON_HAND; i++)
	{
		if ((status.playerLetter[i].onUse == false) || (status.playerLetter[i].playerLetter = '?'))
		{
			all = false;
		}
	}
}

bool HorizontalInput(dict_tree_node *pRoot, game_status &status, char word[], int counter, int who, bool checkOnly,int wordxStart,int wordyStart)//check and put the word on board for horizontal orientation
{
	bool positionMatch = false;
	bool positionConnected = false;
	bool letterFind;
	bool all = true;

	if (wordyStart + counter > BOARD)//check the length of word match to board size
	{
		return false;
	}
	//checking connection of word on a board, if word isn't connected, put on board is impossible
	player_letter playerletter[LETTER_ON_HAND];

	if (who == COMPUTER)
	{
		for (int i = 0; i < LETTER_ON_HAND; i++)
		{
			playerletter[i] = status.playerLetter[i];
			status.playerLetter[i] = status.computerLetter[i];
		}
	}
	positionConnected = HorizontalConnectionCheck(status, counter,wordxStart,wordyStart);

	if (positionConnected)//checking that we have all neccesery letter to put in this position and if in this position isn't any invalid letter
	{
		if (wordyStart + counter <= BOARD)
		{
			for (int i = 0; i < counter; i++)
			{
				letterFind = false;
				if (status.boardStatus[wordxStart][wordyStart + i].letter == ' ')//first check if player have this letter
				{
					for (int j = 0; j < LETTER_ON_HAND; j++)
					{
						if (status.playerLetter[j].playerLetter == word[i]
							&& status.playerLetter[j].onUse == false)
						{
							status.playerLetter[j].onUse = true;
							letterFind = true;
							break;
						}
					}
					if (letterFind == false)
					{
						for (int j = 0; j < LETTER_ON_HAND; j++)//second check blank option
						{
							if (status.playerLetter[j].playerLetter == '!'
								&& status.playerLetter[j].onUse == false)
							{
								status.playerLetter[j].onUse = true;
								letterFind = true;
								break;
							}
						}
					}
				}
				else if (status.boardStatus[wordxStart][wordyStart + i].letter == word[i])//third existence of this letter already on board
				{
					letterFind = true;
				}
				if (letterFind == false)//if any letter is not found, we put false value on use for every letter checked to point of function
				{
					MakeLetterFalse(status);
					GetBackPlayerLetter(who, status, playerletter);
					return false;
				}
			}
			if (CheckAllNewWord(pRoot, status, wordxStart, wordyStart, wordxStart, wordyStart + counter - 1, word) == false)//check all new words created by this word
			{
				MakeLetterFalse(status);
				GetBackPlayerLetter(who, status, playerletter);
				return false;
			}
			if (AnyLetterUse(status) == false)//check if the player use any letter
			{
				GetBackPlayerLetter(who, status, playerletter);
				return false;
			}
			AllCheck(status, all);
			if (checkOnly)//if we call this function only to check, we return true
			{
				MakeLetterFalse(status);
				GetBackPlayerLetter(who, status, playerletter);
				return true;
			}
			if (who == COMPUTER)
			{
				for (int i = 0; i < LETTER_ON_HAND; i++)
				{
					status.computerLetter[i].onUse = status.playerLetter[i].onUse;
					status.playerLetter[i] = playerletter[i];
				}
			}
			for (int i = 0; i < counter; i++)//if during process we not meet any error, we finally put word on board
			{
				status.boardStatus[wordxStart][wordyStart + i].letter = word[i];
			}
			//finally, we get points and letters
			CountPoint(status, word, counter, wordxStart, wordyStart, HORIZONTAL, who, all,false);
			LetterGet(status,who);
			return false;
		}
	}
	GetBackPlayerLetter(who, status, playerletter);
	return false;
}





bool VerticalInput(dict_tree_node *pRoot, game_status &status, char word[], int counter, int who, bool checkOnly,int wordxStart,int wordyStart)//check and put the word for vertical orientation
{
	bool positionMatch = false;
	bool positionConnected = false;
	bool letterFind;
	bool all = true;

	if (wordxStart + counter > BOARD)//check the length of word match to board size
	{
		return false;
	}
	player_letter playerletter[LETTER_ON_HAND];
	if (who == COMPUTER)
	{
		for (int i = 0; i < LETTER_ON_HAND; i++)
		{
			playerletter[i] = status.playerLetter[i];
			status.playerLetter[i] = status.computerLetter[i];
		}
	}
	positionConnected = VerticalConnectionCheck(status, counter,wordxStart,wordyStart);

	if (positionConnected)//if position is connected, we check match to board and have player all neccesery letter
	{
		if (wordxStart + counter <= BOARD)
		{
			for (int i = 0; i < counter; i++)
			{
				letterFind = false;
				if (status.boardStatus[wordxStart + i][wordyStart].letter == ' ')//first if on position is no letter, we check if we have this letter
				{
					for (int j = 0; j < LETTER_ON_HAND; j++)
					{
						if (status.playerLetter[j].playerLetter == word[i]
							&& status.playerLetter[j].onUse == false)
						{
							status.playerLetter[j].onUse = true;
							letterFind = true;
							break;
						}
					}
					if (letterFind == false)//if not we check blank
					{
						for (int j = 0; j < LETTER_ON_HAND; j++)
						{
							if (status.playerLetter[j].playerLetter == '!'
								&& status.playerLetter[j].onUse == false)
							{
								status.playerLetter[j].onUse = true;
								letterFind = true;
								break;
							}
						}
					}
				}
				else if (status.boardStatus[wordxStart + i][wordyStart].letter == word[i])//if on position is any letter, we check what is this letter
				{
					letterFind = true;
				}
				if (letterFind == false)//id we don't find letter, we return
				{
					MakeLetterFalse(status);
					GetBackPlayerLetter(who, status, playerletter);
					return false;
				}
			}
			if (CheckAllNewWord(pRoot, status, wordxStart, wordyStart, wordxStart + counter - 1, wordyStart, word) == false)//if everything is correct, we check all new word in dictionary
			{
				MakeLetterFalse(status);
				GetBackPlayerLetter(who, status, playerletter);
				return false;
			}
			if (AnyLetterUse(status) == false)
			{
				GetBackPlayerLetter(who, status, playerletter);
				return false;
			}
			AllCheck(status, all);
			
			if (checkOnly)//if we use this function only to check, we return true, if we can't put this word, we return false earlier
			{
				MakeLetterFalse(status);
				GetBackPlayerLetter(who, status, playerletter);
				return true;
			}
			if (who == COMPUTER)
			{
				for (int i = 0; i < LETTER_ON_HAND; i++)
				{
					status.computerLetter[i].onUse = status.playerLetter[i].onUse;
					status.playerLetter[i] = playerletter[i];
				}
			}
			for (int i = 0; i < counter; i++)
			{
				status.boardStatus[wordxStart + i][wordyStart].letter = word[i];
			}
			//if every thing is correct, we add point and get new letter
			CountPoint(status, word, counter, wordxStart, wordyStart, VERTICAL, who, all, false);
			LetterGet(status, who);
			return false;
		}
	}
	GetBackPlayerLetter(who, status, playerletter);
	return false;
}


bool CheckWord(game_status status, char word[], int counter, dict_tree_node *pRoot)//check that it is possible to put this word on a board
{
	bool posMatch = true;
	for (int i = 0; i <= BOARD - counter; i++)//we checked possibility of use for every position on board
	{
		for (int j = 0; j < BOARD; j++)
		{
			posMatch = VerticalInput(pRoot, status, word, counter, PLAYER, true, i, j);
			if (posMatch)
			{
				return true;
			}
		}
	}
	for (int i = 0; i < BOARD; i++)
	{
		for (int j = 0; j <= BOARD - counter; j++)
		{
			posMatch = HorizontalInput(pRoot, status, word, counter, PLAYER, true, i, j);
			if (posMatch)
			{
				return true;
			}
		}
	}
	return false;
}


void PrintPointStatus(game_status status, int orientation, cursor_position cursorPosition, int counter, char word[])
{
	int wordyStart = (cursorPosition.x - BOARD_X_POS - 2) / 2;
	int wordxStart = cursorPosition.y - 1 - BOARD_Y_POS;

	textbackground(BLACK);
	textcolor(LIGHTGRAY);
	gotoxy(BOARD_X_POS, BOARD_Y_POS + BOARD + 2);
	clreol();
	gotoxy(BOARD_X_POS, BOARD_Y_POS + BOARD + 2);
	if ((orientation == HORIZONTAL && wordyStart + counter > BOARD) || (orientation == VERTICAL && wordxStart + counter > BOARD))
	{
		cputs("Miplace");
		return;
	}
	char * tab = new char[counter * 8];
	if (tab == NULL)
	{
		cputs("MEMORY ERROR");
		return;
	}


	char wordBonus[BOARD];
	int wordBonusCount = 0;
	int position = 1;
	tab[0] = '(';
	for (int i = 0; i < counter; i++)
	{
		if (word[i] == 'q' || word[i] == 'z')
		{
			tab[position] = '1';
			tab[position + 1] = '0';
			position += 2;
		}
		else
		{
			tab[position] = '0' + LETTER_POINT[word[i] - 'a'];
			position++;
		}
		tab[position] = '(';
		tab[position+1] = word[i];
		tab[position+2] = ')';
		position += 3;
		if (orientation == HORIZONTAL)
		{
			if (status.boardStatus[wordxStart][wordyStart + i].special == doubleLetter)
			{
				tab[position] = '*';
				tab[position + 1] = '0'+DOUBLE_LETTER;
				position += 2;
			}
			else if (status.boardStatus[wordxStart][wordyStart + i].special == tripleLetter)
			{
				tab[position] = '*';
				tab[position + 1] = '0' + TRIPLE_LETTER;
				position += 2;
			}
			else if (status.boardStatus[wordxStart][wordyStart + i].special == doubleWord)
			{
				wordBonus[wordBonusCount] = '0'+DOUBLE_WORD;
				wordBonusCount++;
			}
			else if (status.boardStatus[wordxStart][wordyStart + i].special == tripleWord)
			{
				wordBonus[wordBonusCount] = '0'+TRIPLE_WORD;
				wordBonusCount++;
			}
		}
		else if (orientation == VERTICAL)
		{
			if (status.boardStatus[wordxStart + i][wordyStart].special == doubleLetter)
			{
				tab[position] = '*';
				tab[position + 1] = '0' + DOUBLE_LETTER;
				position += 2;
			}
			else if (status.boardStatus[wordxStart + 1][wordyStart].special == tripleLetter)
			{
				tab[position] = '*';
				tab[position + 1] = '0' + TRIPLE_LETTER;
				position += 2;
			}
			else if (status.boardStatus[wordxStart + i][wordyStart].special == doubleWord)
			{
				wordBonus[wordBonusCount] = '0' + DOUBLE_WORD;
				wordBonusCount++;
			}
			else if (status.boardStatus[wordxStart + i][wordyStart].special == tripleWord)
			{
				wordBonus[wordBonusCount] = '0' + TRIPLE_WORD;
				wordBonusCount++;
			}
		}
		if (i < counter - 1)
		{
			tab[position] = '+';
			position++;
		}
	}
	tab[position] = ')';
	position++;
	for (int i = 0; i < wordBonusCount; i++)
	{
		tab[position] = '*';
		tab[position + 1] = wordBonus[i];
		position += 2;
	}
	tab[position] = ')';
	tab[position + 1] = NULL;
	gotoxy(BOARD_X_POS, BOARD_Y_POS + BOARD + 2);
	cputs(tab);
	delete[] tab;
}


bool PutWord(dict_tree_node *pRoot, game_status &status, cursor_position &cursorPosition, char word[], int counter, bool LearningMode)//player chose position and orientation of the word
{
	int zn;
	int orientation = HORIZONTAL;
	
	do
	{
		if (LearningMode)
		{
			PrintPointStatus(status, orientation, cursorPosition, counter, word);
		}
		BoardPrint(status);
		PrintWord(status, cursorPosition, word, counter, orientation);
		zn = getch();
		
		if (zn == 'o')
		{
			if (orientation == HORIZONTAL)
			{
				orientation = VERTICAL;
			}
			else
			{
				orientation = HORIZONTAL;
			}
		}
		else if (zn == 0)
		{
			zn = getch();		// the next code knowing that this
			CursorPositioning(cursorPosition, zn);
		}
		else if (zn == ENTER)//enter
		{
			int wordyStart = (cursorPosition.x - BOARD_X_POS - 2) / 2;
			int  wordxStart = cursorPosition.y - 1 - BOARD_Y_POS;
			if (orientation == HORIZONTAL)
			{
				return HorizontalInput(pRoot, status, word, counter, PLAYER, false, wordxStart, wordyStart);
			}
			else
			{
				return VerticalInput(pRoot, status, word, counter,PLAYER, false, wordxStart, wordyStart);
			}
		}
	} while (zn != ESC);//esc
	
	return true;
}


bool WordInput(dict_tree_node *pRoot, game_status &status, cursor_position &cursorPosition, bool LearningMode)//initialize after i click, putting word,checking, and put on a board, if we quit the function using escape-return true, else-false
{
	int zn;//zn remember player input
	char word[BOARD];//array with the word to put
	int counter = 0;//number of letters in word
	
	for (int i = 0; i < BOARD; i++)
	{
		word[i] = '\0';
	}
	
	do
	{
		PrintWordToPut(word, status, counter);//adding new letter to our word
		zn = getch();
		if (zn >= 'a' && zn <= 'z')
		{
			if (counter < BOARD)
			{
				word[counter] = zn;
				counter++;
			}
		}
		else if (zn == BACKSPACE)//backspace
		{
			if (counter > 0)
			{				
				counter--;
			}
			word[counter] = '\0';

		}
		else if (zn == ENTER)//enter
		{
			textbackground(BLACK);
			textcolor(LIGHTGRAY);
			if (counter > 0 && counter<BOARD)
			{
				if (DictTreeFindWord(pRoot, word, counter))
				{
					if (CheckWord(status, word, counter,pRoot))
					{
						int score = status.playerScore;
						if(PutWord(pRoot, status, cursorPosition, word, counter, LearningMode) == false)
						{
							if (score == status.playerScore)
							{
								gotoxy(BOARD_X_POS, BOARD_Y_POS + BOARD + 9);
								cputs("Blad Wstawiania slowa");
							}
						}
						else
						{
							return false;
						}
					}
					else
					{
						gotoxy(BOARD_X_POS, BOARD_Y_POS + BOARD + 9);
						cputs("Slowa nie da sie wstawic");
					}
				}
				else
				{
					gotoxy(BOARD_X_POS, BOARD_Y_POS + BOARD + 9);
					cputs("Slowa nie ma w slowniku");
				}
			}
			else
			{
				gotoxy(BOARD_X_POS, BOARD_Y_POS + BOARD + 9);
				cputs("Nie podano slowa");
			}
			return true;
		}
		
		if (counter > 0 && LearningMode==true)
		{
			gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 19);
			textcolor(LIGHTGRAY);
			textbackground(BLACK);
			clreol();
			bool status = DictTreeFindWord(pRoot, word, counter);
			if (status == true)
			{
				cputs("word is in dictionary");
			}
			else
			{
				cputs("word is NOT in dictionary");
			}
		}
	} while (zn!=ESC);//esc
	
	return false;
}


void swap(char &x, char &y) //function swap two characters
{
	char tmp = x;
	x = y;
	y = tmp;
}


int NewGame(game_status &status)//create new game, rand a bag, player and computer hand
{
	clrscr();
	MakeTableEmpty(status);
	status.computerScore = 0;
	status.playerScore = 0;
	status.letterOnBag = 0;
	
	for (int i = 0; i < LETTERS_ON_ALPHABET; i++)//count how many letters will be on bag
	{
		status.letterOnBag += LETTER_COUNT[i];
	}
	status.letterOnBag += BLANK_COUNT;
	status.bag = (char*)realloc(status.bag, sizeof(char)*status.letterOnBag);//resizing bag from default range to correct one
	
	if (status.bag == NULL)
	{
		cputs("BLAD, brak pamieci!!!");
		return 0;
	}

	int counter = 0;
	for (int i = 0; i < LETTERS_ON_ALPHABET; i++)//put all letters on a bag, depending on their configuration
	{
		for (int j = 0; j < LETTER_COUNT[i]; j++)
		{
			status.bag[counter] = 'a' + i;
			counter++;
		}
	}
	
	for (int i = 0; i < BLANK_COUNT; i++)
	{
		status.bag[counter] = '!';
		counter++;
	}
	srand(time(NULL));
	
	int x;
	for (int i = 0; i < 10; i++)//randomise position on all letters
	{
		for (int j = 0; j < status.letterOnBag; j++)
		{
			x = rand() % status.letterOnBag;
			swap(status.bag[i], status.bag[x]);
		}
	}

	for (int i = 0; i < LETTER_ON_HAND; i++)//taking player hand
	{
		status.playerLetter[i].playerLetter = status.bag[status.letterOnBag-1];
		status.playerLetter[i].onUse = false;
		status.letterOnBag--;
	}
	
	for (int i = 0; i < LETTER_ON_HAND; i++)//taking computer hand
	{
		status.computerLetter[i].playerLetter = status.bag[status.letterOnBag-1];
		status.computerLetter[i].onUse = false;
		status.letterOnBag--;
	}
	PrintPoint(status);
	
	return 1;
}


void PrintPlayerLetter(game_status status)//function prints player letter
{
	gotoxy(BOARD_X_POS, BOARD_Y_POS + BOARD + 5);
	cputs("Litery gracza:");
	textcolor(LIGHTGRAY);
	gotoxy(BOARD_X_POS, BOARD_Y_POS + BOARD + 5);
	
	for (int i = 0; i < LETTER_ON_HAND; i++)
	{
		if (status.playerLetter[i].onUse == true)
		{
			textbackground(BLUE);
		}
		putch(status.playerLetter[i].playerLetter);
		textbackground(BLACK);
		cputs(", ");
	}
}


bool SwapLetter(game_status &status)//swap letter on user hand
{
	int zn;
	int counter = 0;
	char letterOnHand = '0' + LETTER_ON_HAND;
	int number;
	
	srand(time(NULL));
	if (status.letterOnBag <= 0)
	{
		gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 20);
		cputs("Nie ma liter");
		return false;
	}
	while (1)
	{
		PrintPlayerLetter(status);
		zn = getch();
		if (zn >= '1'&&zn <= letterOnHand)//if player put a number, we change on use parameter
		{
			if (counter < status.letterOnBag)
			{
				zn -= '1';
				if (status.playerLetter[zn].onUse == false)
				{
					status.playerLetter[zn].onUse = true;
					counter++;
				}
				else
				{
					status.playerLetter[zn].onUse = false;
					counter--;
				}
			}
		}
		else if (zn == 'w' || zn == ENTER)//enter
		{
			if (counter >= status.letterOnBag)
			{
				gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 20);
				cputs("Za malo liter");
				return false;
			}
			//after enter player letters are remembered on table, player letter are changed, then player letter are put on a random position in bag
			char * tab = new char[counter];
			
			for (int i = 0; i < counter; i++)
			{
				for (int j = 0; j < LETTER_ON_HAND; j++)
				{
					if (status.playerLetter[j].onUse == true && status.playerLetter[j].playerLetter!='?')
					{
						status.playerLetter[j].onUse = false;
						tab[i] = status.playerLetter[j].playerLetter;
						status.playerLetter[j].playerLetter = status.bag[status.letterOnBag];
						status.letterOnBag--;
						break;
					}
				}	
			}
			
			for (int i = 0; i < counter; i++)
			{
				number = rand() % status.letterOnBag;
				status.letterOnBag++;
				status.bag[status.letterOnBag] = status.bag[number];
				status.bag[number] = tab[i];
			}
			delete[] tab;
		
			return true;
		}
		else if (zn == ESC)
		{
			break;
		}
	}
	return false;
}


void SaveGame(game_status status)//save actual game status to the file
{
	int zn;
	char tab[1000];
	int counter = 0;

	gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 20);
	cputs("Podaj nazwe pliku:");
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 21);

	do//in this part we get file name from user
	{
		zn = getch();
		gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 21);
		clreol();
		if (zn == BACKSPACE)
		{
			counter--;
		}
		else if (zn != ENTER)
		{
			tab[counter] = zn;
			counter++;
		}	
		for (int i = 0; i < counter; i++)
		{
			putch(tab[i]);
		}
	} while (zn != ENTER);
	
	tab[counter] = '\0';

	FILE *ptf = fopen( tab, "w");//we open the file or create with name get from user, and save all data
	if(ptf==NULL)
	{
		cputs("BLAD PLIKU!!!");
		return;
	}
	for (int i = 0; i < BOARD; i++)//save board
	{
		for (int j = 0; j < BOARD; j++)
		{
			if (status.boardStatus[i][j].letter != ' ')
			{
				fprintf(ptf, "%c ", status.boardStatus[i][j].letter);
			}
			else
			{
				fprintf(ptf, ".");
			}
		}
	}
	fprintf(ptf, "\n%d\n", status.letterOnBag);//save letter count and letter in proper order
	
	for (int i = 0; i < status.letterOnBag; i++)
	{
		fprintf(ptf, "%c ", status.bag[i]);
	}
	fprintf(ptf, "\n%d\n", status.playerScore);//save player letter and point
	
	for (int i = 0; i < LETTER_ON_HAND; i++)
	{
		fprintf(ptf, "%c ", status.playerLetter[i].playerLetter);
	}
	fprintf(ptf, "\n%d\n", status.computerScore);//save computer letter and score
	
	for (int i = 0; i < LETTER_ON_HAND; i++)
	{
		fprintf(ptf, "%c ", status.computerLetter[i].playerLetter);
	}
	fclose(ptf);
}


void LoadGame(game_status &status)//load game status form external file, file must exist, name get from user
{
	int zn;
	char tab[1000];
	int counter = 0;

	gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 20);
	cputs("Podaj nazwe pliku do wczytania:");
	gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 21);

	do//getting file name
	{
		zn = getch();
		gotoxy(LEGEND_X_POS, LEGEND_Y_POS + 21);
		clreol();
		if (zn == BACKSPACE)
		{
			counter--;
		}
		else if (zn != ENTER)
		{
			tab[counter] = zn;
			counter++;
		}
		for (int i = 0; i < counter; i++)
		{
			putch(tab[i]);
		}
	} while (zn != ENTER);
	tab[counter] = NULL;
	FILE *ptf; 
	ptf = fopen(tab, "r");
	if (ptf == NULL)
	{
		cputs("BLAD PLIKU!!!");
		return;
	}
	char zmienna;
	for (int i = 0; i < BOARD; i++)//getting board status
	{
		for (int j = 0; j < BOARD; j++)
		{
			fscanf(ptf,"%c ", &zmienna);
			if (zmienna == '.')
			{
				status.boardStatus[i][j].letter = ' ';
			}
			else
			{
				status.boardStatus[i][j].letter =zmienna;
			}
		}
	}
	fscanf(ptf, "%d\n", &status.letterOnBag);//getting number of letter in bag and letter in proper order
	
	for (int i = 0; i < status.letterOnBag; i++)
	{
		fscanf(ptf, "%c ", &status.bag[i]);
	}
	fscanf(ptf, "%d\n", &status.playerScore);//getting player letter and score
	
	for (int i = 0; i < LETTER_ON_HAND; i++)
	{
		fscanf(ptf, "%c ", &status.playerLetter[i].playerLetter);
	}
	fscanf(ptf, "%d\n", &status.computerScore);//getting computer letter and score
	
	for (int i = 0; i < LETTER_ON_HAND; i++)
	{
		fscanf(ptf, "%c ", &status.computerLetter[i].playerLetter);
	}
	fclose(ptf);
}


bool CreateDictionary(FILE* dictionary, dict_tree_node **ppRoot)//function create dictionary
{
	char ch = 0;
	char word[BOARD + 1];
	int counter = 0;

	while (fscanf(dictionary, "%c", &ch) != EOF)//while we met EOF, we read word
	{
		if (ch >= 'A' && ch <= 'Z')
		{
			ch = 'a' - 'A' + ch;
		}
		if (ch >= 'a' && ch <= 'z')
		{
			word[counter] = ch;
			counter++;
			if (counter >= BOARD)//if current word is longer then we simple ignore that word
			{
				while ((ch >= 'a' && ch <= 'z') || 
					   (ch >= 'A' && ch <= 'Z'))
				{
					fscanf(dictionary, "%c", &ch);
				}
				counter = 0;;
			}
		}
		else//if we met any char, which isn't a letter, we end word finding and add this word to dictionary
		{
			if (counter > 0 && counter < BOARD)
			{
				word[counter] = NULL;
				if (DictTreeAddWord(ppRoot, word, counter) == false)
				{
					cputs("NO MEMORY");
					
					return false;//dictionary error
				}
				counter = 0;
			}
		}

	}
	
	return true;
}

//function put to the board copy letter from hand, after it check the possibility of put and return result of maksimum possibility
computer_Word CompWordCheck(char word[], char pos[], int x, int y, int count, game_status status, dict_tree_node *pRoot, bool orientation)
{
	int counter = 0;//count letter from hand we use
	int blankCount = 0;//count number of blank
	int blankPos[BLANK_COUNT];//save blank position
	bool all = false;
	computer_Word computerWord;//return value

	for (int i = 0; i < count; i++)//we put letter position to the word
	{
		if (word[i] == NULL)
		{
			if (pos[counter] == '!')
			{
				blankPos[blankCount] = i;
				blankCount++;
				word[i] = pos[counter];
			}
			else
			{
				word[i] = pos[counter];
			}
			counter++;
		}
	}
	if (counter >= 7)//if we use all letter, all parameter true
	{
		all = true;
	}
	if (counter < 1)//if we don't use any letter, we return 
	{
		return computerWord;
	}
	if (blankCount > 0)//if any blank is use, we need to check all 26 version of use this blank
	{
		long long int power = Power(LETTERS_ON_ALPHABET, blankCount);
		for (int i = 0; i < power; i++)
		{
			int number = i;
			for (int j = 0; j < blankCount; j++)
			{
				word[blankPos[j]] = 'a' + (number % 26);
				number /= 26;
			}
			if (DictTreeFindWord(pRoot, word, count))//check existance in dictionary
			{
				if (orientation == HORIZONTAL)
				{
					if (HorizontalInput(pRoot, status, word, count, COMPUTER, true, x, y))
					{
						int point = CountPoint(status, word, count, x, y, HORIZONTAL, COMPUTER, all, true);
						if (point > computerWord.maks)
						{
							computerWord.maks = point;
							for (int k = 0; k < count; k++)
							{
								computerWord.word[k] = word[k];
							}
						}
					}
				}
				else
				{
					if (VerticalInput(pRoot, status, word, count, COMPUTER, true, x, y))
					{
						int point = CountPoint(status, word, count, x, y, VERTICAL, COMPUTER, all, true);
						if (point > computerWord.maks)
						{
							computerWord.maks = point;
							for (int k = 0; k < count; k++)
							{
								computerWord.word[k] = word[k];
							}
						}
					}
				}
			}
		}
	}
	else
	{
		if (DictTreeFindWord(pRoot, word, count))//check if the word is in dictionary, if not, there is no point in checking posibility of use
		{
			if (orientation == HORIZONTAL)
			{
				if (HorizontalInput(pRoot, status, word, count, COMPUTER, true, x, y))
				{
					int point = CountPoint(status, word, count, x, y, HORIZONTAL, COMPUTER, all, true);
					if (point > computerWord.maks)
					{
						computerWord.maks = point;
						for (int k = 0; k < count; k++)
						{
							computerWord.word[k] = word[k];
						}
					}
				}
			}
			else
			{
				if (VerticalInput(pRoot, status, word, count, COMPUTER, true, x, y))
				{
					int point = CountPoint(status, word, count, x, y, VERTICAL, COMPUTER, all, true);
					if (point > computerWord.maks)
					{
						computerWord.maks = point;
						for (int k = 0; k < count; k++)
						{
							computerWord.word[k] = word[k];
						}
					}
				}
			}
		}
	}
	
	return computerWord;
}


void ComputerLetterSwap(game_status &status)
{
	srand(time(NULL));
	int count;
	if (status.letterOnBag <= 0)
	{
		return;
	}
	if (status.letterOnBag <= LETTER_ON_HAND)
	{
		count = rand() % status.letterOnBag;
	}
	else
	{
		count = rand() % (LETTER_ON_HAND + 1);
	}
	for (int i = 0; i < count; i++)
	{
		int number = rand() % LETTER_ON_HAND;
		if (status.computerLetter[number].onUse == false)
		{
			status.computerLetter[number].onUse = true;
		}
		else if (status.computerLetter[number].playerLetter != '?')
		{
			i--;
		}
	}
	char * tab = new char[count];
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < LETTER_ON_HAND; j++)
		{
			if (status.computerLetter[j].onUse == true && status.computerLetter[j].playerLetter != '?')
			{
				status.computerLetter[j].onUse = false;
				tab[i] = status.computerLetter[j].playerLetter;
				status.computerLetter[j].playerLetter = status.bag[status.letterOnBag];
				status.letterOnBag--;
				break;
			}
		}
	}

	for (int i = 0; i < count; i++)
	{
		int number = rand() % status.letterOnBag;
		status.letterOnBag++;
		status.bag[status.letterOnBag] = status.bag[number];
		status.bag[number] = tab[i];
	}
	delete[] tab;
}


void ComputerMove(game_status &status, dict_tree_node *pRoot)//function do computer move
{
	computer_Word computerWords;
	int x, y, orientation, number1=0;
	
	for (int i = 0; i < BOARD; i++)
	{
		for (int j = 0; j < BOARD; j++)
		{
			int count = 0;//count of maksimum potential letter in horizontal orientation
			int count1 = 0;//same for vertical
			if (((j > 0) && status.boardStatus[i][j - 1].letter == ' ') || (j == 0))//if any letter is position before, there is no chance to get better word
			{
				for (int k = 0; k < LETTER_ON_HAND + 1 + count; k++)//getting letter posible to use from board
				{
					if (j + k < BOARD)
					{
						if (status.boardStatus[i][j + k].letter != ' ')
						{
							count++;
						}
					}
				}
				count += LETTER_ON_HAND;
				if (count >= BOARD)
				{
					count = BOARD - 1;
				}
			}
			char word[BOARD + 1];//after we get all potential letter from board, we allocate word and put the letter from board to it
			memset(word, NULL, BOARD+1);

			for (int k = 0; k < count; k++)
			{
				if (j + k < BOARD)
				{
					if (status.boardStatus[i][j + k].letter != ' ')
					{
						word[k] = status.boardStatus[i][j + k].letter;
					}
				}
			}
			word[count] = NULL;
			//same for the vertical orientation
			if (((i > 0) && status.boardStatus[i-1][j].letter == ' ') || (i == 0))
			{
				for (int k = 0; k < LETTER_ON_HAND + 1 + count1; k++)
				{
					if (i + k < BOARD)
					{
						if (status.boardStatus[i + k][j].letter != ' ')
						{
							count1++;
						}
					}
				}
				count1 += LETTER_ON_HAND;
				if (count1 >= BOARD)
				{
					count1 = BOARD - 1;
				}
			}
			char word1[BOARD+1];//vertical orientation word
			memset(word1, NULL, BOARD+1);

			for (int k = 0; k < count; k++)
			{
				if (i + k < BOARD)
				{
					if (status.boardStatus[i + k][j].letter != ' ')
					{
						word1[k] = status.boardStatus[i + k][j].letter;
					}
				}
			}
			word[count1] = NULL;

			long long int strong = Strong(LETTER_ON_HAND);//
			char letterPos[LETTER_ON_HAND];
			memset(letterPos, NULL, sizeof(char)*LETTER_ON_HAND);
			
			char wordCopy[BOARD];
			char word1Copy[BOARD];

			//we check for every possible letter combination
			for (int m = 0; m < LETTER_ON_HAND; m++)//for every possible letter count
			{
				for (int k = 0; k < strong; k++)//we test every combination of the letter,
				{
					for (int l = 0; l < count; l++)//copy a word, due to original position can't be move
					{
						wordCopy[l] = word[l];
					}
					for (int l = 0; l < count1; l++)
					{
						word1Copy[l] = word1[l];
					}
					int number = k;
					for (int l = 0; l < LETTER_ON_HAND; l++)
					{
						int pos = number % (LETTER_ON_HAND - l);
						for (int m = 0; m <= pos; m++)
						{
							if (letterPos[m] != NULL)
							{
								pos++;
							}
						}
						letterPos[pos] = status.computerLetter[l].playerLetter;
						number /= LETTER_ON_HAND - l;
					}
					computer_Word tmp = CompWordCheck(wordCopy, letterPos, i, j, count, status, pRoot, HORIZONTAL);//copy result word,if it is better then current, chang current to it
					if(tmp.maks>computerWords.maks)
					{
						computerWords = tmp;
						x = i;
						y = j;
						number1 = count;
						orientation = HORIZONTAL;
					}
					tmp = CompWordCheck(word1Copy, letterPos, i, j, count1, status, pRoot, VERTICAL);
					if (tmp.maks > computerWords.maks)
					{
						computerWords = tmp;
						x = i;
						y = j;
						number1 = count1;
						orientation = VERTICAL;
					}

					memset(letterPos, NULL, LETTER_ON_HAND);//make letterpos null as well as words copy
					memset(wordCopy, NULL, count);
					memset(word1Copy, NULL, count1);
				}
				while (count > 0 && word[count - 1] != NULL) //after check for seven letter, we check for six etc
				{
					count--;//if in the last position of word is letter, we can't use it, co count go one down
				}
				count--;
				while (count1 > 0 && word1[count1 - 1] != NULL)
				{
					count1--;
				}
				count1--;
				if (count <= 0 || count1 <= 0)//if we go lower than zero we break
				{
					break;
				}
			}
		}
	}
	if (computerWords.maks > 0)//if any word find, we put it on a board
	{
		if (orientation == HORIZONTAL)
		{
			HorizontalInput(pRoot, status, computerWords.word, number1, COMPUTER, false, x, y);
		}
		else
		{
			VerticalInput(pRoot, status, computerWords.word, number1, COMPUTER, false, x, y);
		}
	}
	else//if any word can't be put, computer chang letter
	{
		ComputerLetterSwap(status);
	}
}


char *DefaultDictionaryPath(char *pProgramPath)//function get a path to a default dictionary
{
	int pathLength = 0;
	int current = 0;
	char *pDictPath;

	for (pathLength = 0, current=0; pProgramPath[current] != 0; current++)
	{
		if (pProgramPath[current] == '\\')
		{
			pathLength = current;
		}
	}

	pathLength++;	// Include last backslash in the path length

	pDictPath = (char *)malloc(pathLength + strlen(DEFAULT_DICTIONARY) + 1);
	if (!pDictPath)
		return NULL;
	memcpy(pDictPath, pProgramPath, pathLength);
	memcpy(pDictPath + pathLength, DEFAULT_DICTIONARY, strlen(DEFAULT_DICTIONARY));
	pDictPath[pathLength + strlen(DEFAULT_DICTIONARY)] = 0;

	return pDictPath;
}


int main(int argc, char *argv[]) 
{
	cursor_position cursorPosition;
	cursorPosition.x = BOARD_X_POS + 2;
	cursorPosition.y = BOARD_Y_POS + 1;//inizialize cursor
	char zn = 0;
	dict_tree_node *pRoot=NULL;//take dictionary
	if (DictTreeCreateTree(&pRoot) == false)
	{
		cputs("NO MEMORY");
		return 3;//dictionary error
	}
	FILE* dictionary;
	if (argc > 1 && argv[1]!=NULL)
	{
		dictionary = fopen(argv[1], "r");
	}
	else
	{
		char *pDictPath = DefaultDictionaryPath(argv[0]);
		dictionary = fopen(pDictPath, "r");
		free(pDictPath);
	}
	if (dictionary == NULL)
	{
		cputs("DICTIONARY ERROR");
		return 3;//dictionary error
	}
	if (CreateDictionary(dictionary, &pRoot) == false)//create dictionary
	{
		return 3;//dictionary error
	}
	game_status status;//create status
	if (status.bag == NULL)
	{
		cputs("BLAD, brak pamieci!!!");
		return 2;//no memory error
	}
	MakeTableEmpty(status);
#ifndef __cplusplus
	Conio2_Init();
#endif
	// settitle sets the window title
	settitle("Domink, Kwiatkowski, 175454");

	// hide the blinking cursor
	_setcursortype(_NOCURSOR);
	NewGame(status);
	LegendPrinting(cursorPosition);
	PrintPoint(status);
	bool LearningMode = false;
	do
	{//clear all old setups
		gotoxy(LEGEND_X_POS, LEGEND_Y_POS + BOARD + 7);
		clreol();
		gotoxy(LEGEND_X_POS, LEGEND_Y_POS + BOARD + 8);
		clreol();
		gotoxy(BOARD_X_POS, BOARD_Y_POS + BOARD + 9);
		clreol();
		bool computerMove = false;
		//print all meeded things
		LegendPrinting(cursorPosition);
		PrintPoint(status);
		textbackground(BLACK);
		BoardPrint(status);
		PrintPlayerLetter(status);
		gotoxy(cursorPosition.x, cursorPosition.y);
		textcolor(WHITE);
		putch('*');
		zn = getch();
		if (zn == 0)
		{
			zn = getch();		// the next code knowing that this
			CursorPositioning(cursorPosition, zn);
		}
		else if (zn == 't')
		{
			if (LearningMode == false)
			{
				LearningMode = true;
			}
			else
			{
				LearningMode = false;
			}
		}
		else if (zn == 'n')
		{

			if (NewGame(status) == 0)
			{
				return 1;//creating new game error
			}
		}

		else if (zn == 'i')
		{
			computerMove = WordInput(pRoot, status, cursorPosition, LearningMode);
		}
		else if (zn == 'w')
		{
			computerMove = SwapLetter(status);
		}
		else if (zn == 's')
		{
			SaveGame(status);
			clrscr();
		}
		else if (zn == 'l')
		{
			LoadGame(status);
			clrscr();
		}
		for (int i = 2; i < 5; i++)
		{
			gotoxy(1, LEGEND_Y_POS + BOARD + i);
			textbackground(BLACK);
			clreol();
		}
		if (computerMove)
		{
			ComputerMove(status, pRoot);
		}
		bool EndGame = true;
		bool EndGame1 = true;
		for (int i = 0; i < LETTER_ON_HAND; i++)
		{
			if (status.playerLetter[i].onUse == false)
			{
				EndGame = false;
			}
		}
		for (int i = 0; i < LETTER_ON_HAND; i++)
		{
			if (status.computerLetter[i].onUse == false)
			{
				EndGame1 = false;
			}
		}
		
		if (EndGame || EndGame1)
		{
			gotoxy(BOARD_X_POS, BOARD_Y_POS + BOARD + 9);
			if (status.playerScore > status.computerScore)
			{		
				cputs("Gracz Wygral!!!");
			}
			else
			{
				cputs("Komputer wygral!!!");
			}
			gotoxy(BOARD_X_POS, BOARD_Y_POS + BOARD + 10);
			cputs("Nacisnij klawisz by zaczac nowa gre");
			getch();
			NewGame(status);
		}
	}while (zn != 'q');

	_setcursortype(_NORMALCURSOR);	// show the cursor so it will be
	// visible after the program ends
	free(status.bag);
	fclose(dictionary);
	DictTreeRealease(pRoot);
	return 0;
}