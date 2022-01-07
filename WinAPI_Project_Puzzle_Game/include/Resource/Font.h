#pragma once

#include "../Ref.h"

class CFont : public CRef {
	friend class CResourceManager;
protected: 
	CFont();
	 CFont(const CFont& Font);
	 virtual ~CFont() override;
private :
	LOGFONT m_FontInfo;
	HFONT m_Font;
	HFONT m_PrevFont;
public :
	bool LoadFont(const TCHAR* FontName, int Width = 0, int Height = 0);
	void SetFont(HDC hDC);
	void ResetFont(HDC hDC);
};