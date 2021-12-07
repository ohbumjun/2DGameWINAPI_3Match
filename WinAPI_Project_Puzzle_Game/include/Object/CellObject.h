#pragma once

#include "../GameInfo.h"


class CCellObject 
{
public :
	CCellObject();
	~CCellObject();
	Vector2 m_Pos;
private :
	class CCellBehaviour* m_CellBehaviour;
public :
	void SetPos(int col, int row)
	{
		m_Pos = Vector2((float)col, (float)row);
	}
public :
	bool Init();
};

