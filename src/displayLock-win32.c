// displayLock-win32.cpp : Defines the entry point for the application.
//

#include "header.h"
#include "displayLock-win32.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

Args *ComboBoxArgs;

Menu menu;
HWND hWndListBox;

// messages
HANDLE hMessageStop;
HANDLE hMessageEmpty;

HANDLE CLIP;

HANDLE mutex;

BOOL active;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    // initalize menu
    initMenu(&menu);
    hMessageStop = CreateEvent(NULL, FALSE, TRUE, _T("STOP"));
    hMessageEmpty = CreateEvent(NULL, FALSE, FALSE, _T("EMPTY"));
    SetEvent(hMessageEmpty);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DISPLAYLOCKWIN32, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DISPLAYLOCKWIN32));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DISPLAYLOCKWIN32));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DISPLAYLOCKWIN32);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));


    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    SetEvent(hMessageEmpty);
    Args *ComboBoxArgs = malloc(sizeof(Args));

    ComboBoxArgs->menu = &menu;
    ComboBoxArgs->hWnd = &hWndListBox;
    ComboBoxArgs->mutex = &mutex;

    winArgs *windowArgs = malloc(sizeof(winArgs));

    windowArgs->mutex = &mutex;
    windowArgs->active = &active;
    char selector[500];
    GetWindowTextA(hWndListBox, selector, 500);
    windowArgs->window = sortWindow(ComboBoxArgs, selector, menu.windows.count);


    switch (message)
    {
    case WM_CREATE:
    {


        // create dropdown menu
        hWndListBox = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            TEXT("COMBOBOX"),
            NULL,
            WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_AUTOHSCROLL | CBS_DISABLENOSCROLL | WS_VSCROLL,
            30,
            20,
            200,
            200,
            hWnd,
            (HMENU)IDC_DROPDOWNLIST,
            NULL,
            NULL);

        // start/top button
        HWND hwndButtonTranslate = CreateWindowEx(
            WS_EX_WINDOWEDGE,
            TEXT("BUTTON"),  // Predefined class; Unicode assumed 
            TEXT("Start"),      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            230,         // x position 
            230,         // y position 
            100,        // Button width
            35,        // Button height
            hWnd,     // Parent window
            (HMENU)IDC_OPBUTTON_START,       // No menu.
            NULL,
            NULL); // Pointer not needed.
        
                   // start/top button
        HWND hwndButtonStop = CreateWindowEx(
            WS_EX_WINDOWEDGE,
            TEXT("BUTTON"),  // Predefined class; Unicode assumed 
            TEXT("Stop"),      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            330,         // x position 
            230,         // y position 
            100,        // Button width
            35,        // Button height
            hWnd,     // Parent window
            (HMENU)IDC_OPBUTTON_STOP,       // No menu.
            NULL,
            NULL); // Pointer not needed.


        HANDLE handle = (HANDLE)_beginthread(updateComboBox, 0, (void*)ComboBoxArgs);

        
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDC_OPBUTTON_START:
            {
                if(!menu.active)
                {
                    WaitForSingleObject(&ComboBoxArgs->mutex, INFINITE);    // wait for mutex

                    ComboBoxArgs->mutex = CreateMutex(NULL, FALSE, NULL);   // lock the mutex

                    *(windowArgs->active) = TRUE;

                    //ComboBoxArgs->menu->selectedWindow = sortWindow(ComboBoxArgs, selector, menu.windows.count);
                    //menu.selectedWindow = sortWindow(ComboBoxArgs, selector, menu.windows.count);
                    ComboBoxArgs->menu->active = TRUE;
                    menu.active = TRUE;

                    windowArgs->window = sortWindow(ComboBoxArgs, selector, menu.windows.count);
                    active = TRUE;

                    if(windowArgs->window != NULL)
                    {
                        ReleaseMutex(&ComboBoxArgs->mutex);
                        CLIP = (HANDLE)_beginthreadex(NULL, 0, cursorLockEx, (void*)windowArgs, 0, NULL);
                        //CLIP = (HANDLE)_beginthread(cursorLock, 0, (void*)windowArgs);
                    }

                }
                
                break;
            }
            case IDC_OPBUTTON_STOP:
            {
                if (menu.active)
                {
                    menu.active = FALSE;
                    *(windowArgs->active) = FALSE;
                    active = FALSE;
                    ComboBoxArgs->mutex = CreateMutex(NULL, FALSE, NULL);   // lock the mutex
                    //SetEvent(hMessageStop);

                    ReleaseMutex(&ComboBoxArgs->mutex);
                    //WaitForSingleObject(CLIP, INFINITE);
                    ClipCursor(NULL);
                    CloseHandle(CLIP);
                }
                break;
            }
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                menu.live = FALSE;
                free(ComboBoxArgs);
                free(windowArgs);
                CloseHandle(CLIP);
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
