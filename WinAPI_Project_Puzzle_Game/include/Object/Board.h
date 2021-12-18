#pragma once

#include "../GameInfo.h"
#include "../Resource/Texture.h"
#include "Cell.h"
#include "Block.h"

class CBoard : public CRef
{
	friend class CStage;
public:
	CBoard();
	~CBoard();
private :
	int m_BlockCount;
	int m_BlockCapacity;
	CCell** m_vecCells;
	CBlock** m_vecBlocks;

	// Block Texture
	CSharedPtr<CTexture> m_BlockTexture;

	// Board 전체 크기
	Vector2 m_Size;
	// Board 위치
	Vector2 m_Pos;
	// 행, 열 , 한칸 크기
	int m_RowCount;
	int m_ColCount;
	Vector2 m_SquareSize;

	int m_StartX;
	int m_StartY;
	int m_EndX;
	int m_EndY;

	bool m_ClickFirst;
	bool m_ClickSecond;
	Vector2 m_ClickFirstPos;
	Vector2 m_ClickSecondPos;

	bool m_Start;
	Vector2 m_Resolution; // Width, Height

	/*
	std::list<CSharedPtr<CGameObject>> m_ObjList;
	int m_RenderCount;
	int m_RenderCapacity;
	CGameObject** m_RenderArray;
	*/
public :
	int GetRowCount() const
	{
		return m_RowCount;
	}
	int GetColCount() const
	{
		return m_ColCount;
	}
	class CCell* GetCell(const int &IndexRow, const int &IndexCol)
	{
		return m_vecCells[IndexRow * m_ColCount + IndexCol];
	}
	class CCell* GetCell(int Index)
	{
		return m_vecCells[Index];
	}
	class CBlock* GetBlock(const int& IndexRow, const int& IndexCol)
	{
		int Index = IndexRow * m_ColCount + IndexCol;

		return m_vecBlocks[IndexRow * m_ColCount + IndexCol];
	}
	class CBlock* GetBlock(int Index)
	{
		return m_vecBlocks[Index];
	}
	const Vector2& GetPos()
	{
		return m_Pos;
	}
	const Vector2& GetSize()
	{
		return m_Size;
	}

public :
	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}
	void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}
	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}
	void SetSize(float x, float y)
	{
		m_Size = Vector2(x, y);
	}
public :
	bool CreateBoard(int RowCount, int ColCount, const Vector2& SquareSize);
	void MouseLButton(float DeltaTime);
	void CompareClicks();
	void ReceiveClicks();
public:
	void Start();
	bool Init();
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Render(HDC hDC);
public :
	void RenderElementsInOrder(int order, HDC hdc);
	void ChangeUpperBlockStates(int RowIndex, int ColIndex);
};

