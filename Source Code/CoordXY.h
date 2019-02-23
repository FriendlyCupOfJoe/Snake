#pragma once
#include <iostream>
#include <string>

class CoordXY
{
private:
	int posX;
	int posY;
public:
	CoordXY() {};
	~CoordXY() {};
	void setPosX(int x) { this->posX = x; };
	void setPosY(int y) { this->posY = y; };
	int rtnPosX() { return this->posX; };
	int rtnPosY() { return this->posY; };;
};
