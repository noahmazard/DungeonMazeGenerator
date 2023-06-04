#include "MazeGenerator.h"
#include <stack>

void MazeGenerator::Generate()
{
	//Set random seed
	random.Initialize(Seed);
	
	//Init cells
	if (cells)
	{
		delete[] cells;
	}
	cells = new MazeCell[Width * Height];

	//Init ChestCellsId
	if (ChestCellsId.Num() > 0)
	{
		ChestCellsId.Empty();
	}

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

		//init values for current cell
		const int ID = y * Width + x;
		cells[ID].Visited = true;
		cells[ID].id = ID;
		cells[ID].x = x;
		cells[ID].y = y;

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
			cells[ID].Walls &= ~direction;

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
			
			//Add to chest cells
			if (cells[ID].GetNbWalls() == 3)
			{
				ChestCellsId.Add(ID);
			}
		}
	}

	//Shuffle chest cells
	Shuffle(&ChestCellsId);

	//Set entrance and exit
	MazeCell* c = GetRandomOuterCell(0);
	if (!c) return;
	entrance = c->id;
	
	do {
		c = GetRandomOuterCell(Width * Height - 1);
		if (!c) return;
		exit = c->id;
	} while (exit == entrance);

}

const MazeCell* MazeGenerator::GetCell(int _x, int _y) const
{
	if (!cells) return nullptr;
	if (_x < 0 || _x >= Width || _y < 0 || _y >= Height)
	{
		return nullptr;
	}
	return &cells[_y * Width + _x];
}

const MazeCell* MazeGenerator::GetEntranceCell(int& _direction) const
{
	_direction = GetDirection(entrance);
	return &cells[entrance];
}

const MazeCell* MazeGenerator::GetExitCell(int& _direction) const
{
	_direction = GetDirection(exit);
	return &cells[exit];
}

const MazeCell* MazeGenerator::GetChestCell(int i, int& _direction) const
{
	if (i < 0 || i >= ChestCellsId.Num()) return nullptr;
	const MazeCell* c = &cells[ChestCellsId[i]];
	if (c) _direction = ~c->Walls & 0b1111;
	return c;
}

MazeCell* MazeGenerator::GetRandomOuterCell(int defaultId = 0) const
{
	switch (1 << random.RandRange(0, 3))
	{
	case North:
		return &cells[Height * (Width - 1) + random.RandRange(0, Height - 1)];
	case South:
		return &cells[random.RandRange(0, Height - 1)];
	case East:
		return &cells[random.RandRange(0, Width - 1) * Height + Height - 1];
	case West:
		return &cells[random.RandRange(0, Width - 1) * Height];
	default:
		return &cells[defaultId];
	}
}

//Get the direction of the outer wall cell
int MazeGenerator::GetDirection(int cellId) const
{
	int direction = 0;
	direction |= cells[cellId].x == 0 ? South : 0;
	direction |= cells[cellId].x == Width - 1 ? North : 0;
	direction |= cells[cellId].y == 0 ? West : 0;
	direction |= cells[cellId].y == Height - 1 ? East : 0;
	return direction;
}

void MazeGenerator::Shuffle(TArray<int>* _array) const
{
	for (int i = 0; i < _array->Num() ; i++)
	{
		const int j = random.RandRange(0, _array->Num() - 1);
		_array->Swap(i, j);
	}
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
