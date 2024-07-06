#include "Process.h"

using namespace opensees::utilities;

bool Process::runOpensees(const std::string& inputFile)
{
	STARTUPINFOW startInfo = { 0 };

	PROCESS_INFORMATION processInfo = { 0 };

	LPWSTR input = convertString(inputFile);
	bool success = CreateProcessW(L"C:\\Codes\\Opensees\\OpenSees.exe", input, NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &processInfo);
	WaitForSingleObject(processInfo.hProcess, INFINITE);
	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);

	delete[] input;

	return success;
}

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