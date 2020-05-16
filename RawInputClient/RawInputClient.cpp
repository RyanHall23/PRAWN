// RawInputClient.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "RawInputClient.h"
#include "Devices.h"
#include "DeviceProperties.h"
#include "Registration.h"

#include <windows.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <CommCtrl.h>

#include <iostream>
#include <fstream>
#include <strsafe.h>
#include <string>

#define _CRTDBG_MAP_ALLOC
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // Current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // The main window class name

BOOL bConfigRead = FALSE;

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

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RAWINPUTCLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    RAWINPUTDEVICE rID[1];

    rID[0].usUsagePage = 0x01;
    rID[0].usUsage = 0x06; // HID keyboard
    rID[0].dwFlags = RIDEV_INPUTSINK;
    rID[0].hwndTarget = GetActiveWindow();

    if (RegisterRawInputDevices(rID, 1, sizeof(rID[0])) == FALSE)
    {
        return -1;
    }

    MSG msg = { 0 };

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAWINPUTCLIENT));

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAWINPUTCLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_RAWINPUTCLIENT);
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
//        create and display the main program window, also builds GUI components that are constant.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   HWND hwndParent = hWnd; // Handle to the parent window

   int xpos = 50;            // Horizontal position of the window.
   int ypos = 50;            // Vertical position of the window.
   int nwidth = 200;         // Width of the window
   int nheight = 200;        // Height of the window


   HWND hWndComboBoxScannerA = CreateWindow(WC_COMBOBOX, TEXT(" & "),
       CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
       xpos, ypos, nwidth, nheight, hwndParent, NULL, hInst, NULL);

   ypos = 100;            // Vertical position of the window.

   HWND hWndComboBoxScannerB = CreateWindow(WC_COMBOBOX, TEXT(" & "),
       CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
       xpos, ypos, nwidth, nheight, hwndParent, NULL, hInst, NULL);


   if (!hWnd & !hWndComboBoxScannerA & !hWndComboBoxScannerB)
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
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_INPUT:  // HID Input Intercept
    {
        UINT dwSize;

        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
        LPBYTE lpb = new BYTE[dwSize];
        if (lpb == NULL)
        {
            return 0;
        }

        if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
        {
            OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));
        }

        RAWINPUT* raw = (RAWINPUT*)lpb;

        UINT lSize;
        GetRawInputDeviceInfo(raw->header.hDevice, RIDI_DEVICENAME, NULL, &lSize);
        LPCSTR dvcInfo = new char[lSize + 1];   // Add one to counter no null terminator
        GetRawInputDeviceInfo(raw->header.hDevice, RIDI_DEVICENAME, (LPVOID)dvcInfo, &lSize);

        if (raw->header.dwType == RIM_TYPEKEYBOARD) // If keyboard input event
        {
            if (!bConfigRead)
            {
                std::unique_ptr<CDeviceProperties> pDeviceProperties(new CDeviceProperties()); // Create smart pointer of DeviceProperties class // TODO: Make Singleton

                pDeviceProperties->ReadDeviceProperties();

                bConfigRead = TRUE;
            }

            std::unique_ptr<CDevices> pDevices(new CDevices());                   // Create smart pointer of Devices class
            std::unique_ptr<CRegistration> pRegistration(new CRegistration());    // Create smart pointer of Registration class // TODO: Make Singleton

            if (raw->data.keyboard.Flags == pDevices->m_sKeyDownFlag)  // If flag is down
            {
                std::string strDeviceName = pDevices->TruncateHIDName(dvcInfo);      // Truncate device name to remove excess data

                unsigned char cTranslatedKey = (char)raw->data.keyboard.VKey;       // Converts Virtual Key to Numerical key, using an unsigned to char to avoid assertions with negative chars on isdigit & isalpha checks
                std::string strCurrentKey;
                std::string regPlate;

                // TODO: Handling and registration building

                if (isdigit(cTranslatedKey) || isalpha(cTranslatedKey))             // Check for valid alphanumeric key
                {
                    strCurrentKey = std::toupper(cTranslatedKey);                   // Convert to Upper case to avoid mixing cases
                    regPlate = pRegistration->BuildRegistration(strCurrentKey);
                }
                else if (cTranslatedKey == pDevices->m_cReturnKeyVirtualCode)       // Check if Return Key/End of string key has been entered
                {
                    strCurrentKey = pDevices->m_strReturnKeyMessage;                // Mark with '!'
                    regPlate = pRegistration->BuildRegistration(strCurrentKey);

                    #ifndef DEBUG
                    OutputDebugString((LPCSTR)regPlate.c_str());   // Debug output built registration 
                    OutputDebugString(" ");

                    #endif
                }

                #ifndef DEBUG
                OutputDebugString((LPCSTR)strDeviceName.c_str());   // Debug output device name
                OutputDebugString(" ");

                OutputDebugString((LPCSTR)strCurrentKey.c_str());   // Debug output entered key
                OutputDebugString("\n");
                #endif
            }
        }

        delete[] lpb;                   // Delete LPByte object
        delete[] dvcInfo;               // Delete dvcInfo 

        InvalidateRect(hWnd, NULL, TRUE);	// Clear Window
        InvalidateRect(hWnd, NULL, NULL);	// Update Window

        _CrtDumpMemoryLeaks();
    }
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
