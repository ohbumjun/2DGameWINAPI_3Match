#pragma once

#include "../GameInfo.h"
#include "../Ref.h"

class CCell : public CRef
{
public :
	CCell();
	~CCell();
private :
	CellType m_CellType;
	Vector2 m_RowColPos; 
	AnimalType m_AnimalType;
public :
	void SetCellType(CellType Type)
	{
		m_CellType = Type;
	}
	void SetRowColPos(int Row, int Col)
	{
		m_RowColPos = Vector2((float)Row, (float)Col);
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

