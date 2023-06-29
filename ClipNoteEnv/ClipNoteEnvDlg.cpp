
// ClipNoteEnvDlg.cpp: ���� ����
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


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CClipNoteEnvDlg ��ȭ ����



CClipNoteEnvDlg::CClipNoteEnvDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIPNOTEENV_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClipNoteEnvDlg::SetUI()
{
	// ������ ���� �� �ڵ����� ���
	std::wstring strFilePath = L"";	// << �������α׷����� ��ϵ� ���
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

		std::wstring strValue;
		RegReader.Get(L"MostTop_SubKey", strValue);

		CComboBox* pCbBox = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_SUBKEY));
		if (pCbBox)
			pCbBox->SetWindowTextW(strValue.c_str());
		
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
		if (0 == strComboBoxText.CompareNoCase(L"����"))
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


// CClipNoteEnvDlg �޽��� ó����

BOOL CClipNoteEnvDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	SetUI();

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���ø����̼��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CClipNoteEnvDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CClipNoteEnvDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClipNoteEnvDlg::OnBnClickedOk()
{
	// ���� ���α׷� ��� ����
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


	// �ֻ��� ����Ű
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
			// ������Ʈ�� ������ �����´�.
			// ������Ʈ�� ���ο� �� ��Ű ������ ����Ǿ� �ִ��� Ȯ���ϰ�,
			// ����Ǿ� �ִٸ� ���¸� UI�� ǥ���Ѵ�.

			int nMostTop = 1;
			RegWriter.Set(L"MostTop", nMostTop);


			int nValue = 0;

			nValue = pChkBtnTopCtrl->GetCheck();
			RegWriter.Set(L"MostTop_CTRL", nValue);

			nValue = pChkBtnTopCtrl->GetCheck();
			RegWriter.Set(L"MostTop_ALT", nValue);

			nValue = pChkBtnTopCtrl->GetCheck();
			RegWriter.Set(L"MostTop_WIN", nValue);


			CString strValue = L"";
			pCbBox->GetWindowText(strValue);
			RegWriter.Set(L"MostTop_SubKey", std::wstring(strValue));

		}
	}

	CDialogEx::OnOK();
}
