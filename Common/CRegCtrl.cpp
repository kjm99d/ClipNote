#include "CRegCtrl.h"

// ========================================================================================== //

CRegCtrl::CRegCtrl(HKEY hKey, std::wstring strRegPath)
{
	m_hKey = hKey;
	m_strRegPath = strRegPath;
}


CRegCtrl::~CRegCtrl()
{
	if (IsHandleAvailable())
		Close();
}

BOOL CRegCtrl::Generate(CRegReader& inst)
{
	BOOL bResult = FALSE;
	if (IsHandleAvailable())
	{
		if (Open())
		{
			inst.SetHandle(m_handle);
			bResult = TRUE;
		}
		
	}

	return bResult;
}

BOOL CRegCtrl::Generate(CRegWriter& inst)
{
	BOOL bResult = FALSE;
	if (IsHandleAvailable())
	{
		if (Create())

		inst.SetHandle(m_handle);

		bResult = TRUE;
	}

	return bResult;
}

BOOL CRegCtrl::Open()
{
	BOOL bResult = FALSE;

	auto nResult = RegOpenKeyEx(m_hKey, m_strRegPath.c_str(), 0, KEY_QUERY_VALUE, &m_handle);

	if (ERROR_SUCCESS == nResult)
		bResult = TRUE;

	return bResult;
}

BOOL CRegCtrl::Create()
{
	BOOL bResult = FALSE;

	auto nResult = RegCreateKeyEx(m_hKey, m_strRegPath.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &m_handle, nullptr);

	if (ERROR_SUCCESS == nResult)
		bResult = TRUE;

	return bResult;
}

BOOL CRegCtrl::Close()
{
	
	RegCloseKey(m_hKey);
	m_hKey = NULL;
	return TRUE;
}

BOOL CRegCtrl::IsHandleAvailable()
{
	if (NULL == m_hKey)
		return FALSE;

	return TRUE;
}

// ========================================================================================== //

CRegReader::CRegReader() : m_hKey(NULL)
{
}

CRegReader::~CRegReader()
{
}

void CRegReader::SetHandle(HKEY hKey)
{
	m_hKey = hKey;
}

BOOL CRegReader::Get(std::wstring strValueName, int& nValue)
{
	BOOL bResult = FALSE;
	DWORD dwBufferSize = sizeof(nValue);
	 auto nResult = RegGetValue(m_hKey, nullptr, strValueName.c_str(), RRF_RT_DWORD, nullptr, (PVOID)&nValue, &dwBufferSize);

	 if (ERROR_SUCCESS == nResult)
		 bResult = TRUE;

	 return bResult;
}

BOOL CRegReader::Get(std::wstring strValueName, std::wstring& strValue)
{
	WCHAR szValue[1024];
	SecureZeroMemory(szValue, sizeof(szValue));

	BOOL bResult = FALSE;
	DWORD dwBufferSize = sizeof(szValue);
	auto nResult = RegGetValue(m_hKey, nullptr, strValueName.c_str(), RRF_RT_REG_SZ, nullptr, (PVOID)szValue, &dwBufferSize);

	if (ERROR_SUCCESS == nResult)
	{
		strValue = szValue;

		bResult = TRUE;
	}

	return bResult;
}


// ========================================================================================== //

CRegWriter::CRegWriter() : m_hKey(NULL)
{
}

CRegWriter::~CRegWriter()
{
}

void CRegWriter::SetHandle(HKEY hKey)
{
	m_hKey = hKey;
}

BOOL CRegWriter::Set(std::wstring strValueName, int& nValue)
{
	auto nResult = RegSetValueEx(m_hKey, strValueName.c_str(), 0, REG_DWORD, 
		reinterpret_cast<LPBYTE>(&nValue), sizeof(nValue));


	return nResult == ERROR_SUCCESS;
}

BOOL CRegWriter::Set(std::wstring strValueName, std::wstring& strValue)
{
	auto nResult = RegSetValueEx(m_hKey, strValueName.c_str(), 0, REG_DWORD, 
		reinterpret_cast<LPBYTE>(strValue.data()), strValue.size());

	return nResult == ERROR_SUCCESS;
}


// ========================================================================================== //