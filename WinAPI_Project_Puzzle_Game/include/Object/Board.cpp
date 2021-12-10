#include "Board.h"
#include "Cell.h"
#include "../Resource/ResourceManager.h"
#include "Block.h"

CBoard::CBoard() :
	m_RowCount(0),
	m_ColCount(0),
	m_vecCells{},
	m_vecBlocks{}
{
}

CBoard::~CBoard()
{
	size_t Size = m_vecCells.size();
	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecCells[i]);
	}
	m_vecCells.clear();

	Size = m_vecBlocks.size();
	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecBlocks[i]);
	}
	m_vecBlocks.clear();
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


bool CBoard::CreateBoard(int RowCount, int ColCount, const Vector2& SquareSize)
{
	m_RowCount = RowCount;
	m_ColCount = ColCount;
	m_SquareSize = SquareSize;

	// 기존 Board 제거 
	{
		auto iter = m_vecCells.begin();
		auto iterEnd = m_vecCells.end();
		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE((*iter));
		}
		m_vecCells.clear();
	}
	{
		auto iter = m_vecBlocks.begin();
		auto iterEnd = m_vecBlocks.end();
		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE((*iter));
		}
		m_vecBlocks.clear();
	}

	m_Size = m_SquareSize * Vector2((float)m_RowCount, (float)m_ColCount);
	int Size = m_RowCount * m_ColCount;
	m_vecCells.reserve(Size);
	m_vecBlocks.reserve(Size);


	// 여기에서, Random 하게 섞일 수 있는 idx 들을 마련하는 함수를 호출
	// while 문을 돌려서, 
	// 1) Set가 없을 때까지, 찾은 다음 , 찾으면 return 
	// 2) (심화) 적어도 1개는 세팅될 수 있도록 세팅하기 
	// 그 다음 구한 idx 들을 차례대로 Cell에 세팅해서 해당 idx에 맞는 Cell Type --> Texture 세팅하기 


	// 기본 Board Texture Loading 
	CTexture* BlockTexture = CResourceManager::GetInst()->FindTexture("BlockTexture");

	for (int r = 0; r < m_RowCount; r++)
	{
		for (int c = 0; c < m_ColCount; c++)
		{
			// 현재 위치
			Vector2 Pos = Vector2((float)r, (float)c)* m_SquareSize;

			// Cell
			CCell* NewCell = new CCell;
			NewCell->Init();
			// SetCellInitInfo(const Vector2 Pos, const Vector2& Size, int RowIndex, int ColIndex, int Index)
			NewCell->SetCellInitInfo(Pos, m_SquareSize, r, c, r * m_ColCount + c);
			m_vecCells.push_back(NewCell);

			// Block
			CBlock* NewBlock = new CBlock;
			NewBlock->Init(); // SetBlockInitInfo(const Vector2 Pos, const Vector2& Size, int RowIndex, int ColIndex, int Index, class CTexture* Texture)
			NewBlock->SetBlockInitInfo(Pos, m_SquareSize, r, c, r * m_ColCount + c, BlockTexture);
			m_vecBlocks.push_back(NewBlock);
		}
	}
	return true;
}

bool CBoard::Init()
{
	// Load Basic Block Texture
	CResourceManager::GetInst()->LoadTexture("BlockTexture", TEXT("block.bmp"));
	return true;
}

bool CBoard::Update(float DeltaTime)
{

	return false;
}

bool CBoard::PostUpdate(float DeltaTime)
{

	return true;
}

bool CBoard::Render(HDC hDC)
{

	return true;
}
