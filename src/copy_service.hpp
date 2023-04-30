#pragma once

#include <filesystem>
#include <vector>
#include "message_buffer.hpp"

namespace fcs
{
	class CopyService 
	{
		fcs::MessageBuffer<fcs::DirectoryFileMessage>& m_message_buffer;

		bool PathExists(const std::string& fpath) const;

	public:
		CopyService(
			fcs::MessageBuffer<fcs::DirectoryFileMessage>& message_queue) :
			m_message_buffer(message_queue)
		{}

		/** runs service copying files from 
         * source to target directories specified in message buffer
         * @param stoken stop source to stop the thread
         */
		void RunCopyService(std::stop_token stoken);
	};
}
