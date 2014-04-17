#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/shared_array.hpp>
#include <boost/tuple/tuple.hpp>
#include <vector>
#include <string>
#include <algorithm>

using namespace boost::filesystem;

namespace MsgMngr
{
	enum Status{NotReady, Ready, DataLoaded, DataWrote, Error};

	class MessageManager
	{
	public:
		explicit MessageManager(const path& inputPath, const std::vector<boost::uint32_t>& types);
		bool Read();
		bool Write();
		Status GetStatus();
		~MessageManager();

	private:
		path _inputPath;
		path _outputPath;
		std::vector<boost::tuple<boost::uint32_t, boost::uint32_t, boost::uint32_t, std::string>*> _data;
		Status _status;
		std::vector<boost::uint32_t> _types;
	};
}
#endif
