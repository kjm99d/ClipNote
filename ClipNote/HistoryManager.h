#pragma once
class CHistoryManager
{
public:
	static CHistoryManager* GetInstance();
	BOOL Write(const char * data);
	
private:
	std::string GetCurrentDate();
	void CreateLogDirectory();
	std::string GetLogPath();
	std::string GetBase64String(const char * data);
};

