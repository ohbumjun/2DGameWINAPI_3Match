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
	void SetBlockInitInfo(const Vector2 Pos, const Vector2& Size, int RowIndex, int ColIndex, int Index, class CTexture* Texture)
	{
		m_Pos = Pos;
		m_Size = Size;
		m_RowIndex = RowIndex;
		m_ColIndex = ColIndex;
		m_Index = Index;

		// Texture
		m_Texture = Texture;
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

