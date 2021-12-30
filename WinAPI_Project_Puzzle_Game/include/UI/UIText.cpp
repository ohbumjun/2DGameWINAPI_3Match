#include "UIText.h"

CUIText::CUIText()
{
	m_TextCapacity = 22;
	m_TextCount = 0;

	m_Text = new TCHAR[m_TextCapacity];

	memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);

	m_TextColor = RGB(0, 0, 1);
}


CUIText::CUIText(const CUIText& Text)
{
	m_TextCapacity = Text.m_TextCapacity;
	m_Text = Text.m_Text;
	m_TextColor = Text.m_TextColor;

	m_Text = new TCHAR[m_TextCapacity];
	memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);

	memcpy(m_Text, Text.m_Text, sizeof(TCHAR) * m_TextCapacity);
}

CUIText::~CUIText()
{
	SAFE_DELETE_ARRAY(m_Text);
}

bool CUIText::Init()
{
	// m_Font = CResou
	return true;
}

void CUIText::Render(HDC hDC)
{
	// 투명한 배경 색상 이용
	SetBkMode(hDC, TRANSPARENT);
	// 글자 출력 색상 세팅 
	::SetTextColor(hDC, m_TextColor);
	TextOut(hDC, (int)m_Pos.x, (int)m_Pos.y, m_Text, m_TextCount);
}

CUIText* CUIText::Clone()
{
	return new CUIText(*this);
}
