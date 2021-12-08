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
	// Stage 객체 생성 
	CStage* stage = new CStage(this, row, col);

	// Stage 객체 초기화 
	stage->Init();

	// Cell, Block 초기값 생성
	for (int r = 0; r < row; r++)
	{
		for (int c = 0; c < col; c++)
		{
			stage->GetBoard()->SetCell(CreateCell(r,c), r, c);
			stage->GetBoard()->SetBlock(CreateBlock(r,c), r, c);
		}
	}
	// Owner 세팅
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
	CStageBuilder* m_StageBuilder = new CStageBuilder(0); // 0번 레벨 
	CStage* Stage = m_StageBuilder->ComposeStage(row, col);
	
	return Stage;
}
