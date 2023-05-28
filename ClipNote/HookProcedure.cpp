#include "pch.h"
#include "HookProcedure.h"

HookProcedure::HookProcedure() : hHook_Msg(nullptr)
{
}

HookProcedure::~HookProcedure()
{
    UnHook();
}

BOOL HookProcedure::SetHook()
{
    hHook_Msg = SetWindowsHookEx(WH_KEYBOARD_LL, CallWndProc, NULL, 0);

    return nullptr != hHook_Msg;
}

BOOL HookProcedure::UnHook()
{
    if (nullptr != hHook_Msg && FALSE == UnhookWindowsHookEx(hHook_Msg))
    {
        OutputDebugString(_T("HookProcedure::UnHook Failed KeybardHook"));
        return FALSE;
    }

    return TRUE;
}

LRESULT HookProcedure::CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0) 
    {
        // 키보드 이벤트가 발생한 경우
        CWPSTRUCT* pCwpStruct = reinterpret_cast<CWPSTRUCT*>(lParam);
        //if (wParam == WM_KEYDOWN || wParam == WM_COPY)

        if (pCwpStruct->message == WM_DRAWCLIPBOARD)
        {
            OutputDebugString(_T("Try DRAWC Copy"));

        }

        if (pCwpStruct->message == WM_COPY)
        {
                // Ctrl + C가 눌린 경우, 클립보드에서 데이터 취득
                if (OpenClipboard(NULL))
                {
                    OutputDebugString(_T("Try Clipboard Copy"));
                    if (IsClipboardFormatAvailable(CF_TEXT) || IsClipboardFormatAvailable(CF_UNICODETEXT))
                    {
                        HANDLE hClipboardData = GetClipboardData(CF_TEXT);
                        char* pszData = (char*)GlobalLock(hClipboardData);

                        char szMsg[1024];
                        SecureZeroMemory(szMsg, sizeof(szMsg));
                        sprintf_s(szMsg, _countof(szMsg), "HANDLE[%d] Data[%s] \n", hClipboardData, pszData);

                        OutputDebugStringA(szMsg);
                        //std::cout << "복사된 내용: " << pszData << std::endl;
                        GlobalUnlock(hClipboardData);
                    }
                    else if (IsClipboardFormatAvailable(CF_BITMAP) || IsClipboardFormatAvailable(CF_DIB))
                    {
                        OutputDebugString(_T("Capture Image \n"));

                        HANDLE hClipboardData = GetClipboardData(CF_BITMAP);
                        HBITMAP hBitmap = (HBITMAP)hClipboardData;

                        // 이미지 처리 로직
                        // 예: 파일로 저장
                        BITMAP bmp;
                        GetObject(hBitmap, sizeof(BITMAP), &bmp);
                        int width = bmp.bmWidth;
                        int height = bmp.bmHeight;

                        HDC hdcScreen = GetDC(NULL);
                        HDC hdcMem = CreateCompatibleDC(hdcScreen);
                        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

                        // 이미지를 파일로 저장
                        // 예: BMP 파일로 저장
                        BITMAPFILEHEADER fileHeader;
                        BITMAPINFOHEADER infoHeader;
                        DWORD imageSize = width * height * 3;
                        DWORD fileSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + imageSize;

                        memset(&fileHeader, 0, sizeof(BITMAPFILEHEADER));
                        memset(&infoHeader, 0, sizeof(BITMAPINFOHEADER));

                        fileHeader.bfType = 0x4D42; // 'BM'
                        fileHeader.bfSize = fileSize;
                        fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

                        infoHeader.biSize = sizeof(BITMAPINFOHEADER);
                        infoHeader.biWidth = width;
                        infoHeader.biHeight = height;
                        infoHeader.biPlanes = 1;
                        infoHeader.biBitCount = 24;
                        infoHeader.biCompression = BI_RGB;

                        std::string fileName = "clipboard_image.bmp";
                        HANDLE hFile = CreateFileA(fileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                        if (hFile != INVALID_HANDLE_VALUE)
                        {
                            DWORD bytesWritten = 0;
                            WriteFile(hFile, &fileHeader, sizeof(BITMAPFILEHEADER), &bytesWritten, NULL);
                            WriteFile(hFile, &infoHeader, sizeof(BITMAPINFOHEADER), &bytesWritten, NULL);
                            WriteFile(hFile, bmp.bmBits, imageSize, &bytesWritten, NULL);

                            CloseHandle(hFile);
                        }
                    }

                    CloseClipboard();
                }
            
        }
    }

    // 다음 후킹 함수 호출
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

