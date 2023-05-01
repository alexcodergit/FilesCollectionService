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
			std::string const m_const_usage = "'FilesCollectionService test': -run tests\n'FilesCollectionService directory: ' -generates files with different name patterns into directory\n'source_dir' 'target_dir': -run service\n";
			int m_run_option;
			std::string m_source_dir;
			std::string m_target_dir;
			ProgramArgumentsParser(int argc, const char* argv[], std::ostream& os = std::cout);
			void TrimString(std::string& str) const;
		};
	}
}