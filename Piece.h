#pragma once
#include <iostream>
#include <string>
using namespace std;

class Piece {
public:
	char symbol;
	int posX;
	int posY;
	bool white;
	int pawnMoveLength = 2;
	Piece() {};
	Piece(char _symbol, bool _white, int _posX, int _posY) {
		symbol = _symbol;
		posX = _posX;
		posY = _posY;
		white = _white;
	}
};