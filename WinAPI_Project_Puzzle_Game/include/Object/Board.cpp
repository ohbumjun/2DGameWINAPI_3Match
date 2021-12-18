#include "Board.h"
#include "../Input.h"
#include "../Resource/ResourceManager.h"
#include "../GameManager.h"

CBoard::CBoard() :
	m_RowCount(0),
	m_ColCount(0),
	m_EndX(-1),
	m_EndY(-1),
	m_StartX(-1),
	m_StartY(-1),
	m_ClickFirst(false),
	m_ClickSecond(false),
	m_ClickFirstPos{},
	m_ClickSecondPos{},
	m_Start(false),
	m_BlockCount(0),
	m_BlockCapacity(100)
{
	m_vecCells = new CCell * [m_BlockCapacity];
	m_vecBlocks = new CBlock * [m_BlockCapacity];
}

CBoard::~CBoard()
{
	for (int i = 0; i < m_BlockCount; i++)
	{
		SAFE_DELETE(m_vecCells[i]);
	}
	SAFE_DELETE_ARRAY(m_vecCells);

	for (int i = 0; i < m_BlockCount; i++)
	{
		SAFE_DELETE(m_vecBlocks[i]);
	}
	SAFE_DELETE_ARRAY(m_vecBlocks);
}

bool CBoard::CreateBoard(int RowCount, int ColCount, const Vector2& SquareSize)
{
	m_RowCount = RowCount;
	m_ColCount = ColCount;
	m_SquareSize = SquareSize;

	// ���� Board ���� 
	// m_vecCells.clear();
	// m_vecBlocks.clear();

	m_Size = m_SquareSize * Vector2((float)m_ColCount, (float)m_RowCount);
	m_BlockCount = m_RowCount * m_ColCount;

	// ũ�� ���Ҵ� 
	if (m_BlockCount > m_BlockCapacity)
	{
		m_BlockCapacity *= 2;

		CCell** n_vecCells      = new CCell * [m_BlockCapacity];
		CBlock** n_vecBlocks = new CBlock * [m_BlockCapacity];

		memcpy(n_vecCells, m_vecCells, sizeof(CCell*) * m_BlockCount);
		memcpy(n_vecBlocks, m_vecBlocks, sizeof(CCell*) * m_BlockCount);

		delete[] m_vecCells;
		delete[] m_vecBlocks;

		m_vecCells   = n_vecCells;
		m_vecBlocks = n_vecBlocks;
	}


	// ���⿡��, Random �ϰ� ���� �� �ִ� idx ���� �����ϴ� �Լ��� ȣ��
	// while ���� ������, 
	// 1) Set�� ���� ������, ã�� ���� , ã���� return 
	// 2) (��ȭ) ��� 1���� ���õ� �� �ֵ��� �����ϱ� 
	// �� ���� ���� idx ���� ���ʴ�� Cell�� �����ؼ� �ش� idx�� �´� Cell Type --> Texture �����ϱ� 


	// �⺻ Board Texture Loading 
	CTexture* BlockTexture = CResourceManager::GetInst()->FindTexture("BlockTexture");
	Vector2 StartOffset = Vector2(25.f, 0.f);
	int Index = -1;

	for (int r = 0; r < m_RowCount; r++)
	{
		for (int c = 0; c < m_ColCount; c++)
		{
			// ���� ��ġ
			// Vector (x,y) : x�� ��, y�� �� 
			Vector2 Pos = Vector2((float)c, (float)r) * m_SquareSize;

			// AnimType
			AnimalType Type = (AnimalType)(rand() % AnimalType::END);

			Index = r * m_ColCount + c;

			// Cell
			CCell* NewCell = new CCell;
			NewCell->Init(Type);
			NewCell->SetBoard(this);
			// SetCellInitInfo(const Vector2 Pos, const Vector2& Size, int RowIndex, int ColIndex, int Index)
			NewCell->SetCellInitInfo(Pos, m_SquareSize, r, c, Index);
			NewCell->SetSize(m_SquareSize);
			m_vecCells[Index] = NewCell; 

			// Block
			CBlock* NewBlock = new CBlock;
			NewBlock->Init(); // SetBlockInitInfo(const Vector2 Pos, const Vector2& Size, int RowIndex, int ColIndex, int Index, class CTexture* Texture)
			NewBlock->SetBoard(this);
			NewBlock->SetBlockInitInfo(Pos, m_SquareSize, r, c, Index, BlockTexture);
			NewBlock->SetSize(m_SquareSize);
			m_vecBlocks[Index] = NewBlock;
		}
	}
	
	return true;
}

void CBoard::MouseLButton(float DeltaTime)
{
	bool MouseDown = CInput::GetInst()->GetMouseDown();
	if (!MouseDown)
		return;
	Vector2 MousePos = CInput::GetInst()->GetMousePos();

	// Tile ����
	if (MousePos.x < 0.f || MousePos.x > m_Size.x || MousePos.y < 0.f || MousePos.y > m_Size.y)
		return;

	// ���õ� Block �� Idx 
	int BlockIdxX = (int)(MousePos.x / m_SquareSize.x); // �� 
	int BlockIdxY = (int)(MousePos.y / m_SquareSize.y); // �� 

	// Tile ������ �ȵǸ�, return ó�� 
	if (!m_ClickFirst)
	{
		m_ClickFirst = true;
		m_ClickFirstPos = MousePos;
	}
	else
	{
		m_ClickSecond = true;
		m_ClickSecondPos = MousePos;
		// ���õ� �༮ Block ���� �ٲٱ� 
		int SIndex = BlockIdxY * m_RowCount + BlockIdxX ;
		m_vecBlocks[SIndex]->SetBlockType(BlockType::EMPTY);
		m_vecBlocks[SIndex]->SetMoveEnable(true);

		m_ClickFirst = false;
	}


}

void CBoard::CompareClicks()
{
	if (!m_ClickFirst || !m_ClickSecond)
		return;
}

void CBoard::ReceiveClicks()
{
}

void CBoard::Start()
{
	if (!m_Start)
	{
		m_Start = true;
		CInput::GetInst()->SetCallback<CBoard>("MouseLButton", KeyState_Down, this, &CBoard::MouseLButton);
	}
}

bool CBoard::Init()
{
	// Load Basic Block Texture
	CResourceManager::GetInst()->LoadTexture("BlockTexture", TEXT("block.bmp"));
	m_BlockTexture = CResourceManager::GetInst()->FindTexture("BlockTexture");

	Vector2 TextureSize = Vector2(m_BlockTexture->GetWidth(), m_BlockTexture->GetHeight());
	CreateBoard(5,5, TextureSize);

	return true;
}

bool CBoard::Update(float DeltaTime)
{
	Start();

	if (m_BlockCount > 0)
	{
		for (size_t i = 0; i < m_BlockCount; i++)
		{
			if (!m_vecCells[i])
				continue;
			if (!m_vecCells[i]->IsActive())
			{
				SAFE_DELETE(m_vecCells[i]);
				continue;
			}
			m_vecCells[i]->Update(DeltaTime);
		}

		for (size_t i = 0; i < m_BlockCount; i++)
		{
		
			m_vecBlocks[i]->Update(DeltaTime);
		}
	}
	
	CompareClicks();
	ReceiveClicks();
	return true;
}

bool CBoard::PostUpdate(float DeltaTime)
{
	if (m_BlockCount > 0)
	{
		for (size_t i = 0; i < m_BlockCount; i++)
		{
			if (!m_vecCells[i])
				continue;
			if (!m_vecCells[i]->IsActive())
			{
				SAFE_DELETE(m_vecCells[i]);
				continue;
			}
			m_vecCells[i]->PostUpdate(DeltaTime);
		}

		for (size_t i = 0; i < m_BlockCount; i++)
		{
			m_vecBlocks[i]->PostUpdate(DeltaTime);
		}
	}

	return true;
}

bool CBoard::Render(HDC hDC)
{
	if (m_BlockCount > 0)
	{
		RenderElementsInOrder(1, hDC);
		RenderElementsInOrder(2, hDC);
	}

	return true;
}

void CBoard::RenderElementsInOrder(int order, HDC hDC)
{
	if (order == 1)
	{
		for (size_t i = 0; i < m_BlockCount; i++)
		{
			m_vecBlocks[i]->PrevRender();
			m_vecBlocks[i]->Render(hDC);
			// m_vecBlocks[i]->SetBoard(this);
		}
	}
	else if(order == 2)
	{
		for (size_t i = 0; i < m_BlockCount; i++)
		{
			if (!m_vecCells[i])
				continue;
			m_vecCells[i]->PrevRender();
			m_vecCells[i]->Render(hDC);
			// m_vecCells[i]->SetBoard(this);
		}
	}
}

void CBoard::ChangeUpperBlockStates(int RowIndex, int ColIndex)
{
	for (int i = RowIndex - 1; i >= 0; i--)
	{
		m_vecBlocks[i * m_ColCount + ColIndex]->SetMoveEnable(true);
	}
}
