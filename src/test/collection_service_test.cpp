#include <filesystem>
#include <regex>
#include <cassert>
#include "collection_service_test.hpp"
#include "../collection_service_runner.hpp"
#include "../utils/files_generator.hpp"

void fcs::test::CollectionServiceTest(std::ostream& os)
{
	std::filesystem::path tmp_path = std::filesystem::temp_directory_path();
	std::filesystem::path source_path = tmp_path / "source";
	std::filesystem::path target_path = tmp_path / "target";
	std::filesystem::remove_all(source_path);
	std::filesystem::remove_all(target_path);
	std::filesystem::create_directory(source_path);
	std::filesystem::create_directory(target_path);

	fcs::CollectionServiceRunner runner;
	runner.AddDirectoryMapping(source_path.string(), target_path.string());
	runner.AssignFileNamePattern(fcs::FILE_NAME_PATTERN);

	runner.StartService();

	std::filesystem::path source_dir = source_path / "dir";
	std::filesystem::path source_dir_dir1 = source_dir / "dir1";
	std::filesystem::path source_dir_dir1_dir2 = source_dir_dir1 / "dir2";
	std::filesystem::path source_liz4 = source_path / "core.ServiceName.57d.lz4";

	std::filesystem::path target_dir = target_path / "dir";
	std::filesystem::path target_dir_dir1 = target_dir / "dir1";
	std::filesystem::path target_dir_dir1_dir2 = target_dir_dir1 / "dir2";
	std::filesystem::path target_liz4 = target_dir / "core.ServiceName.57d.lz4";

	std::vector<std::pair<std::string, std::string>>source_target_dirs;
	source_target_dirs.push_back({ source_dir.string() , target_dir.string() });
	source_target_dirs.push_back({ source_dir_dir1.string() , target_dir_dir1.string() });
	source_target_dirs.push_back({ source_dir_dir1_dir2.string() , target_dir_dir1_dir2.string() });
	source_target_dirs.push_back({ source_liz4.string() , target_liz4.string() });

	std::string matching_file_prefix = "core.ServiceName.";
	std::string matching_file_suffix = ".lz4";

	fcs::utils::FilesGenerator files_generator(source_target_dirs, 100, 1000,
		matching_file_prefix, matching_file_suffix);

	files_generator.GenerateFiles();

	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	runner.StopService();

	auto non_equal = files_generator.CompareFiles();
	auto extra_files = files_generator.CheckNonExisting();

	assert(non_equal.empty());
	assert(extra_files.empty());

	files_generator.RemoveFiles();

	std::cout << "##############################################" << std::endl;
	std::cout << __FUNCTION__ << " tests ok. " << std::endl;
	std::cout << "##############################################" << std::endl;
}


void fcs::test::FileNameRegexTest()
{
	const std::regex file_name_regex(fcs::FILE_NAME_PATTERN);

	std::vector<std::pair<std::string, bool>> file_names =
	{
	{"core.ServiceName.3057.lz4", 1},
	{"core.ServiceName.Service.3057.lz4", 0},
	{"core.ServiceName.3057.3717.16479758050.lz4", 1},
	{"core.Service.3057.abcd.16479758050.lz4", 1},
	{"core.ServiceNameName.3057.lz4", 1},
	{"core.ab02.lz4", 0},
	{"core.abcdefgh.ab02.lz4", 1},
	{"core.abcdefgh.ab02lz4",0},
	{"Core.lz4",0},
	{"core.lz4",0},
	{"core.a.lz4", 0},
	{"corea.lz4",0},
	{"core.a1.service.lz4",0},
	{"core.service.a1.lz4",1},
	{"core.service.a1.f3.lz4",1},
	{"core.service.a1.ser.f3.lz4",0},
	{"core.service.a1..lz4", 0},
	};

	for (auto& name : file_names) {
		bool res = std::regex_match(name.first, file_name_regex);
		assert(res == name.second);
	}

	std::cout << "##############################################" << std::endl;
	std::cout << __FUNCTION__ << " tests ok. " << std::endl;
	std::cout << "##############################################" << std::endl;
}

int fcs::test::RunAllTests()
{
	FileNameRegexTest();
	CollectionServiceTest();
	return 0;
}