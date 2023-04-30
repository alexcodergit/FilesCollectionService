#include "copy_service.hpp"


bool fcs::CopyService::PathExists(const std::string& fpath) const
{
	const std::filesystem::path& path(fpath);
	std::filesystem::file_status fstatus = std::filesystem::file_status{};
	if (std::filesystem::status_known(fstatus) ? std::filesystem::exists(fstatus) : 
		std::filesystem::exists(path)) {
		return true;
	}
	return false;
}


void fcs::CopyService::RunCopyService(std::stop_token stoken)
{
	while (!stoken.stop_requested()) {
		{
			std::unique_lock<std::mutex>ulock(m_message_buffer.m_mut);
			if (m_message_buffer.m_cond.wait_for(ulock, std::chrono::microseconds(1),
				[&]() {return !m_message_buffer.m_messages.empty(); }))
			{
				for (auto const& dir_file_message : m_message_buffer.m_messages)
				{
					auto const relatve_src_path = std::filesystem::relative(dir_file_message.m_source_file.parent_path(), dir_file_message.m_source_directory);
					auto const target_path = dir_file_message.m_target_directory / relatve_src_path;
					std::filesystem::create_directory(target_path);
					std::filesystem::copy(dir_file_message.m_source_file, target_path, dir_file_message.m_copy_options);
					std::filesystem::remove(dir_file_message.m_source_file);
				}
				m_message_buffer.m_messages.clear();
				m_message_buffer.m_cond.notify_all();
			}
		}
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}