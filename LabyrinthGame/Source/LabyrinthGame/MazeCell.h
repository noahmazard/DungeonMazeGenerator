#pragma once

class MazeCell
{
public:

	int id = -1;
	int x = -1;
	int y = -1;
	int Walls = 0b1111;
	bool Visited = false;
	bool HasWall(int Direction) const;
};
