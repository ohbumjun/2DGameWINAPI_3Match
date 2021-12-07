#pragma once
#include "../GameInfo.h"

class CStageBuilder
{
public :
	CStageBuilder(int nStage);
	~CStageBuilder();
private :
	class CStage* m_Owner;
	int m_StageLevel;
public :
	void SetStageLevel(int Stage)
	{
		m_StageLevel = Stage;
	}
	void SetOwner(class CStage* stage)
	{
		m_Owner = stage;
	}
public :
	class CStage* ComposeStage(int row, int col);
	class CBlock* CreateBlock(int row, int col);
	class CCell* CreateCell(int row, int col);
public :
	static class CStage* Init(int nStage, int row, int col);
};

