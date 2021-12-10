#pragma once

#include "../GameInfo.h"
#include "../Object/GameObject.h"

class CCell : public CGameObject
{
public :
	CCell();
	CCell(const CCell& Cell);
	~CCell();
private :
	CellType m_CellType;
	AnimalType m_AnimalType;
	Vector2     m_Dir;
	float		m_MoveSpeed;
	float		m_TimeScale;
public :
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
	bool Init();
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Render(HDC hDC);
};

