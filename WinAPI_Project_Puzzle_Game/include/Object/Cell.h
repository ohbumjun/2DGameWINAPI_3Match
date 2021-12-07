#pragma once
#include "../GameInfo.h"

class CCell
{
public :
	CCell();
	~CCell();
private :
	CellType m_CellType;
public :
	void SetCellType(CellType Type)
	{
		m_CellType = Type;
	}
public :
	CellType GetCellType()
	{
		return m_CellType;
	}
};

