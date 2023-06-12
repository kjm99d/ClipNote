#pragma once
#include <vector>
#include <string>

class CHistoryManager
{
public:
	static CHistoryManager* GetInstance();
	BOOL Write(const char * data);
	std::vector<std::string> GetDailyLog();
	
private:
	std::string GetCurrentDate();
	void CreateLogDirectory();
	std::string GetLogPath();
	std::string GetBase64String(const char* data);
	std::string GetBase64DecodeString(const char * data);
};

