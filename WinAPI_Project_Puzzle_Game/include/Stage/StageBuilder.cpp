#include "StageBuilder.h"
#include "Stage.h"

CStageBuilder::CStageBuilder()
{
}

CStageBuilder::~CStageBuilder()
{
}

CStage* CStageBuilder::ComposeStage(int row, int col)
{
	// Stage ��ü ���� 
	CStage* stage = new CStage(this, row, col);

	// Cell, Block �ʱⰪ ����
	for (int r = 0; r < row; r++)
	{
		for (int c = 0; c < col; c++)
		{
			// 
		}
	}
	return nullptr;
}
