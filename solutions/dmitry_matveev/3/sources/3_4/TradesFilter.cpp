
#include "MessageManager.h"
#include <boost/filesystem.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/regex.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
#include <string>
//#include <iostream>

using namespace boost::filesystem;

bool IsValidFilename(const path *input);

int main()
{
	boost::lockfree::queue<const path*> inputFiles(32);

	if(inputFiles.is_lock_free())
	{
		auto it = directory_iterator(path(BINARY_DIR));
		while(it != directory_iterator())
		{
			if(IsValidFilename(&(it->path())))
				inputFiles.push(new path(it->path()));
			it++;
		}
	}

	std::vector<boost::uint32_t> types;
	types.push_back(1);
	types.push_back(2);
	types.push_back(3);
	types.push_back(4);

	auto coresCount = boost::thread::hardware_concurrency();

	boost::thread_group threadGroup;
	for (auto i = 0u; i < coresCount; i++)
	{
		threadGroup.create_thread(boost::bind<void>([&types](boost::lockfree::queue<const path*>* inputFiles)
		{
			auto threadId = boost::this_thread::get_id();

			path* tmpFileItem = NULL;
			while(inputFiles->pop(tmpFileItem))
			{
				boost::scoped_ptr<path> fileItem(tmpFileItem);
				MsgMngr::MessageManager messageManager(*(fileItem.get()), types);
				if(messageManager.GetStatus() == MsgMngr::Status::Ready)
				{
					if(messageManager.Read() && messageManager.GetStatus() == MsgMngr::Status::DataLoaded)
					{
						if(messageManager.Write() && messageManager.GetStatus() != MsgMngr::Status::DataWrote)
							return;
					}
					else
						return;
				}
				else
					return;

				//std::cout << std::endl << threadId << " " << fileItem->filename();
				//boost::this_thread::sleep(boost::posix_time::milliseconds(100));
			}

		}, &inputFiles));
	}
	threadGroup.join_all();

	//system("pause");
	return 0;
}

bool IsValidFilename(const path *input)
{
	boost::regex e("input_\\d{3}.txt");

	boost::match_results<std::string::const_iterator> what;
	auto str = input->filename().generic_string();
	if(0 != boost::regex_match(input->filename().generic_string(), what, e, boost::match_default | boost::match_partial))
		if(what[0].matched)
			return true;

	return false;
}