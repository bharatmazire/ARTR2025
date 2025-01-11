#include<windows.h>
#include<stdio.h>

#include"VulkanBase.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND ghwnd;
BOOL bIsFullScreen = FALSE;
DWORD dwStyle;
WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};
//WINDOWPLACEMENT wpPrev.length = sizeof(WINDOWPLACEMENT);

BOOL gbActiveWindow = FALSE;

FILE *gpFile = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    int initialize(void);
    void display(void);
    void update(void);
    void ToggleFullScreen(void);
    void uninitialize(void);

    MSG msg;
    WNDCLASSEX wndclass;
    TCHAR szAppName[] = TEXT("Vulkan Base");
    HWND hwnd;
    BOOL bDone = FALSE;
    int iRet = 0;

    gpFile = fopen("Log.txt","w");
    if(gpFile == NULL)
    {
        MessageBox(NULL, TEXT("Unable To Create Log File!!!"), TEXT("ERROR"), MB_OK|MB_ICONERROR);
        exit(0);
    }
    else
    {
        fprintf(gpFile, "Log.txt Created Successfully!!! \n");
        fflush(gpFile);
    }

    wndclass.cbClsExtra = 0;
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.cbWndExtra = 0;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
    wndclass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(MY_ICON));
    wndclass.hIconSm = LoadIcon(hInstance,MAKEINTRESOURCE(MY_ICON));
    wndclass.hInstance = hInstance;
    wndclass.lpfnWndProc = WndProc;
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName = NULL;
    wndclass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;

    if(!RegisterClassEx(&wndclass))
    {
        MessageBox(NULL, TEXT("ERROR : Unable To Register Class!!!"), TEXT("ERROR"), MB_OK|MB_ICONERROR);
        exit(0);
    }

    hwnd = CreateWindowEx(WS_EX_APPWINDOW,
                            szAppName,
                            TEXT("Vulkan Base Window"),
                            WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
                            (GetSystemMetrics(SM_CXSCREEN) - WIN_WIDTH)/2,
                            (GetSystemMetrics(SM_CYSCREEN) - WIN_HEIGHT)/2,
                            WIN_WIDTH,WIN_HEIGHT,
                            NULL,
                            NULL,
                            hInstance,
                            NULL);
    
    ghwnd = hwnd;

    iRet = initialize();
    
    ShowWindow(hwnd, iCmdShow);
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);
    //ToggleFullScreen();  // default fullscreen

    while(bDone == FALSE)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
            {
                bDone = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        /*
        else
        {
            if(gbActiveWindow == TRUE)
            {
                update();
            }
            display();
        }
        */   
    }

    uninitialize();

    return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    void ToggleFullScreen(void);
    void resize(int, int);
    void uninitialize(void);

    switch (iMsg)
    {
    case WM_CREATE:
        break;
    
    case WM_SETFOCUS:
        gbActiveWindow = TRUE;
        break;
    
    case WM_KILLFOCUS:
        gbActiveWindow = FALSE;
        break;

    case WM_SIZE:
        resize(LOWORD(lParam), HIWORD(lParam));
        break;
    
    case WM_ERASEBKGND:
        return(0);

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            DestroyWindow(hwnd);
            break;
        }
        break;

    case WM_CHAR:
        switch (wParam)
        {
        case 'F':
        case 'f':
            ToggleFullScreen();
            break;
        }
        break;

    case WM_DESTROY:
        uninitialize();
        PostQuitMessage(0);
        break;
    }

    return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullScreen(void)
{
    fprintf(gpFile, "IN: ToggleFullScreen()\n");
    fflush(gpFile);

    MONITORINFO mi;

    if(bIsFullScreen == FALSE)
    {
        dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
        if(dwStyle & WS_OVERLAPPEDWINDOW)
        {
            // mi = {sizeof(MONITORINFO)}; // This works with .cpp extension
            mi.cbSize = sizeof(MONITORINFO);
            if(GetWindowPlacement(ghwnd, &wpPrev) &&
                GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
            {
                SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
                SetWindowPos(ghwnd,
                            HWND_TOP,
                            mi.rcMonitor.left,
                            mi.rcMonitor.top,
                            (mi.rcMonitor.right - mi.rcMonitor.left),
                            (mi.rcMonitor.bottom - mi.rcMonitor.top),
                            SWP_NOZORDER | SWP_FRAMECHANGED);
            }
        }
        ShowCursor(FALSE);
        bIsFullScreen = TRUE;
    }
    else
    {
        SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(ghwnd, &wpPrev);
        SetWindowPos(ghwnd, HWND_TOP, 0,0,0,0, SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);
        ShowCursor(TRUE);
        bIsFullScreen = FALSE;
    }
    fprintf(gpFile, "OUT : ToggleFullScreen()\n");
    fflush(gpFile);
}

int initialize(void)
{
    return(0);
}

void resize(int iWidth, int iHeight)
{
    if(iHeight == 0)
        iHeight = 1;
}

void update(void)
{

}

void display(void)
{

}

void uninitialize(void)
{
    fprintf(gpFile, "IN : uninitialize()\n");
    fflush(gpFile);

    if(bIsFullScreen == TRUE)
    {
        SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(ghwnd, &wpPrev);
        SetWindowPos(ghwnd, HWND_TOP, 0,0,0,0, SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);
        ShowCursor(TRUE);
    }

    if(gpFile)
    {
        fprintf(gpFile, "Log File Closing...\n\n");
        fflush(gpFile);
        fclose(gpFile);
        gpFile = NULL;
    }
}
