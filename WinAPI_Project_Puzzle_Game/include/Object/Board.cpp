#include "Board.h"
#include "../Resource/ResourceManager.h"

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

	// ���� Board ���� 
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


	// ���⿡��, Random �ϰ� ���� �� �ִ� idx ���� �����ϴ� �Լ��� ȣ��
	// while ���� ������, 
	// 1) Set�� ���� ������, ã�� ���� , ã���� return 
	// 2) (��ȭ) ��� 1���� ���õ� �� �ֵ��� �����ϱ� 
	// �� ���� ���� idx ���� ���ʴ�� Cell�� �����ؼ� �ش� idx�� �´� Cell Type --> Texture �����ϱ� 


	// �⺻ Board Texture Loading 
	CTexture* BlockTexture = CResourceManager::GetInst()->FindTexture("BlockTexture");

	for (int r = 0; r < m_RowCount; r++)
	{
		for (int c = 0; c < m_ColCount; c++)
		{
			// ���� ��ġ
			Vector2 Pos = Vector2((float)r, (float)c)* m_SquareSize;

			// Cell
			CSharedPtr<CCell> NewCell = new CCell;
			NewCell->Init();
			// SetCellInitInfo(const Vector2 Pos, const Vector2& Size, int RowIndex, int ColIndex, int Index)
			NewCell->SetCellInitInfo(Pos, m_SquareSize, r, c, r * m_ColCount + c);
			m_vecCells.push_back(NewCell);

			// Block
			CSharedPtr<CBlock> NewBlock = new CBlock;
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
	m_BlockTexture = CResourceManager::GetInst()->FindTexture("BlockTexture");
	Vector2 TextureSize = Vector2(m_BlockTexture->GetWidth(), m_BlockTexture->GetHeight());
	// CreateBoard(5,5,);
	return true;
}

bool CBoard::Update(float DeltaTime)
{
	if (!m_vecCells.empty())
	{
		size_t vecCellSize = m_vecCells.size();
		for (size_t i = 0; i < vecCellSize; i++)
		{
			m_vecCells[i]->Update(DeltaTime);
		}
	}

	if (!m_vecBlocks.empty())
	{
		size_t vecBlockSize = m_vecBlocks.size();
		for (size_t i = 0; i < vecBlockSize; i++)
		{
			m_vecBlocks[i]->Update(DeltaTime);
		}
	}

	return true;
}

bool CBoard::PostUpdate(float DeltaTime)
{
	if (!m_vecCells.empty())
	{
		size_t vecCellSize = m_vecCells.size();
		for (size_t i = 0; i < vecCellSize; i++)
		{
			m_vecCells[i]->PostUpdate(DeltaTime);
		}
	}

	if (!m_vecBlocks.empty())
	{
		size_t vecBlockSize = m_vecBlocks.size();
		for (size_t i = 0; i < vecBlockSize; i++)
		{
			m_vecBlocks[i]->PostUpdate(DeltaTime);
		}
	}

	return true;
}

bool CBoard::Render(HDC hDC)
{
	if (!m_vecCells.empty())
	{
		size_t vecCellSize = m_vecCells.size();
		for (size_t i = 0; i < vecCellSize; i++)
		{
			m_vecCells[i]->Render(hDC);
		}
	}

	if (!m_vecBlocks.empty())
	{
		size_t vecBlockSize = m_vecBlocks.size();
		for (size_t i = 0; i < vecBlockSize; i++)
		{
			m_vecBlocks[i]->Render(hDC);
		}
	}

	return true;
}
