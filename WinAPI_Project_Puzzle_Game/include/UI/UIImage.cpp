#include "UIImage.h"
#include "../Resource/ResourceManager.h"
#include "UIWindow.h"

CUIImage::CUIImage() :
m_FrameIndex(0),
m_PlayTime(1.f),
m_AnimTime(0.f)
{}

CUIImage::CUIImage(const CUIImage& Image)
{
	m_Texture = Image.m_Texture;
	m_PlayTime = Image.m_PlayTime;
	m_AnimTime = 0.f;
	m_FrameIndex = 0;
	m_vecFrameData = Image.m_vecFrameData;
}

CUIImage::~CUIImage()
{}

void CUIImage::SetTexture(const CTexture* Texture)
{
	m_Texture = const_cast<CTexture*>(Texture);
	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
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

void CUIImage::SetTexture(const std::string& Name, std::vector<std::wstring> vecFileName, const std::string& PathName)
{
	CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName);
	m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CUIImage::AddAnimationFrameData(const Vector2& Pos, const Vector2& Size)
{
	AnimationFrameData data = {};
	data.StartPos = Pos;
	data.Size = Size;
	m_vecFrameData.push_back(data);
}

bool CUIImage::Init()
{
	if (!CUIWidget::Init())
		return false;
	return true;
}

void CUIImage::Update(float DeltaTime)
{
	CUIWidget::Update(DeltaTime);

	if (!m_vecFrameData.empty())
	{
		m_AnimTime += DeltaTime;

		float FrameTime = m_PlayTime / m_vecFrameData.size();

		if (m_AnimTime >= FrameTime)
		{
			m_AnimTime -= FrameTime;
			m_FrameIndex = (m_FrameIndex + 1) % m_vecFrameData.size();
		}
	}
}

void CUIImage::PostUpdate(float DeltaTime)
{
	CUIWidget::PostUpdate(DeltaTime);
}

void CUIImage::Render(HDC hDC)
{
	if (m_Texture)
	{
		Vector2 Pos = m_Pos + m_Owner->GetPos();
		Vector2 ImagePos;
		Vector2 Size = m_Size;

		if (!m_vecFrameData.empty())
		{
			ImagePos = m_vecFrameData[m_FrameIndex].StartPos;
			Size = m_vecFrameData[m_FrameIndex].Size;
			if (m_Texture->GetTextureType() == ETexture_Type::Atlas)
			{
				m_Texture->Render(hDC, Pos + m_Offset, ImagePos, Size);
			}
			else
			{
				m_Texture->Render(hDC, Pos + m_Offset, ImagePos, Size, m_FrameIndex);
			}
		}
		else
		{
						if (m_Texture->GetTextureType() == ETexture_Type::Atlas)
			{
				m_Texture->Render(hDC, Pos + m_Offset, ImagePos, Size);
			}
			else
			{
				m_Texture->Render(hDC, Pos + m_Offset, ImagePos, Size, m_FrameIndex);
			}
		}
	}
}

void CUIImage::Render(const Vector2& Pos, HDC hDC)
{}

CUIImage* CUIImage::Clone()
{
	return CUIImage::Clone();
}
