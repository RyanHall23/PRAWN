// RawInputClient.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "RawInputClient.h"
#include "Persistence.h"
#include "InputManager.h"
#include "MenuCLI.h"
#include "MenuNavigation.h"

#include <windows.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <strsafe.h>

#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <limits>
#include <algorithm>

// #define _CRTDBG_MAP_ALLOC
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // Current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // The main window class name
                                                                                // TODO: Verify memory safety
std::unique_ptr<CInputManager> pInputManager(new CInputManager());              // Create safe smart pointer of InputManager class
std::unique_ptr<CPersistence> pPersistence  (new CPersistence());                             // Create safe smart pointer of Devices class
std::unique_ptr<CMenuCLI> pMainMenuCLI (new CMenuCLI());                        // Create safe smart pointer of MenuCLI class
std::unique_ptr<CMenuNavigation> pMenuNavigation(new CMenuNavigation());        // Create safe smart pointer of MenuCLI class

std::unique_ptr<CPersistence::DeviceProperties> pDeviceSettings(new CPersistence::DeviceProperties());

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void CreateConsole();
void DisableConsoleEcho(bool bRet);


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

    CreateConsole();
    ShowWindow(GetActiveWindow(), SW_HIDE);
    pDeviceSettings = pPersistence->ReadSettings();
    std::thread PurgeVehicle = std::thread([=] { pInputManager->PurgeVehicles(); });

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    //PurgeVehicle.join();
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
      CW_USEDEFAULT, 0, 200, 200, nullptr, nullptr, hInstance, nullptr);

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
        UINT dwSize = 0;

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

        UINT lSize = 0;
        GetRawInputDeviceInfo(raw->header.hDevice, RIDI_DEVICENAME, NULL, &lSize);
        LPCSTR dvcInfo = new char[lSize + 1];   // Add one to counter no null terminator
        GetRawInputDeviceInfo(raw->header.hDevice, RIDI_DEVICENAME, (LPVOID)dvcInfo, &lSize);

        HRESULT  hResult;
        TCHAR    szTempOutput[256];

        PRAWINPUTDEVICELIST pRawInputDeviceList;
        UINT  uiNumDevices = 0;
        UINT  cbSize = sizeof(RAWINPUTDEVICELIST);

        GetRawInputDeviceList(NULL, &uiNumDevices, cbSize);
        pRawInputDeviceList = (PRAWINPUTDEVICELIST)malloc(cbSize * uiNumDevices);
        GetRawInputDeviceList(pRawInputDeviceList, &uiNumDevices, cbSize);

        hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT(" DeviceList Num Devices %d \n"), uiNumDevices);

        if (SUCCEEDED(hResult))
        {
            // OutputDebugString(szTempOutput);            
        }
        else 
        {
            // TODO: write error handler
        }


        TCHAR* psDwType[] = {
            TEXT("RIM_TYPEMOUSE"),
            TEXT("RIM_TYPEKEYBOARD"),
            TEXT("RIM_TYPEHID")
        };

        for (unsigned int i = 0; i < uiNumDevices; i++) {
            UINT             cbDataSize = 1000;
            TCHAR* pType = TEXT("Unknown");
            RID_DEVICE_INFO  DevInfo = { 0 };
            char             pData[1000] = { 0 };

            // For each device get the device name and then the device information
            cbDataSize = sizeof(pData);
            GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice, RIDI_DEVICENAME, pData, &cbDataSize);
            DevInfo.cbSize = cbDataSize = sizeof(DevInfo);  // specify the buffer size
            GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice, RIDI_DEVICEINFO, &DevInfo, &cbDataSize);

            if (pRawInputDeviceList[i].dwType <= sizeof(psDwType) / sizeof(psDwType[0]))
                pType = psDwType[pRawInputDeviceList[i].dwType];

            hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT("Device Name = %s\n"), pData);
            if (SUCCEEDED(hResult))
            {
                // OutputDebugString(szTempOutput);
                std::string strTruncatedDeviceName = pPersistence->TruncateHIDName(pData);
                if (std::find(pDeviceSettings->m_vecstrAllDevices.begin(), pDeviceSettings->m_vecstrAllDevices.end(), strTruncatedDeviceName) == pDeviceSettings->m_vecstrAllDevices.end())
                {
                    pDeviceSettings->m_vecstrAllDevices.push_back(strTruncatedDeviceName);
                }
            }
            else 
            {
                // TODO: write error handler
            }

        }
        if (raw->header.dwType == RIM_TYPEKEYBOARD) // If keyboard input event
        {
            std::string strTruncatedDeviceName = pPersistence->TruncateHIDName(dvcInfo);    // Truncate device name to remove excess data

            if (std::find(pDeviceSettings->m_vecstrRegisteredDevices.begin(), pDeviceSettings->m_vecstrRegisteredDevices.end(), strTruncatedDeviceName) != pDeviceSettings->m_vecstrRegisteredDevices.end())
            {
                if (raw->data.keyboard.Flags == pPersistence->m_sKeyDownFlag)               // If keyboard flag is down
                {
                    unsigned char cTranslatedKey = (char)raw->data.keyboard.VKey;           // Converts Virtual Key to Numerical key, using an unsigned to char to avoid assertions with negative chars on isdigit & isalpha checks
                    pInputManager->InputDetected(strTruncatedDeviceName, cTranslatedKey); // Filter with inupt manager class
                }
            }
            else    // Handle keyboard press in CLI menu
            {
                if (raw->data.keyboard.Flags == pPersistence->m_sKeyDownFlag)               // If keyboard flag is down
                {
                    unsigned char cTranslatedKey = (char)raw->data.keyboard.VKey;           // Converts Virtual Key to Numerical key, using an unsigned to char to avoid assertions with negative chars on isdigit & isalpha checks
                    pMenuNavigation->BuildCommand(cTranslatedKey);                          // Filter with inupt manager class
                    pMenuNavigation->RegisterNavigationDevice(strTruncatedDeviceName);
                }
            }
        }

        delete[] lpb;                   // Delete LPByte object
        delete[] dvcInfo;               // Delete dvcInfo 

        InvalidateRect(hWnd, NULL, TRUE);	// Clear Window
        InvalidateRect(hWnd, NULL, NULL);	// Update Window

        // _CrtDumpMemoryLeaks();
    }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
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

/// <summary>
/// Disable echo output in console window
/// Key presses are handled using raw input API
/// </summary>
/// <param name="bDisable"></param>
void DisableConsoleEcho(bool bDisable)
{
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);

    bDisable? mode &= ~ENABLE_ECHO_INPUT  : mode |= ENABLE_ECHO_INPUT;

    SetConsoleMode(hStdin, mode);
}

/// <summary>
/// Creates console window
/// Disables Echo & prints main menu
/// </summary>
void CreateConsole()
{
    if (!AllocConsole())
    {
        return;
    }

    // std::cout, std::clog, std::cerr, std::cin
    FILE* fDummy;
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();

    // std::wcout, std::wclog, std::wcerr, std::wcin
    HANDLE hConOut = CreateFile(_T("CONOUT$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hConIn = CreateFile(_T("CONIN$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
    SetStdHandle(STD_ERROR_HANDLE, hConOut);
    SetStdHandle(STD_INPUT_HANDLE, hConIn);
    std::wcout.clear();
    std::wclog.clear();
    std::wcerr.clear();
    std::wcin.clear();

    DisableConsoleEcho(true);                       // Disable echoing in console
    pMainMenuCLI->PrintMainMenu();
}