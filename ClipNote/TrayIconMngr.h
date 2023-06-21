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
	afx_msg LRESULT OnTrayIcon(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	NOTIFYICONDATA m_nid;
};

