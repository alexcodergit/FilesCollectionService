#pragma once

#include <ostream>
#include <iostream>
#include <string>

namespace fcs
{
	namespace utils
	{
		struct ProgramArgumentsParser 
		{
			std::string const m_const_usage = "'test': run tests\n'source_dir' 'target_dir': source directory, target directory\n";
			int m_run_option;
			std::string m_source_dir;
			std::string m_target_dir;
			ProgramArgumentsParser(int argc, const char* argv[], std::ostream& os = std::cout);
			void TrimString(std::string& str) const;
		};
	}
}