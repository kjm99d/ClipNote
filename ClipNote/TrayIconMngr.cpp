#include "pch.h"
#include "TrayIconMngr.h"
#include "resource.h"
#include "WM_MSG_ID.h"

#define WM_TRAY_ICON_X (WM_USER + 1)

BEGIN_MESSAGE_MAP(CTrayIconMngr, CWnd)
    // ...
    ON_MESSAGE(WM_TRAY_ICON_X, OnTrayIcon)
END_MESSAGE_MAP()

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
    m_nid.uID = IDR_MAINFRAME; // << this
    m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    m_nid.uCallbackMessage = WM_TRAY_ICOM;
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
    //case WM_RBUTTONDOWN: // 오른쪽 버튼 클릭
        
    case WM_LBUTTONDBLCLK: // 왼쪽 버튼 더블 클릭
        // 프로그램 복원
        AfxGetMainWnd()->ShowWindow(SW_RESTORE);
        AfxGetMainWnd()->SetForegroundWindow();
        break;
    }

    return 0;
}
