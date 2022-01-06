#include "Font.h"

CFont::CFont() :
m_FontInfo{},
m_Font(0),
m_PrevFont(0)
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
	m_FontInfo.lfCharSet = HANGEUL_CHARSET;
	m_FontInfo.lfWeight = FW_NORMAL;
	m_FontInfo.lfItalic = 0;
	m_FontInfo.lfUnderline = 0;
	m_FontInfo.lfStrikeOut = 0;
	m_FontInfo.lfPitchAndFamily = 1; // ÀÚ°£

	lstrcpy(m_FontInfo.lfFaceName, FontName);

	m_Font = CreateFontIndirect(&m_FontInfo);

	return m_Font == nullptr ? false : true;
}

void CFont::SetFont(HDC hDC)
{
	m_PrevFont = (HFONT)SelectObject(hDC, m_Font);
}

void CFont::ResetFont(HDC hDC)
{
	SelectObject(hDC, m_PrevFont);
}
