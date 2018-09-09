#pragma once

#include "resource.h"
#include "win.h"

// custom messages
#define RESET_SETTINGS (WM_USER + 0x1)

typedef struct MAIN_WINDOW_CONTROLS
{
    HWND tabCtrl;
    HWND windowView;
    HWND settingsView;
    // HWND monitorView;
} MAIN_WINDOW_CONTROLS;

typedef struct WINDOW_VIEW_CONTROLS
{
    HWND comboBox;
    HWND startButton;
    HWND stopButton;
    WINDOWLIST windows;

    volatile BOOL runningClip;
    HANDLE clipThread;
} WINDOW_VIEW_CONTROLS;

typedef struct SETTINGS_VIEW_CONTROLS
{
    BOOL settingsChanged;

    HWND minimize;
    HWND foreground;
    HWND borderless;
    HWND fullScreen;
} SETTINGS_VIEW_CONTROLS;

typedef struct PREVIOUSRECT
{
    int width;
    int height;
    int x;
    int y;
} PREVIOUSRECT;

typedef struct WINDOW
{
    char title[500];
    int x;
    int y;
    RECT size;
    BOOL selected;
    HWND hWnd;
} WINDOW;

typedef struct WINDOWLIST
{
    int count;
    WINDOW windows[35];
} WINDOWLIST;