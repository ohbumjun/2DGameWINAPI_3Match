
#include "UIText.h"
#include "UIWindow.h"
#include "../Resource/ResourceManager.h"

CUIText::CUIText()
{
	m_TextCapacity = 32;
	m_TextCount = 0;

	m_Text = new TCHAR[m_TextCapacity];

	memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);

	m_TextColor = RGB(0, 0, 0);
}

CUIText::CUIText(const CUIText& widget) :
	CUIWidget(widget)
{
	m_TextCapacity = widget.m_TextCapacity;
	m_TextCount = widget.m_TextCount;
	m_TextColor = widget.m_TextColor;

	m_Text = new TCHAR[m_TextCapacity];

	memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);

	memcpy(m_Text, widget.m_Text, sizeof(TCHAR) * m_TextCount);

	m_Font = widget.m_Font;
}

CUIText::~CUIText()
{
	SAFE_DELETE_ARRAY(m_Text);
}


void CUIText::SetFont(const std::string& Name)
{
	m_Font = CResourceManager::GetInst()->FindFont(Name);
}

bool CUIText::Init()
{
	m_Font = CResourceManager::GetInst()->FindFont("DefaultFont");
	return true;
}

void CUIText::Update(float DeltaTime)
{
}

void CUIText::PostUpdate(float DeltaTime)
{
}

void CUIText::Collision(float DeltaTime)
{
}

void CUIText::Render(HDC hDC)
{
	m_Font->SetFont(hDC);

	Vector2 Pos = m_Pos + m_Owner->GetPos();

	SetBkMode(hDC, TRANSPARENT);
	// 앞에 :: 을 붙여서 전역함수 SetTextColor 를 호출한다.
	::SetTextColor(hDC, m_TextColor);
	TextOut(hDC, (int)Pos.x - 10.f, (int)Pos.y - 10.f, m_Text, m_TextCount);

	m_Font->ResetFont(hDC);
}

void CUIText::Render(const Vector2& Pos, HDC hDC)
{
	m_Font->SetFont(hDC);
	Vector2 NewPos = Pos + m_Owner->GetPos();

	SetBkMode(hDC, TRANSPARENT);
	// 앞에 :: 을 붙여서 전역함수 SetTextColor 를 호출한다.
	::SetTextColor(hDC, m_TextColor);
	TextOut(hDC, (int)NewPos.x - 10.f, (int)NewPos.y - 10.f, m_Text, m_TextCount);
	m_Font->ResetFont(hDC);
}

CUIText* CUIText::Clone()
{
	return new CUIText(*this);
}
