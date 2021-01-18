#include "Nodes.h"
#include <iostream>

int getStartPosition(std::vector<int> _maze)
{
	int startPostion = 0;

	for (int i = 0; i < _maze.size(); i++)
	{
		if (_maze[i] == 2)
		{
			std::cout << "Start Position = " << i << std::endl;
			startPostion = i;
			break;
		}
	}

	return startPostion;
}

int getFinishPosition(std::vector<int> _maze)
{
	int FinishPostion = 0;

	for (int i = 0; i < _maze.size(); i++)
	{
		if (_maze[i] == 3)
		{
			std::cout << "Finish Position = " << i << std::endl;
			FinishPostion = i;
			break;
		}
	}

	return FinishPostion;
}

int getHCost(std::vector<int> _openList, int _finish, int _pos)
{
	/*if (_openList[_pos] == 100)
		return 100;*/

	return _finish - _openList[_pos];
}

int getFCost(std::vector<int> _gCost, std::vector<int> _hCost, int _pos)
{
	/*if (_gCost[_pos] == 100 && _hCost[_pos] == 100)
		return 100;*/

	return _gCost[_pos] + _hCost[_pos];
}
