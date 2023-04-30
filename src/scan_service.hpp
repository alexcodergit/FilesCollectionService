#pragma once

#include <filesystem>
#include <regex>
#include <vector>
#include "message_buffer.hpp"

namespace fcs
{
	class ScanService 
	{
		const std::regex m_file_name_pattern;
		fcs::MessageBuffer<fcs::DirectoryFileMessage>& m_message_buffer;
		std::vector<fcs::DirectoryFileMessage> m_scan_directories;

	public:

		ScanService(
			const std::string& file_name_pattern,
			fcs::MessageBuffer<fcs::DirectoryFileMessage>& message_buffer,
			const std::vector<fcs::DirectoryFileMessage>& scan_directories) :

			m_file_name_pattern{ file_name_pattern },
			m_message_buffer{ message_buffer },
			m_scan_directories{ scan_directories }
		{}

		/** runs service scaning files in
        * source directory
		* found files put into message buffer and processed 
		* by another thread running copying service
        * @param stoken stop source to stop the thread
        */
		void ScanDirectories(std::stop_token stoken);
	};
}