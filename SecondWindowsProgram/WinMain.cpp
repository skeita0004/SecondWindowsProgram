///
/// @file WinMain.cpp
/// @brief アプリケーションのエントリポイント
/// @author 齋藤啓大及び東北電子の教員
///

#include "framework.h"
#include "SecondWindowsProgram.h"
#include "Direct3D.h"
#include <string>

#include <d3d11.h>

#include "Quad.h"
#include "Dice.h"
#include "Camera.h"

#pragma comment(lib, "d3d11.lib")

#define MAX_LOADSTRING 100

// グローバル変数:
namespace
{
	HINSTANCE hInst;                                // 現在のインターフェイス
	WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
	WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名
	
	const wchar_t* WIN_CLASS_NAME = L"SampleGame";

	const int WINDOW_WIDTH  = 800;
	const int WINDOW_HEIGHT = 600;

	int winW;
	int winH;

	HWND hWnd = nullptr;

	Quad* quad{ nullptr };
	Dice* dice{ nullptr };
	Dice* dice2{ nullptr };

	std::vector<Dice*> dices(0, nullptr);
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
int __stdcall wWinMain(_In_ HINSTANCE hInstance,
					 _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPWSTR    lpCmdLine,
					 _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// グローバル文字列を初期化する
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_SECONDWINDOWSPROGRAM, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	winW = winRect.right - winRect.left; // ウィンドウ幅
	winH = winRect.bottom - winRect.top; // ウィンドウ高

	// アプリケーション初期化の実行:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	if (FAILED(Direct3D::Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, hWnd)))
	{
		return -1;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SECONDWINDOWSPROGRAM));

	MSG msg{};
	//ZeroMemory(&msg, sizeof(msg));
	dice = new Dice(3.0f, { 0, 0, 0, 0 });
	dice2 = new Dice(1.0f, { 2, 0, 3, 0 });
	float deg = 0.f;
	float deg2 = 0.f;
	//if (FAILED(/*quad->Initialize()*/))
	//{
	//	return -1;
	//}
	//quad->Initialize();
	dice->Initialize();
	dice2->Initialize();

	Camera::Initialize();
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
			//描画処理
			Direct3D::BeginDraw();

			//ゲームの処理
			Camera::Update();

			deg += 0.1f;
			deg2 += 0.05f;
			
			if (deg >= 360)
			{
				deg = 0.f;
			}
			
			XMMATRIX rotXmat = XMMatrixRotationX(XMConvertToRadians(45));
			XMMATRIX rotYmat = XMMatrixRotationY(XMConvertToRadians(deg));
			XMMATRIX rotZmat = XMMatrixRotationZ(XMConvertToRadians(45));

			//quad->Draw(rotYmat * rotXmat);
			//quad->Draw(rotYmat);
			//dice->Draw(rotXmat * rotZmat * rotYmat);
			dice->Draw(rotYmat);
			//dice2->Draw(rotXmat * rotZmat * rotYmat);

			//XMMATRIX mat = XMMatrixRotationY(XMConvertToRadians(-90));
			//quad->Draw(mat);

			Direct3D::EndDraw();
		}
	}

	//quad->Release();
	dice->Release();
	dice2->Release();
	SafeDelete(dice2);
	SafeDelete(dice);
	
	//SafeDelete(quad);

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

   //
   // CreateWindowW
   // 
   // マクロ
   // 
   // 引数：
   //   lpClassName
   //       Windowクラスの名前
   // 
   //   lpWindowName
   //       Windowの名前
   // 
   //   dwStyle
   //       Windowのスタイル
   // 
   //   x
   //       初期位置（水平）
   // 
   //   y
   //       初期位置（垂直）
   // 
   //   nWidth
   //       ウィンドウの幅
   // 
   //   nHeight
   //       ウィンドウの高さ
   // 
   //   hWndParent
   //       親ウィンドウ
   //   
   //   hMenu
   //       メニュー
   // 
   //   hInstance
   //       インスタンスハンドル
   // 
   //   lpParam
   //       CREATESTRUCT構造体へのポインタ
   // 
   // 戻り値：
   //   作成されたウィンドウへのハンドル
   //
   /***********************************************************
	        マクロ：CreateWindowW
	***********************************************************

			引数：
			lpClassName
			lpWindowName
			dwStyle


   ************************************************************/
   hWnd = CreateWindowW(WIN_CLASS_NAME, WIN_CLASS_NAME,
	      WS_OVERLAPPEDWINDOW,
	      CW_USEDEFAULT,
	      0,
	      winW, winH,
	      nullptr, nullptr,
	      hInstance, nullptr);

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
	// 53行目で取得したメッセージをここで処理
	switch (message)
	{
	case WM_COMMAND:
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
