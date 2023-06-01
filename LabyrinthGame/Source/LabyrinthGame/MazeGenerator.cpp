#include "MazeGenerator.h"
#include <stack>

void MazeGenerator::Generate()
{
	//Set random seed
	FRandomStream random(Seed);

	//Init cells
	if (cells)
	{
		delete[] cells;
	}
	cells = new MazeCell[Width * Height];

	//Init stack
	std::stack<std::pair<int, int>> stack;

	// === Depth-first search ===

	//Set start cell as 0 0
	stack.push(std::make_pair(0, 0));
	
	while (!stack.empty())
	{
		//Get current cell
		auto pos = stack.top();
		
		int x = pos.first;
		int y = pos.second;

		cells[y * Width + x].Visited = true;

		//Get unvisited neighbours
		std::vector<EDirection> unvisitedNeighbours;
		if (x > 0 && !cells[y * Width + x - 1].Visited)
		{
			unvisitedNeighbours.push_back(South);
		}
		if (x < Width - 1 && !cells[y * Width + x + 1].Visited)
		{
			unvisitedNeighbours.push_back(North);
		}
		if (y > 0 && !cells[(y - 1) * Width + x].Visited)
		{
			unvisitedNeighbours.push_back(West);
		}
		if (y < Height - 1 && !cells[(y + 1) * Width + x].Visited)
		{
			unvisitedNeighbours.push_back(East);
		}
		
		//If there are unvisited neighbours
		if (!unvisitedNeighbours.empty())
		{
			//Choose random neighbour
			EDirection direction = unvisitedNeighbours[random.RandRange(0, unvisitedNeighbours.size() - 1)];

			//Remove current cell's wall
			cells[y * Width + x].Walls &= ~direction;

			//Push selected neighbour to stack and remove his wall
			switch (direction)
			{
			case North:
				stack.push(std::make_pair(x + 1, y));
				cells[y * Width + x + 1].Walls &= ~South;
				break;
			case South:
				stack.push(std::make_pair(x - 1, y));
				cells[y * Width + x - 1].Walls &= ~North;
				break;
			case East:
				stack.push(std::make_pair(x, y + 1));
				cells[(y + 1) * Width + x].Walls &= ~West;
				break;
			case West:
				stack.push(std::make_pair(x , y - 1));
				cells[(y - 1) * Width + x].Walls &= ~East;
				break;
			}
		}
		else
		{
			//Backtrack
			stack.pop();
		}
	}
}

MazeCell* MazeGenerator::GetCell(int _x, int _y)
{
	if (!cells) return nullptr;
	if (_x < 0 || _x >= Width || _y < 0 || _y >= Height)
	{
		return nullptr;
	}
	return &cells[_y * Width + _x];
}

void MazeGenerator::SetSize(int _w, int _h)
{
	Width = _w;
	Height = _h;
}

void MazeGenerator::SetSeed(int _seed)
{
	Seed = _seed;
}
