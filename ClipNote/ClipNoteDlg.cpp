
// ClipNoteDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ClipNote.h"
#include "ClipNoteDlg.h"
#include "afxdialogex.h"

#include "HistoryManager.h"
#include "TrayIconMngr.h"
#include "WM_MSG_ID.h"
#include "CRegCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//static int m_stop_flag = 0;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClipNoteDlg 대화 상자

HHOOK CClipNoteDlg::m_hHook = NULL;

CClipNoteDlg::CClipNoteDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIPNOTE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pTray = std::make_unique<CTrayIconMngr>(pParent);
	
}

void CClipNoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
	DDX_Control(pDX, IDC_STATIC_Preview, m_TextPreview);
}

char* CClipNoteDlg::CopyClipboardToText()
{
	unsigned int priority_list = CF_TEXT;
	char* p_string = NULL;
	// 클립보드에는 문자열만 저장할수 있는것이 아니기 때문에 
	// 현재 문자열이 저장되어 있는지 확인한다.
	if (::GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT) {
		// 클립보드에 있는 비트맵 정보를 얻기 위해서 클립보드를 연다.
		if (::OpenClipboard(m_hWnd)) {
			// 클립보드에서 문자열이 저장된 메모리의 핸들 값을 얻는다.
			HANDLE h_clipboard_data = ::GetClipboardData(CF_TEXT);
			if (h_clipboard_data != NULL) {
				// 메모리 핸들 값을 이용하여 실제 사용가능한 주소를 얻는다.
				char* p_clipboard_data = (char*)::GlobalLock(h_clipboard_data);
				// 클립보드에 저장된 문자열의 길이를 구한다. ('\0'포함 크기)
				int string_len = strlen(p_clipboard_data) + 1;
				// 클립보드에 저장된 문자열을 저장하기 위해 메모리를 할당한다.
				p_string = new char[string_len];
				// 할당된 메모리에 클리보드 문자열을 복사한다.
				memcpy(p_string, p_clipboard_data, string_len);

				CHistoryManager::GetInstance()->Write(p_string);

				// 문자열을 복사하기 위해서 Lock했던 메모리를 해제한다.
				::GlobalUnlock(h_clipboard_data);
			}
			// 클립보드를 닫는다.
			::CloseClipboard();
		}
	}
	// 클립보드 문자열을 복사한 메모리의 주소를 반환한다.
	return p_string;
}

LRESULT CClipNoteDlg::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		// 키보드 이벤트가 발생한 경우 처리
		KBDLLHOOKSTRUCT* pKeyboardStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

		if (wParam == WM_KEYDOWN)
		{
			bool bClickShift = HIWORD(GetKeyState(VK_SHIFT)) != 0;
			bool bClickAlt = HIWORD(GetKeyState(VK_MENU)) != 0;
			bool bClickCtrl = HIWORD(GetKeyState(VK_CONTROL)) != 0;
			bool bClickWin = HIWORD(GetKeyState(VK_LWIN)) || HIWORD(GetKeyState(VK_RWIN));


			CRegCtrl RegCtrl(HKEY_LOCAL_MACHINE, L"SOFTWARE\\ClipNote");
			CRegReader RegReader;
			if (TRUE == RegCtrl.Generate(RegReader))
			{
				int nValueAlt = 0;
				RegReader.Get(L"MostTop_ALT", nValueAlt);

				int nValueCtrl = 0;
				RegReader.Get(L"MostTop_CTRL", nValueCtrl);

				int nValueWin = 0;
				RegReader.Get(L"MostTop_WIN", nValueWin);

				std::wstring strSubKey = L"";
				RegReader.Get(L"MostTop_SubKey", strSubKey);
				
				// WM_KEYDOWN 이벤트인 경우
				DWORD vkCode = pKeyboardStruct->vkCode;
				if (bClickAlt == nValueAlt &&
					bClickCtrl == nValueCtrl &&
					bClickWin == nValueWin &&
					vkCode == strSubKey[0])
				{
					::SetForegroundWindow(AfxGetMainWnd()->GetSafeHwnd());
				}
			}
		}
	}

	// 다음 후킹 프로시저로 이벤트를 전달
	return CallNextHookEx(m_hHook, nCode, wParam, lParam);
}


BOOL CClipNoteDlg::SetHook()
{
	m_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, AfxGetInstanceHandle(), NULL);

	if (m_hHook)
		return TRUE;
	return FALSE;
}

BOOL CClipNoteDlg::UnHook()
{
	BOOL bKeyboardUnHook = UnhookWindowsHookEx(m_hHook);

	return bKeyboardUnHook;
}


BEGIN_MESSAGE_MAP(CClipNoteDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_CHANGECBCHAIN()
	ON_WM_DRAWCLIPBOARD()
	ON_BN_CLICKED(IDC_BUTTON2, &CClipNoteDlg::OnBnClickedButton2)
	ON_WM_KEYDOWN()
	ON_WM_CLIPBOARDUPDATE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CClipNoteDlg::OnLvnItemchangedList1)
	ON_WM_CREATE()
	ON_MESSAGE(WM_TRAY_ICON, &CTrayIconMngr::OnTrayIcon)
	ON_COMMAND(ID_TRAY_EXIT, &CTrayIconMngr::OnTrayExit)
	ON_COMMAND(ID_ENV_TOOL, &CTrayIconMngr::OnTrayEnv)
END_MESSAGE_MAP()


// CClipNoteDlg 메시지 처리기

void CClipNoteDlg::SetupListCtrl()
{
	// List Control 초기화 및 스타일 설정
	//m_listCtrl.Create(WS_VISIBLE | WS_CHILD | LVS_REPORT, CRect(10, 10, 300, 200), this, IDC_LIST1);
	m_listCtrl.InsertColumn(0, _T("Column 1"), LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(1, _T("Column 2"), LVCFMT_LEFT, 100);
}

BOOL CClipNoteDlg::UseClickCopy()
{
	BOOL bResult = FALSE;
	CRegCtrl RegCtrl(HKEY_LOCAL_MACHINE, L"SOFTWARE\\ClipNote");
	CRegReader RegReader;
	if (TRUE == RegCtrl.Generate(RegReader))
	{
		int nValue = 0;
		RegReader.Get(L"ClickCopy", nValue);

		if (1 == nValue)
			bResult = TRUE;
	}

	return bResult;
}

BOOL CClipNoteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	if (FALSE == SetHook())
	{
		OutputDebugString(L"aaa!");
	}

	mh_next_chain = SetClipboardViewer();
#if 0
	CRect rect;
	m_listCtrl.GetClientRect(&rect);
	
	m_listCtrl.InsertColumn(1, _T("B"), LVCFMT_LEFT, rect.Width() - 300);
#endif
	m_listCtrl.InsertColumn(0, _T("클립보드 내용"), LVCFMT_CENTER);
	//m_listCtrl.SetColumnWidth(0, LVSCW_AUTOSIZE);
	m_listCtrl.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);

	m_listCtrl.SetExtendedStyle(LVS_REPORT | LVS_EX_FULLROWSELECT);


	GetDailyLog();

	return TRUE;  // 포커스를  컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CClipNoteDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CClipNoteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CClipNoteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClipNoteDlg::GetDailyLog()
{
	auto items = CHistoryManager::GetInstance()->GetDailyLog();
	for (int i = items.size() - 1; i >= 0; --i)
	{
		CString cs(items.at(i).c_str());

		m_listCtrl.InsertItem(0, cs);
		m_listCtrl.SetItem(0, 0, LVIF_TEXT, cs, 0, 0, 0, 0);
	}
}

void CClipNoteDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (mh_next_chain != NULL)
	{
		ChangeClipboardChain(mh_next_chain);
		mh_next_chain = NULL;
	}

	if (m_hHook)
		UnHook();

	if (m_pTray.get())
		m_pTray->Destroy();
}


void CClipNoteDlg::OnChangeCbChain(HWND hWndRemove, HWND hWndAfter)
{
	CDialogEx::OnChangeCbChain(hWndRemove, hWndAfter);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.


	// 자신 다음에 있던 윈도우가 종료되었으면 종료된 윈도우의 다음 윈도우의 핸들을 받아서 
	// 자신의 다음 윈도우로 보관한다.
	if (hWndRemove == mh_next_chain) 
		mh_next_chain = (HWND)hWndAfter;
	else if (mh_next_chain != NULL)
	{
		// 자신의 다음 윈도우가 종료된 것이 아니라면 이 메세지를 자신의 다음 윈도우로 전송한다.
		::SendMessage(mh_next_chain, WM_CHANGECBCHAIN, (WPARAM)hWndRemove, (LPARAM)hWndAfter);
	}

}


void CClipNoteDlg::OnDrawClipboard()
{
	CDialogEx::OnDrawClipboard();

	//if (m_stop_flag == 0) return void();

	static BOOL bIsFirstDraw = TRUE;
	if (TRUE == bIsFirstDraw)
	{
		bIsFirstDraw = FALSE;
		return void();
	}

	if (::GetActiveWindow() == m_hWnd)
		return void();

	// 클립보드에서 문자열을 가져온다.
	char* p_string = CopyClipboardToText();
	CString cs(p_string);
	if (NULL != p_string)
	{

		m_listCtrl.InsertItem(0, cs);
		m_listCtrl.SetItem(0, 0, LVIF_TEXT, cs, 0, 0, 0, 0);

	}
	
}

void CClipNoteDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_listCtrl.DeleteAllItems();
}


void CClipNoteDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	WCHAR szMsg[1024];
	SecureZeroMemory(szMsg, sizeof(szMsg));
	swprintf_s(szMsg, _countof(szMsg), L"nChar[%d] nbRepCnt[%d] nFlags[%d]", nChar, nRepCnt, nFlags);
	OutputDebugStringW(szMsg);

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CClipNoteDlg::OnClipboardUpdate()
{
	// 이 기능을 사용하려면 Windows Vista 이상이 있어야 합니다.
	// _WIN32_WINNT 기호는 0x0600보다 크거나 같아야 합니다.
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	OutputDebugString(_T("OnClipboardUpdate() !!!"));

	CDialogEx::OnClipboardUpdate();
}


void CClipNoteDlg::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (TRUE == UseClickCopy())
	{

		// 선택한 항목의 상태가 변경된 경우
		if ((pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVIS_SELECTED))
		{
			// 현재 선택된 항목을 가져옴
			int nItem = pNMLV->iItem;

			// 선택된 항목의 값이 변경된 경우
			if ((pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED))
			{
				// 선택된 항목의 값을 가져옴
				CString strSelectedValue = m_listCtrl.GetItemText(nItem, 0);

				// 처리할 내용 작성
				m_TextPreview.SetWindowTextW(strSelectedValue);

				const int nSize = (strSelectedValue.GetLength() + 1) * sizeof(WCHAR);
				HANDLE hMem = GlobalAlloc(GMEM_DDESHARE, nSize); // 설마 NULL도 2바이라고 이거 +1은 안되는거야 ?

				WCHAR* p_clipboard_data = (WCHAR*)::GlobalLock(hMem);

				memcpy_s(p_clipboard_data, nSize, strSelectedValue.GetBuffer(), nSize);
				::GlobalUnlock(p_clipboard_data);

				if (TRUE == OpenClipboard())
				{
					EmptyClipboard();
					HANDLE hClip = SetClipboardData(CF_UNICODETEXT, hMem);
					if (hClip)
						OutputDebugString(L"Success ! \n");

					CloseClipboard();
				}

				//GlobalFree(hMem);
			}
		}

	}

	*pResult = 0;
}

int CClipNoteDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if (m_pTray)
		m_pTray->Create();

	return 0;
}
