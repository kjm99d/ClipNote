#pragma once
class CTrayIconMngr : public CDialogEx
{
public:
	CTrayIconMngr(CWnd* parent);
	~CTrayIconMngr();

public:
	BOOL Create();
	BOOL Destroy();

public:
	LRESULT OnTrayIcon(WPARAM wParam, LPARAM lParam);
	
	void OnTrayExit();
	void OnTrayEnv();
	
private:
	NOTIFYICONDATA m_nid;
};

