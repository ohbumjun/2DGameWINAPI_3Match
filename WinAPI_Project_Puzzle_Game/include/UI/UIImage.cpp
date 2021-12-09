#include "UIImage.h"

CUIImage::CUIImage() :
	m_FrameIndex(0),
	m_PlayTime(1.f),
	m_AnimaTime(0.f)
{
}

CUIImage::CUIImage(const CUIImage& Widget) : CUIWidget(Widget)
{
	m_Texture = Widget.m_Texture;
	m_PlayTime = Widget.m_PlayTime;
	m_AnimaTime = 0.f;
	m_FrameIndex = 0;
	m_vecFrameData = Widget.m_vecFrameData;
}

CUIImage::~CUIImage()
{
}

void CUIImage::SetTexture(const std::string& Name)
{
	
}

void CUIImage::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
}

void CUIImage::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
}

void CUIImage::SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
}

void CUIImage::SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
}

bool CUIImage::Init()
{
	return false;
}

void CUIImage::Update(float DeltaTime)
{
}

void CUIImage::PostUpdate(float DeltaTime)
{
}

void CUIImage::Collision(float DeltaTime)
{
}

void CUIImage::Render(HDC hDC)
{
}

void CUIImage::Render(const Vector2& Pos, HDC hDC)
{
}

CUIImage* CUIImage::Clone()
{
	return nullptr;
}
