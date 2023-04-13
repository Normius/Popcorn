// Popcorn.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Main.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

///////////////////MINE///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum EBrickColor
{
    NONE,
    PURPLE,
    BLUE
};

const unsigned int ResolutionScale = 3;
const unsigned int BrickWidth = 15;
const unsigned int BrickHeight = 7;
const unsigned int CellWidth = 16; //BrickWidth + 1 pxl for gorizontal space between bricks
const unsigned int CellHeight = 8; //Brickheight + 1 pxl for vertical space between bricks
const int LevelOffset_X = 8;
const int LevelOffset_Y = 6;

char LevelFirst[14][12]
{
    0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,
    1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
};
/////////////////////////////////////////////////////////////


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

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_POPCORN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_POPCORN));

    MSG msg;

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_POPCORN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(0, 0, 0));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_POPCORN);
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

   //////////////////////Mine///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Create a window with certain size (resolution), consider menu (only working window's space)
   RECT windowRect;
   windowRect.left = 0;
   windowRect.top = 0;
   windowRect.right = 320 * 3;
   windowRect.bottom = 200 * 3;

   AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, TRUE);

   ////////////////////////////////////////////////////////////////

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

////////////////MINE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Draw every frame in game

void DrawBrick(HDC hdc, int x, int y, char brickColor) //TO DO: change char param to ENUM ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{

    HPEN pen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));

    switch (brickColor)
    {
        case PURPLE:
            {
                pen = CreatePen(PS_SOLID, 0, RGB(255, 85, 255));
                brush = CreateSolidBrush(RGB(255, 85, 255));
            } 
            break;
        case BLUE:
            {
                pen = CreatePen(PS_SOLID, 0, RGB(85, 255, 255));
                brush = CreateSolidBrush(RGB(85, 255, 255));
            }
            break;
        default: //Nothing, empty
            return;
    }

    SelectObject(hdc, pen);
    SelectObject(hdc, brush);

    Rectangle(hdc, x * ResolutionScale, y * ResolutionScale, (x + BrickWidth) * ResolutionScale, (y + BrickHeight) * ResolutionScale);
}


void DrawFrame(HDC hdc)
{
    for (int i = 0; i < 14; ++i)
    { 
        for (int j = 0; j < 12; ++j)
        {
            DrawBrick(hdc, LevelOffset_X + j * CellWidth, LevelOffset_Y + i * CellHeight, LevelFirst[i][j]); //TO DO: Cast to ENUM or NOT? ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }
    }
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...

            DrawFrame(hdc);

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
