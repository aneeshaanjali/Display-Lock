#pragma once

#include <Windows.h>
#include "../../Display-Lock/resources/resource.h"

#define WINDOW_VIEW 0
#define SETTINGS_VIEW 1

// custom messages

#define NOTIFY_MSG (WM_USER + 0x1)

// HOTKEY MESSAGES
#define START_STOP 1


typedef struct MAIN_WINDOW_CONTROLS MAIN_WINDOW_CONTROLS;
typedef struct WINDOWLIST WINDOWLIST;
typedef struct WINDOW_VIEW_CONTROLS WINDOW_VIEW_CONTROLS;
typedef struct SETTINGS_VIEW_CONTROLS SETTINGS_VIEW_CONTROLS;
typedef struct PREVIOUSRECT PREVIOUSRECT;
typedef struct MENU MENU;
typedef struct SETTINGS SETTINGS;
typedef struct ARGS ARGS;
typedef struct HOTKEY HOTKEY;

struct MAIN_WINDOW_CONTROLS
{
    HWND tabCtrl;
    HWND windowView;
    HWND settingsView;
};

typedef struct WINDOW
{
    char title[500];
    int x;
    int y;
    RECT size;
    BOOL selected;
    HWND hWnd;
} WINDOW;

struct WINDOWLIST
{
    int count;
    WINDOW windows[35];
};

struct WINDOW_VIEW_CONTROLS
{
    HWND comboBox;
    HWND startButton;
    HWND stopButton;
    WINDOWLIST windows;

    volatile BOOL *runningClip;
    HANDLE clipThread;
};

struct SETTINGS_VIEW_CONTROLS
{
    BOOL settingsChanged;

    HWND minimize;
    HWND foreground;
    HWND borderless;
    HWND fullScreen;
    HWND hotkey;
};

struct PREVIOUSRECT
{
    int width;
    int height;
    int x;
    int y;
};


struct MENU
{
    void (*closeThread)(HANDLE thread, volatile BOOL *status);
    void (*updateComboBox)(HWND control, WINDOWLIST *windows, void(*callback)(WINDOWLIST*));
    BOOL(*startThread)(HANDLE *thread, int(*callback)(void* parameters), void* args);
};

struct HOTKEY
{
    int id;
    int modifiers;
    unsigned int key;
};

struct SETTINGS
{
    char header[6];
    int version;
    BOOL minimize;
    BOOL foreground;
    BOOL borderless;
    BOOL fullScreen;
    int numOfHotkeys;
    HOTKEY hotkeys[1];
};

struct ARGS
{
    SETTINGS *settings;
    volatile BOOL *clipRunning;
    WINDOW selectedWindow;
};

