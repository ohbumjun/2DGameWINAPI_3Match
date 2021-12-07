#include "Board.h"

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
