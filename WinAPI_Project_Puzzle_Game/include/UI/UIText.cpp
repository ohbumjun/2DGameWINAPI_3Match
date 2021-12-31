#include "UIText.h"
#include "UIWindow.h"

CUIText::CUIText() :
	m_TextCapacity(22),
	m_TextCount(0),
	m_TextColor(RGB(0, 1, 0))
{
	m_Text = new TCHAR[m_TextCapacity];
	memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
}

CUIText::CUIText(const CUIText& Text) : CUIWidget(Text)
{
	m_TextCapacity = Text.m_TextCapacity;
	m_TextCount = Text.m_TextCount;
	m_TextColor = Text.m_TextColor;

	m_Text = new TCHAR[m_TextCapacity];
	memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
	memcpy(m_Text, Text.m_Text, sizeof(TCHAR) * Text.m_TextCount);
}

CUIText::~CUIText()
{
	SAFE_DELETE_ARRAY(m_Text);
}

bool CUIText::Init()
{
	return true;
}

void CUIText::Render(HDC hDC)
{
	Vector2 Pos = m_Pos + m_Owner->GetPos();
	SetBkColor(hDC, TRANSPARENT);
	::SetTextColor(hDC, m_TextColor);
	TextOut(hDC, (int)Pos.x, (int)Pos.y, m_Text, m_TextCount);
}
#include "UIText.h"
#include "UIWindow.h"

CUIText::CUIText() :
	m_TextCapacity(22),
	m_TextCount(0),
	m_TextColor(RGB(0, 1, 0))
{
	m_Text = new TCHAR[m_TextCapacity];
	memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
}

CUIText::CUIText(const CUIText& Text) : CUIWidget(Text)
{
	m_TextCapacity = Text.m_TextCapacity;
	m_TextCount = Text.m_TextCount;
	m_TextColor = Text.m_TextColor;

	m_Text = new TCHAR[m_TextCapacity];
	memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
	memcpy(m_Text, Text.m_Text, sizeof(TCHAR) * Text.m_TextCount);
}

CUIText::~CUIText()
{
	SAFE_DELETE_ARRAY(m_Text);
}

bool CUIText::Init()
{
	return true;
}

void CUIText::Render(HDC hDC)
{
	Vector2 Pos = m_Pos + m_Owner->GetPos();
	SetBkColor(hDC, TRANSPARENT);
	::SetTextColor(hDC, m_TextColor);
	TextOut(hDC, (int)Pos.x, (int)Pos.y, m_Text, m_TextCount);
}
