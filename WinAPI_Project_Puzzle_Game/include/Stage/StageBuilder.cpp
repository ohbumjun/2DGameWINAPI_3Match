#include "StageBuilder.h"
#include "Stage.h"
#include "../Object/Block.h"
#include "../Object/Cell.h"

CStageBuilder::CStageBuilder(int nStage) :
	m_Owner(nullptr)
{
	m_StageLevel = nStage;
}

CStageBuilder::~CStageBuilder()
{
}

CStage* CStageBuilder::ComposeStage(int row, int col)
{
	// Stage ��ü ���� 
	CStage* stage = new CStage(this, row, col);

	// Stage ��ü �ʱ�ȭ 
	stage->Init();

	// Cell, Block �ʱⰪ ����
	for (int r = 0; r < row; r++)
	{
		for (int c = 0; c < col; c++)
		{
			stage->GetBoard()->SetCell(CreateCell(r,c), r, c);
			stage->GetBoard()->SetBlock(CreateBlock(r,c), r, c);
		}
	}
	// Owner ����
	m_Owner = stage;
	stage->SetBuilder(this);

	return stage;
}

CBlock* CStageBuilder::CreateBlock(int row, int col)
{
	return new CBlock;
}

CCell* CStageBuilder::CreateCell(int row, int col)
{
	return new CCell;
}

CStage* CStageBuilder::Init(int nStage, int row, int col)
{
	CStageBuilder* m_StageBuilder = new CStageBuilder(0); // 0�� ���� 
	CStage* Stage = m_StageBuilder->ComposeStage(row, col);
	
	return Stage;
}
