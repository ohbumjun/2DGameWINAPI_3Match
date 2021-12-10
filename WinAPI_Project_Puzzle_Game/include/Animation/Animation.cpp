#include "Animation.h"
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
}

void CAnimation::SetPlayTime(const std::string& Name, float PlayTime)
{
}

void CAnimation::SetPlayScale(const std::string& Name, float PlayScale)
{
}

void CAnimation::SetCurrentAnimation(const std::string& Name)
{
}

void CAnimation::ChangeAnimation(const std::string& Name)
{
}

bool CAnimation::CheckCurrentAnimation(const std::string& Name)
{
	return false;
}

void CAnimation::SetReverse(bool Reverse)
{
}

void CAnimation::SetLoop(bool Loop)
{
}

void CAnimation::Update(float DeltaTime)
{
}

CAnimation* CAnimation::Clone()
{
	return nullptr;
}

AnimationInfo* CAnimation::FindInfo(const std::string& Name)
{
	return nullptr;
}
