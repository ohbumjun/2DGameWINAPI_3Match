#include "Board.h"
#include "Cell.h"
#include "Block.h"

CBoard::CBoard() :
	m_Row(0),
	m_Col(0),
	m_Cells{},
	m_Blocks{}
{
}

CBoard::~CBoard()
{
}

void CBoard::SetCell(CCell* Cell, int row, int col)
{
	m_Cells[row][col] = Cell;
}

void CBoard::SetBlock(CBlock* Block, int row, int col)
{
	m_Blocks[row][col] = Block;
}

void CBoard::ComposeStage()
{
	float InitX = CalcInitX(0.5f); // Col
	float InitY = CalcInitY(0.5f); // Row

	for (int row = 0; row < m_Row; row++)
	{
		for (int col = 0; col < m_Col; col++)
		{
			CCellObject* Cell = m_Cells[row][col]->CreateCellObject();
			Cell->SetPos(InitY + row, InitX + col);
		}
	}
}

float CBoard::CalcInitX(float offset)
{
	return -m_Col / 2.0f + offset;
}

float CBoard::CalcInitY(float offset)
{
	return -m_Row / 2.0f + offset;
}
