
#include "MessageManager.h"

namespace MsgMngr
{

	MessageManager::MessageManager(const path& inputPath, const std::vector<boost::uint32_t>& types) : _inputPath(inputPath), _types(types)
	{
		if(this->_inputPath.has_filename())
		{
			const auto fileName = this->_inputPath.filename().string();
			const std::string digits("0123456789");
			auto position = fileName.find_first_of(digits);
			if(position != std::string::npos)
			{
				auto ending = fileName.substr(position);
				this->_outputPath = this->_inputPath.parent_path();
				this->_outputPath /= std::string("output_") + ending;
				this->_status = Status::Ready;
			}
			else
				this->_status = Status::NotReady;
		}
	}

	Status MessageManager::GetStatus()
	{
		return this->_status;
	}

	bool MessageManager::Read()
	{
		if(this->_types.size() < 1)
		{
			this->_status = Status::Error;
			return false;
		}

		if(exists(this->_inputPath) && is_regular_file(this->_inputPath))
		{
			ifstream input(this->_inputPath, std::ios_base::binary);

			if(input.is_open())
			{
				auto fileSize = file_size(_inputPath);
				boost::uint32_t tMax(0);

				while (!input.eof())
				{
					if(fileSize - input.tellg() < 3*sizeof(boost::uint32_t))
						break;

					boost::uint32_t type(0);
					input.read(reinterpret_cast<char*>(&type), sizeof(type));
					boost::uint32_t time(0);
					input.read(reinterpret_cast<char*>(&time), sizeof(time));
					boost::uint32_t length(0);
					input.read(reinterpret_cast<char*>(&length), sizeof(length));

					if(tMax > 1)
					{
						if(time > tMax - 2 && (std::find(this->_types.begin(), this->_types.end(), type) != this->_types.end()))
						{
							if(time > tMax)
								tMax = time;

							boost::shared_array<char> str(new char[length]);
							input.read(str.get(), length);

							auto newTuple = new boost::tuple<boost::uint32_t, boost::uint32_t, boost::uint32_t, std::string>(type, time, length, std::string(str.get()));
							this->_data.push_back(newTuple);
						}
						else
						{
							input.seekg((boost::uint32_t)input.tellg() + length);
						}
					}
					else
					{
						if(time > tMax)
							tMax = time;

						if(std::find(this->_types.begin(), this->_types.end(), type) != this->_types.end())
						{
							boost::shared_array<char> str(new char[length]);
							input.read(str.get(), length);

							auto newTuple = new boost::tuple<boost::uint32_t, boost::uint32_t, boost::uint32_t, std::string>(type, time, length, std::string(str.get()));
							this->_data.push_back(newTuple);
						}
						else
						{
							input.seekg((boost::uint32_t)input.tellg() + length);
						}
					}
				}
				input.close();
				this->_status = Status::DataLoaded;
				return true;
			}
		}
		return false;
	}

	bool MessageManager::Write()
	{
		if(this->_status == Status::DataLoaded && !exists(this->_outputPath) && !is_regular_file(this->_outputPath))
		{
			ofstream output(this->_outputPath, std::ios_base::binary);

			if(output.is_open())
			{
				for (auto it = this->_data.begin(); it != this->_data.end(); it++)
				{
					output.write(reinterpret_cast<char*>(&(*it)->get<0>()), sizeof((*it)->get<0>()));
					output.write(reinterpret_cast<char*>(&(*it)->get<1>()), sizeof((*it)->get<1>()));
					output.write(reinterpret_cast<char*>(&(*it)->get<2>()), sizeof((*it)->get<2>()));
					output.write((*it)->get<3>().c_str(), (*it)->get<2>());
				}
				output.flush();
				output.close();
				this->_status = Status::DataWrote;
				return true;
			}
		}
		return false;
	}

	MessageManager::~MessageManager()
	{
		for (auto it = this->_data.begin(); it != this->_data.end(); it++)
		{
			delete (*it);
		}
	}
}
