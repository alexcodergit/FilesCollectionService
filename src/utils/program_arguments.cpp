#include <filesystem>
#include "program_arguments.hpp"

fcs::utils::ProgramArgumentsParser::ProgramArgumentsParser(int argc, const char* argv[], std::ostream& os):
	m_run_option{ -1 }
{
	if (argc < 2 || argc > 3) {
		os << "Wrong arguments." << std::endl;
		os << "Usage:" << std::endl;
		os << m_const_usage;
		m_run_option = -1;
	}
	else if (argc == 2) {
		std::string opt(argv[1]);
		TrimString(opt);
		if (opt == "test") {
			m_run_option = 1;
		}
		else {
			os << "Wrong arguments provided." << std::endl;
			os << "Usage:" << std::endl;
			os << m_const_usage;
			m_run_option = -1;
		}
	}
	else {
		std::string  src(argv[1]);
		std::string  tar(argv[2]);
		TrimString(src);
		TrimString(tar);
		std::filesystem::path path_src(src);
		std::filesystem::path path_tar(tar);
		if (!std::filesystem::is_directory(path_src)) {
			os << src << " is not vaid path." << std::endl;
			m_run_option = -1;
		}
		if (!std::filesystem::is_directory(path_tar)) {
			os << tar << " is not vaid path." << std::endl;
			m_run_option = -1;
		}
		if (std::filesystem::is_directory(path_src) && std::filesystem::is_directory(path_tar)) {
			m_source_dir = src;
			m_target_dir = tar;
			m_run_option = 2;
		}
	}
}

void fcs::utils::ProgramArgumentsParser::TrimString(std::string& str) const
{
	while (!str.empty() && str.back() == ' ') {
		str.pop_back();
	}
	while (!str.empty() && str.front() == ' ') {
		str.erase(str.begin());
	}
}