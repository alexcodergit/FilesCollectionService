#pragma once

#include <mutex>
#include <vector>
#include <condition_variable>
#include <filesystem>

namespace fcs
{
	struct DirectoryFileMessage 
	{
		std::filesystem::path m_source_directory;
		std::filesystem::path m_source_file;
		std::filesystem::path m_target_directory;
		std::filesystem::copy_options m_copy_options;

		DirectoryFileMessage(
			std::filesystem::path source_dir = std::filesystem::path(),
			std::filesystem::path source_file = std::filesystem::path(),
			std::filesystem::path target_dir = std::filesystem::path(),
			std::filesystem::copy_options options = std::filesystem::copy_options::recursive) :
			m_source_directory{ source_dir },
			m_source_file{ source_file },
			m_target_directory{ target_dir },
			m_copy_options{ options }
		{}

	};

	template<typename T>
	struct MessageBuffer 
	{
		mutable std::mutex m_mut;
		std::vector<T>m_messages;
		std::condition_variable m_cond;

		void push(std::vector<T>& messages) 
		{
			std::unique_lock<std::mutex> ulock(m_mut);
			m_cond.wait(ulock, [this] {return m_messages.empty(); });
			std::swap(m_messages, messages);
			m_cond.notify_all();
		}
	};
}
