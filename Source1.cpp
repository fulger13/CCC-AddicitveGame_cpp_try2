#include <iostream>
#include <fstream>
#include <cstring>
#include <utility>
#include <vector>
#include <queue>

#define BOARD_SIZE 1000
#define FILE_LENGTH 100

#define BOARD_FULLSIZE 1000000
#define COLORS_NUMBER 200000

//#define BOARD_FULLSIZE 1000000
//#define COLORS_NUMBER 20000


using namespace std;

class Board {
public:
	int rows, cols, numberOfDots;
	int* colors = new int[COLORS_NUMBER];
	int* boardComplete = new int[BOARD_FULLSIZE];
	int numberOfTests = 0;



	char inputFileName[FILE_LENGTH], outputFileName[FILE_LENGTH];
	ifstream inputFile;
	ofstream outputFile;

	Board(char fileName[FILE_LENGTH])
	{

		///create file names

		//give name to the files
		strcpy_s(inputFileName, fileName);
		strcpy_s(outputFileName, fileName);

		//add extensions
		strcat_s(inputFileName, ".in");
		strcat_s(outputFileName, ".out");

		//open files
		inputFile.open(inputFileName);
		outputFile.open(outputFileName);

		///initialize vectors
		initializeVector(this->colors, COLORS_NUMBER);
		initializeVector(this->boardComplete, BOARD_FULLSIZE);

		inputFile >> this->numberOfTests;
	}
	~Board()
	{
		//cout << "You have been called ... destructor\n";
		if (inputFile.is_open())
			inputFile.close();

		if (outputFile.is_open())
			outputFile.close();

		delete[] this->colors;
		delete[] this->boardComplete;
	}



	/*
		Below are useful functions for operations like read and write
	*/

	bool read()
	{
		if (this->numberOfTests == 0)
			return false;
		this->numberOfTests--;

		initializeVector(this->colors, COLORS_NUMBER);
		initializeVector(this->boardComplete, BOARD_FULLSIZE);

		if (!inputFile.is_open())
			inputFile.open(inputFileName);

		inputFile >> rows >> cols >> numberOfDots;

		int currentPosition, currentColor;

		for (int i = 0; i < numberOfDots; i++)
		{
			inputFile >> currentPosition >> currentColor;
			if (this->colors[currentColor] == 0)
				this->colors[currentColor] = currentPosition;
			else
				this->colors[currentColor + COLORS_NUMBER / 2] = 0; //in the second half we keep the connectability of the points
			this->boardComplete[currentPosition] = currentColor;
		}

		int pathsNumber, startingPosition, pathLength;
		int pathCurrentPosition;
		inputFile >> pathsNumber;

		for (int i = 0; i < pathsNumber; i++)
		{
			char direction; // positibilies are: N,E,S,W
			inputFile >> currentColor >> startingPosition >> pathLength;
			pathCurrentPosition = startingPosition;
			this->colors[currentColor + COLORS_NUMBER / 2] = 1;

			for (int j = 0; j < pathLength; j++)
			{
				inputFile >> direction;
				pathCurrentPosition = updatePosition(pathCurrentPosition, direction);
				this->boardComplete[pathCurrentPosition] = currentColor;
			}

		}

		//inputFile.close();

		return true; //finished the reading
	}

	void write(int value)
	{
		outputFile << value << " ";

	}
	void write(pair<int, int> pairValue)
	{
		outputFile << pairValue.first << " " << pairValue.second << " ";
	}
	void write(int first, int second)
	{
		outputFile << first << " " << second << " ";
	}

	void draw()
	{
		//cout << "boardComplete size = " << sizeof(this->boardComplete) / sizeof(int) << "\n";
		if (!this->outputFile.is_open())
		{
			this->outputFile.open(this->outputFileName);
		}
		for (int i = 1; i <= (this->rows * this->cols); i++)
		{
			if (i%cols == 1 && i != 1)
			{
				this->outputFile << "\n";
				//cout << "\n";
			}

			this->outputFile << this->boardComplete[i] << " ";
			//cout << this->boardComplete[i] << " ";
		}
	}


	/*
		Next are initialization functions
	*/

	void initializeVector(int vector[], int size)
	{
		for (int i = 0; i < size; i++)
			vector[i] = 0;
	}

	void initializeBoolVector(bool vector[], int size)
	{
		for (int i = 0; i < size; i++)
			vector[i] = 0;
	}

	void initializePairVector(pair<int, int> pairVector[], int size)
	{
		pair<int, int> nullPair = make_pair(0, 0);
		for (int i = 0; i < size; i++)
		{
			pairVector[i] = nullPair;
		}
	}

	void resetBoard()
	{
		for (int i = 1; i <= (this->rows * this->cols); i++)
		{
			if (boardComplete[i] == -1)
				boardComplete[i] = 0;
		}
	}


	/*
		Below are different functions used for computing varius tasks on the dots
	*/
	void checkConnectability()
	{
		for (int i = 1; i <= numberOfDots / 2; i++)
		{
			if (this->colors[i + COLORS_NUMBER / 2] != 0)
			{
				this->outputFile << this->colors[i + COLORS_NUMBER / 2] << " ";
			}
			else
			{
				this->outputFile << connectable(this->colors[i], i) << " ";
				//cout << connectable(this->colors[i], i) << " ";
			}
		}
	}

	pair<int, int> getCoordinates(int position)
	{
		int row, col;
		row = position / this->cols + 1;
		col = position % this->cols;

		if (position % this->cols == 0)
		{
			row--;
			col = this->cols;
		}

		pair<int, int> result = make_pair(row, col);

		return result;
	}

	int getManhattenDistance(int position1, int position2)
	{
		position1 = position1 * (position1 >= 0) + position1 * -1 * (position1 < 0);
		position2 = position2 * (position2 >= 0) + position2 * -1 * (position2 < 0);

		pair<int, int> coord1, coord2;
		coord1 = getCoordinates(position1);
		coord2 = getCoordinates(position2);

		int distRow = coord1.first - coord2.first;
		int distColl = coord1.second - coord2.second;

		distRow = distRow * (distRow >= 0) + distRow * -1 * (distRow < 0);
		distColl = distColl * (distColl >= 0) + distColl * -1 * (distColl < 0);

		int result = distRow + distColl;
		return result;
	}

	int updatePosition(int currentPosition, char direction)
	{
		if (direction == 'N')
		{
			return (currentPosition - this->cols); //one row up
		}
		else if (direction == 'E')
		{
			if (currentPosition % this->cols != 0)
				return (currentPosition + 1); //one column right
			else
				return -1;
		}
		else if (direction == 'S')
		{
			return (currentPosition + this->cols); //one row down
		}
		else
		{
			if (currentPosition % this->cols != 1)
				return (currentPosition - 1); //one column left
			else
				return -1;
		}
	}

	int connectable(int pointPosition, int pointColor)//1- connected | 2- connectable | 3- not connected
	{
		//cout << "entered connectable\n";
		//cout << "boardComplete size = " << sizeof(this->boardComplete) / sizeof(int) << "\n";

		queue<int> visited;
		//int directions[4] = {-1, this->cols * (-1), 1, this->cols};
		char directions[4] = { 'W','N','E','S' };
		visited.push(pointPosition);

		int currentPosition = pointPosition;
		int nextPosition;
		bool doneChecking = false;
		bool foundPath = false;
		while (!doneChecking && !visited.empty())
		{
			//cout << "inside the while: " << visited.front() << "\n";
			currentPosition = visited.front();//take the next element to be checked
			visited.pop(); //pop the element which is in use for the current iteration

			//checking surrounding directions
			for (int directionsIndex = 0; directionsIndex < 4; directionsIndex++)
			{
				//nextPosition = currentPosition + directions[directionsIndex];
				nextPosition = updatePosition(currentPosition, directions[directionsIndex]);
				if (nextPosition > 0 && nextPosition <= (this->rows * this->cols))
				{
					//checking for end condition
					if (this->boardComplete[nextPosition] == pointColor && nextPosition != pointPosition)
					{
						//cout << nextPosition << " - " << boardComplete[nextPosition] << "\n";
						doneChecking = true;
						foundPath = true;
					}

					//or saving the position for further checking
					else if (this->boardComplete[nextPosition] == 0)
					{
						visited.push(nextPosition);
						//toBeResetted.push(nextPosition);
						this->boardComplete[nextPosition] = -1;
					}
				}
			}
		}

		resetBoard();

		//returning the result
		if (foundPath == true)
			return 2;
		return 3;
	}
};





int main()
{
	char fileName[FILE_LENGTH];

	//strcpy_s(fileName, "level5/level5-0");
	//strcpy_s(fileName, "level5/level5-1");
	//strcpy_s(fileName, "level5/level5-2");
	//strcpy_s(fileName, "level5/level5-3");
	strcpy_s(fileName, "level5/level5-4");


	Board board{ fileName };
	int i = 1;
	while (board.read() == true)
	{
		cout << i++ << " ";
		//board.draw();
		board.checkConnectability();
	}


	//int a;cin >> a;

	return 0;
}