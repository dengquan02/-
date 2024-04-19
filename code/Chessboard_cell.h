#pragma once
#include <iostream>

using namespace std;

class Chessboard_cell
{
public:
	Chessboard_cell()
	{
		this->r = -1;
		this->c = -1;
		this->f = NULL;
	}
	Chessboard_cell(int r, int c)
	{
		this->r = r;
		this->c = c;
		this->f = NULL;
	}
	Chessboard_cell(int r, int c, Chessboard_cell* f)
	{
		this->r = r;
		this->c = c;
		this->f = f;
	}
	bool operator==(Chessboard_cell cell)
	{
		return (this->r == cell.r && this->c == cell.c && this->f == cell.f);
	}
	bool operator!=(Chessboard_cell cell)
	{
		return (this->r != cell.r || this->c != cell.c || this->f != cell.f);
	}

	int r, c;
	Chessboard_cell* f;
};
