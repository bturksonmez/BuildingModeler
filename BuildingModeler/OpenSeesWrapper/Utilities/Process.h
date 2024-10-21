#pragma once

#include <string>

#ifdef _WIN64
#include <Windows.h>
#elif defined(__APPLE__)
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#endif

namespace opensees
{
    namespace utilities
    {
        class Process
        {
        public:
            static bool runOpensees(const std::string& inputFile);

        #ifdef _WIN64
            static LPWSTR convertString(const std::string& inStr);
        #endif
        };
    }
}