#pragma once

#include "Texture.h"

class CAnimationSequence :
	public CRef
{
public:
	CAnimationSequence();
	~CAnimationSequence();
public :
	class CSharedPtr<CTexture> m_Texture;
	std::vector<AnimationFrameData> m_FrameDatas;
public :
	void SetTexture()
};

