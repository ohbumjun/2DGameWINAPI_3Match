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
	int m_Tick;			 // ������ ������ ����
	float m_DeltaTime; // �� ������ ������ �ð� ����

	float m_FPS;		  // 1�ʿ� �帣�� ������ 
	float m_FPSTime; // Ư�� �������� �帥 �ð�  

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