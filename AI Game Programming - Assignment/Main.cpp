#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>
#include "Nodes.h"

std::ifstream fileOpen(std::string _fileName);
void drawMaze(std::vector<int> _grid, int _border);
void runAI(std::vector<int> _grid, int _start, int _finish, int _currentPos, int _border);
int getIndex(std::vector<int> _fCost, int temp);
bool valueExists(std::vector<int> _vec, int _move);

int main()
{
	std::ifstream file;
	std::string fileName;

	std::cout << "Please enter the file name (file_name.txt): ";
	std::cin >> fileName;

	file = fileOpen(fileName);

	char tempInput;
	std::vector<char> mazeNumbers;

	while (file >> tempInput)
		mazeNumbers.push_back(tempInput - 48);

	int sizeX = mazeNumbers[2];
	int sizeY = mazeNumbers[5];
	
	// Represents the size of the maze
	std::cout << "Size X: " << sizeX << " " << "Size Y: " 
		<< sizeY << std::endl;

	std::vector<int> grid;
	// The border is the size of the maze + 2;
	int border = sizeX + 2;

	for (int i = 0; i < border + 1; i++)
		// Adds border to the top
		grid.push_back(5);

	int temp = 0;
	for (int i = 0; i < mazeNumbers.size(); i++)
	{
		if (mazeNumbers[i] == 0)
		{
			// 0 represents an walkable space
			grid.push_back(0);
			temp++;
		}

		if (mazeNumbers[i] == 1)
		{
			// 1 represents an obstacle
			grid.push_back(1);
			temp++;
		}

		if (mazeNumbers[i] == 2)
		{
			// 2 represents the start and AI
			grid.push_back(2);
			temp++;
		}

		if (mazeNumbers[i] == 3)
		{
			// 3 represents the finish
			grid.push_back(3);
			temp++;
		}

		if (temp == sizeX)
		{
			// Adds a border to the end of the row
			grid.push_back(5);
			// Adds a border to the start of the row
			grid.push_back(5);
			temp = 0;
		}
	}

	for (int i = 0; i < sizeX + 1; i++)
	{
		// adds border at bottom
		grid.push_back(5);
	}

	// Gets the start, finish and first avaiable positions
	int start = getStartPosition(grid);
	int finish = getFinishPosition(grid);
	// Sets the AIs position to the start
	int currentPosition = start;
	
	runAI(grid, start, finish, currentPosition, border);

	return 0;
}

std::ifstream fileOpen(std::string fileName)
{
	std::ifstream mazeFile;
	// Opens file
	mazeFile.open(fileName);

	// Checks to see if file doesn't exist/open
	if (!mazeFile.is_open())
	{
		std::cout << "Couldn't Open File" << std::endl;
	}

	return mazeFile;
}

void drawMaze(std::vector<int> _grid, int _border)
{
	int j = 0;
	for (int i = 0; i < _grid.size(); i++)
	{
		if (_grid[i] == 5) // Draws border
		{
			std::cout << _grid[i] << "   ";

			j++;
			if (j == _border)
			{
				std::cout << std::endl;
				std::cout << std::endl;
				j = 0;
			}
		}
		if (_grid[i] != 5) // Will draw the values in the maze
		{
			std::cout << _grid[i] << "   ";
			j++;
			if (j == _border)
			{
				std::cout << std::endl;
				std::cout << std::endl;
				j = 0;
			}
		}
	}
}

void runAI(std::vector<int> _grid, int _start, int _finish, int _currentPos, int _border)
{
	// Creates openList, closedList, gCost,
	// hCost and fCost vector
	std::vector<int> openList;
	std::vector<int> closedList;
	std::vector<int> gCost;
	std::vector<int> hCost;
	std::vector<int> fCost;

	// Draws maze
	drawMaze(_grid, _border);

	// Adds start position to openList
	openList.push_back(_start);

	// Creates values for all available moves
	moves availableMoves;
	int up = availableMoves.up = -_border;
	int down = availableMoves.down = _border;
	int left = availableMoves.left = -1;
	int right = availableMoves.right = 1;

	// Used to erase start position from openList
	int eraseStart = 0;

	while (!openList.empty())
	{
		// Checks to see if currentPos equals finish
		if (_currentPos == _finish)
			break;

		// Removes first the start position from openList
		if (eraseStart != 1)
		{
			// Adds start postion to openList
			closedList.push_back(openList[0]);
			openList.erase(openList.begin());
			eraseStart++;
		}

		// Distance away from start position
		int distance = 1;
		// Temporary variable to check when it has gone through all 8 directions
		// that it can move to
		int tempSpots = 0;
		int j = 8;
		for (int i = 0; i < _grid.size(); i++)
		{
			// Used to see if value already exists in openList
			bool exists = false;
			switch (i)
			{
			case 1:
				exists = valueExists(openList, (_currentPos + up) + left);
				if (_grid[(_currentPos + up) + left] == 0 && exists == false)
				{
					// Adds value to openList and adds gCost to vector
					openList.push_back((_currentPos + up) + left);
					gCost.push_back(14 * distance);
					// Adds one onto temporary variable
					tempSpots++;
				}
				else
				{
					//openList.push_back(100);
					//gCost.push_back(100);
					tempSpots++;
				}
				// Checks to see if position is equal to 3
				// If it is sets previous position equal to 8
				// (8 represents previous moves) and sets current position
				// equal to finish
				if (_grid[(_currentPos + up) + left] == 3 && exists == false)
				{
					openList.push_back((_currentPos + up) + left);
					gCost.push_back(14 * distance);
					tempSpots = j - tempSpots;
				}
				break;
			case 2:
				exists = valueExists(openList, _currentPos + up);
				if (_grid[_currentPos + up] == 0 && exists == false)
				{
					openList.push_back(_currentPos + up);
					gCost.push_back(10 * distance);
					tempSpots++;
				}
				else
				{
					//openList.push_back(100);
					//gCost.push_back(100);
					tempSpots++;
				}
				if (_grid[_currentPos + up] == 3 && exists == false)
				{
					openList.push_back(_currentPos + up);
					gCost.push_back(10 * distance);
					tempSpots = j - tempSpots;
				}
				break;
			case 3:
				exists = valueExists(openList, (_currentPos + up) + right);
				if (_grid[(_currentPos + up) + right] == 0 && exists == false)
				{
					openList.push_back((_currentPos + up) + right);
					gCost.push_back(14 * distance);
					tempSpots++;
				}
				else
				{
					//openList.push_back(100);
					//gCost.push_back(100);
					tempSpots++;
				}
				if (_grid[(_currentPos + up) + right] == 3 && exists == false)
				{
					openList.push_back((_currentPos + up) + right);
					gCost.push_back(14 * distance);
					tempSpots = j - tempSpots;
				}
				break;
			case 4:
				exists = valueExists(openList, _currentPos + left);
				if (_grid[_currentPos + left] == 0 && exists == false)
				{
					openList.push_back(_currentPos + left);
					gCost.push_back(10 * distance);
					tempSpots++;
				}
				else
				{
					//openList.push_back(100);
					//gCost.push_back(100);
					tempSpots++;
				}
				if (_grid[_currentPos + left] == 3 && exists == false)
				{
					openList.push_back(_currentPos + left);
					gCost.push_back(10 * distance);
					tempSpots = j - tempSpots;
				}
				break;
			case 5:
				exists = valueExists(openList, _currentPos + right);
				if (_grid[_currentPos + right] == 0 && exists == false)
				{
					openList.push_back(_currentPos + right);
					gCost.push_back(10 * distance);
					tempSpots++;
				}
				else
				{
					//openList.push_back(100);
					//gCost.push_back(100);
					tempSpots++;
				}
				if (_grid[_currentPos + right] == 3 && exists == false)
				{
					openList.push_back(_currentPos + right);
					gCost.push_back(10 * distance);
					tempSpots = j - tempSpots;
				}
				break;
			case 6:
				exists = valueExists(openList, (_currentPos + down) + left);
				if (_grid[(_currentPos + down) + left] == 0 && exists == false)
				{
					openList.push_back((_currentPos + down) + left);
					gCost.push_back(14 * distance);
					tempSpots++;
				}
				else
				{
					//openList.push_back(100);
					//gCost.push_back(100);
					tempSpots++;
				}
				if (_grid[(_currentPos + down) + left] == 3 && exists == false)
				{
					openList.push_back((_currentPos + down) + left);
					gCost.push_back(14 * distance);
					tempSpots = j - tempSpots;
				}
				break;
			case 7:
				exists = valueExists(openList, _currentPos + down);
				if (_grid[_currentPos + down] == 0 && exists == false)
				{
					openList.push_back(_currentPos + down);
					gCost.push_back(10 * distance);
					tempSpots++;
				}
				else
				{
					//openList.push_back(100);
					//gCost.push_back(100);
					tempSpots++;
				}
				if (_grid[_currentPos + down] == 3 && exists == false)
				{
					openList.push_back(_currentPos + down);
					gCost.push_back(10 * distance);
					tempSpots = j - tempSpots;
				}
				break;
			case 8:
				exists = valueExists(openList, _currentPos + (_currentPos + down) + right);
				if (_grid[(_currentPos + down) + right] == 0 && exists == false)
				{
					openList.push_back((_currentPos + down) + right);
					gCost.push_back(14 * distance);
					tempSpots++;
				}
				else
				{
					//openList.push_back(100);
					//gCost.push_back(100);
					tempSpots++;
				}
				if (_grid[(_currentPos + down) + right] == 3 && exists == false)
				{
					openList.push_back((_currentPos + down) + right);
					gCost.push_back(14 * distance);
					tempSpots = j - tempSpots;
				}
				break;
			default:
				break;
			}

			// Checks to see if temporary variable is equal to 8
			// If so breaks for loop
			if (tempSpots == 8)
				break;
		}

		// Gets the hCost for each move
		for (int i = 0; i < openList.size(); i++)
			hCost.push_back(getHCost(openList, _finish, i));
			
		// Gets the fCost for each move
		for (int i = 0; i < openList.size(); i++)
		{
			int tempFCost = getFCost(gCost, hCost, i);
			bool exists = valueExists(fCost, tempFCost);
			if (exists == false)
				fCost.push_back(tempFCost);
		}
			
		// Creates temporary variable to find lowest cost
		int temp = fCost[0];
		for (int i = 0; i < fCost.size(); i++)
			if (fCost[i] < temp)
				temp = fCost[i];

		// Gets index from the fCost vector
		int index = getIndex(fCost, temp);

		int moveCurrentPos = 0;

		try
		{
			// Uses index value from fCost vector to find the
			// position the AI will need to move to in openList
			moveCurrentPos = openList[index];
		}
		catch (const std::out_of_range& oor)
		{
			index--;
		}

		moveCurrentPos = openList[index];

		// Sets previous position equal to 8
		// (8 represents previous moves) and sets current position
		// equal to 2
		_grid[_currentPos] = 8;
		_currentPos = moveCurrentPos;
		_grid[_currentPos] = 2;

		// Adds new position to closedList
		closedList.push_back(openList[index]);
		// Removes new position from openList
		// and removes the gCost, hCost and fCost
		// for the new position
		openList.erase(openList.begin() + index);

		// Clears debugger and draws maze with new positions
		// and previous positions
		system("cls");
		drawMaze(_grid, _border);
		// Adds 1 to distances and resets tempSpots
		// to 0
		tempSpots = 0;
		distance++;
	}
}

int getIndex(std::vector<int> _vec, int _temp)
{
	int index = 0;

	// Uses iterator to find value in fCost vector
	std::vector<int>::iterator it = std::find(_vec.begin(), _vec.end(), _temp);

	if (it != _vec.end())
	{
		// Sets index to position in the vector
		index = it - _vec.begin();
	}
	else
	{
		// If the value cannot be found it will return -1
		index = -1;
	}

	// Returns index value
	return index;
}

bool valueExists(std::vector<int> _vec, int _move)
{
	// Uses iterator to find see if the value exists in the openList
	std::vector<int>::iterator it = std::find(_vec.begin(), _vec.end(), _move);

	// If the value is found it will return true
	if (it != _vec.end())
		return true;

	// If the value isn't found it will return false
	return false;
}