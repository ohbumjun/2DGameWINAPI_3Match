#include "Font.h"

CFont::CFont()
{}

CFont::~CFont()
{
	if (m_Font)
		DeleteObject(m_Font);
}

bool CFont::LoadFont(const TCHAR* FontName, int Width, int Height)
{
	m_FontInfo.lfWidth  = (LONG)Width;
	m_FontInfo.lfHeight = (LONG)Height;
	m_FontInfo.lfUnderline = 0;
	m_FontInfo.lfWeight = FW_NORMAL;
	m_FontInfo.lfStrikeOut = 0;
	m_FontInfo.lfCharSet = HANGEUL_CHARSET;
	m_FontInfo.lfItalic = 0;
	m_FontInfo.lfPitchAndFamily = 2; // ÀÚ°£

	lstrcpy(m_FontInfo.lfFaceName, FontName);

	m_Font = CreateFontIndirect(&m_FontInfo);

	return m_Font == nullptr ? false : true;
}

void CFont::SetFont(HDC hDC)
{
	m_PrevFont = (HFONT)SelectObject(hDC, m_Font);
}

void CFont::ReserFont(HDC hDC)
{
	SelectObject(hDC, m_PrevFont);
}
