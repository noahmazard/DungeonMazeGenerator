#include "MazeCell.h"

bool MazeCell::HasWall(int Direction) const
{
	return Walls & Direction;
}

