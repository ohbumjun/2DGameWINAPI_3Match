#pragma once

#include "../GameInfo.h"
#include "GameObject.h"

class CBoard;

class CCell : public CGameObject
{
public :
	CCell();
	CCell(const CCell& Cell);
	~CCell();
private :
	CellType			 m_CellType;
	AnimalType     m_AnimalType;
	Vector2          m_Dir;
	Vector2          m_NewPos;
	class CBoard* m_Board;
	float				 m_MoveSpeed;
	float				 m_TimeScale;
	bool               m_NewlyCreated;
	std::vector<const char*> m_AnimalNames;
	BlockType		 m_BlockType;
	
public :
	void SetBoard(CBoard* Board)
	{
		m_Board = Board;
	}
	void SetCellType(CellType Type)
	{
		m_CellType = Type;
	}
	void SetCellInitInfo(const Vector2 Pos, const Vector2& Size, int RowIndex, int ColIndex, int Index)
	{
		m_Pos = Pos;
		m_Size = Size;
		m_RowIndex = RowIndex;
		m_ColIndex = ColIndex;
		m_Index = Index;
	}
	void SetNewlyCreated(bool Created)
	{
		m_NewlyCreated = Created;
	}
	void SetNewYPos(float XPos, float YPos)
	{
		m_NewPos = Vector2(XPos,YPos);
	}
	void SetIdxInfos(int RowIdx, int ColIdx, int Index)
	{
		m_RowIndex = RowIdx;
		m_ColIndex  = ColIdx;
		m_Index		= Index;
	}
public :
	CellType GetCellType()
	{
		return m_CellType;
	}
	float GetYPos() const
	{
		return m_Pos.y;
	}
public :
	virtual void Start() override;
	bool Init(AnimalType Type) ;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
	virtual void Render(HDC hDC) override;
	CCell* Clone();
	void Move(const Vector2& Dir);
	void Move(const Vector2& Dir, float Speed);
};

