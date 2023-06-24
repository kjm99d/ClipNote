#include "SDKCommon.h"

std::wstring CSDKCommon::GetModulePath()
{
	WCHAR szModulePath[1024];
	SecureZeroMemory(szModulePath, sizeof(szModulePath));

	GetModuleFileName(NULL, szModulePath, _countof(szModulePath));
	PathRemoveFileSpec(szModulePath);

	return szModulePath;
}

std::wstring CSDKCommon::GetEnvToolPath()
{
	std::wstring strModulePath = GetModulePath();
	
	WCHAR lastChar = strModulePath.back();
	if (L'\\' != lastChar)
		strModulePath += L'\\';
	
	strModulePath += L"ClipNoteEnv.exe";

	return strModulePath;
}
