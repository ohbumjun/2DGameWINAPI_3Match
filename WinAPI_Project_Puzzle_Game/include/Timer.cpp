#include "Timer.h"
#include "GameManager.h"

CTimer::CTimer() :
	m_FPS(0.f),
	m_FPSTime(0.f),
	m_Tick(0),
	m_DeltaTime(0.f)
{
	// ���ػ� Ÿ�̸��� �ʴ� ƽ�� ���´�
	// ex) 1�ʴ� Frame ���� : 1�ʿ� 1000 ������ --> 1 / 1000 �� ������ �ɰ� �� �ִٴ� �ǹ��̴�.
	QueryPerformanceFrequency(&m_Second);

	// ���ػ� Ÿ�̸��� ���� ƽ�� ���´�
	QueryPerformanceCounter(&m_Time);
}

CTimer::~CTimer()
{
}

float CTimer::Update()
{
	LARGE_INTEGER Time;

	// ���� ƽ�� �ѹ� �� ���´� 
	QueryPerformanceCounter(&Time);

	// 100 ������ - 30 ������ / 1000 == 70 / 1000 == 0.07 �� ���� ���̴� 
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
