#include "Timer.h"
#include "GameManager.h"

CTimer::CTimer() :
	m_FPS(0.f),
	m_FPSTime(0.f),
	m_Tick(0),
	m_DeltaTime(0.f)
{
	// 고해상도 타이머의 초당 틱을 얻어온다
	// ex) 1초당 Frame 정보 : 1초에 1000 프레임 --> 1 / 1000 초 단위로 쪼갤 수 있다는 의미이다.
	QueryPerformanceFrequency(&m_Second);

	// 고해상도 타이머의 현재 틱을 얻어온다
	QueryPerformanceCounter(&m_Time);
}

CTimer::~CTimer()
{
}

float CTimer::Update()
{
	LARGE_INTEGER Time;

	// 현재 틱을 한번 더 얻어온다 
	QueryPerformanceCounter(&Time);

	// 100 프레임 - 30 프레임 / 1000 == 70 / 1000 == 0.07 초 지난 것이다 
	m_DeltaTime = (Time.QuadPart - m_Time.QuadPart) / (float)m_Second.QuadPart;
	m_Time			= Time;

	m_FPSTime += m_DeltaTime;
	++m_Tick;

	if (m_Tick == 60)
	{
		m_FPS = 60 / m_FPSTime;
		m_FPSTime = 0.f;
		m_Tick = 0;

#ifdef _DEBUG
		wchar_t buff[101];
		swprintf_s(buff, L"FPS : %zu, DT : %.10f", (unsigned __int64)m_FPS, m_DeltaTime);
		SetWindowText(CGameManager::GetInst()->GetWindowHandle(), buff);
#endif
	}

	return m_DeltaTime;
}
