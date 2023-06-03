#include "MazeCell.h"

#include "MazeGenerator.h"

bool MazeCell::HasWall(int Direction) const
{
	return Walls & Direction;
}

int MazeCell::GetNbWalls() const
{
	int nbWalls = 0;
	if (Walls & North) nbWalls++;
	if (Walls & South) nbWalls++;
	if (Walls & East) nbWalls++;
	if (Walls & West) nbWalls++;
	return nbWalls;
}

