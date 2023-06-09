#pragma once
class CHistoryManager
{
public:
	static CHistoryManager* GetInstance();
	BOOL Write(char * data);
	
private:
	std::string GetCurrentDate();
	void CreateLogDirectory();
	std::string GetLogPath();
};

