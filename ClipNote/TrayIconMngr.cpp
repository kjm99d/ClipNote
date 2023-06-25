#include "pch.h"
#include "TrayIconMngr.h"
#include "resource.h"
#include "WM_MSG_ID.h"


CTrayIconMngr::CTrayIconMngr(CWnd * parent) 
    : CDialogEx(IDD_CLIPNOTE_DIALOG, parent)
{
}

CTrayIconMngr::~CTrayIconMngr()
{
}

BOOL CTrayIconMngr::Create()
{
    m_nid.cbSize = sizeof(NOTIFYICONDATA);
    m_nid.hWnd = AfxGetMainWnd()->GetSafeHwnd();
    m_nid.uID = IDD_CLIPNOTE_DIALOG; // << this
    m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    m_nid.uCallbackMessage = WM_TRAY_ICON;
    m_nid.hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME);
    _tcscpy_s(m_nid.szTip, _T("ClipNote"));

    return Shell_NotifyIcon(NIM_ADD, &m_nid);

}

BOOL CTrayIconMngr::Destroy()
{
    // 트레이 아이콘 삭제
    return Shell_NotifyIcon(NIM_DELETE, &m_nid);
}

LRESULT CTrayIconMngr::OnTrayIcon(WPARAM wParam, LPARAM lParam)
{
    // WM_TRAY_ICON 메시지 처리
    switch (lParam)
    {
    case WM_RBUTTONDOWN: // 오른쪽 버튼 클릭
    {
        CPoint pt;
        GetCursorPos(&pt);

        CMenu menu;
        VERIFY(menu.LoadMenu(IDR_MENU2));

        CMenu* pSubMenu = menu.GetSubMenu(0);
        ASSERT(pSubMenu != NULL);

        pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
    }
    break;

    case WM_LBUTTONDBLCLK: // 왼쪽 버튼 더블 클릭
    {
        OutputDebugString(L"WM_LBUTTONDBLCLK \n");
        AfxGetMainWnd()->ShowWindow(SW_SHOW);
        AfxGetMainWnd()->SetForegroundWindow();
    }
    break;


    default:
    {
        switch (LOWORD(wParam))
        {
        case ID_TRAY_EXIT:
            // 종료 버튼 클릭 시 어플리케이션 종료
            if (MessageBox(_T("정말 종료하시겠습니까?"), _T("종료"), MB_YESNO | MB_ICONQUESTION) == IDYES)
            {
                PostMessage(WM_CLOSE);
            }
            break;

        case ID_ENV_TOOL:
            MessageBox(L"Message");
            break;
        }
    }
    break;

    } // end


    return 0;
}

void CTrayIconMngr::OnTrayExit()
{
    if (MessageBox(_T("정말 종료하시겠습니까?"), _T("종료"), MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        PostMessage(WM_CLOSE);
    }
    
}

void CTrayIconMngr::OnTrayEnv()
{
    const std::wstring strEnvPath = CSDKCommon::GetEnvToolPath();
    CSDKCommon::RunProgram(strEnvPath, L"", TRUE);
}

