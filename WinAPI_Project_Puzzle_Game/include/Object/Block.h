#pragma once

#include "../GameInfo.h"
#include "GameObject.h"

class CBoard;

class CBlock : public CGameObject
{
public :
	CBlock();
	~CBlock();
private :
	BlockType	 	 m_BlockType;
	bool               m_MoveEnable;
	Vector2			 m_RowColPos;
	class CBoard* m_Board;
public :
	void SetBoard(CBoard* Board)
	{
		m_Board = Board;
	}
	void SetBlockType(BlockType Type)
	{
		m_BlockType = Type;
	}
	void SetBlockInitInfo(const Vector2 Pos, const Vector2& Size, int RowIndex, int ColIndex, int Index, class CTexture* Texture)
	{
		m_Pos      = Pos; 
		m_Size     = Size;
		m_RowIndex	= RowIndex;
		m_ColIndex =	 ColIndex;
		m_Index    =	Index;

		// Texture
		m_Texture = Texture;
		if (m_Texture)
		{
			m_Size.x = (float)m_Texture->GetWidth();
			m_Size.y = (float)m_Texture->GetHeight();
		}
	}
	void SetMoveEnable(bool Enable)
	{
		m_MoveEnable = Enable;
	}
public :
	bool GetMoveEnable() const
	{
		return m_MoveEnable;
	}
	BlockType GetBlockType() const
	{
		return m_BlockType;
	}
public :
	virtual void Start() override;
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
	virtual void Render(HDC hDC) override;
	CBlock* Clone();
};

