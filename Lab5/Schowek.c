#include <windows.h>


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc;

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInst,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }
    ShowWindow(hwnd, cmdline);

    // Run the message loop.

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndNxtVwr;
    switch (uMsg)
    {
    case WM_CREATE:
        hwndNxtVwr = SetClipboardViewer(hwnd);
        return 0;
    case WM_DESTROY:
        ChangeClipboardChain(hwnd, hwndNxtVwr);
        PostQuitMessage(0);
        return 0;
    case WM_CHANGECBCHAIN:
        if (wParam == hwndNxtVwr) /* jestem poprzednikiem usuwajacego sie ? */
        {
            hwndNxtVwr = LOWORD(lParam); /* handle nowego nastepnika */
        }
        else
        {
            if (hwndNxtVwr != NULL) /* jesli nie jestem ostatni w lancuchu to */
            { /* musze powiadomic mojego nastepnika */
                SendMessage(hwndNxtVwr, uMsg, wParam, lParam);
            }
        }
        return 0;
    case WM_DRAWCLIPBOARD:
        if (hwndNxtVwr != NULL)
            /* jesli nie jestem ostatni w lancuchu to musze powiadomic nastepny CbV */
        {
            SendMessage(hwndNxtVwr, uMsg, wParam, lParam);
        }
        OpenClipboard(hwnd);
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (hData == NULL)
            return -1;
        LPSTR* data = GlobalLock(hData);
        //MessageBoxA(0, data, "", 0);
        if (strlen(data) != 28) {
            CloseClipboard();
            return 0;
        }
        const char* output = "Nie wklejaj";
        const size_t len = strlen(output) + 1;
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
        memcpy(GlobalLock(hMem), output, len);
        GlobalUnlock(hMem);
        
        EmptyClipboard();
        SetClipboardData(CF_TEXT, hMem);
        CloseClipboard();
        InvalidateRect(hwnd, NULL, TRUE); /* wygeneruje WM_PAINT */
        return 0;
        /*
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }*/
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}