#include "UIImage.h"
#include "UIWindow.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Texture.h"

CUIImage::CUIImage() :
	m_FrameIndex(0),
	m_PlayTime(1.f),
	m_AnimTime(0.f)
{
}

CUIImage::CUIImage(const CUIImage& Widget) : CUIWidget(Widget)
{
	m_Texture = Widget.m_Texture;
	m_PlayTime = Widget.m_PlayTime;
	m_AnimTime = 0.f;
	m_FrameIndex = 0;
	m_vecFrameData = Widget.m_vecFrameData;
}

CUIImage::~CUIImage()
{
}

void CUIImage::SetTexture(const std::string& Name)
{
	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CUIImage::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CUIImage::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath);

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CUIImage::SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName);

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CUIImage::SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
	if (m_Texture)
		m_Texture->SetColorKey(r, g, b, Index);
}

bool CUIImage::Init()
{
	return true;
}

void CUIImage::Update(float DeltaTime)
{
	size_t VecSize = m_vecFrameData.size();
	if (!m_vecFrameData.empty())
	{
		m_AnimTime += DeltaTime;

		float FrameTime = m_PlayTime / VecSize;

		if (m_AnimTime >= FrameTime)
		{
			m_AnimTime -= FrameTime;

			m_FrameIndex = (m_FrameIndex + 1) % VecSize;
		}
	}
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
	if (m_Texture)
	{
		Vector2 Pos = m_Pos;
		if (m_Owner)
			Pos += m_Owner->GetPos();
		Vector2 ImagePos = Vector2(0.f, 0.f);
		Vector2 Size = m_Size;

		// Animation Frame이 각각 정해져 있을 경우 
		// 해당 Animation Frame 의 Size로 Size를 정해서 출력하고
		// 그렇지 않다면, Texture 자체의 크기로 정해서 출력한다.
		if (!m_vecFrameData.empty())
		{
			ImagePos = m_vecFrameData[m_FrameIndex].StartPos;
			Size = m_vecFrameData[m_FrameIndex].Size;
		}

		if (m_Texture->GetTextureType() == ETexture_Type::Frame)
		{
			// 이미지를 이용해서 출력한다
			m_Texture->Render(hDC, Pos + m_Offset, ImagePos, Size, m_FrameIndex);
		}
		else
		{
			m_Texture->Render(hDC, Pos + m_Offset, ImagePos, Size);
		}
	}
}

CUIImage* CUIImage::Clone()
{
	return new CUIImage(*this);
}
