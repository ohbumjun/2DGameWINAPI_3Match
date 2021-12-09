#pragma once

#include "../Ref.h"

struct  TextureInfo
{
	// ��������� ��Ʈ���� ȭ�� DC�� ����ϴ� �Լ��� �������� �ʴ´�
	//  ��Ʈ���� ũ�Ⱑ ū ����̰�, ��� �ӵ��� ������� �����̴�.
	// �޸� DC��, ȭ�� DC�� ������ Ư���� ������ , �� ���ο� ��� ǥ���� ���� �޸� ����
	// �޸� DC ǥ�鿡 �׸��� �׸���, �� ȭ���� ȭ�� DC�� ��� �����ϴ� ���� 
	HDC hDC;
	
	// ��Ʈ�� �ڵ� ���� �̿��ؼ�, ��Ʈ�� ������ HBITMAP  ����ü �������� ���� �� �ִ�.
	HBITMAP hBmp;
	HBITMAP hPrev;

	// ���� Bitmap ���� 
	BITMAP BmpInfo;

	// Color Key ��� ���� 
	bool ColorKeyEnable;

	// Color Key ���� 
	unsigned int ColorKey;

	// ���� TextureInfo���� FileName�� ���ϰ� �Ѵ�.
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
		// ������ HGDIOBJ�� �ǵ����ش� --> SelectObject : DC�� ����� GDI obj�� �ڵ� ���� ������ �� ����Ѵ�. 
		// SelectObject �Լ��� ��ü ������ obj�� Bitmap, Pen, Brush, Font, Regin 
		// Bitmap�� ���, DC�� �޸� DC�� ��츸 ��ü ����
		// �׸��� Gdi Obj ��ü �۾��� �����Ǹ�, DC�� ������ ����ϰ� �ִ� GDI obj �� �ڵ鰪�� ��ȯ�Ѵ�.
		SelectObject(hDC, hPrev);

		// DeleteObject : ����� GDI ������Ʈ�� �����ϱ� ���� ��� 
		// ��, ���� DX�� ���õ� obj �� ���, ���Ű� �Ұ����ϴ�. 
		// �׷��� SelectObj�� ���� ������ ������ ���õǾ� �ִ� �귯�÷� ��ü���ش�
		// �׷��� ���Ϸ��� �귯�ø� �����ְ�
		DeleteObject(hBmp);

		// DC�� �����ش�
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