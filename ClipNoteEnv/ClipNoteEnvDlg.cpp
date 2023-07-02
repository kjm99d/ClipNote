
// ClipNoteEnvDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ClipNoteEnv.h"
#include "ClipNoteEnvDlg.h"
#include "afxdialogex.h"
#include "SDKCommon.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CClipNoteEnvDlg 대화 상자



CClipNoteEnvDlg::CClipNoteEnvDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIPNOTEENV_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClipNoteEnvDlg::SetUI()
{
	// 윈도우 시작 시 자동실행 기능
	std::wstring strFilePath = L"";	// << 시작프로그램으로 등록된 경로
	if (TRUE == CSDKCommon::IsExistsStartProgram(STR_STARTPROGRAM_NAME, strFilePath))
	{
		CButton* pCheckBtn = (CButton*)GetDlgItem(IDC_CHECK1);
		pCheckBtn->SetCheck(BST_CHECKED);
	}

	CRegCtrl RegCtrl(HKEY_LOCAL_MACHINE, L"SOFTWARE\\ClipNote");
	CRegReader RegReader;
	if (TRUE == RegCtrl.Generate(RegReader))
	{
		int nValue = 0;
		RegReader.Get(L"MostTop", nValue);

		if (0 == nValue)
		{
			SetButtonDisable(IDC_TOP_KEY_CTRL);
			SetButtonDisable(IDC_TOP_KEY_ALT);
			SetButtonDisable(IDC_TOP_KEY_WIN);
		}
		else
		{
			int nValue = 0;

			RegReader.Get(L"MostTop_ALT", nValue);
			if (nValue == 1)
				static_cast<CButton*>(GetDlgItem(IDC_TOP_KEY_ALT))->SetCheck(BTNS_CHECK);

			RegReader.Get(L"MostTop_CTRL", nValue);
			if (nValue == 1)
				static_cast<CButton*>(GetDlgItem(IDC_TOP_KEY_CTRL))->SetCheck(BTNS_CHECK);

			RegReader.Get(L"MostTop_WIN", nValue);
			if (nValue == 1)
				static_cast<CButton*>(GetDlgItem(IDC_TOP_KEY_WIN))->SetCheck(BTNS_CHECK);
		}

		std::wstring strValue;
		RegReader.Get(L"MostTop_SubKey", strValue);

		CComboBox* pCbBox = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_SUBKEY));
		if (pCbBox)
			pCbBox->SetWindowTextW(strValue.c_str());

		RegReader.Get(L"ClickCopy", nValue);
		if (1 == nValue)
		{
			static_cast<CButton*>(GetDlgItem(IDC_CLICK_COPY))->SetCheck(BTNS_CHECK);
		}

		
	}

	
}

BOOL CClipNoteEnvDlg::IsUserSetMostTopIsDisabled()
{
	BOOL bResult = FALSE;

	CComboBox* pCbBox = (CComboBox*)GetDlgItem(IDC_COMBO_SUBKEY);
	if (pCbBox)
	{
		CString strComboBoxText;
		pCbBox->GetWindowTextW(strComboBoxText);
		if (0 == strComboBoxText.CompareNoCase(L"없음"))
		{
			bResult = TRUE;
		}
	}

	return bResult;
}

void CClipNoteEnvDlg::SetButtonDisable(int nId)
{
	CButton* pButton = static_cast<CButton *>(GetDlgItem(nId));
	if (pButton)
		pButton->EnableWindow(FALSE);
}

void CClipNoteEnvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClipNoteEnvDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CClipNoteEnvDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CClipNoteEnvDlg 메시지 처리기

BOOL CClipNoteEnvDlg::OnInitDialog()
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

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetUI();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CClipNoteEnvDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClipNoteEnvDlg::OnPaint()
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
HCURSOR CClipNoteEnvDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClipNoteEnvDlg::OnBnClickedOk()
{
	// 시작 프로그램 등록 로직
	CButton* pBtnWinStartup = (CButton*)GetDlgItem(IDC_CHECK1);
	switch (pBtnWinStartup->GetCheck())
	{
	case BST_CHECKED:
		CSDKCommon::AddStartProgram(STR_STARTPROGRAM_NAME, CSDKCommon::GetClipNotePath());
		break;
	case BST_UNCHECKED:
		if (TRUE == CSDKCommon::IsExistsStartProgram(STR_STARTPROGRAM_NAME, CSDKCommon::GetClipNotePath()))
			CSDKCommon::DelStartProgram(STR_STARTPROGRAM_NAME);
		break;
	}


	// 최상위 단축키
	CRegCtrl RegCtrl(HKEY_LOCAL_MACHINE, L"SOFTWARE\\ClipNote");
	CRegWriter RegWriter;
	RegCtrl.Generate(RegWriter);
	if (TRUE == IsUserSetMostTopIsDisabled())
	{
		int nMostTop = 0;
		RegWriter.Set(L"MostTop", nMostTop);
	}
	else
	{
		CButton* pChkBtnTopCtrl = (CButton*)GetDlgItem(IDC_TOP_KEY_CTRL);
		CButton* pChkBtnTopAlt = (CButton*)GetDlgItem(IDC_TOP_KEY_ALT);
		CButton* pChkBtnTopWin = (CButton*)GetDlgItem(IDC_TOP_KEY_WIN);
		CComboBox* pCbBox = (CComboBox*)GetDlgItem(IDC_COMBO_SUBKEY);

		if (pChkBtnTopCtrl && pChkBtnTopAlt && pChkBtnTopWin)
		{
			// 레지스트리 정보를 가져온다.
			// 레지스트리 내부에 위 핫키 정보가 저장되어 있는지 확인하고,
			// 저장되어 있다면 상태를 UI에 표시한다.

			int nMostTop = 1;
			RegWriter.Set(L"MostTop", nMostTop);


			int nValue = 0;

			nValue = pChkBtnTopCtrl->GetCheck();
			RegWriter.Set(L"MostTop_CTRL", nValue);

			nValue = pChkBtnTopAlt->GetCheck();
			RegWriter.Set(L"MostTop_ALT", nValue);

			nValue = pChkBtnTopWin->GetCheck();
			RegWriter.Set(L"MostTop_WIN", nValue);


			CString strValue = L"";
			pCbBox->GetWindowText(strValue);
			RegWriter.Set(L"MostTop_SubKey", std::wstring(strValue));

		}

		CButton* pChkBtnClickCopy = (CButton*)GetDlgItem(IDC_CLICK_COPY);
		if (pChkBtnClickCopy)
		{
			int nValue = pChkBtnClickCopy->GetCheck();
			RegWriter.Set(L"ClickCopy", nValue);
		}
	}

	CDialogEx::OnOK();
}
