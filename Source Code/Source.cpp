#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "CoordXY.h"
#include "PrintField.h"
#include <stdlib.h>
#include <time.h>
#include <conio.h> //biblioteka za _getch();
#include <Windows.h>

using namespace std;

void Startup(vector <CoordXY> &, bool &); //Prints the welcoming message and puts the first part of the snake and an apple on the field.
void InputCheck(char &, vector <CoordXY>); //Asks for an input with a 1 second timer.
bool MovementCheck(char &, vector <CoordXY>); //Checks if the input was w/s/a/d and if the snake was going backwards.
void Move(char &, vector <CoordXY> &, char[][8], bool &); //
bool EndCheck(vector <CoordXY>, char[][8], bool &); //Check if the player has won or if he crashed into the wall or into a part of the snake.
bool Exit(); //Asks the player for a new game.

int main()
{
	srand(time(NULL));
	do
	{
		char movement = 's';
		vector <CoordXY> snake; //Snake that contains multiple parts each with its own X and Y coordinates.
		CoordXY apple;
		char field[8][8];
		bool end = true;
		bool newApple = false; //Request for a new apple spawn.
		system("cls");
		Startup(snake, newApple);
		while (end)
		{
			system("cls");
			PrintField(snake, field, newApple, apple);
			InputCheck(movement, snake);
			Move(movement, snake, field, newApple);
			system("cls");
			end = EndCheck(snake, field, newApple);
			PrintField(snake, field, newApple, apple);
		}
	} while (Exit());
}

void Startup(vector <CoordXY> & snake, bool & newApple)
{
	CoordXY head;
	head.setPosX(1); //Spawns the first part of the snake (head) on the X=1, Y=1.
	head.setPosY(1);
	snake.insert(snake.begin(), head);
	newApple = true; //Spawn the first apple.
	cout << "Welcome to the game of Snake! The goal of the game is to feed the snake apples (@) so it can grow as much as possible." << endl
		<< "You can move the snake (O - head, o - body, * - tail) with w/s/a/d keys to go up/down/left/right." << endl
		<< "Don't crash into the wall (X) or the body (o/*) of the snake. Good luck!" << endl;
	cout << "Press any key to continue." << endl;
	_getch();
}

void InputCheck(char & movement, vector <CoordXY> snake)
{
	DWORD timerStart, timerStop;
	timerStart = GetTickCount();
	timerStop = timerStart + 1000;
	//GetTickCount() - The return value is the number of milliseconds that have elapsed since the system was started.
	while ((timerStop > GetTickCount())) //The player has a second to input a new one otherwise the game will continue with the last correct input.
	{
		/*
		_kbhit returns a nonzero value if a key has been pressed. Otherwise, it returns 0.
		The _kbhit function checks the console for a recent keystroke. If the function returns a nonzero value, a keystroke is waiting in the buffer. The program can then call _getch or _getche to get the keystroke.
		*/
		if (_kbhit())
		{
			movement = _getch();
			if (MovementCheck(movement, snake))
				break;
			else
				continue;
		}
	}
}

bool MovementCheck(char & movement, vector <CoordXY> snake)
{
	static char lastOkMove = 's';
	if (snake.size() == 1 && (movement == 'w' || movement == 's' || movement == 'a' || movement == 'd')) //While the snake only has a head the player can move wherever he wishes.
	{
		lastOkMove = movement; //The new input and the new direction are good.
		return true;
	}
	else
	{
		if ((movement == 'w' || movement == 's' || movement == 'a' || movement == 'd'))
		{
			if (movement == 'w' && lastOkMove != 's')
			{
				lastOkMove = movement;
				return true;
			}
			else if (movement == 's' && lastOkMove != 'w')
			{
				lastOkMove = movement;
				return true;
			}
			else if (movement == 'a' && lastOkMove != 'd')
			{
				lastOkMove = movement;
				return true;
			}
			else if (movement == 'd' && lastOkMove != 'a')
			{
				lastOkMove = movement;
				return true;
			}
		}
		movement = lastOkMove; //The new input or the new direction are not correct, snake moves in the last correctly inputted direction.
		return false;
	}
}

void Move(char & movement, vector <CoordXY> & snake, char field[][8], bool & newApple)
{
	bool coX = false, coY = false, inc = false, dec = false; //Variables that shorten the code.
	CoordXY newPart; //New body part if an apple is eaten.
	int x, y, tempx, tempy;
	bool eaten = false;
	switch (movement) //For every movement first the head is moved to new coordinates, then every other body part is moved to the old coordinates of the previous body part. 
	{
		case('w') :
		{
			coX = true; //If 'w' was pressed the snake moves up, it decreases the X coordinates.
			dec = true;
			break;
		}
		case('s') :
		{
			coX = true; //If 's' was pressed the snake moves down, it increases the X coordinates.
			inc = true;
			break;
		}
		case('a') :
		{
			coY = true; //If 'a' was pressed the snake moves left, it decreases the Y coordinates.
			dec = true;
			break;
		}
		case('d') :
		{
			coY = true; //If 'd' was pressed the snake moves right, it increases the Y coordinates.
			inc = true;
			break;
		}
		default:
		{
			break;
		}
	}
	for (int i = 0; i < snake.size(); i++)
	{
		if (i == 0)
		{
			if (coX == true)
			{
				x = snake[i].rtnPosX();
				tempx = x;
				tempy = snake[i].rtnPosY();
				if (inc == true)
					x++;
				else if (dec == true)
					x--;
				snake[i].setPosX(x); //New head coordinates.
				if (field[x][tempy] == '@') //New head coordinates are on an apple which means the snake has eaten an apple.
					eaten = true;
				else
					eaten = false;
			}
			else if (coY == true)
			{
				y = snake[i].rtnPosY();
				tempy = y;
				tempx = snake[i].rtnPosX();
				if (inc == true)
					y++;
				else if (dec == true)
					y--;
				snake[i].setPosY(y);
				if (field[tempx][y] == '@')
					eaten = true;
				else
					eaten = false;
			}
		}
		else
		{
			x = tempx; //Every body part is moved to the old coordinates of the previous body part.
			y = tempy;
			tempx = snake[i].rtnPosX();
			tempy = snake[i].rtnPosY();
			snake[i].setPosX(x);
			snake[i].setPosY(y);
		}
	}
	if (eaten == true) //An apple has been eaten, new body part is spawned on the old coordinates of the previous last body part and a new apple is spawned.
	{
		newPart.setPosX(tempx);
		newPart.setPosY(tempy);
		snake.push_back(newPart);
		newApple = true;
	}
}
	

bool EndCheck(vector <CoordXY> snake, char field[][8], bool & newApple)
{
	if (snake.size() == 36) //The filed is 8x8 but without the walls it is 6x6 so if the snake fills the whole playing field the player has won.
	{
		cout << "Congratulations! You have won the game!" << endl;
		newApple = false; //The last apple was eaten, no need for another apple to spawn.
		return false;
	}
	else
	{
		int i, j;
		i = snake[0].rtnPosX(); //Coordinates of the head of the snake.
		j = snake[0].rtnPosY();
		if (field[i][j] == 'X') //Head of the snake has crashed into the wall
		{
			cout << "You have lost the game because the snake has crashed into a wall (X), better luck next time." << endl;
			return false;
		}
		else if (field[i][j] == 'o' || field[i][j] == '*') //Head of the snake has crashed into itself.
		{
			cout << "You have lost the game because the snake has crashed into itself (o), better luck next time." << endl;
			return false;
		}
		else
			return true;
	}
}

bool Exit()
{
	string temp;
	cout << "Would you like to play again? Y/y for yes or any other key for no: ";
	getline(cin, temp);
	if (temp[0] == 'y' || temp[0] == 'Y')
		return true;
	else
		return false;
}

