#pragma once

#include "UIWidget.h"

class CUIImage : public CUIWidget {
public :
	CUIImage();
	CUIImage(const CUIImage& Image);
	virtual ~CUIImage() override;
protected :
	CSharedPtr<class CTexture> m_Texture;
	std::vector<AnimationFrameData> m_vecFrameData;
	float m_PlayTime;
	float m_AnimTime;
	int m_FrameIndex;
	Vector2 m_Offset;
public :
	void SetTexture(class CTexture* const Texture);
	void SetTexture(const std::string& Name);
	void SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	void SetTexture(const std::string& Name, const std::vector<std::wstring> vecFileName, const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	void SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index = 0);
	void AddFrameData(const Vector2& StartPos, const Vector2& Size);
public :
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(HDC hDC, const Vector2& Pos);

};