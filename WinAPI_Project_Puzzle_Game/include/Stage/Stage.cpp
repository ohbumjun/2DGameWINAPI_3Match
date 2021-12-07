#include "Stage.h"
#include "../Object/Board.h"

CStage::CStage(CStageBuilder* builder, int row, int col)  :
	m_Col(0),
	m_Row(0)
{
	m_Builder = builder;
	
}

CStage::~CStage()
{
}

bool CStage::Init()
{
	m_Board = new CBoard;
	return false;
}

bool CStage::Update(float DeltaTime)
{
	return false;
}

bool CStage::PostUpdate(float DeltaTime)
{
	return false;
}

bool CStage::Render(HDC hDC)
{
	return false;
}

void CStage::ComposeStage()
{
	m_Board->ComposeStage();
}
