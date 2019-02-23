#pragma once
#include <iostream>
#include <string>
#include "CoordXY.h"
#include <stdlib.h>
#include <time.h>

using namespace std;

void SpawnApple(vector <CoordXY>, CoordXY &); //Spawns an apple '@' on randomly generated coordinates.

void PrintField(vector <CoordXY> snake, char field[][8], bool & newApple, CoordXY & apple)
{
	int x, y;
	cout << endl;
	for (int i = 0; i < 8; i++) //Refreshes the playing field, erases the previous snake and apple coordinates.
	{
		for (int j = 0; j < 8; j++)
		{
			if (i == 0 || i == 7)
				field[i][j] = 'X';
			else if (j == 0 || j == 7)
				field[i][j] = 'X';
			else
				field[i][j] = '.';
		}
	}
	if (newApple) //Request for a new apple on the field.
	{
		newApple = false;
		SpawnApple(snake, apple); 
	}
	x = apple.rtnPosX();
	y = apple.rtnPosY();
	field[x][y] = '@'; //Apple is spawned on the field.

	for (int i = snake.size() - 1; i >= 0; i--) //Snake coordinates are placed on the field.
	{
		if (i == 0)
		{
			x = snake[i].rtnPosX();
			y = snake[i].rtnPosY();
			field[x][y] = 'O';
		}
		else if (i == snake.size() - 1)
		{
			x = snake[i].rtnPosX();
			y = snake[i].rtnPosY();
			field[x][y] = '*';
		}
		else
		{
			x = snake[i].rtnPosX();
			y = snake[i].rtnPosY();
			field[x][y] = 'o';
		}
	}
	for (int i = 0; i < 8; i++) //Prints field with the new apple and snake coordinates.
	{
		for (int j = 0; j < 8; j++)
		{
			if (j == 0)
			{
				cout << "   " << field[i][j];
			}
			else
				cout << field[i][j];
		}
		cout << endl;
	}
	cout << endl;
	cout << "w/s/a/d - up/down/left/right." << endl << "X - wall, O - head of snake, o - body of snake, * - tail of snake." << endl;
	cout << endl;
}

void SpawnApple(vector <CoordXY> snake, CoordXY & apple)
{
	int posx, posy;
	bool spawn = true;
	posx = rand() % 6 + 1; //Playing field (without the walls) is 6x6 from 1 to 8.
	posy = rand() % 6 + 1;
	for (int i = 0; i < snake.size(); i++) //Checks if the apple coordinates are already taken by the snake.
	{
		if (posx == snake[i].rtnPosX())
		{
			if (posy == snake[i].rtnPosY()) //Coordinates taken by the snake, generate coordinates again.
			{
				SpawnApple(snake, apple);
				spawn = false;
				break;
			}
		}
	}
	if (spawn == true)
	{
		apple.setPosX(posx);
		apple.setPosY(posy);
	}
}
