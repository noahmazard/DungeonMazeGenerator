#pragma once

class MazeCell
{
public:
	int Walls = 0b1111;
	bool Visited = false;

	bool HasWall(int Direction) const;
};
