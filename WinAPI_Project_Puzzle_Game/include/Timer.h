#pragma once

#include "GameInfo.h"

class CTimer
{
public :
	CTimer();
	~CTimer();
private :
	LARGE_INTEGER m_Time;
	LARGE_INTEGER m_Second;
private :
	int m_Tick;			 // 지나간 프레임 개수
	float m_DeltaTime; // 한 프레임 사이의 시간 간격

	float m_FPS;		  // 1초에 흐르는 프레임 
	float m_FPSTime; // 특정 시점까지 흐른 시간  

public :
	float GetDeltaTime() const
	{
		return m_DeltaTime;
	}
	float GetFPS() const
	{
		return m_FPS;
	}
public :
	float Update();

};