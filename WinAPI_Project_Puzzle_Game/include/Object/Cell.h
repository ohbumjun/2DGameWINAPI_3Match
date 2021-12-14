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
	class CBoard* m_Board;
	float				 m_MoveSpeed;
	float				 m_TimeScale;
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
public :
	CellType GetCellType()
	{
		return m_CellType;
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

