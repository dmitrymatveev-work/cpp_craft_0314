
#include "trades_filter.h"

using namespace boost::filesystem;

void write(ofstream &output, unsigned int type, unsigned int time, unsigned int length, char* str)
{
	output.write((char*)&type, sizeof(type));
	output.write((char*)&time, sizeof(time));
	output.write((char*)&length, sizeof(length));
	output.write(str, length);
}

int main()
{

	path inPath(BINARY_DIR "/input.txt");
	path outPath(BINARY_DIR "/output.txt");
	if(exists(inPath) && is_regular_file(inPath))
	{
		ifstream input(inPath, std::ios_base::binary);
		ofstream output(outPath, std::ios_base::binary);

		if(input)
		{
			auto fSize = file_size(inPath);

			unsigned int type(0);
			unsigned int time(0);
			unsigned int tMax(0);
			unsigned int length(0);

			while (!input.eof())
			{
				if(fSize - input.tellg() < 3*sizeof(int))
					break;

				input.read((char*)&type, sizeof(type));
				input.read((char*)&time, sizeof(time));
				input.read((char*)&length, sizeof(length));
				if(tMax > 1)
				{
					if(time > tMax - 2 && (type == 1 || type == 2 || type == 3 || type == 4))
					{
						if(time > tMax)
							tMax = time;

						boost::shared_array<char> str(new char[length]);
						input.read(str.get(), length);

						write(output, type, time, length, str.get());
					}
					else
					{
						input.seekg((int)input.tellg() + length);
					}
				}
				else
				{
					if(time > tMax)
						tMax = time;

					if(type == 1 || type == 2 || type == 3 || type == 4)
					{
						boost::shared_array<char> str(new char[length]);
						input.read(str.get(), length);

						write(output, type, time, length, str.get());
					}
					else
					{
						input.seekg((int)input.tellg() + length);
					}
				}
			}
		}
	}
}

