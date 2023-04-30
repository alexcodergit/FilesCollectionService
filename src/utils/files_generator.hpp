#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>
#include <atomic>

namespace fcs
{
	namespace utils
	{
		struct DataFile
		{
			const char m_content;
			const std::string m_directory;
			const std::string m_file_name;
			unsigned m_char_count;


			DataFile(std::string directory, std::string file_name, unsigned char_count = 0) :
				m_content{ 'a' },
				m_directory{ directory },
				m_file_name{ file_name },
				m_char_count{ char_count }
			{}

			void WriteToDisk()
			{
				std::filesystem::create_directory(std::filesystem::path(m_directory));
				std::filesystem::path full_name = m_directory / std::filesystem::path(m_file_name);
				std::ofstream ofs(full_name.c_str());
				if (ofs.is_open()) {
					for (unsigned i = 0; i < m_char_count; i++) {
						ofs << m_content;
					}
					ofs.close();
				}
				else {
					const std::string message = "Can't create file in " + std::string((const char*)full_name.c_str());
					throw std::exception(message.c_str());
				}
			}

			void ReadFromDisk()
			{
				std::filesystem::path full_name = m_directory / std::filesystem::path(m_file_name);
				std::ifstream ifs(full_name.c_str());
				if (ifs.is_open()) {
					m_char_count = 0;
					char ch;
					while (ifs >> ch) {
						m_char_count++;
					}
					ifs.close();
				}
				else {
					const std::string message = "Can't open file " + std::string((const char*)full_name.c_str());
					throw std::exception(message.c_str());
				}
			}

			bool operator==(const DataFile& df) const
			{
				return (this->m_file_name == df.m_file_name) &&
					(this->m_char_count == df.m_char_count);
			}

			bool operator != (const DataFile& df) const
			{
				return !(*this == df);
			}

			std::filesystem::path GetFullFileName() const
			{
				return std::filesystem::path(m_directory) / std::filesystem::path(m_file_name);
			}
		};


		class FilesGenerator {
			std::vector<std::pair<std::string, std::string>>m_source_target_dirs;
			unsigned m_num_of_files_per_dir;
			unsigned m_file_size;
			const std::string m_prefix;
			const std::string m_suffix;
			std::vector<DataFile> m_matching_source_files;
			std::vector<DataFile> m_matching_target_files;
			std::vector<DataFile> m_non_matching_source_files;
			std::vector<DataFile> m_non_matching_target_files;
			std::atomic_bool m_generated;

			std::string GetHex(int n);

		public:
			FilesGenerator(
				std::vector<std::pair<std::string, std::string>>source_target_dirs,
				unsigned num_of_files_per_dir,
				unsigned file_size,
				std::string prefix,
				std::string suffix) :
				m_source_target_dirs{ source_target_dirs },
				m_num_of_files_per_dir{ num_of_files_per_dir },
				m_file_size{ file_size },
				m_prefix{ prefix },
				m_suffix{ suffix },
				m_generated{ false }
			{}

			void GenerateFiles(std::ostream& os = std::cout);
			void RemoveFiles(std::ostream& os = std::cout);
			std::vector<std::pair<DataFile, DataFile>> CompareFiles();
			std::vector<DataFile> CheckNonExisting() const;
			bool FilesGenerationDone() const;
			bool CreateTargetArchive() const;
		};
	}
}
