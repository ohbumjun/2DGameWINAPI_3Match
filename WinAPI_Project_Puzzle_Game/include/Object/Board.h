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
	// 초기 Block 개수, 및 Allocator  , Pos 정보 
	int m_BlockCount;
	int m_BlockCapacity;
	float m_TopYPos;
	float m_MiddelYPos;
	CCell** m_vecCells;
	CBlock** m_vecBlocks;
	std::vector<std::vector<int>> m_ChangedCellRowInfo;
	std::vector<int> m_NewCellNeeded;
	std::vector<std::vector<bool>> m_vecDestroyedCells;
	std::vector<std::vector<bool>> m_vecCellsPosChanged;
	std::vector<std::vector<bool>> m_vecCellsPosChangedEnd;

	// Block Texture
	CSharedPtr<CTexture> m_BlockTexture;
	CSharedPtr<CTexture> m_WhiteTexture;
	CSharedPtr<CTexture> m_ShuffleTexture;
	CSharedPtr<CTexture> m_NoticeTexture;

	// Board 전체 크기
	Vector2 m_Size;

	// Board 위치
	Vector2 m_Pos;

	// 행, 열 , 한칸 크기
	int m_RowCount;
	int m_ColCount;
	Vector2 m_SingleBlockSize;

	// Click
	bool m_ClickEnable;
	bool m_IsTwoMoving;
	int   m_Click;

	CCell* m_ClickFirstCell;
	Vector2 m_ClickFirstPos;
	int   m_ClickFirstIdxX; // 열
	int   m_ClickFirstIdxY; // 행 

	CCell* m_ClickSecCell;
	Vector2 m_ClickSecPos;
	int   m_ClickSecIdxX; // 열
	int   m_ClickSecIdxY; // 행

	Vector2 m_InitFirstCellDiff;
	Vector2 m_InitSecCellDiff;

	// Etc.
	bool m_Start;
	Vector2 m_Resolution; // Width, Height

	// Cell Match
	int m_DX[4]; // 열 
	int m_DY[4]; // 행
	bool m_PrevMisMatched;

	// Delay
	float m_DelayTime;

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
	int GetNewIndex(int RowIndex, int ColIndex)
	{
		return RowIndex * m_ColCount + ColIndex;
	}
	bool GetCellPosChanged(int RowIndex, int ColIndex)
	{
		return m_vecCellsPosChanged[RowIndex][ColIndex];
	}

public :
	void SetCell(int RowIndex, int ColIndex,  CCell* NewCell)
	{
		m_vecCells[RowIndex * m_ColCount + ColIndex] = NewCell;
	}
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
	void SetCellPosChangedComplete(int RowIndex, int ColIndex)
	{
		m_vecCellsPosChangedEnd[RowIndex][ColIndex] = true;
	}
public : // Init
	bool CreateBoard(int RowCount, int ColCount, const Vector2& SquareSize);
public : // Call Back
	void MouseLButton(float DeltaTime);
public : // Updates
	void CompareClicks();
	void ReceiveClicks();
	bool ChangeUpperCellsPos(int RowIndex, int ColIndex);
	bool ChangeUpperCellIdxInfo(int RowIndex, int ColIndex);
	bool ChangeCellsInfos();
	void RemoveCells();
	bool CheckRemoveEnable();
	void CreateNewCells();
	void ChangeCellYIdx(int RowIndex, int ColIndex);
	bool CheckUpdateEnable();
	void MoveTwoClickedCells(float DeltaTime);
public : // Check Logic
	bool CheckMatchCells();
	Vector2   GetOppositeDirection(int curDx, int curDy);
	bool CheckMatchPossible();
	bool MakeMatchableBoard(float DeltaTime);
	void ShuffleCells();
public : // Render
	void SortRenderObject(int Left, int Right, std::vector<CSharedPtr<CGameObject>>& RenderObjects);
	int   SortPartition(int Left, int Right, std::vector<CSharedPtr<CGameObject>>& RenderObjects);
public:
	void Start();
	bool Init();
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Render(HDC hDC);
};

