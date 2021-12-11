#include "Animation.h"
#include "AnimationInfo.h"
#include "../Resource/ResourceManager.h"

CAnimation::CAnimation() :
	m_CurrentAnimation(nullptr),
	m_Owner(nullptr),
	m_mapAnimation{},
	m_WidgetOwner(nullptr)
{
}

CAnimation::CAnimation(const CAnimation& Anim)
{
	m_CurrentAnimation = nullptr;
	m_Owner = nullptr;
	m_WidgetOwner = nullptr;

	auto iter = Anim.m_mapAnimation.begin();
	auto iterEnd = Anim.m_mapAnimation.end();
	for (; iter != iterEnd; ++iter)
	{
		AnimationInfo* Info = new AnimationInfo;
		Info->Sequence = iter->second->Sequence;
		Info->PlayTime = iter->second->PlayTime;
		Info->PlayScale = iter->second->PlayScale;
		Info->Loop = iter->second->Loop;
		Info->Reverse = iter->second->Reverse;

		if (Anim.m_CurrentAnimation->Sequence->GetName() == Info->Sequence->GetName())
			m_CurrentAnimation = Info;
		m_mapAnimation.insert(std::make_pair(iter->first, Info));
	}
}

CAnimation::~CAnimation()
{
	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

void CAnimation::AddAnimation(const std::string& SequenceName, bool Loop, float PlayTime, float PlayScale, bool Reverse)
{
	// 애니메이션 제작에 필요한 Sequence 존재 여부
	CAnimationSequence* Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);
	if (!Sequence)
		return;
	AnimationInfo* Anim = new AnimationInfo;
	Anim->Loop = Loop;
	Anim->PlayScale = PlayScale;
	Anim->PlayTime = PlayTime;
	Anim->Reverse = Reverse;
	Anim->Sequence = Sequence;
	Anim->FrameTime = PlayTime / Sequence->GetFrameCount();

	if (m_mapAnimation.empty())
		m_CurrentAnimation = Anim;
	m_mapAnimation.insert(std::make_pair(SequenceName, Anim));
}

void CAnimation::SetPlayTime(const std::string& Name, float PlayTime)
{
	AnimationInfo* Info = FindInfo(Name);
	if (!Info)
		return;
	Info->PlayTime = PlayTime;
}

void CAnimation::SetPlayScale(const std::string& Name, float PlayScale)
{
	AnimationInfo* Info = FindInfo(Name);
	if (!Info)
		return;
	Info->PlayScale = PlayScale;
}

void CAnimation::SetCurrentAnimation(const std::string& Name)
{
	if (m_CurrentAnimation->Sequence->GetName() == Name)
		return;
	
	AnimationInfo* Anim = FindInfo(Name);
	if (!Anim)
		return;

	m_CurrentAnimation = Anim;
	m_CurrentAnimation->Frame = 0;
	m_CurrentAnimation->m_Time = 0.f;

	size_t  Size = m_CurrentAnimation->m_vecNotify.size();
	for (size_t i = 0; i < Size; i++)
	{
		m_CurrentAnimation->m_vecNotify[i]->Call = false;
	}
}

void CAnimation::ChangeAnimation(const std::string& Name)
{
	if (m_CurrentAnimation->Sequence->GetName() == Name)
		return;

	AnimationInfo* Anim = FindInfo(Name);
	if (!Anim)
		return;

	// Before
	m_CurrentAnimation->Frame = 0;
	m_CurrentAnimation->m_Time = 0.f;
	size_t  Size = m_CurrentAnimation->m_vecNotify.size();
	for (size_t i = 0; i < Size; i++)
	{
		m_CurrentAnimation->m_vecNotify[i]->Call = false;
	}

	// New
	m_CurrentAnimation = Anim;
	m_CurrentAnimation->Frame = 0;
	m_CurrentAnimation->m_Time = 0.f;
}

bool CAnimation::CheckCurrentAnimation(const std::string& Name)
{
	return m_CurrentAnimation->Sequence->GetName() == Name;
}

void CAnimation::SetReverse(const std::string& Name, bool Reverse)
{
	AnimationInfo* Info = FindInfo(Name);
	if (!Info)
		return;
	Info->Reverse = Reverse;
}

void CAnimation::SetLoop(const std::string& Name, bool Loop)
{
	AnimationInfo* Info = FindInfo(Name);
	if (!Info)
		return;
	Info->Loop = Loop;
}

void CAnimation::Update(float DeltaTime)
{
	if (m_CurrentAnimation)
	{
		m_CurrentAnimation->m_Time += DeltaTime;

		bool AnimEnd = false;
		// Frame 증가 및 감소 처리 + 애니메이션 종료 여부 확인 
		if (m_CurrentAnimation->m_Time == m_CurrentAnimation->FrameTime)
		{
			m_CurrentAnimation->m_Time -= m_CurrentAnimation->FrameTime;
			if (m_CurrentAnimation->Reverse)
			{
				--m_CurrentAnimation->Frame;
				if (m_CurrentAnimation->Frame == 0)
					AnimEnd = true;
			}
			else
			{
				++m_CurrentAnimation->Frame;
				if (m_CurrentAnimation->Frame == m_CurrentAnimation->Sequence->GetFrameCount())
					AnimEnd = true;
			}
		}

		// Frame 별 할당된 함수 포인터를 통해 함수 호출 
		size_t Size = m_CurrentAnimation->m_vecNotify.size();
		for (size_t i = 0; i < Size; i++)
		{
			if (!m_CurrentAnimation->m_vecNotify[i]->Call && m_CurrentAnimation->m_vecNotify[i]->Frame == m_CurrentAnimation->Frame)
			{
				m_CurrentAnimation->m_vecNotify[i]->Call = true;
				m_CurrentAnimation->m_vecNotify[i]->Function();
			}
		}

		if (AnimEnd)
		{
			if (m_CurrentAnimation->Loop)
			{
				if (!m_CurrentAnimation->Reverse)
					m_CurrentAnimation->Frame = m_CurrentAnimation->Sequence->GetFrameCount() - 1;
				else
					m_CurrentAnimation->Frame = 0;
			}
			else
			{
				if (!m_CurrentAnimation->Reverse)
					m_CurrentAnimation->Frame = 0;
				else
					m_CurrentAnimation->Frame = m_CurrentAnimation->Sequence->GetFrameCount() - 1;
			}

			// 모션이 끝났다면, EndFunction이 있을 경우 호출한다
			if (m_CurrentAnimation->m_EndFunction)
				m_CurrentAnimation->m_EndFunction();

			if (m_CurrentAnimation->Loop)
			{
				size_t Size = m_CurrentAnimation->m_vecNotify.size();
				for (size_t i = 0; i < Size; i++)
				{
					m_CurrentAnimation->m_vecNotify[i]->Call = false;
				}
			}
		}
	}
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

AnimationInfo* CAnimation::FindInfo(const std::string& Name)
{
	auto iter = m_mapAnimation.find(Name);
	if (iter == m_mapAnimation.end())
		return nullptr;
	return iter->second;
}
