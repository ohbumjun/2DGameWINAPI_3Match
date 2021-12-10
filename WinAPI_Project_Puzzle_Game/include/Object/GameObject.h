#pragma once

#include "../Ref.h"

class CGameObject : public CRef
{
public :
	CGameObject();
	CGameObject(const CGameObject& Obj);
	~CGameObject();
protected :
	CSharedPtr<class CTexture>	m_Texture;
	Vector2 m_Pos;
	Vector2 m_Size;
	int m_RowIndex;
	int m_ColIndex;
	int m_Index;
	Vector2 m_Dir;
	float m_TimeScale;
	float m_MoveSpeed;
public :
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
	void SetPos(const Vector2& Size) 
	{
		m_Size = Size;
	}
};

