
// ClipNoteDlg.h: 헤더 파일
//

#pragma once

// CClipNoteDlg 대화 상자
class CClipNoteDlg : public CDialogEx
{
// 생성입니다.
public:
	CClipNoteDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIPNOTE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

public:
	char * CopyClipboardToText();

private:
	static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
	BOOL SetHook();
	BOOL UnHook();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	HWND mh_next_chain;

	static HHOOK m_hHook;

private:
	CListCtrl m_listCtrl;
	void SetupListCtrl();

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void GetDailyLog();

public:
	afx_msg void OnDestroy();
	afx_msg void OnChangeCbChain(HWND hWndRemove, HWND hWndAfter);
	afx_msg void OnDrawClipboard();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClipboardUpdate();
};
