#include <thread>
#include "scan_service.hpp"

void fcs::ScanService::ScanDirectories(std::stop_token stoken)
{
	while (!stoken.stop_requested()) {
		{
			std::vector<fcs::DirectoryFileMessage>messages;
			std::unique_lock<std::mutex>ulock(m_message_buffer.m_mut);
			if (m_message_buffer.m_cond.wait_for(ulock, std::chrono::milliseconds(1),
				[&]() {return m_message_buffer.m_messages.empty(); }))
			{
				for (auto const& dir_file_message : m_scan_directories) {
					for (std::filesystem::directory_entry const& dir_entry :
						std::filesystem::recursive_directory_iterator(dir_file_message.m_source_directory))
					{
						if (!std::filesystem::is_directory(dir_entry))
						{
							if (std::regex_match(dir_entry.path().filename().string(), m_file_name_pattern))
							{
								messages.push_back(
									{
									 dir_file_message.m_source_directory,
									 dir_entry.path(),
									 dir_file_message.m_target_directory,
									 dir_file_message.m_copy_options
									}
								);
							}
						}
					}
				}
			}
			if (!messages.empty()) {
				std::swap(m_message_buffer.m_messages, messages);
				m_message_buffer.m_cond.notify_all();
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
