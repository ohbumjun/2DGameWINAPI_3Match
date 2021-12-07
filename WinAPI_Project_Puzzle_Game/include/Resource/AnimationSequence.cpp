
#include "AnimationSequence.h"

CAnimationSequence::CAnimationSequence()
{
}

CAnimationSequence::~CAnimationSequence()
{
}

bool CAnimationSequence::Init(const std::string& Name, CTexture* Texture)
{
	m_Name = Name;
	m_Texture = Texture;
	m_TextureType = m_Texture->GetTextureType();

	return true;
}

void CAnimationSequence::AddFrameData(const Vector2& StartPos,
	const Vector2& Size)
{
	AnimationFrameData	Data;
	Data.StartPos = StartPos;
	Data.Size = Size;

	m_vecFrameData.push_back(Data);
}

void CAnimationSequence::AddFrameData(float PosX, float PosY,
	float SizeX, float SizeY)
{
	AnimationFrameData	Data;
	Data.StartPos = Vector2(PosX, PosY);
	Data.Size = Vector2(SizeX, SizeY);

	m_vecFrameData.push_back(Data);
}
