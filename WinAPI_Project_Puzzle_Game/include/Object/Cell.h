#pragma once

#include "../GameInfo.h"
#include "CellObject.h"

class CCell
{
public :
	CCell();
	~CCell();
private :
	CellType m_CellType;
	class CCellObject* m_CellObject;

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
public:
	CCellObject* CreateCellObject()
	{
		m_CellObject = new CCellObject;

		if (!m_CellObject->Init())
			return nullptr;

		return m_CellObject;
	}
};

