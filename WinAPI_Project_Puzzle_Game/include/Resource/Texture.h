#pragma once

#include "../Ref.h"

struct  TextureInfo
{
	// 윈도우즈는 비트맵을 화면 DC로 출력하는 함수는 제공하지 않는다
	//  비트맵은 크기가 큰 덩어리이고, 출력 속도가 형편없기 때문이다.
	// 메모리 DC란, 화면 DC와 동일한 특성을 가지고 , 그 내부에 출력 표면을 가진 메모리 영역
	// 메모리 DC 표면에 그림을 그리고, 그 화면을 화면 DC에 고속 복사하는 원리 
	HDC hDC;
	
	// 비트맵 핸들 값을 이용해서, 비트맵 정보를 HBITMAP  구조체 형식으로 얻을 수 있다.
	HBITMAP hBmp;
	HBITMAP hPrev;

	// 실제 Bitmap 정보 
	BITMAP BmpInfo;

	// Color Key 등록 여부 
	bool ColorKeyEnable;

	// Color Key 정보 
	unsigned int ColorKey;

	// 각각 TextureInfo마다 FileName을 지니게 한다.
	TCHAR FileName[MAX_PATH];

	TextureInfo() :
		hDC(0),
		hBmp(0),
		hPrev(0),
		BmpInfo{},
		ColorKeyEnable(false),
		ColorKey(0),
		FileName{}
	{

	}
	~TextureInfo()
	{
		// 원래의 HGDIOBJ로 되돌려준다 --> SelectObject : DC에 저장된 GDI obj의 핸들 값을 변경할 때 사용한다. 
		// SelectObject 함수로 교체 가능한 obj는 Bitmap, Pen, Brush, Font, Regin 
		// Bitmap의 경우, DC가 메모리 DC인 경우만 교체 가능
		// 그리고 Gdi Obj 교체 작업이 성공되면, DC가 이전에 사용하고 있던 GDI obj 의 핸들값을 반환한다.
		SelectObject(hDC, hPrev);

		// DeleteObject : 사용한 GDI 오브젝트를 제거하기 위해 사용 
		// 단, 현재 DX에 선택된 obj 의 경우, 제거가 불가능하다. 
		// 그래서 SelectObj를 통해 위에서 이전에 선택되어 있던 브러시로 교체해준다
		// 그래서 사용완료한 브러시를 지워주고
		DeleteObject(hBmp);

		// DC를 지워준다
		DeleteDC(hDC);
	}
};

class CTexture : public CRef
{
public :
	CTexture();
	~CTexture();
private :
	ETexture_Type m_TextureType;
	std::vector<TextureInfo*> m_vecTextureInfo;
public :
	ETexture_Type GetTextureType() const
	{
		return m_TextureType;
	}
	unsigned int GetWidth(int idx = 0) const
	{
		return (unsigned int)m_vecTextureInfo[idx]->BmpInfo.bmWidth;
	}
	unsigned int GetHeight(int idx = 0) const
	{
		return (unsigned int)m_vecTextureInfo[idx]->BmpInfo.bmHeight;
	}
	void SetColorKey(const unsigned char r, const unsigned char g, const unsigned char b, int Index = 0)
	{
		m_vecTextureInfo[Index]->ColorKeyEnable = true;
		m_vecTextureInfo[Index]->ColorKey = RGB(r,g,b);
	}
	void SetColorKey(unsigned int ColorKey, int Index = 0)
	{
		m_vecTextureInfo[Index]->ColorKeyEnable = true;
		m_vecTextureInfo[Index]->ColorKey = ColorKey;
	}
public :
	bool LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName = TEXTURE_PATH);
public :
	void Render(HDC hDC, const Vector2& WindowPos, const Vector2& ImagePos, const Vector2& Size, int Index = 0);
};