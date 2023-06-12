#include "pch.h"
#include "HistoryManager.h"

CHistoryManager* CHistoryManager::GetInstance()
{
    static CHistoryManager * inst = nullptr;
    if (nullptr == inst)
        inst = new CHistoryManager();

    return inst;
}

BOOL CHistoryManager::Write(const char* data)
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

    std::string strEncode = GetBase64String(data);

    out << strEncode.c_str() << std::endl;

    return TRUE;
}

std::vector<std::string> CHistoryManager::GetDailyLog()
{
    std::vector<std::string> vecItems;

    std::string strLogPath = GetLogPath();
    std::string strCurrentDate = GetCurrentDate();

    std::string strFullPath = strLogPath + strCurrentDate;

    if (TRUE == PathFileExistsA(strFullPath.c_str()))
    {
        std::ifstream input;
        input.open(strFullPath.c_str());

        if (input)
        {
            std::string line;

            // 파일 끝까지 계속 읽음
            while (std::getline(input, line)) {
                vecItems.push_back(GetBase64DecodeString(line.c_str()));
            }
        }
    }

    return vecItems;
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

std::string CHistoryManager::GetBase64String(const char* data)
{
    return base64_encode(reinterpret_cast<const unsigned char*>(data), strlen(data));
}

std::string CHistoryManager::GetBase64DecodeString(const char* data)
{
    std::string decoded = base64_decode(data);
    return decoded;
}
