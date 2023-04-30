#pragma once

#include <ostream>
#include <iostream>

namespace fcs
{
	namespace test
	{
		void CollectionServiceTest(std::ostream & os = std::cout);
		void FileNameRegexTest();
		int RunAllTests();
	}
}
