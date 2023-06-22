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
	

private:
	NOTIFYICONDATA m_nid;
};

