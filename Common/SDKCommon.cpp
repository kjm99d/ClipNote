#include "SDKCommon.h"
#include "CRegCtrl.h"

/**
 * \brief 프로그램이 설치된 위치를 구한다..
 * 
 * \return 모듈의 경로
 */
std::wstring CSDKCommon::GetModulePath()
{
	WCHAR szModulePath[1024];
	SecureZeroMemory(szModulePath, sizeof(szModulePath));

	GetModuleFileName(NULL, szModulePath, _countof(szModulePath));
	PathRemoveFileSpec(szModulePath);

	return szModulePath;
}

/**
 * \brief 프로그램이 설치된 위치에서 환경설정 툴의 경로를 구한다.
 *
 * \return 모듈의 경로
 */
std::wstring CSDKCommon::GetEnvToolPath()
{
	std::wstring strModulePath = GetModulePath();
	
	WCHAR lastChar = strModulePath.back();
	if (L'\\' != lastChar)
		strModulePath += L'\\';
	
	strModulePath += L"ClipNoteEnv.exe";

	return strModulePath;
}

std::wstring CSDKCommon::GetClipNotePath()
{
    std::wstring strModulePath = GetModulePath();

    WCHAR lastChar = strModulePath.back();
    if (L'\\' != lastChar)
        strModulePath += L'\\';

    strModulePath += L"ClipNote.exe";

    return strModulePath;
}

/**
 * \brief 프로그램을 실행시킨다.
 * 
 * \param strPath		프로그램의 경로
 * \param strArgumnets	프로그램의 아규먼트
 * \param bWaitExit		프로그램이 종료 대기 여부
 * \return 성공(TRUE), 실패( WinError )
 */
BOOL CSDKCommon::RunProgram(std::wstring strPath, std::wstring strArgumnets, BOOL bWaitExit)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    SecureZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    SecureZeroMemory(&pi, sizeof(pi));

    // 실행할 프로그램의 경로와 인자값 설정
    const WCHAR * command = strPath.c_str();
    const WCHAR* args = strArgumnets.c_str();

    // 새로운 프로세스 생성
    if (!CreateProcess(command, 
        const_cast<WCHAR *>(args), 
        NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        return GetLastError();
    }

    if (TRUE == bWaitExit)
    {
        WaitForSingleObject(pi.hProcess, INFINITE);
    }

    // 핸들 해제
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

BOOL CSDKCommon::AddStartProgram(std::wstring strName, std::wstring strFilePath)
{
    CRegCtrl RegCtrl(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");

    CRegWriter RegWriter;
    if (TRUE == RegCtrl.Generate(RegWriter) && TRUE == RegWriter.Set(strName, strFilePath))
    {
        return TRUE;
    }


    return FALSE;
}

BOOL CSDKCommon::DelStartProgram(std::wstring strName)
{
    CRegCtrl RegCtrl(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");

    CRegDeleter RegDeleter;
    if (TRUE == RegCtrl.Generate(RegDeleter) && TRUE == RegDeleter.Delete(strName))
    {
        return TRUE;
    }


    return FALSE;
}

BOOL CSDKCommon::IsExistsStartProgram(std::wstring strName, std::wstring& strFilePath)
{
    CRegCtrl RegCtrl(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");

    CRegReader RegReader;
    std::wstring strValue;
    if (TRUE == RegCtrl.Generate(RegReader) && TRUE == RegReader.Get(strName, strValue))
    {
        return TRUE;
    }

    return FALSE;
}
