﻿#pragma once
#include "MazeCell.h"

enum EDirection
{
	//Direction as bitmask
	North = 0b0001, //x+1
	South = 0b0010, //x-1
	East  = 0b0100, //y+1
	West  =	0b1000  //y-1
};

class MazeGenerator
{
public:
	void SetSize(int _w, int _h);
	void SetSeed(int _seed);
	
	void Generate();
	
	MazeCell* GetCell(int _x, int _y);
	MazeCell* GetEntranceCell(int& _direction);
	MazeCell* GetExitCell(int& _direction);
	
protected:
	int Width = 0;
	int Height = 0;
	int Seed = 0;

	MazeCell* cells = nullptr;
	MazeCell* GetRandomOuterCell(int defaultId);

	int GetDirection(int cellId) const;

	int entrance = 0;
	int exit = 0;

	FRandomStream random;
};
