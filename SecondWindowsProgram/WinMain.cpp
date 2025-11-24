///
/// @file WinMain.cpp
/// @brief アプリケーションのエントリポイント
/// @author 齋藤啓大
///

#include "framework.h"
#include "Direct3D.h"
#include <string>
#include <d3d11.h>
#include "Camera.h"
#include "Input.h"
#include "RootJob.h"
#include "Screen.h"
#include <timeapi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "winmm.lib")

#define MAX_LOADSTRING 100

// ファイル内変数:
namespace
{
	HINSTANCE hInst;                                // 現在のインターフェイス
	WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
	WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名
	
	const wchar_t* WIN_CLASS_NAME = L"SampleGame";

	int winW;
	int winH;

	HWND hWnd = nullptr;
	RootJob* pRootJob{nullptr};
}


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

/// <summary>
/// エントリポイント
/// </summary>
/// <param name="hInstance">     インスタンスハンドル                           </param>
/// <param name="hPrevInstance"> Win16APIとの互換性のためのインスタンスハンドル </param>
/// <param name="lpCmdLine">     コマンドライン引数の文字列へのポインタ         </param>
/// <param name="nCmdShow">      当該プログラムをどういった形式で表示するか     </param>
/// <returns></returns>
int WINAPI wWinMain(_In_     HINSTANCE hInstance,
					_In_opt_ HINSTANCE hPrevInstance,
					_In_     LPWSTR    lpCmdLine,
					_In_     INT       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// グローバル文字列を初期化する
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_SECONDWINDOWSPROGRAM, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	RECT winRect = { 0, 0, Screen::WIDTH, Screen::HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	winW = winRect.right - winRect.left; // ウィンドウ幅
	winH = winRect.bottom - winRect.top; // ウィンドウ高

	// アプリケーション初期化の実行:
	if (not(InitInstance(hInstance, nCmdShow)))
	{
		return FALSE;
	}

	if (FAILED(Direct3D::Initialize(Screen::WIDTH, Screen::HEIGHT, hWnd)))
	{
		return -1;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SECONDWINDOWSPROGRAM));

	MSG msg{};
	//ZeroMemory(&msg, sizeof(msg));

	// カメラとかインプットとか
	Camera::Initialize();
	Input::Initialize(hWnd);

	pRootJob = new RootJob();
	pRootJob->Init();

	// メイン メッセージ ループ:
	while (msg.message != WM_QUIT) // ここでメッセージを取得
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))	//メッセージあり
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else//メッセージなし
		{
			static DWORD fps{0};

			timeBeginPeriod(1);
			static DWORD startTime = timeGetTime();
			DWORD nowTime = timeGetTime();
			static DWORD lastUpdateTime = nowTime;
			timeEndPeriod(1);

			if (nowTime - startTime >= 1000)
			{
				#if _DEBUG
				wchar_t str[16]{ L"\0"};
				wsprintf(str, L"%u", fps);
				SetWindowText(hWnd, str);
				#endif

				fps = 0;
				startTime = nowTime;
			}

			if (nowTime - lastUpdateTime <= 1000.f / 60)
			{
				continue;
			}
			lastUpdateTime = nowTime;

			fps++;

			pRootJob->UpdateSub();
			Input::Update();

			//描画処理
			Direct3D::BeginDraw();

			pRootJob->DrawSub();

			Direct3D::EndDraw();
		}
	}
	
	pRootJob->ReleaseSub();
	Input::Release();
	Direct3D::Release();
	return (int)msg.wParam;
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize        = sizeof(WNDCLASSEX); // この構造体のサイズ
	wcex.style         = NULL;               // クラスのスタイル
	wcex.lpfnWndProc   = WndProc;            // ウィンドウプロシージャへのポインタ
	wcex.cbClsExtra    = 0;                  // この構造体のパディング
	wcex.cbWndExtra    = 0;                  // インスタンスの末尾のパディング
	wcex.hInstance     = hInstance;          // 当該プログラムのハンドル
	wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SECONDWINDOWSPROGRAM)); // アイコンのハンドル
	wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);                                 // マウスカーソルへのハンドル
	wcex.hbrBackground = (HBRUSH)(6);                                                    // 背景ブラシのハンドル
	wcex.lpszMenuName  = NULL;               // メニュー
	wcex.lpszClassName = WIN_CLASS_NAME;                                                 // Windowクラスの名前
	wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));           // 小さいアイコンへのハンドル

	return RegisterClassExW(&wcex);
}

/// <summary>
/// InitInstance
/// インスタンス ハンドルを保存して、メイン ウィンドウを作成する
/// <remarks>
/// この関数で、グローバル変数でインスタンス ハンドルを保存し、
/// メイン プログラム ウィンドウを作成および表示します。
/// </remarks>
/// </summary>
/// <param name="hInstance">インスタンスハンドル</param>
/// <param name="nCmdShow">当該プログラムの表示形式</param>
/// <returns></returns>

/**
 * @brief 
 * @
 * @param hInstance 
 * @param nCmdShow 
 * @return 
 */
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   /***********************************************************
	        マクロ：CreateWindow
	***********************************************************

			lpClassName   Window Class Name
			lpWindowName  Window Title Name
			dwStyle       Window Stile
			x             Initial Pos (Horizontal)
			y             Initial Pos (Vertical)
			nWidth        Window Width
			nHeight       Window Height
			hWndParent    Parent Window Handle
			hMenu         Menu Bar Handle
			hInstance     Application Instance Handle
			lpParam       Pointer to CREATESTRUCT

			戻り値：
			Type : HWND
			Handle of Created Window

   ************************************************************/
   hWnd = CreateWindow(
          WIN_CLASS_NAME,
	      WIN_CLASS_NAME,
	      WS_OVERLAPPEDWINDOW,
	      CW_USEDEFAULT,
	      CW_USEDEFAULT,
	      winW,
	      winH,
	      nullptr,
	      nullptr,
	      hInstance,
	      nullptr);

   if (!hWnd)
   {
	  return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// メッセージループで取得したメッセージをここで処理
	switch (message)
	{
	case WM_MOUSEMOVE:
		Input::SetMousePosition({ static_cast<float>( LOWORD(lParam) ), static_cast<float>( HIWORD(lParam) ) });
		return S_OK; // breakじゃなくていいのか？

	case WM_COMMAND:
		// plane block
		{
			int wmId = LOWORD(wParam);
			// 選択されたメニューの解析:
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
		// plane block
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

// バージョン情報ボックスのメッセージ ハンドラ。
// これは別に使っていない -- 20250911
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
