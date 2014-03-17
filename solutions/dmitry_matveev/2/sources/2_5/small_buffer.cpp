
#include "small_buffer.h"

using namespace boost::filesystem;

int main()
{

	path inPath(BINARY_DIR "/input.txt");
	path outPath(BINARY_DIR "/output.txt");
	if(exists(inPath) && is_regular_file(inPath))
	{
		ifstream input(inPath, std::ios_base::binary);
		ofstream output(outPath, std::ios_base::binary);

		const size_t maxSize(2048);
		int overalTime(0);
		boost::container::map<int, size_t> currentSize;
		boost::container::map<int, int> overalCount;

		if(input)
		{
			auto fSize = file_size(inPath);

			unsigned int type(0);
			unsigned int time(0);
			unsigned int length(0);

			while (!input.eof())
			{
				if(fSize - input.tellg() < 3*sizeof(int))
					break;

				input.read((char*)&type, sizeof(type));
				input.read((char*)&time, sizeof(time));
				input.read((char*)&length, sizeof(length));

				if(currentSize.find(type) == currentSize.end())
					currentSize[type] = 0;
				if(overalCount.find(type) == overalCount.end())
					overalCount[type] = 0;

				if(time - overalTime >= 1)
				{
					overalTime = time;
					for (auto it = currentSize.begin(); it != currentSize.end(); it++)
					{
						it->second = 0;
					}
				}

				if(currentSize[type] + 3*sizeof(int) + length < maxSize)
				{
					currentSize[type] += 3*sizeof(int) + length;
					overalCount[type]++;
				}

				input.seekg((int)input.tellg() + length);
			}

			if(overalTime > 0)
			{
				int mean(0);
				for (auto it = overalCount.begin(); it != overalCount.end();it++)
				{
					output.write((char*)&(it->first), sizeof(it->first));
					mean = it->second / overalTime;
					output.write((char*)&(mean), sizeof(mean));
				}
			}
		}
	}
}

