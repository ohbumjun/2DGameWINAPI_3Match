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
	for (int i = 0; i < m_Row; i++)
	{
		for (int j = 0; j < m_Col; j++)
		{
			SAFE_DELETE(m_Cells[i][j]);
			SAFE_DELETE(m_Blocks[i][j]);
		}
	}
}

void CBoard::SetCell(CCell* Cell, int row, int col)
{
	m_Cells[row][col] = Cell;
	m_Cells[row][col]->SetRowColPos(row, col);
}

void CBoard::SetBlock(CBlock* Block, int row, int col)
{
	m_Blocks[row][col] = Block;
	m_Blocks[row][col]->SetRowColPos(row, col);
}

/*
void CBoard::ComposeStage()
{
	float InitX = CalcInitX(0.5f); // Col
	float InitY = CalcInitY(0.5f); // Row

	for (int row = 0; row < m_Row; row++)
	{
		for (int col = 0; col < m_Col; col++)
		{
			m_Cells[row][col]->SetRowColPos(InitY + row, InitX + col);
			m_Blocks[row][col]->SetRowColPos(InitY + row, InitX + col);
		}
	}
}
*/

float CBoard::CalcInitX(float offset)
{
	return -m_Col / 2.0f + offset;
}

float CBoard::CalcInitY(float offset)
{
	return -m_Row / 2.0f + offset;
}

bool CBoard::Init(int row, int col)
{
	m_Row = row;
	m_Col = col;
	// Cell, Block 초기값 생성
	for (int r = 0; r < m_Row; r++)
	{
		for (int c = 0; c < m_Col; c++)
		{
			SetCell(CreateCell(), r, c);
			SetBlock(CreateBlock(), r, c);
		}
	}
	return true;
}

bool CBoard::Update(float DeltaTime)
{
	for (int row = 0; row < m_Row; row++)
	{
		for (int col = 0; col < m_Col; col++)
		{
			m_Cells[row][col]->SetRowColPos(row, col);
			m_Blocks[row][col]->SetRowColPos(row, col);
		}
	}
	return false;
}

bool CBoard::PostUpdate(float DeltaTime)
{
	for (int row = 0; row < m_Row; row++)
	{
		for (int col = 0; col < m_Col; col++)
		{
			m_Cells[row][col]->PostUpdate(DeltaTime);
			m_Blocks[row][col]->PostUpdate(DeltaTime);
		}
	}
	return true;
}

bool CBoard::Render(HDC hDC)
{
	for (int row = 0; row < m_Row; row++)
	{
		for (int col = 0; col < m_Col; col++)
		{
			m_Cells[row][col]->Render(hDC);
			m_Blocks[row][col]->Render(hDC);
		}
	}
	return true;
}

CBlock* CBoard::CreateBlock()
{
	return new CBlock;
}

CCell* CBoard::CreateCell()
{
	return new CCell;
}

