#pragma once

#include "../Ref.h"
#include "../Resource/Texture.h"

class CGameObject : public CRef
{
public:
	CGameObject();
	CGameObject(const CGameObject& Obj);
	~CGameObject();
protected:
	CSharedPtr<CTexture>	m_Texture;
	Vector2 m_Pos;
	Vector2 m_Size;
	int m_RowIndex;
	int m_ColIndex;
	int m_Index;
	Vector2 m_Dir;
	float m_TimeScale;
	float m_MoveSpeed;
	class CAnimation* m_Animation;
public:
	Vector2 GetPos() const
	{
		return m_Pos;
	}
	Vector2 GetSize() const
	{
		return m_Size;
	}
	int GetRowIndex() const
	{
		return m_RowIndex;
	}
	int GetColIndex() const
	{
		return m_ColIndex;
	}
public:
	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}
	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}
public: // Animation ==========
	void CreateAnimation();
	void AddAnimation(const std::string& SequenceName, bool Loop = true, float PlayTime = 1.f, float PlayScale = 1.f, bool Reverse = false);
	void SetAnimationPlayTime(const std::string& Name, float PlayTime);
	void SetAnimationPlayScale(const std::string& Name, float PlayScale);
	void SetCurrentAnimation(const std::string& Name);
	void ChangeAnimation(const std::string& Name);
	bool CheckCurrentAnimation(const std::string& Name);
	void SetAnimationReverse(const std::string& Name, bool Reverse);
	void SetAnimationLoop(const std::string& Name, bool Loop);
public : // Texture   ==========
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
};

