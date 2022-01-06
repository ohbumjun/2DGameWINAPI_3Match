#pragma once

#include "../Ref.h"

class CFont : public CRef {
public :
	CFont();
	~CFont();
private :
	LOGFONT m_FontInfo;
	HFONT m_Font;
	HFONT m_PrevFont;
public :
	bool LoadFont(const TCHAR* FontName, int Width = 0, int Height = 0);
	void SetFont(HDC hDC);
	void ReserFont(HDC hDC);
};