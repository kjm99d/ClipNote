#pragma once
#include <Windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <string>

namespace CSDKCommon
{
	std::wstring GetModulePath();
	std::wstring GetEnvToolPath();
	std::wstring GetClipNotePath();

	BOOL RunProgram(std::wstring strPath, std::wstring strArgumnets = L"", BOOL bWaitExit = TRUE);

	BOOL AddStartProgram(std::wstring strName, std::wstring strFilePath);
	BOOL DelStartProgram(std::wstring strName);
	BOOL IsExistsStartProgram(std::wstring strName, std::wstring& strFilePath);
};

