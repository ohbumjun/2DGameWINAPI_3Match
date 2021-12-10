#include "Stage.h"
#include "../Object/Board.h"

CStage::CStage(int row, int col) :
	m_Row(row),
	m_Col(col),
	m_StageLevel(0)
{
}

CStage::~CStage()
{
}

bool CStage::Init()
{
	m_Board = new CBoard;

	if (!m_Board->Init(m_Row, m_Col))
		return false;

	return true;
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

/*
void CStage::ComposeStage()
{
	m_Board->ComposeStage();
}
*/