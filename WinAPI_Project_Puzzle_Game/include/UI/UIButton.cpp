#include "UIButton.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Texture.h"
#include "../Input.h"
#include "UIWindow.h"


CUIButton::CUIButton() :
m_ButtonState(EButton_State::Normal),
m_Texture(nullptr)
{}

CUIButton::CUIButton(const CUIButton& Button)
{
	m_Texture = Button.m_Texture;
	m_ButtonState = Button.m_ButtonState;
	memcpy(m_FrameData, Button.m_FrameData, sizeof(AnimationFrameData) * (int)EButton_State::End);
}

CUIButton::~CUIButton()
{}

void CUIButton::SetTexture(CTexture* const Texture)
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

void CUIButton::SetTexure(const std::string& Name, const std::vector<std::wstring> vecFileName,
	const std::string& PathName)
{
	CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName);
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
	CResourceManager::GetInst()->LoadTexture(Name, FullPath);
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
		m_Texture->SetColorKey(r, g, b, Index);
}

void CUIButton::SetFrameData(EButton_State State, const Vector2& StartPos, const Vector2& Size)
{
	m_FrameData[(int)State].StartPos = StartPos;
	m_FrameData[(int)State].Size = Size;
}

void CUIButton::Enable(bool Enable)
{
	m_ButtonState = Enable ? EButton_State::Normal : EButton_State::Disable;
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
			else if (CInput::GetInst()->GetMouseUp() && m_ButtonState == EButton_State::Click)
			{
				if (m_ButtonClickCallback)
					m_ButtonClickCallback();
				m_ButtonState = EButton_State::Normal;
			}
			else if (CInput::GetInst()->GetMousePush() && m_ButtonState == EButton_State::Click)
			{
				m_ButtonState = EButton_State::Click;
			}
			else
			{
				// 그냥 계속 위에 올려두는 것
				m_ButtonState = EButton_State::MouseOn;
			}
		}
		else
		{
			m_ButtonState = EButton_State::Normal;
		}
	}
}

void CUIButton::PostUpdate(float DeltaTime)
{
	CUIWidget::PostUpdate(DeltaTime);
}

void CUIButton::Render(HDC hDC, const Vector2& Pos)
{
	CUIWidget::Render(hDC, Pos);
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

void CUIButton::Render(HDC hDC)
{
	CUIWidget::Render(hDC);
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

CUIButton* CUIButton::Clone()
{
	return new CUIButton(*this);
}

void CUIButton::CallMouseHoveredCallback(float DeltaTime)
{
	CUIWidget::CallMouseHoveredCallback(DeltaTime);
	if (m_ButtonHoverCallback)
		m_ButtonHoverCallback();
}

void CUIButton::CallMouseReleaseCallback(float DeltaTime)
{
	CUIWidget::CallMouseReleaseCallback(DeltaTime);
}
