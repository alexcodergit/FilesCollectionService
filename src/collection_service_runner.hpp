#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <iostream>
#include <atomic>
#include "../src/message_buffer.hpp"

namespace fcs
{
	std::string const FILE_NAME_PATTERN = "core\\.([a-zA-Z]+){1}(\\.[0-9a-fA-F]+)+\\.lz4";

	class CollectionServiceRunner 
	{
		std::string m_file_name_pattern;
		std::vector<fcs::DirectoryFileMessage> m_scan_directories;
		std::atomic_bool  m_service_running;
		std::stop_source m_stop_source;
		std::jthread m_j_scan_thread;
		std::jthread m_j_copy_thread;
		fcs::MessageBuffer<fcs::DirectoryFileMessage> m_message_buffer;


	public:
		CollectionServiceRunner() : m_service_running{ false } {}

		void StartService(std::ostream& os = std::cout);

		void StopService(std::ostream& os = std::cout);

		bool AssignFileNamePattern(const std::string fname_pattern, std::ostream& os = std::cout);

		bool AddDirectoryMapping(const std::string source, const std::string target, std::ostream& os = std::cout);

		~CollectionServiceRunner();
	};	
}
