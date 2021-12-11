
#include "GameManager.h"
#include "Timer.h"
// Stage
#include "Stage/Stage.h"
#include "Stage/BasicStage.h"
#include "Stage/StageManager.h"
// Input 
#include "Input.h"
// Path
#include "Path/PathManager.h"
// Resource
#include "Resource/ResourceManager.h"

CGameManager* CGameManager::m_Inst = nullptr;

bool CGameManager::m_Loop = true;

CGameManager::CGameManager() :
	m_Timer(nullptr),
	m_TimeScale(1.f),
	m_EditorMode(false),
	m_RS{},
	m_hBackBmp{},
	m_hBackDC{},
	m_hDC{},
	m_hGreenBrush{},
	m_hGreenPen{},
	m_hBluePen{},
	m_hDarkBlueBrush{},
	m_hRedBrush{},
	m_hRedPen{},
	m_hBlueBrush{},
	m_hLightBlueBrush{},
	m_MasterVolume(1.f),
	m_BGVolume(10),
	m_EffectVolume(10)
{
	// init commit 
}

CGameManager::~CGameManager()
{
	CInput::DestroyInst();
	CResourceManager::DestroyInst();
	CPathManager::DestroyInst();
	CStageManager::DestroyInst();

	SAFE_DELETE(m_Timer);

	SelectObject(m_hBackDC, m_hPrevBackBmp);

	DeleteObject(m_hBackBmp);
	DeleteDC(m_hBackDC);
	DeleteObject(m_hGreenBrush);
	DeleteObject(m_hRedBrush);
	DeleteObject(m_hBlueBrush);
	DeleteObject(m_hLightBlueBrush);
	DeleteObject(m_hDarkBlueBrush);
	DeleteObject(m_hGreenPen);
	DeleteObject(m_hRedPen);
	DeleteObject(m_hBluePen);

	ReleaseDC(m_hWnd, m_hDC);
}

void CGameManager::SetTimeScale(float TimeScale)
{
	m_TimeScale = TimeScale;
}

float CGameManager::GetTimeScale() const
{
	return m_TimeScale;
}

float CGameManager::GetDeltaTime() const
{
	return m_Timer->GetDeltaTime() * m_TimeScale;
}

void CGameManager::Exit()
{
	DestroyWindow(m_hWnd);
}

bool CGameManager::Init(HINSTANCE hInst)
{
	m_hInst = hInst;

	m_RS.Width = 1280;
	m_RS.Height = 720;

	Register();
	Create();

	m_hDC = GetDC(m_hWnd);

	if (!CPathManager::GetInst()->Init())
		return false;

	if (!CResourceManager::GetInst()->Init())
		return false;

	if (!CInput::GetInst()->Init(m_hWnd))
		return false;

	if (!CStageManager::GetInst()->Init())
		return false;

	CStageManager::GetInst()->CreateStage<CBasicStage>();

	m_Timer = new CTimer;

	m_hBackDC = CreateCompatibleDC(m_hDC);

	m_hBackBmp = CreateCompatibleBitmap(m_hDC, m_RS.Width, m_RS.Height);

	m_hPrevBackBmp = (HBITMAP)SelectObject(m_hBackDC, m_hBackBmp);

	m_hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
	m_hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
	m_hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));
	m_hLightBlueBrush = CreateSolidBrush(RGB(255, 255, 255));
	m_hDarkBlueBrush = CreateSolidBrush(RGB(0, 0, 63));

	m_hGreenPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_hRedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_hBluePen = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));

	return true;
}

int CGameManager::Run()
{
	MSG msg = {};

	while (m_Loop)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);
		}
		else
		{
			Logic();
		}
	}
	return (int)msg.wParam;
}

void CGameManager::Logic()
{
	float DeltaTime = m_Timer->Update() * m_TimeScale;

	CResourceManager::GetInst()->Update();
	CInput::GetInst()->Update(DeltaTime);

	if (Update(DeltaTime))
		return;
	if (PostUpdate(DeltaTime))
		return;
	Render(DeltaTime);
}

bool CGameManager::Update(float DeltaTime)
{
	return CStageManager::GetInst()->Update(DeltaTime);
}

bool CGameManager::PostUpdate(float DeltaTime)
{
	return CStageManager::GetInst()->PostUpdate(DeltaTime);
}

void CGameManager::Render(float DeltaTime)
{
	Rectangle(m_hBackDC, -1, -1, m_RS.Width + 1, m_RS.Height + 1);
	CStageManager::GetInst()->Render(m_hBackDC);
	CInput::GetInst()->Render(m_hBackDC);
	BitBlt(m_hDC, 0, 0, m_RS.Width, m_RS.Height, m_hBackDC, 0, 0, SRCCOPY);
}

ATOM CGameManager::Register()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;

	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	wcex.hInstance = m_hInst;

	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON1));

	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = TEXT("GameFramework");
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}

BOOL CGameManager::Create()
{
	m_hWnd = CreateWindowW(TEXT("GameFramework"), TEXT("GameFramework"), WS_OVERLAPPEDWINDOW,
		0, 100, m_RS.Width, m_RS.Height, nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd)
	{
		return FALSE;
	}

	RECT rc = { 0, 0, m_RS.Width, m_RS.Height };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 100, rc.right - rc.left,
		rc.bottom - rc.top,
		SWP_NOZORDER);

	ShowWindow(m_hWnd, SW_SHOW);

	UpdateWindow(m_hWnd);

	return TRUE;
}

LRESULT CGameManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;

		HDC hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		m_Loop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
