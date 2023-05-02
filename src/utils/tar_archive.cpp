#include <filesystem>
#include "tar_archive.hpp"

std::string fcs::utils::TarArchive::CreateTarArchive(std::string dir_name) const
{
	std::filesystem::path dpath(dir_name);
	if (!std::filesystem::exists(dpath) || !std::filesystem::is_directory(dpath)) {
		return "";
	}
	
	const auto cur_time = std::chrono::system_clock::now();
	auto count = std::chrono::duration_cast<std::chrono::milliseconds>(
			cur_time.time_since_epoch()).count();
	if (dir_name.back() == std::filesystem::path::preferred_separator) {
		dir_name.pop_back();
	}
	std::string tar_dir_name = dir_name + "_" + std::to_string(count) + ".tar";
	std::filesystem::create_directory(tar_dir_name);
	std::filesystem::copy(dpath, tar_dir_name, std::filesystem::copy_options::recursive);
	return tar_dir_name;
}
