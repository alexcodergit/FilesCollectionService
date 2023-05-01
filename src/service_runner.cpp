#include <iostream>
#include <algorithm>
#include "collection_service_runner.hpp"
#include "test/collection_service_test.hpp"
#include "utils/program_arguments.hpp"
#include "utils/tar_archive.hpp"
#include "utils/files_generator.hpp"

int main(int argc, const char* argv[])
{
	fcs::utils::ProgramArgumentsParser parser(argc, argv);
	if (parser.m_run_option < 0) {
		return -1;
	}
	if (parser.m_run_option == 1) {
		fcs::test::RunAllTests();
		std::cout << "Tests Ok!" << std::endl;

		return 0;
	}
	else if (parser.m_run_option == 2) {
		std::string matching_file_prefix = "core.ServiceName.";
		std::string matching_file_suffix = ".lz4";

		fcs::utils::FilesGenerator files_generator({ {parser.m_source_dir,parser.m_source_dir} },
			100, 1000,
			matching_file_prefix, matching_file_suffix);

		files_generator.GenerateFiles();

		return 0;
	}
	fcs::CollectionServiceRunner runner;
	runner.AddDirectoryMapping(parser.m_source_dir, parser.m_target_dir);
	runner.AssignFileNamePattern(fcs::FILE_NAME_PATTERN);
	runner.StartService();
	std::string stop;
	while (stop != "stop") {
		std::cout << "Enter 'stop' to stop service." << std::endl;
		std::cin >> stop;
		std::transform(stop.begin(), stop.end(), stop.begin(),
			[](char c) { return std::tolower(c); });
	}
	runner.StopService();
	fcs::utils::TarArchive archive;
	std::string tar_file_name = archive.CreateTarArchive(parser.m_target_dir);
	std::cout << "Tar File '" << tar_file_name << "' created.";

	return 0;
}

