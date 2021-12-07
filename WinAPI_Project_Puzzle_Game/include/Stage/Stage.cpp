#include "Stage.h"

CStage::CStage(CStageBuilder* builder, int row, int col) :
	m_Board(nullptr),
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
