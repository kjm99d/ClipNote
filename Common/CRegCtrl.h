#pragma once
#include <Windows.h>
#include <string>


class CRegReader;
class CRegWriter;
class CRegCtrl;

class CRegReader
{
public:
	CRegReader();
	~CRegReader();

public:
	void SetHandle(HKEY hKey);

	BOOL Get(std::wstring strValueName, int& nValue);
	BOOL Get(std::wstring strValueName, std::wstring& strValue);

private:
	HKEY m_hKey;
};

class CRegWriter
{
public:
	CRegWriter();
	~CRegWriter();

public:
	void SetHandle(HKEY hKey);

	BOOL Set(std::wstring strValueName, int& nValue);
	BOOL Set(std::wstring strValueName, std::wstring& strValue);

private:
	HKEY m_hKey;
};

class CRegDeleter
{
public:
	CRegDeleter();
	~CRegDeleter();

public:
	void SetHandle(HKEY hKey);

	BOOL Delete(std::wstring strValueName);

private:
	HKEY m_hKey;
};

class CRegCtrl
{
	CRegCtrl() = delete;
	CRegCtrl(CRegCtrl& _) = delete;

public:
	CRegCtrl(HKEY hKey, std::wstring strRegPath);
	~CRegCtrl();

public:
	BOOL Generate(CRegReader& inst);
	BOOL Generate(CRegWriter& inst);
	BOOL Generate(CRegDeleter& inst);

private:
	BOOL Open();	// CRegReader 용도
	BOOL Open(REGSAM regsam);	// CRegReader, CRegDeleter 용도
	BOOL Create();	// CRegWriter 용도
	BOOL Close();
	BOOL IsHandleAvailable();

private:
	HKEY m_hKey;
	std::wstring m_strRegPath;

	HKEY m_handle;
};

