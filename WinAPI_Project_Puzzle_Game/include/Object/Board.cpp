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

	// Cell, Block 檬扁蔼 积己
	m_Cells.reserve(m_Row);
	m_Blocks.reserve(m_Row);

	for (int r = 0; r < m_Row; r++)
	{
		// vector 积己 
		std::vector<CCell*> cV;
		m_Cells.push_back(cV);

		std::vector<CBlock*> bV;
		m_Blocks.push_back(bV);

		// 凯 积己 
		m_Cells[r].reserve(m_Col);
		m_Blocks[r].reserve(m_Col);
		for (int c = 0; c < m_Col; c++)
		{
			// Cell
			CCell* NewCell = CreateCell();
			NewCell->SetRowColPos(row, col);
			m_Cells[r].push_back(NewCell);

			// Block
			CBlock* NewBlock = CreateBlock();
			NewBlock->SetRowColPos(row, col);
			m_Blocks[r].push_back(NewBlock);
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

