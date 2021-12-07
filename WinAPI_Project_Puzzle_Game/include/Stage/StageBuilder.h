#pragma once
#include "../GameInfo.h"

class CStageBuilder
{
public :
	CStageBuilder();
	~CStageBuilder();
private :
	int m_StageLevel;
public :
	void SetStageLevel(int Stage)
	{
		m_StageLevel = Stage;
	}
public :
	class CStage* ComposeStage(int row, int col);

};

