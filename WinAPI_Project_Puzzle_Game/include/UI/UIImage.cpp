#include "UIImage.h"
#include "../Resource/ResourceManager.h"
#include "UIWindow.h"

CUIImage::CUIImage() :
	m_Texture(nullptr),
m_PlayTime(1.f),
m_AnimTime(0.f),
m_FrameIndex(0),
m_Offset{}
{}

CUIImage::CUIImage(const CUIImage& Image)
{
	m_vecFrameData = Image.m_vecFrameData;
	m_Texture = Image.m_Texture;
	m_PlayTime = Image.m_PlayTime;
	m_AnimTime = 0.f;
	m_FrameIndex = 0;
}

CUIImage::~CUIImage()
{}

void CUIImage::SetTexture(CTexture* const Texture)
{
	m_Texture = Texture;
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

void CUIImage::SetTexture(const std::string& Name, const std::vector<std::wstring> vecFileName,
	const std::string& PathName)
{
	CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName);
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

void CUIImage::SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
	if (m_Texture)
		m_Texture->SetColorKey(r, g, b, Index);
}

void CUIImage::AddFrameData(const Vector2& StartPos, const Vector2& Size)
{
	AnimationFrameData Data = {};
	Data.Size = Size;
	Data.StartPos = StartPos;
	m_vecFrameData.push_back(Data);
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
		float FrameTime = m_PlayTime / m_vecFrameData.size();
		m_AnimTime += DeltaTime;
		if (m_AnimTime > FrameTime)
		{
			m_AnimTime -= DeltaTime;
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
			if (m_Texture->GetTextureType() == ETexture_Type::Atlas)
			{
				ImagePos = m_vecFrameData[m_FrameIndex].StartPos;
				Size = m_vecFrameData[m_FrameIndex].Size;
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

void CUIImage::Render(HDC hDC, const Vector2& Pos)
{
}
