#include "UIButton.h"

#include "UIWindow.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Texture.h"
#include "../Input.h"

CUIButton::CUIButton() :
m_ButtonState(EButton_State::Normal)
{}

CUIButton::CUIButton(const CUIButton& Button) : CUIWidget(Button)
{
	memcpy(m_FrameData, Button.m_FrameData, sizeof(AnimationFrameData) * (int)EButton_State::End);
}

CUIButton::~CUIButton()
{}

void CUIButton::SetTexture(CTexture* Texture)
{
	m_Texture = Texture;
	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
		for (int i = 0; i < (int)EButton_State::End; i++)
		{
			m_FrameData[i].Size = m_Size;
		}
	}
}

void CUIButton::SetTexture(const std::string& Name)
{
	m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
		for (int i = 0; i < (int)EButton_State::End; i++)
		{
			m_FrameData[i].Size = m_Size;
		}
	}
}

void CUIButton::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);
	m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
		for (int i = 0; i < (int)EButton_State::End; i++)
		{
			m_FrameData[i].Size = m_Size;
		}
	}
}

void CUIButton::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath);
	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
		for (int i = 0; i < (int)EButton_State::End; i++)
		{
			m_FrameData[i].Size = m_Size;
		}
	}
}

void CUIButton::SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
	if (m_Texture)
	{
		m_Texture->SetColorKey(r, g, b, Index);
	}
}

bool CUIButton::Init()
{
	if (!CUIWidget::Init())
		return false;
	return true;
}

void CUIButton::Update(float DeltaTime)
{
	CUIWidget::Update(DeltaTime);
	if (m_ButtonState != EButton_State::Disable)
	{
		if (m_MouseHovered)
		{
			if (CInput::GetInst()->GetMouseDown())
			{
				m_ButtonState = EButton_State::Click;
			}
			else if(CInput::GetInst()->GetMouseUp() && m_ButtonState == EButton_State::Click)
			{
				if (m_ButtonClickFunction)
					m_ButtonClickFunction();
				m_ButtonState = EButton_State::MouseOn;
			}
			else if (m_ButtonState == EButton_State::Click && CInput::GetInst()->GetMousePush())
			{
				m_ButtonState = EButton_State::Click;
			}
			else
			{
				if (m_ButtonMouseOnFunction)
					m_ButtonMouseOnFunction();
				m_ButtonState = EButton_State::MouseOn;
			}
		}
		else
		{
			m_ButtonState = EButton_State::Normal;
		}
	}
	m_Size = m_FrameData[(int)m_ButtonState].Size;
}

void CUIButton::PostUpdate(float DeltaTime)
{
	CUIWidget::PostUpdate(DeltaTime);
}

CUIButton* CUIButton::Clone()
{
	return CUIButton::Clone();
}

void CUIButton::Render(HDC hDC)
{
	if (m_Texture)
	{
		Vector2 Pos = m_Pos + m_Owner->GetPos();
		if (m_Texture->GetTextureType() == ETexture_Type::Atlas)
		{
			m_Texture->Render(hDC, Pos, m_FrameData[(int)m_ButtonState].StartPos, m_FrameData[(int)m_ButtonState].Size);
		}
		else
		{
			m_Texture->Render(hDC, Pos, Vector2(0.f, 0.f), m_Size, (int)m_ButtonState);
		}
	}
}

void CUIButton::Render(const Vector2& Pos, HDC hDC)
{
	if (m_Texture)
	{
		if (m_Texture->GetTextureType() == ETexture_Type::Atlas)
		{
			m_Texture->Render(hDC, Pos, m_FrameData[(int)m_ButtonState].StartPos, m_FrameData[(int)m_ButtonState].Size);
		}
		else
		{
			m_Texture->Render(hDC, Pos, Vector2(0.f, 0.f), m_Size, (int)m_ButtonState);
		}
	}
}


void CUIButton::CollisionMouseHoveredCallback(float DeltaTime)
{
	CUIWidget::CollisionMouseHoveredCallback(DeltaTime);
}

void CUIButton::CallMouseReleaseCallback(float DeltaTime)
{
	CUIWidget::CallMouseReleaseCallback(DeltaTime);
}
