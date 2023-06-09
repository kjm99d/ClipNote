#include "pch.h"
#include "HistoryManager.h"

CHistoryManager* CHistoryManager::GetInstance()
{
    static CHistoryManager * inst = nullptr;
    if (nullptr == inst)
        inst = new CHistoryManager();

    return inst;
}

BOOL CHistoryManager::Write(char* data)
{
    CreateLogDirectory();

    std::string strLogFileName = GetCurrentDate();
    std::string strLogFullPath = GetLogPath();
    strLogFullPath += strLogFileName;

    std::ofstream out;
    out.open(strLogFullPath, std::ios::out | std::ios::app); // 기존 파일에 하위에 쓰기
    if (false == out.is_open()) {
        out.open(strLogFullPath, std::ios::out);
    }
    OutputDebugStringA(strLogFullPath.c_str());

    out << data << std::endl;

    return TRUE;
}

std::string CHistoryManager::GetCurrentDate()
{
    time_t timer = time(NULL);
    struct tm t;
    localtime_s(&t, &timer);

    int nYear = t.tm_year + 1900;
    int nMonth = t.tm_mon + 1;
    int nDay = t.tm_mday;

    char szCurrentDate[64];
    SecureZeroMemory(szCurrentDate, sizeof(szCurrentDate));
    sprintf_s(szCurrentDate, _countof(szCurrentDate), "%04d_%02d_%02d.log", nYear, nMonth, nDay);

    return szCurrentDate;
}

void CHistoryManager::CreateLogDirectory()
{
    char szFullPath[1024];
    SecureZeroMemory(szFullPath, sizeof(szFullPath));
    GetModuleFileNameA(NULL, szFullPath, _countof(szFullPath));
    PathRemoveFileSpecA(szFullPath);
    PathAppendA(szFullPath, "Log");

    if (FALSE == PathFileExistsA(szFullPath))
        CreateDirectoryA(szFullPath, 0);
}

std::string CHistoryManager::GetLogPath()
{
    char szFullPath[1024];
    SecureZeroMemory(szFullPath, sizeof(szFullPath));
    GetModuleFileNameA(NULL, szFullPath, _countof(szFullPath));
    PathRemoveFileSpecA(szFullPath);
    PathAppendA(szFullPath, "Log\\");

    return szFullPath;
}
