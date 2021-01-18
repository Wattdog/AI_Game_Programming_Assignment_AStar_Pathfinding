#pragma once
#ifndef _NODES_H_
#define _NODES_H_
#include <vector>

struct moves
{
	int up, down;
	int left, right;
};

int getStartPosition(std::vector<int> _maze);
int getFinishPosition(std::vector<int> _maze);
int getHCost(std::vector<int> _openList, int _finish, int _pos);
int getFCost(std::vector<int> _gCost, std::vector<int> _hCost, int _pos);

#endif // !_NODES_H_
