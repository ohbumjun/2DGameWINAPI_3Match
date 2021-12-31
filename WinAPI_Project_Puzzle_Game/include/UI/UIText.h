#pragma once

#include "UIWidget.h"

class CUIText : public CUIWidget
{
public:
	CUIText();
	CUIText(const CUIText& Text);
	virtual ~CUIText();
private:
	TCHAR* m_Text;
	int m_TextCount;
	int m_TextCapacity;
	COLORREF m_TextColor;
public:
	void SetTextColor(float r, float g, float b)
	{
		m_TextColor = RGB(r, g, b);
	}
	void SetText(const TCHAR* Text)
	{
		int Length = lstrlen(Text);
		if (Length + 1 > m_TextCapacity)
		{
			m_TextCapacity *= 2;
			SAFE_DELETE_ARRAY(m_Text);
			m_Text = new TCHAR[m_TextCapacity];
			memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
		}
		lstrcpy(m_Text, Text);
		m_TextCount = Length;
	}
	void AddText(const TCHAR* Text)
	{
		int Length = lstrlen(m_Text) + lstrlen(Text);
		if (Length + 1 > m_TextCapacity)
		{
			m_TextCapacity *= 2;
			TCHAR* Text = new TCHAR[m_TextCapacity];
			memset(Text, 0, sizeof(TCHAR) * m_TextCapacity);
			lstrcpy(Text, m_Text);
			SAFE_DELETE_ARRAY(m_Text);
			m_Text = Text;
		}
		lstrcat(m_Text, Text);
		m_TextCount += lstrlen(Text);
	}
	void PopBack()
	{
		m_TextCount--;
		m_Text[m_TextCount] = 0;
	}
	void ClearText()
	{
		m_TextCount = 0;
		memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
	}
public:
	virtual bool Init();
	virtual void Render(HDC hDC);
};