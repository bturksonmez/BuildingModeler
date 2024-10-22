#include "Process.h"

#ifdef _WIN64
#include <windows.h>
#elif defined(__APPLE__)
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#endif

using namespace opensees::utilities;

bool Process::runOpensees(const std::string& inputFile)
{
#ifdef _WIN64
    STARTUPINFOW startInfo = { 0 };
    PROCESS_INFORMATION processInfo = { 0 };
    LPWSTR input = convertString(inputFile);
    bool success = CreateProcessW(L"C:\\Codes\\Opensees\\OpenSees.exe", input, NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &processInfo);
    WaitForSingleObject(processInfo.hProcess, INFINITE);
    CloseHandle(processInfo.hThread);
    CloseHandle(processInfo.hProcess);
    delete[] input;
    return success;
#elif defined(__APPLE__)
    pid_t pid = fork();
    if (pid == -1) {
        return false;
    } else if (pid == 0) {
        execl("/Users/bturksonmez/Desktop/Codes/OpenSees/OpenSees3.7.0/bin/OpenSees", "OpenSees", inputFile.c_str(), (char *)NULL);
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }
#endif
}

#ifdef _WIN64
LPWSTR Process::convertString(const std::string& inStr)
{
    int bufferLength = MultiByteToWideChar(CP_ACP, 0, inStr.c_str(), static_cast<int>(inStr.size()), NULL, 0);
    if (bufferLength == 0)
        return 0;
    LPWSTR wideStr = new WCHAR[bufferLength + 1];
    MultiByteToWideChar(CP_ACP, 0, inStr.c_str(), static_cast<int>(inStr.size()), wideStr, bufferLength);
    wideStr[bufferLength] = 0;
    return wideStr;
}
#endif