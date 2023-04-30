#pragma once

#include <ostream>
#include <iostream>
#include <string>

namespace fcs
{
	namespace utils
	{
		struct TarArchive 
		{
			std::string CreateTarArchive(std::string const dir_name) const;
		};
	}
}