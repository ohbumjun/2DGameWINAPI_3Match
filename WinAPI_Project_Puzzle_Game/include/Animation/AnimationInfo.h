#pragma once

#include "../Resource/AnimationSequence.h"

struct  AnimationNotify
{
	int Frame;
	bool Call;
	std::function<void()> Function;

	AnimationNotify() : 
		Frame(0),
		Call(false){}
};

struct AnimationInfo
{
	CSharedPtr<CAnimationSequence> Sequence;
	int Frame;
	int FrameTime;
	int PlayTime;
	float PlayScale;
	float m_Time;
	bool Loop;
	bool Reverse;
	std::function<void()> m_EndFunction;
	std::vector<AnimationNotify*> m_vecNotify;

	AnimationInfo() :
		Frame(0),
		FrameTime(0.f),
		PlayTime(0),
		PlayScale(1.f),
		m_Time(0),
		Loop(false),
		Reverse(false){}

	~AnimationInfo()
	{
		size_t Size = m_vecNotify.size();
		for (size_t i = 0; i < Size; i++)
		{
			SAFE_DELETE(m_vecNotify[i]);
		}
	}
};