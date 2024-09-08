#pragma once

#ifdef _WIN64

#include <Windows.h>
#include <string>

namespace opensees
{
	namespace utilities
	{
		class Process
		{
		public:
			static bool runOpensees(const std::string& inputFile);
			static LPWSTR convertString(const std::string& inStr);

		};
	}
}

#endif // _WIN64