#include <random>
#include <thread>
#include "files_generator.hpp"

std::string fcs::utils::FilesGenerator::GetHex(int n)
{
	std::stringstream stream;
	stream << std::hex << (n);
	return stream.str();
}

void fcs::utils::FilesGenerator::GenerateFiles(std::ostream& os)
{
	if (m_source_target_dirs.empty()) {
		os << __FUNCTION__ << ": No source/target directories specified." << std::endl;
		return;
	}
	if (m_num_of_files_per_dir < 1) {
		os << __FUNCTION__ << ": To genrate files, specify a positive number of files." << std::endl;
		return;
	}

	os << __FUNCTION__ << ": Generating files..." << std::endl;

	m_matching_source_files.clear();
	m_matching_target_files.clear();
	m_non_matching_source_files.clear();
	m_non_matching_target_files.clear();

	for (auto& dir : m_source_target_dirs) {
		for (unsigned i = 0; i < m_num_of_files_per_dir; i++) {
			std::string hex_num = GetHex(i);
			fcs::utils::DataFile match_src_data_file(dir.first, m_prefix + hex_num + m_suffix, m_file_size);
			fcs::utils::DataFile match_target_data_file(dir.second, m_prefix + hex_num + m_suffix, 0);

			fcs::utils::DataFile non_match_src_data_file(dir.first, hex_num + m_suffix, m_file_size);
			fcs::utils::DataFile non_match_target_data_file(dir.second, hex_num + m_suffix, 0);

			m_matching_source_files.push_back(match_src_data_file);
			m_matching_target_files.push_back(match_target_data_file);

			m_non_matching_source_files.push_back(non_match_src_data_file);
			m_non_matching_target_files.push_back(non_match_target_data_file);
		}
	}

	std::random_device rdev;
	std::mt19937 rng(rdev());
	std::uniform_int_distribution<std::mt19937::result_type> dist10(1, 10);

	for (unsigned i = 0; i < m_matching_source_files.size(); i++) {
		m_matching_source_files[i].WriteToDisk();
		m_non_matching_source_files[i].WriteToDisk();
		std::this_thread::sleep_for(std::chrono::milliseconds(dist10(rng)));
	}

	m_generated = true;
	os << __FUNCTION__ << ": Total " << m_num_of_files_per_dir * m_source_target_dirs.size() << " files generated.";
}

std::vector<std::pair<fcs::utils::DataFile, fcs::utils::DataFile>>
fcs::utils::FilesGenerator::CompareFiles()
{
	std::vector<std::pair<fcs::utils::DataFile, fcs::utils::DataFile>> result;
	if (!m_generated) {
		return result;
	}
	for (unsigned i = 0; i < m_num_of_files_per_dir; i++) {
		m_matching_target_files[i].ReadFromDisk();
		if (m_matching_source_files[i] != m_matching_target_files[i]) {
			result.push_back({ m_matching_source_files[i], m_matching_target_files[i] });
		}
	}
	return result;
}

std::vector<fcs::utils::DataFile>
fcs::utils::FilesGenerator::CheckNonExisting() const
{
	std::vector<DataFile> result;
	if (!m_generated) {
		return result;
	}
	for (unsigned i = 0; i < m_num_of_files_per_dir; i++) {
		if (std::filesystem::exists(m_non_matching_target_files[i].GetFullFileName())) {
			result.push_back(m_non_matching_target_files[i]);
		}
	}
	return result;
}

void fcs::utils::FilesGenerator::RemoveFiles(std::ostream& os)
{
	os << __FUNCTION__ << ": Removing generated files..." << std::endl;
	std::filesystem::path source_parent = std::filesystem::path(m_source_target_dirs[0].first).parent_path();
	std::filesystem::path target_parent = std::filesystem::path(m_source_target_dirs[0].second).parent_path();

	std::filesystem::remove_all(source_parent);
	std::filesystem::remove_all(target_parent);

	std::filesystem::create_directory(source_parent);
	std::filesystem::create_directory(target_parent);

	m_generated = false;
}

bool fcs::utils::FilesGenerator::FilesGenerationDone() const
{
	return m_generated;
}

bool fcs::utils::FilesGenerator::CreateTargetArchive() const
{
	if (m_source_target_dirs.empty()) {
		return false;
	}
	std::filesystem::path target_parent = std::filesystem::path(m_source_target_dirs[0].second).parent_path();

	std::string tar_file = target_parent.string() + ".tar";
	std::filesystem::remove_all(tar_file);
	std::filesystem::create_directory(tar_file);
	std::filesystem::copy(target_parent, tar_file, std::filesystem::copy_options::recursive);
	return true;
}