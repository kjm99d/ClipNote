#pragma once
#include <Windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <string>

namespace CSDKCommon
{
	std::wstring GetModulePath();
	std::wstring GetEnvToolPath();

	BOOL RunProgram(std::wstring strPath, std::wstring strArgumnets = L"", BOOL bWaitExit = TRUE);
};

