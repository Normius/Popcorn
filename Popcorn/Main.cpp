// Popcorn.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Main.h"

#define MAX_LOADSTRING 100

// Global Variables:
CGameEngine GameEngine;
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	CConfig::CreatePenAndBrush(CConfig::backgroundColor, CConfig::backgroundPen, CConfig::backgroundBrush);

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_POPCORN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_POPCORN));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CConfig::backgroundBrush;
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_POPCORN);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////////////////////MINE///////////////////////////////////////////////////////////////////////
	GameEngine.InitGameEngine(hWnd);

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
	case WM_PAINT:
	{
		//////////////////////////////////////////////////////////////////////////////////////////MINE///////////////////////////////////////////////////////////////////////
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...

		GameEngine.DrawFrame(hdc, ps.rcPaint);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		//////////////////////////////////////////////////////////////////////////////////////////MINE///////////////////////////////////////////////////////////////////////
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			return GameEngine.OnKeyDown(LeftArrowKey);

		case VK_RIGHT:
			return GameEngine.OnKeyDown(RightArrowKey);

		case VK_SPACE:
			return GameEngine.OnKeyDown(SpaceKey);
		}
		break;

	case WM_TIMER:
		if (wParam == TimerID)
		{
			return GameEngine.On_Timer();
		}
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


/*

Game over when losing ball
DONE! 1. Platform meltdown when lose (animation)  
---------------------------------------------------------------------------------
2. Platform appearance at the start (rolling out animations) 
3. Platform appearance at the start (extension animations) 
4. Ball start and ball states (function)

Bricks
1. Simple bricks (purle or blue color)
	1.1. It might has or has not a letter
	1.2. Dissapearing when ball hits it
		1.2.1. Create active (fading) brick if it does not have letter
		1.2.2. Create falling brick with letter if it has letter
2. Unbreakable bricks (never breaks)
	2.1.  Blink animations when ball hits it
3. Multi-hit bricks (breaks when ball hits it and 1 life left, gives bonus score)
	3.1. Has 1-4 states (lifes)
	3.2. State changing when ball hits it
	3.3. Bonus score animation when ball break it (1 life left)
4. Parachute brick (gives falling bonus with extra ball when breaks)
	4.1. Parachure animation when ball hits it
5. Portal bricks (teleportate ball itself when ball hits it)
	5.1. Ball teleportation animation
6. Advertisment hiding  bricks (hide picture-advertisment behind themselves)
	6.1. Adverisment animation or its open fragment, when ball hits brick

Special bricks interactions
1. Falling letter can interact with platform only
2. Active brick (with letter) interact with ball only

Letters action-list:
1. Simple actions:
	1.1. A (abandon) - cancel effects of C, E, F, L, M letters
	1.2. I (inversion) - ball change vertical direction by opposite of current
	1.3. S (slow) - ball starts moving slower
	1.4. M (monsters) - pause current monsters and prevent appearance of new ones for a while
	1.5. V (vie - FR. means life) - gives extra life
2. Complex actions:
	2.1. C (catch) - (catch) platform catches the ball (GLUED to platform), ball doesn't bounce by itself, launches only by player
		2.1.1. New platform state
		2.1.2. Glue animation on platform (appearing and dissapearing)
		2.1.3. Ball fixating on platform (new ball state)
		2.1.4. Ball launch by special key ("space") or by timeout
	2.2. E (expand) - expand platform, make it longer. Effect cancels when platform cath other letter
		2.2.1. New platform state (platform expansion)
		2.2.2. Animtaion
	2.3. F (floor) - create a net-floor at the bottom, so ball bounce out if it, can't fall
		2.3.1. Display net-floor
		2.3.2. Floor duration slace on interface
		2.3.3. Ball interaction with net-floor
		2.3.4. Ball bounce out of net-floor
	2.4. L (laser) - create a laser-guns on platform, it can shoot bricks
		2.4.1. New platform state
		2.4.2. Laser-guns creating animation
		2.4.3. Laser shoot by "space" key ("space" key handling, it usually launch the ball)
		2.4.4. Create laser bullet objects. Animation, interactions with other objects
	2.5. T (three, 3) - gives two extra balls. Extra balls appear out of main ball (three summary)
		2.5.1. Handle multiple ball 
		2.5.2. Display multiple ball 
		2.5.3. Multiple ball interactions with other objects
	2.6. "+" (plus) - Moving to the next level
		2.6.1. Moving to the next level animation
		2.6.2. New special level for moving to the next level

 Ball movement.
 1. Ball bounce out of borders (function)
 2. Ball bounce out of round platform edges
 3. Ball bounce depends of platform movement
 4. Hitting a monster

 Hitting a brick
 1. Brick states
 2. Falling letter out of brick
 3. Platform catches falling letter
 4. Set of letters on falling bricks
 5. Various actions depending on the letter

 Gates on borders
 1. Display gates
 2. Gates animations
 3. Gate states

 Monsters
 1. Monsters list
	1.1. Eye
	1.2. Turtle
	1.3. Saturn
	1.4. Glasses head
	1.5. Aquarium
	1.6. Lighter
 2. Monsters movement and movement directions
 3. Monsters interactions with platform and ball and states
 4. Monsters appearance out of gates
 5. Monsters animations 

 Game interface
 1. Logo
 2. Player name
 3. Scoreboard
 4. Monsters pause scale and flore appearance scale (effects duration)
 5. Change scales (animation)
 6. Window with bonus lifes
 7. Game actions registration and display on interface

 Game and levels
 Levels list (first 10 levels)
 Game states(start screen, level start animation, game process, life loss, starting next level (simple and special), game over


*/



