#pragma once


#include "GameInfo.h"

class CGameManager
{
private:
	CGameManager();
	~CGameManager();

private:
	static bool	m_Loop;
	/*
	- HWND =  핸들 윈도우 약자 , 윈도우의 핸들번호를 저장해서 사용 , 하나의 프로그램에서 많은양의 윈도우를 띄울수 있다.
			   즉 윈도우창의 번호들(HWND) 로 구분하는것이다.
			   이 윈도우의 번호가 몇 번이다 라는 걸 알고 있어야 운영체제에서 관리를 할 수 있다.
				관리를 위한 id를 식별하기 위하여 HWND가 필요하다.
	- HINSTANCE = 핸들 인스턴스  , 프로그램의 인스턴스 식별자 , 쉽게 보면 프로그램 자체의 실체화된 주소.
	- 결론 : HINSTANCE는 프로그램 자체의 핸들이며 , HWND는 프로그램안의 윈도우창의 번호 !!
	*/
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	HDC			m_hDC;
	HDC			m_hBackDC;
	HBITMAP		m_hBackBmp;
	HBITMAP		m_hPrevBackBmp;
	Resolution	m_RS;
	class CTimer* m_Timer;
	float		m_TimeScale;
	// Brush
	HBRUSH		m_hGreenBrush;
	HBRUSH		m_hRedBrush;
	HBRUSH		m_hBlueBrush;
	HBRUSH		m_hLightBlueBrush;
	HBRUSH		m_hDarkBlueBrush;
	// Pen
	HPEN		m_hGreenPen;
	HPEN		m_hRedPen;
	HPEN		m_hBluePen;
	// 현재 에디트 모드인지 아닌지 --> Editor Scene에 들어가면 true로
	bool        m_EditorMode;
private:
	float m_MasterVolume;
	int m_BGVolume;
	int m_EffectVolume;
public:
	void SetMasterVol(float Vol) { m_MasterVolume = Vol; }
	void SetBGVol(int Vol) { m_BGVolume = Vol; }
	void SetEffectVol(int Vol) { m_EffectVolume = Vol; }
public:
	float GetMasterVolume() const { return m_MasterVolume; }
	int GetBGVolume() const { return m_BGVolume; }
	int GetEffectVolume() const { return m_EffectVolume; }
public:
	Resolution GetResolution()	const
	{
		return m_RS;
	}
	HPEN GetGreenPen()	const
	{
		return m_hGreenPen;
	}
	HPEN GetRedPen()	const
	{
		return m_hRedPen;
	}
	HPEN GetBluePen()	const
	{
		return m_hBluePen;
	}
	HBRUSH GetGreenBrush()	const
	{
		return m_hGreenBrush;
	}
	HBRUSH GetRedBrush()	const
	{
		return m_hRedBrush;
	}

	HBRUSH GetLightBlueBrush()	const
	{
		return m_hLightBlueBrush;
	}
	HBRUSH GetBlueBrush()	const
	{
		return m_hBlueBrush;
	}
	HBRUSH GetDarkBlueBrush()	const
	{
		return m_hDarkBlueBrush;
	}
	HDC GetWindowDC()	const
	{
		return m_hDC;
	}
	HWND GetWindowHandle()	const
	{
		return m_hWnd;
	}

	HINSTANCE GetWindowInstance()	const
	{
		return m_hInst;
	}
	bool IsEditorMode() const
	{
		return m_EditorMode;
	}
	CTimer* GetTimer() const { return m_Timer; }

public:
	void SetEditorMode(bool Mode)
	{
		m_EditorMode = Mode;
	}
	void SetTimeScale(float TimeScale);
	float GetTimeScale() const;

public:
	float GetDeltaTime()	const;
	void Exit();

public:
	bool Init(HINSTANCE hInst);
	int Run();

private:
	void Logic();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void Render(float DeltaTime);

private:
	ATOM Register();
	BOOL Create();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


private:
	static CGameManager* m_Inst;

public:
	static CGameManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CGameManager;

		return m_Inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}
};

