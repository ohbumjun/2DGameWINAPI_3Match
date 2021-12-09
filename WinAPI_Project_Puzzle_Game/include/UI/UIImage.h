#pragma once

#include "UIWidget.h"
#include "../Resource/Texture.h"

class CUIImage : public CUIWidget
{
public :
	CUIImage();
	CUIImage(const CUIImage& Widget);
	~CUIImage();
protected :
	CSharedPtr<CTexture> m_Texture;
	std::vector<AnimationFrameData> m_vecFrameData;
	int m_FrameIndex;
	float m_PlayTime;
	float m_AnimaTime;
	Vector2 m_OffSet;
public :
	void SetTexture(CTexture* Texture)
	{
		m_Texture = Texture;
		if (m_Texture)
		{
			m_Size.x = (float)m_Texture->GetWidth();
			m_Size.y = (float)m_Texture->GetHeight();
		}
	}

	void SetTexture(const std::string& Name);
	void SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	void SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName = TEXTURE_PATH);
	void SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index = 0);
	void AddFrameData(const Vector2& Pos, const Vector2& Size)
	{
		AnimationFrameData data = {};
		data.StartPos = Pos;
		data.Size = Size;
		m_vecFrameData.push_back(data);
	}
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& Pos, HDC hDC);
	virtual CUIImage* Clone();
};

