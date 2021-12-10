#pragma once
#include "../GameInfo.h"
#include "../Object/GameObject.h"

class CBlock : public CGameObject
{
public :
	CBlock();
	~CBlock();
private :
	BlockType m_BlockType;
	Vector2 m_RowColPos;
public :
	void SetBlockType(BlockType Type)
	{
		m_BlockType = Type;
	}
	void SetRowColPos(int Row, int Col)
	{
		m_RowColPos = Vector2((float)Row, (float)Col);
	}
public :
	BlockType GetBlockType() const
	{
		return m_BlockType;
	}
public :
	bool Init();
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Render(HDC hDC);
};

