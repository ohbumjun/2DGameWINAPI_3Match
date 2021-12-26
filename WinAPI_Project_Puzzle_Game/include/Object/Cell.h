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
	bool               m_IsSwapping;
	bool               m_IsMoving;
	std::vector<const char*> m_AnimalNames;
	BlockType		 m_BlockType;
	
public :
	void SetIsMoving(bool Moving)
{
		m_IsMoving = Moving;
}
	void SetMoveSpeed(float MoveSpeed)
	{
		m_MoveSpeed = MoveSpeed;
	}
	void SetSwapping(bool IsSwap)
	{
		m_IsSwapping = IsSwap;
		if (IsSwap)
			m_IsMoving = true;
	}
	void SetBoard(const CBoard* Board)
	{
		m_Board = const_cast<CBoard*>(Board);
	}
	void SetCellType(const CellType Type)
	{
		m_CellType = Type;
	}
	void SetDir(const Vector2& Dir)
	{
		m_Dir = Dir;
	}
	void SetCellInitInfo(const Vector2 Pos, const Vector2& Size, int RowIndex, int ColIndex, int Index)
	{
		m_Pos = Pos;
		m_Size = Size;
		m_RowIndex = RowIndex;
		m_ColIndex = ColIndex;
		m_Index = Index;
	}
	void SetNewPos(float XPos, float YPos)
	{
		m_NewPos = Vector2(XPos,YPos);
	}
	void SetNewPos(Vector2 NewPos)
	{
		m_NewPos = NewPos;
	}
	void SetIdxInfos(int RowIdx, int ColIdx, int Index)
	{
		m_RowIndex = RowIdx;
		m_ColIndex  = ColIdx;
		m_Index		= Index;
	}
	void SetAnimalType(AnimalType Type); //

public :
	bool IsSwapping() const
	{
		return m_IsSwapping;
	}
	CellType GetCellType()
	{
		return m_CellType;
	}
	float GetYPos() const
	{
		return m_Pos.y;
	}
	Vector2 GetNewPos() const
	{
		return m_NewPos;
	}
	bool IsMoving() const
	{
		return m_IsMoving;
	}
	AnimalType GetAnimalType() const
	{
		return m_AnimalType;
	}
public :
	virtual void Start() override;
	bool Init(AnimalType Type) ;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
	virtual void Render(HDC hDC) override;
	CCell* Clone();
	bool Move(const Vector2& Dir);
	bool Move(const Vector2& Dir, float Speed);
};

