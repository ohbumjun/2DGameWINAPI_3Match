#pragma once

#include "UIWidget.h"
#include "../Resource/Texture.h"

class CUIImage : public CUIWidget {
public :
	CUIImage();
	CUIImage(const CUIImage& Image);
	~CUIImage();
protected :
	CSharedPtr<CTexture> m_Texture;
	std::vector<AnimationFrameData> m_vecFrameData;
	int m_FrameIndex;
	float m_PlayTime;
	float m_AnimTime;
	Vector2 m_Offset;
public :
	void SetTexture(const CTexture* Texture);
	void SetTexture(const std::string& Name);
	void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	void SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	void SetTexture(const std::string& Name, std::vector<std::wstring> vecFileName, const std::string& PathName = TEXTURE_PATH);
	void AddAnimationFrameData(const Vector2& Post, const Vector2& Size);
public :
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& Pos, HDC hDC);
	virtual CUIImage* Clone();
};