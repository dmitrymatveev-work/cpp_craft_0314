#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

struct Coordinates
{
	int x;
	int y;

	Coordinates(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};

class CFinder
{
private:
	std::vector<std::string> *area;
	size_t x;
	size_t y;

public:
	CFinder(std::vector<std::string> *area)
	{
		CFinder::area = area;
		y = area->size();
		if(y > 0)
			x = area->at(0).length();
	}
	
	void Conquistar(Coordinates &coord);
};

int main( int argc, char* argv[] )
{
	std::ifstream input_file( BINARY_DIR "/input.txt" );
	std::vector<std::string> lines;

	if(input_file)
	{
		while( !input_file.eof() )
		{
			std::string line;
			std::getline( input_file, line );
			lines.push_back(line);
		}
		input_file.close();

		int count(0);

		CFinder finder(&lines);

		for(auto i = 0; i < lines.size(); i++)
		{
			for(auto j = 0; j < lines.at(i).length(); j++)
			{
				if(lines.at(i).at(j) == 'o')
				{
					count++;
					Coordinates coord(j, i);
					finder.Conquistar(coord);
				}
			}
		}

		std::ofstream output_file( BINARY_DIR "/output.txt" );

		output_file << count;

		output_file.close();
	}
}

void CFinder::Conquistar(Coordinates &coord)
{
	std::queue<Coordinates> coordQueue;
	coordQueue.push(coord);

	while (coordQueue.size() > 0)
	{
		auto tmpCoord = coordQueue.front();
		coordQueue.pop();
		this->area->at(tmpCoord.y).at(tmpCoord.x) = '1';

		if(tmpCoord.x > 0  && this->area->at(tmpCoord.y).at(tmpCoord.x - 1) == 'o')
		{
			Coordinates newCoord(tmpCoord.x - 1, tmpCoord.y);
			coordQueue.push(newCoord);
		}

		if(tmpCoord.x + 1 < this->x && this->area->at(tmpCoord.y).at(tmpCoord.x + 1) == 'o')
		{
				Coordinates newCoord(tmpCoord.x + 1, tmpCoord.y);
				coordQueue.push(newCoord);
		}

		if(tmpCoord.y > 0 && this->area->at(tmpCoord.y - 1).at(tmpCoord.x) == 'o')
		{
				Coordinates newCoord(tmpCoord.x, tmpCoord.y - 1);
				coordQueue.push(newCoord);
		}

		if(tmpCoord.y + 1 < this->y && this->area->at(tmpCoord.y + 1).at(tmpCoord.x) == 'o')
		{
				Coordinates newCoord(tmpCoord.x, tmpCoord.y + 1);
				coordQueue.push(newCoord);
		}
	}
}
