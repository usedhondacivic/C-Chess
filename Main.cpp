#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
#include <algorithm>
#include "Piece.h"

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
vector<Piece> pieces;
string letters = "ABCDEFGH";
string buffer;
int winner = 0;

int whiteOnWhite = 127;
int whiteOnBlack = 143;
int blackOnWhite = 112;
int blackOnBlack = 128;

void setupBoard() {
	//White team
	pieces.push_back(Piece('r', true, 0, 0));
	pieces.push_back(Piece('k', true, 1, 0));
	pieces.push_back(Piece('b', true, 2, 0));
	pieces.push_back(Piece('K', true, 3, 0));
	pieces.push_back(Piece('Q', true, 4, 0));
	pieces.push_back(Piece('b', true, 5, 0));
	pieces.push_back(Piece('k', true, 6, 0));
	pieces.push_back(Piece('r', true, 7, 0));
	pieces.push_back(Piece('p', true, 0, 1));
	pieces.push_back(Piece('p', true, 1, 1));
	pieces.push_back(Piece('p', true, 2, 1));
	pieces.push_back(Piece('p', true, 3, 1));
	pieces.push_back(Piece('p', true, 4, 1));
	pieces.push_back(Piece('p', true, 5, 1));
	pieces.push_back(Piece('p', true, 6, 1));
	pieces.push_back(Piece('p', true, 7, 1));

	//Black team
	pieces.push_back(Piece('r', false, 0, 7));
	pieces.push_back(Piece('k', false, 1, 7));
	pieces.push_back(Piece('b', false, 2, 7));
	pieces.push_back(Piece('K', false, 3, 7));
	pieces.push_back(Piece('Q', false, 4, 7));
	pieces.push_back(Piece('b', false, 5, 7));
	pieces.push_back(Piece('k', false, 6, 7));
	pieces.push_back(Piece('r', false, 7, 7));
	pieces.push_back(Piece('p', false, 0, 6));
	pieces.push_back(Piece('p', false, 1, 6));
	pieces.push_back(Piece('p', false, 2, 6));
	pieces.push_back(Piece('p', false, 3, 6));
	pieces.push_back(Piece('p', false, 4, 6));
	pieces.push_back(Piece('p', false, 5, 6));
	pieces.push_back(Piece('p', false, 6, 6));
	pieces.push_back(Piece('p', false, 7, 6));
}

void drawGridLine(bool startWhite) {
	for (int i = 0; i < 8; i++) {
		if (i % 2 == 0) {
			if (!startWhite)
				SetConsoleTextAttribute(hConsole, whiteOnBlack);
			else
				SetConsoleTextAttribute(hConsole, whiteOnWhite);
		}
		else {
			if (!startWhite)
				SetConsoleTextAttribute(hConsole, whiteOnWhite);
			else
				SetConsoleTextAttribute(hConsole, whiteOnBlack);
		}
		cout << "       ";
	}
}

int findPiece(int x, int y) {
	for (int i = 0; i < pieces.size(); i++) {
		if (pieces[i].posX == x && pieces[i].posY == y) {
			return i;
		}
	}
	return -1;
}

bool checkLine(Piece p, string direction, int fixed, int target) {
	bool valid = true;;

	if (direction == "down") {
		for (int i = p.posY + 1; i < target; i++) {
			if (findPiece(fixed, i) != -1) {
				valid = false;
				break;
			}
		}
	}
	else if (direction == "up") {
		for (int i = p.posY - 1; i > target; i--) {
			if (findPiece(fixed, i) != -1) {
				valid = false;
				break;
			}
		}
	}
	else if (direction == "right") {
		for (int i = p.posX + 1; i < target; i++) {
			if (findPiece(i, fixed) != -1) {
				valid = false;
				break;
			}
		}
	}
	else if (direction == "left") {
		for (int i = p.posX - 1; i > target; i--) {
			if (findPiece(i, fixed) != -1) {
				valid = false;
				break;
			}
		}
	}

	for (int i = 1; i < 8; i++) {
		if (direction == "downRight") {
			if (findPiece(p.posX+i, p.posY+i) != -1) {
				if (pieces[findPiece(p.posX - i, p.posY + i)].posX != target)
					valid = false;
				break;
			}
		}
		else if (direction == "downLeft") {
			if (findPiece(p.posX - i, p.posY + i) != -1) {
				if(pieces[findPiece(p.posX - i, p.posY + i)].posX != target)
					valid = false;
				break;
			}
		}
		else if (direction == "upRight") {
			if (findPiece(p.posX + i, p.posY - i) != -1) {
				if (pieces[findPiece(p.posX - i, p.posY + i)].posX != target)
					valid = false;
				break;
			}
		}
		else if (direction == "upLeft") {
			if (findPiece(p.posX - i, p.posY - i) != -1) {
				if (pieces[findPiece(p.posX - i, p.posY + i)].posX != target)
					valid = false;
				break;
			}
		}
	}
	return valid;
}

bool validMove(Piece p, int x, int y) {
	bool valid = false;
	switch (p.symbol)
	{
	case 'p':
		if (p.white && x == p.posX && y <= p.posY + p.pawnMoveLength && y > p.posY) {
			if(findPiece(x, y) == -1)
				valid = true;
			p.pawnMoveLength = 1;
		}
		else if (!p.white && x == p.posX && y >= p.posY - p.pawnMoveLength && y < p.posY) {
			valid = true;
			p.pawnMoveLength = 1;
		}
		else if (p.white && (x == p.posX-1 || x == p.posX+1) && y == p.posY +1 && findPiece(x,y) != -1) {
			if (!pieces[findPiece(x, y)].white)
				valid = true;
		}
		else if (!p.white && (x == p.posX - 1 || x == p.posX + 1) && y == p.posY - 1 && findPiece(x, y) != -1) {
			if (pieces[findPiece(x, y)].white)
				valid = true;
		}
		break;
	case 'r':
		if (y == p.posY && x > p.posX) {
			valid = checkLine(p, "right", p.posY, x);
		}
		else if (y == p.posY && x < p.posX) {
			valid = checkLine(p, "left", p.posY, x);
		}
		else if (x == p.posX && y>p.posY) {
			valid = checkLine(p, "down", p.posX, y);
		}
		else if (x == p.posX && y<p.posY) {
			valid = checkLine(p, "up", p.posX, y);
		}
		else {
			cout << "fail" << endl;
		}
		break;
	case 'k':
		if ((x == p.posX+1 && y == p.posY+2) || (x == p.posX - 1 && y == p.posY + 2) || (x == p.posX + 1 && y == p.posY - 2) || (x == p.posX - 1 && y == p.posY - 2) || (x == p.posX + 2 && y == p.posY + 1) || (x == p.posX - 2 && y == p.posY + 1) || (x == p.posX + 2 && y == p.posY - 1) || (x == p.posX - 2 && y == p.posY - 1)) {
			if (findPiece(x, y) == -1) {
				valid = true;
				break;
			}

			if (pieces[findPiece(x, y)].white != p.white)
				valid = true;
		}
		break;
	case 'b':
		if ((y - p.posY) == (x - p.posX) && (y - p.posY) > 0) {
			valid = checkLine(p, "downRight", p.posY, x);
		}
		else if ((y - p.posY) == (x - p.posX) && (y - p.posY) < 0) {
			valid = checkLine(p, "upLeft", p.posY, x);
		}
		else if ((y - p.posY) == -(x - p.posX) && (y - p.posY) > 0) {
			valid = checkLine(p, "downLeft", p.posY, x);
		}
		else if ((y - p.posY) == -(x- p.posX) && (y - p.posY) < 0) {
			valid = checkLine(p, "upRight", p.posY, x);
		}
		break;
	case 'K':
		if (x >= p.posX - 1 && x <= p.posX + 1 && y >= p.posY - 1 && y <= p.posY + 1 && (x != p.posX || y != p.posY)) {
			if (findPiece(x, y) == -1) {
				valid = true;
				break;
			}

			if (pieces[findPiece(x, y)].white != p.white)
				valid = true;
		}
		break;
	case 'Q':
		if ((y - p.posY) == (x - p.posX) && (y - p.posY) > 0) {
			valid = checkLine(p, "downRight", p.posY, x);
		}
		else if ((y - p.posY) == (x - p.posX) && (y - p.posY) < 0) {
			valid = checkLine(p, "upLeft", p.posY, x);
		}
		else if ((y - p.posY) == -(x - p.posX) && (y - p.posY) > 0) {
			valid = checkLine(p, "downLeft", p.posY, x);
		}
		else if ((y - p.posY) == -(x - p.posX) && (y - p.posY) < 0) {
			valid = checkLine(p, "upRight", p.posY, x);
		}
		else if (y == p.posY && x > p.posX) {
			valid = checkLine(p, "right", p.posY, x);
		}
		else if (y == p.posY && x < p.posX) {
			valid = checkLine(p, "left", p.posY, x);
		}
		else if (x == p.posX && y>p.posY) {
			valid = checkLine(p, "down", p.posX, y);
		}
		else if (x == p.posX && y<p.posY) {
			valid = checkLine(p, "up", p.posX, y);
		}
		else {
			cout << "fail" << endl;
		}
		break;
	default:
		break;
	}

	return valid;
}

void drawBoard() {
	cout << "        1      2      3      4      5      6      7      8" << endl;
	for (int i = 0; i < 8; i++) {
		SetConsoleTextAttribute(hConsole, 7);
		cout << "     ";
		drawGridLine((i%2 == 0));
		cout << endl;
		SetConsoleTextAttribute(hConsole, 7);
		cout << "   " << letters[i] << " ";
		for (int o = 0; o < 8; o++) {
			int whiteColor = whiteOnWhite;
			int blackColor = blackOnBlack;

			if (findPiece(o, i) != -1) {
				if (pieces[findPiece(o, i)].white) {
					whiteColor = whiteOnWhite;
					blackColor = whiteOnBlack;
				}
				else{
					whiteColor = blackOnWhite;
					blackColor = blackOnBlack;
				}
			}
			else {
				whiteColor = whiteOnWhite;
				blackColor = blackOnBlack;
			}

			if (o % 2 == 0) {
				if (i % 2 == 1)
					SetConsoleTextAttribute(hConsole, blackColor);
				else
					SetConsoleTextAttribute(hConsole, whiteColor);
			}
			else {
				if (i % 2 == 1)
					SetConsoleTextAttribute(hConsole, whiteColor);
				else
					SetConsoleTextAttribute(hConsole, blackColor);
			}

			if (findPiece(o, i) == -1)
				cout << "       ";
			else {
				cout << "   " << pieces[findPiece(o, i)].symbol << "   ";
			}
		}
		cout << endl;
		SetConsoleTextAttribute(hConsole, 7);
		cout << "     ";
		drawGridLine((i % 2 == 0));
		cout << endl;
	}
	cout << endl << endl;
	SetConsoleTextAttribute(hConsole, 7);
}

void parseMove(bool whiteMove) {
	string moveSquare;
	string moveTo;
	int movePiece;
	bool again = false;
	cout << "What piece do you want to move? Choose by using the coordinates on the left and top: ";
	while (true) {

		cin >> moveSquare;

		int y = letters.find(moveSquare[0]);
		int x = (moveSquare[1] - '0') - 1;

		if (x == -1 || y < 0 || y > 7 || findPiece(x, y) == -1) {
			cout << "Invalid choice. Please choose again. Use the coordinates on the left and top: ";
			continue;
		}

		if (pieces[findPiece(x, y)].white != whiteMove) {
			cout << "Invalid choice. Please choose again. Pick a square with your color piece on it: ";
			continue;
		}

		movePiece = findPiece(x, y);
		break;
	}
	cout << "What square do you want to move to? Choose by using the coordinates on the left and top: ";
	while (true) {

		cin >> moveTo;

		if (moveTo == "BACK") {
			again = true;
			break;
		}

		int y = letters.find(moveTo[0]);
		int x = (moveTo[1] - '0') - 1;

		if (x == -1 || y < 0 || y > 7 || !validMove(pieces[movePiece], x, y)) {
			cout << "Invalid choice. Please choose again: ";
			continue;
		}

		if (findPiece(x, y) != -1) {
			if (findPiece(x, y) < movePiece)
				movePiece--;
			if (pieces[findPiece(x, y)].white && whiteMove) {
				cout << "Invalid choice. Please choose again: ";
				continue;
			}
			if (pieces[findPiece(x, y)].symbol == 'K') {
				if (whiteMove)
					winner = 1;
				else
					winner = -1;
			}
			pieces.erase(pieces.begin() + findPiece(x, y));
		}

		pieces[movePiece].posX = x;
		pieces[movePiece].posY = y;

		if ((pieces[movePiece].posY == 0 || pieces[movePiece].posY == 7) && pieces[movePiece].symbol == 'p')
		{
			pieces[movePiece].symbol = 'Q';
		}

		break;
	}
	if (again)
		parseMove(whiteMove);
}

int main() {
	setupBoard();
	cout << "  _____ _                   " << endl << " / ____| |                  " << endl << "| |    | |__   ___  ___ ___ " << endl << "| |    | '_ \\ / _ \\/ __/ __|" << endl << "| |____| | | |  __/\\__ \\__ \\ " << endl << " \\_____|_| |_|\\___||___/___/" << endl << endl;
	cout << "Follow the instructions on the screen. Type BACK after picking a piece to restart your turn." << endl << "Type any enter to continue." << endl;
	cin.get();
	drawBoard();
	while (winner == 0) {
		SetConsoleTextAttribute(hConsole, 7);
		cout << "White move!" << endl;
		parseMove(true);
		drawBoard();
		if (winner != 0)
			break;
		cout << "Black move!" << endl;
		parseMove(false);
		drawBoard();
		if (winner != 0)
			break;
	}
	if (winner == -1) {
		cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << "                             BLACK WINS!" << endl << endl << endl << endl << endl << endl << endl << endl << endl;
	}
	else {
		cout << endl << endl << endl << endl << endl << endl << "                             WHITE WINS!" << endl << endl << endl << endl << endl << endl;
	}
}