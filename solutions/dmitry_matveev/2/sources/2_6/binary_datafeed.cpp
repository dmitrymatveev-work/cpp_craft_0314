
#include "binary_datafeed.h"

using namespace boost::filesystem;

int main()
{
	path inPath(BINARY_DIR "/2.6_example.in");
	path outPath(BINARY_DIR "/2.6_example.out");
	if(exists(inPath) && is_regular_file(inPath))
	{
		ifstream input(inPath, std::ios_base::binary);
		ofstream output(outPath, std::ios_base::binary);

		if(input.is_open() && output.is_open())
		{
			auto fSize = file_size(inPath);

			while (!input.eof() && (fSize - input.tellg() >= 8 + 8 + 7*sizeof(double) + sizeof(unsigned int)))
			{
				boost::shared_array<char> stockName(new char[9]);

				input.read(stockName.get(), 8);

				unsigned int dateTime(0);

				size_t yearStringLength = 4;
				boost::shared_array<char> year(new char[yearStringLength]);
				input.read(year.get(), yearStringLength);
				dateTime = 372 * std::stoi(year.get(), &yearStringLength);

				size_t monthStringLength = 2;
				boost::shared_array<char> month(new char[monthStringLength]);
				input.read(month.get(), monthStringLength);
				dateTime += 31 * std::stoi(month.get(), &monthStringLength);

				size_t dayStringLength = 2;
				boost::shared_array<char> day(new char[dayStringLength]);
				input.read(day.get(), dayStringLength);
				dateTime += std::stoi(day.get(), &dayStringLength);

				input.seekg((int)input.tellg() + sizeof(double));

				double price(0.0);
				input.read((char*)&price, sizeof(price));

				unsigned int volume(0);
				input.read((char*)&volume, sizeof(volume));

				input.seekg((int)input.tellg() + 2 * sizeof(double));

				double f2(0.0);
				input.read((char*)&f2, sizeof(f2));

				input.seekg((int)input.tellg() + 2 * sizeof(double));

				output.write(stockName.get(), 9);
				output.write((char*)&dateTime, sizeof(dateTime));
				output.write((char*)&price, sizeof(price));
				output.write((char*)&volume, sizeof(volume));
				output.write((char*)&f2, sizeof(f2));
			}
		}
	}
}

