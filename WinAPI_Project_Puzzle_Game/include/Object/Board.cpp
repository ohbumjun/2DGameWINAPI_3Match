#include "Board.h"
#include "../Input.h"
#include "../Resource/ResourceManager.h"
#include "GameObject.h"
#include "../GameManager.h"

CBoard::CBoard() :
	m_RowCount(0),
	m_ColCount(0),
	m_Click(0),
	m_ClickFirstPos{},
	m_ClickSecPos{},
	m_Start(false),
	m_BlockCount(0),
	m_BlockCapacity(50),
	m_ChangedCellRowInfo{},
	m_IsTwoMoving(false),
	m_InitFirstCellDiff{},
	m_InitSecCellDiff{},
	m_DX{ -1, 1, 0, 0 },
	m_DY{ 0, 0, -1, 1 },
	m_ClickFirstCell(nullptr),
	m_ClickSecCell(nullptr),
	m_PrevMisMatched(false),
	m_DelayTime(1000.f)
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
	// ���� Board ���� 
	// m_vecCells.clear();
	// m_vecBlocks.clear();

	// ���� �ʱ�ȭ 
	m_RowCount = RowCount * 2; // Virtual ���� ���ļ� 
	m_ColCount = ColCount;
	m_SingleBlockSize = SquareSize;

	m_Size = m_SingleBlockSize * Vector2((float)m_ColCount, (float)m_RowCount);
	m_BlockCount = (m_RowCount)*m_ColCount;

	// ũ�� ���Ҵ� 
	if (m_BlockCount > m_BlockCapacity)
	{
		m_BlockCapacity *= 2;

		CCell** n_vecCells = new CCell * [m_BlockCapacity];
		CBlock** n_vecBlocks = new CBlock * [m_BlockCapacity];

		memcpy(n_vecCells, m_vecCells, sizeof(CCell*) * m_BlockCount);
		memcpy(n_vecBlocks, m_vecBlocks, sizeof(CCell*) * m_BlockCount);

		delete[] m_vecCells;
		delete[] m_vecBlocks;

		m_vecCells = n_vecCells;
		m_vecBlocks = n_vecBlocks;
	}

	// ���⿡��, Random �ϰ� ���� �� �ִ� idx ���� �����ϴ� �Լ��� ȣ��
	// while ���� ������, 
	// 1) Set�� ���� ������, ã�� ���� , ã���� return 
	// 2) (��ȭ) ��� 1���� ���õ� �� �ֵ��� �����ϱ� 
	// �� ���� ���� idx ���� ���ʴ�� Cell�� �����ؼ� �ش� idx�� �´� Cell Type --> Texture �����ϱ� 

	// Cell, Block ���� 
	Vector2 StartOffset = Vector2(0, -m_Size.y / 2.f);
	// Vector2 StartOffset = Vector2(0.f, 0.f);

	AnimalType Type = (AnimalType)0;
	m_TopYPos = StartOffset.y;
	m_MiddelYPos = 0.f;
	int Index = -1;
	int ObjectNums = 0;

	for (int r = 0; r < m_RowCount; r++)
	{
		for (int c = 0; c < m_ColCount; c++)
		{
			// Vector (x,y) : x�� ��, y�� �� 
			Vector2 Pos = Vector2((float)c, (float)r) * m_SingleBlockSize + StartOffset;
			// Vector2 Pos = Vector2((float)c, (float)r) * m_SingleBlockSize + 0;

			Type = (AnimalType)(rand() % AnimalType::END);
			Index = r * m_ColCount + c;

			// Upper Virtual Block
			CBlock* NewBlock = new CBlock;
			NewBlock->Init();
			NewBlock->SetBoard(this);
			NewBlock->SetSize(m_SingleBlockSize);
			// NewBlock->SetYIdx(2);
			NewBlock->SetYIdx(0);
			m_vecBlocks[Index] = NewBlock;
			NewBlock->SetBlockInitInfo(Pos, m_SingleBlockSize, r, c, Index, m_WhiteTexture);

			// Cell
			CCell* NewCell = new CCell;
			NewCell->Init(Type);
			NewCell->SetBoard(this);
			NewCell->SetCellInitInfo(Pos, m_SingleBlockSize, r, c, Index);
			NewCell->SetSize(m_SingleBlockSize);
			NewCell->SetYIdx(1);
			NewCell->SetNewPos(Pos.x, Pos.y);
			//  NewCell->SetMoveSpeed(m_SingleBlockSize.x * 2);
			m_vecCells[Index] = NewCell;

			// DownWard Real Block
			if (r >= m_RowCount / 2)
			{
				NewBlock->SetTexture(m_BlockTexture);
				NewBlock->SetYIdx(0);

				// ȭ�鿡 �������� Board ������ NewCell YIdx 
				NewCell->SetYIdx(1);
			}
		}
	}

	return true;
}

void CBoard::MouseLButton(float DeltaTime)
{
	bool UpdateEnable = CheckUpdateEnable();
	if (!UpdateEnable)
		return;
	Vector2 MousePos = CInput::GetInst()->GetMousePos();

	// Tile ����
	if (MousePos.x < 0.f || MousePos.x > m_Size.x || MousePos.y < 0.f || MousePos.y > m_Size.y / 2)
		return;

	// Cell �� ���ֺ��� Test Code 
	// int Col   = (int)(MousePos.x / m_SingleBlockSize.x); // �� 
	// int Row = (int)(MousePos.y / m_SingleBlockSize.y) + (m_RowCount / 2); // �� 
	// m_vecBlocks[Row * m_ColCount + Col]->SetBlockType(BlockType::EMPTY);

	// Tile ������ �ȵǸ�, return ó�� 
	if (m_Click == 0)
	{
		// ���õ� Block �� Idx 
		m_ClickFirstIdxX = (int)(MousePos.x / m_SingleBlockSize.x); // �� 
		m_ClickFirstIdxY = (int)(MousePos.y / m_SingleBlockSize.y) + (m_RowCount / 2); // �� 
		
		m_ClickFirstPos = m_vecBlocks[m_ClickFirstIdxY * m_ColCount + m_ClickFirstIdxX]->GetPos();

		m_Click = 1;
	}
	else if (m_Click == 1)
	{
		// ���õ� Block �� Idx 
		m_ClickSecIdxX = (int)(MousePos.x / m_SingleBlockSize.x); // �� 
		m_ClickSecIdxY = (int)(MousePos.y / m_SingleBlockSize.y) + (m_RowCount / 2); // �� 

		/*
		{
			// Debug �� �ڵ� --> ������ �ָ� Ŭ���� ������ ���ο� Animal Type���� �ٲ��ֱ�
			if (m_ClickFirstIdxX == m_ClickSecIdxX && m_ClickFirstIdxY == m_ClickSecIdxY)
			{
				CCell* CurrentCell = m_vecCells[m_ClickFirstIdxY * m_ColCount + m_ClickFirstIdxX];
				CurrentCell->SetAnimalType((AnimalType)(rand() % (int)(AnimalType::END)));
				m_Click = 0;
				CheckMatchCells();
				return;
			}
		}
		*/

		m_ClickSecPos = m_vecBlocks[m_ClickSecIdxY * m_ColCount + m_ClickSecIdxX]->GetPos();
	
		if (abs(m_ClickFirstIdxX - m_ClickSecIdxX) + abs(m_ClickFirstIdxY - m_ClickSecIdxY) <= 1)
		{
			// Move ó��
			m_IsTwoMoving = true;

			// ���õ� �༮ Block ���� �ٲٱ� 
			int FirstCellIdx = m_ClickFirstIdxY * m_ColCount + m_ClickFirstIdxX;
			int SecCellIdx  = m_ClickSecIdxY * m_ColCount + m_ClickSecIdxX;
	
			// New Pos ���� 
			m_vecCells[FirstCellIdx]->SetNewPos(m_ClickSecPos);
			m_vecCells[SecCellIdx]->SetNewPos(m_ClickFirstPos);
			
			// Dir ���� 
			Vector2 FirstCellDir = m_ClickSecPos - m_ClickFirstPos;
			FirstCellDir.Normalize();
			Vector2 SecCellDir  = m_ClickFirstPos - m_ClickSecPos;
			SecCellDir.Normalize();

			m_vecCells[FirstCellIdx]->SetDir(FirstCellDir);
			m_vecCells[FirstCellIdx]->SetSwapping(true);

			m_vecCells[SecCellIdx]->SetDir(SecCellDir);
			m_vecCells[SecCellIdx]->SetSwapping(true);

			m_ClickFirstCell = m_vecCells[FirstCellIdx];
			m_ClickSecCell  = m_vecCells[SecCellIdx];

			m_InitFirstCellDiff = m_vecCells[FirstCellIdx]->GetNewPos() - m_vecCells[FirstCellIdx]->GetPos();
			m_InitSecCellDiff  = m_vecCells[SecCellIdx]->GetNewPos() - m_vecCells[SecCellIdx]->GetPos();

			m_Click = 0;
		}
		else
		{
			// ���� ���� Cell Ŭ�� 
			m_Click = 0;
			m_IsTwoMoving = false;
			m_ClickFirstCell = nullptr;
			m_ClickSecCell  = nullptr;
		}
	}

	// m_vecBlocks[SIndex]->SetBlockType(BlockType::EMPTY);
	// m_vecBlocks[SIndex]->SetMoveEnable(true);
}

void CBoard::CompareClicks()
{
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
	CResourceManager::GetInst()->LoadTexture("WhiteTexture", TEXT("white.bmp"));
	CResourceManager::GetInst()->LoadTexture("ShuffleTexture", TEXT("shuffle.bmp"));
	CResourceManager::GetInst()->LoadTexture("NoticeTexture", TEXT("notice.bmp"));
	m_BlockTexture = CResourceManager::GetInst()->FindTexture("BlockTexture");
	m_WhiteTexture = CResourceManager::GetInst()->FindTexture("WhiteTexture");
	m_ShuffleTexture = CResourceManager::GetInst()->FindTexture("ShuffleTexture");
	m_NoticeTexture = CResourceManager::GetInst()->FindTexture("NoticeTexture");

	// Board �����ϱ� 
	Vector2 TextureSize = Vector2((float)m_BlockTexture->GetWidth(), (float)m_BlockTexture->GetHeight());
	CreateBoard(4, 4, TextureSize);

	// �ʱ�ȭ
	m_ChangedCellRowInfo.reserve(m_RowCount);
	for (int i = 0; i < m_RowCount; i++)
	{
		std::vector<int> vec(m_ColCount, 0);
		m_ChangedCellRowInfo.push_back(vec);
	}

	// ���� ������ Cell ���� ����
	m_NewCellNeeded.resize(m_ColCount);

	// Destroyed Cells
	m_vecDestroyedCells.reserve(m_RowCount);
	for (int i = 0; i < m_RowCount; i++)
	{
		std::vector<bool> vec(m_ColCount, false);
		m_vecDestroyedCells.push_back(vec);
	}

	// ��ġ �ٲ� Cell �� 
	m_vecCellsPosChanged.reserve(m_RowCount);
	for (int i = 0; i < m_RowCount; i++)
	{
		std::vector<bool> vec(m_ColCount, false);
		m_vecCellsPosChanged.push_back(vec);
	}

	// ��ġ �ٲ� Cell �� ���� ���� 
	m_vecCellsPosChangedEnd.reserve(m_RowCount);
	for (int i = 0; i < m_RowCount; i++)
	{
		std::vector<bool> vec(m_ColCount, false);
		m_vecCellsPosChangedEnd.push_back(vec);
	}

	// Ȥ�� �𸣴�, �ʱ� Board�� Matchable�� Board�� �ǵ��� �����Ѵ�.
	MakeMatchableBoard();

	return true;
}

bool CBoard::Update(float DeltaTime)
{
	Start();
	int Index = -1;

	if (m_BlockCount > 0)
	{
		// 1) ����� Block ǥ���ϱ� ( �˰��� �����ϱ� )
		bool UpdateEnable = CheckRemoveEnable();

		// ��� Cell ���� �������� ��, �� �������� Cell ���� Match �ϴ��� �˻� + ����
		bool IsMatch = true;
		if (UpdateEnable)
		{
			// Match �Ǵ� Cell ���� �ִ��� üũ�Ѵ�. -> Match �Ǵ�Cell ���� ���� ǥ���صд�.
			bool IsMatch = CheckMatchCells();

			// Match �Ǵ� �ְ� ���ٸ� --> �ش� ���� ������ ������ üũ�Ѵ�.
			// �ش� �Լ� �ȿ���, Match ������ ���� �����ϵ��� ���� ������ ���̴�.
			// 1) ����, Match ������ Cell ���� ����� �Ѵ�
			// 2) �׷��鼭, Match ������ Cell ���� �ֵ�
			// 3) ���� Match �Ǵ� Cell ���� ���� ������ Shuffle�� ������.
			if (!IsMatch)
				MakeMatchableBoard();
		}


		// Cells Idx ��ȭ ���� �ʱ�ȭ 
		for (int row = 0; row < m_RowCount; row++)
		{
			for (int col = 0; col < m_ColCount; col++)
			{
				m_ChangedCellRowInfo[row][col] = 0;
			}
		}

		// 2) ���� ������� �ϱ� + New Pos ���� + �������� Real Board�� �˻� 
		RemoveCells();

		// ���� Update
		for (int R = m_RowCount - 1; R >= 0; R--)
		{
			for (int C = m_ColCount - 1; C >= 0; C--)
			{
				Index = R * m_ColCount + C;

				if (!m_vecCells[Index])
					continue;

				// Update 
				m_vecCells[Index]->Update(DeltaTime);

				// Update Cell YIdx 
				ChangeCellYIdx(R, C);
			}
		}

		// 3) ���ο� Idx , ���� ���� ����
		ChangeCellsInfos();

		// 4) ���ο� Cell ����
		CreateNewCells();

		// 5) Ŭ���� 2�� Cell �� ���� �̵�
		if (m_ClickFirstCell && m_ClickSecCell)
		{
			MoveTwoClickedCells(DeltaTime);
		}

		for (size_t i = 0; i < m_BlockCount; i++)
		{
			m_vecBlocks[i]->Update(DeltaTime);
		}
	}

	return true;
}

bool CBoard::PostUpdate(float DeltaTime)
{
	if (m_BlockCount > 0)
	{
		int Index = -1;

		// for (int R = 0; R < m_RowCount; R++)
		for (int R = m_RowCount - 1; R >= 0; R--)
		{
			for (int C = m_ColCount - 1; C >= 0; C--)
			{
				Index = R * m_ColCount + C;

				if (!m_vecCells[Index])
					continue;
				m_vecCells[Index]->PostUpdate(DeltaTime);
			}
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
	// ��� Block, Cell���� ä���� ���� Render
	if (m_BlockCount > 0)
	{
		int RenderLength;
		std::vector<CSharedPtr<CGameObject>> RenderObjects;

		RenderObjects.reserve(m_BlockCount * 2);

		for (int i = 0; i < m_BlockCount; i++)
		{
			if (m_vecBlocks[i])
			{
				m_vecBlocks[i]->AddRef();
				RenderObjects.push_back(m_vecBlocks[i]);
			}
			if (m_vecCells[i])
			{
				m_vecCells[i]->AddRef();
				RenderObjects.push_back(m_vecCells[i]);
			}
		}

		RenderLength = (int)RenderObjects.size();

		// Sort
		RenderLength = (int)RenderObjects.size();
		SortRenderObject(0, RenderLength - 1, RenderObjects);

		for (int i = 0; i < RenderLength; i++)
		{
			RenderObjects[i]->PrevRender();
			RenderObjects[i]->Render(hDC);
		}
	}

	return true;
}


void CBoard::RemoveCells()
{
	int Index = -1;
	for (int R = m_RowCount - 1; R >= 0; R--)
	{
		for (int C = m_ColCount - 1; C >= 0; C--)
		{
			Index = R * m_ColCount + C;

			CCell* Cell = m_vecCells[Index];
			if (!Cell)
				continue;

			// Idx ������ �ٲ� ���Ŀ���
			// ���Ӱ� ���õ� Cell ���� Active�� False�� ���õǾ������

			if (!m_vecCells[Index]->IsActive())
			{
				// �޸� ����
				SAFE_DELETE(m_vecCells[Index]);

				ChangeUpperCellsPos(R, C);

				ChangeUpperCellIdxInfo(R, C);

			}
		}
	}
}

bool CBoard::CheckRemoveEnable()
{
	int N = 0;
	bool AllChangedComplete = true;

	for (int r = 0; r < m_RowCount; r++)
	{
		for (int c = 0; c < m_ColCount; c++)
		{
			if (m_vecCellsPosChanged[r][c])
			{
				N += 1;
			}
		}
	}

	// ��ġ�� ���� Cell�� �ϳ��� ���ٸ�,
	// ���ʿ�, Remove�� ���� �ʿ䰡 ����.
	if (N == 0)
		return false;

	// �̵� �Ϸ� ���� �˻� 
	for (int r = 0; r < m_RowCount; r++)
	{
		for (int c = 0; c < m_ColCount; c++)
		{
			// ����
			if (m_vecCellsPosChanged[r][c])
			{
				// �̵� �Ϸ� x
				if (!m_vecCellsPosChangedEnd[r][c])
				{
					return false;
				}
			}
		}
	}

	// ������� �������� ���� ��� Cell �� �̵� �Ϸ� --> �ʱ�ȭ
	for (int r = 0; r < m_RowCount; r++)
	{
		for (int c = 0; c < m_ColCount; c++)
		{
			m_vecCellsPosChanged[r][c] = false;
			m_vecCellsPosChangedEnd[r][c] = false;
		}
	}

	return true;
}

bool CBoard::ChangeUpperCellsPos(int RowIndex, int ColIndex)
{
	int CurIdx = -1;
	for (int Row = RowIndex - 1; Row >= 0; Row--)
	{
		CurIdx = Row * m_ColCount + ColIndex;

		// Pos ���� �ٲٱ� 
		// Vector2 PrevPos = m_vecCells[CurIdx]->GetPos(); X --> �ǽð� New Pos �ٰ� Update
		Vector2 PrevPos = m_vecCells[CurIdx]->GetNewPos();
		float NewYPos = PrevPos.y + m_SingleBlockSize.y;
		m_vecCells[CurIdx]->SetNewPos(PrevPos.x, NewYPos);

	}

	return true;
}

bool CBoard::ChangeUpperCellIdxInfo(int RowIndex, int ColIndex)
{
	int NxtIdx = -1, CurIdx = -1;

	// ���� �����ؾ��� Cell ���� ���� 
	m_NewCellNeeded[ColIndex] += 1;

	// Cell�� ���� Idx ������ �������ش�.--> ChangeCellsIdx ���� �ش� ������ �°� ���� ����
	for (int Row = RowIndex - 1; Row >= 0; Row--)
	{
		m_ChangedCellRowInfo[Row][ColIndex] += 1;
	}
	return true;
}

bool CBoard::ChangeCellsInfos()
{
	int CurIdx = -1, NxtIdx = -1, AddedRow = -1;

	for (int Row = m_RowCount - 1; Row >= 0; Row--)
	{
		for (int Col = m_ColCount - 1; Col >= 0; Col--)
		{
			CurIdx = Row * m_ColCount + Col;

			if (!m_vecCells[CurIdx])
				continue;

			if (m_ChangedCellRowInfo[Row][Col] == 0)
				continue;

			// ���� �Ʒ��� �������� ���� 
			// m_vecCells[CurIdx]->SetDir(Vector2(0.f, 1.f));

			// ���ο� Idx ���ϱ� 
			AddedRow = m_ChangedCellRowInfo[Row][Col];
			NxtIdx = (Row + AddedRow) * m_ColCount + Col;

			// Idx ���� �ٽ� �������ֱ� 
			m_vecCells[CurIdx]->SetIdxInfos(Row + AddedRow, Col, NxtIdx);

			bool Active = m_vecCells[CurIdx]->IsActive();

			// �迭 �� Cell ���� �ٲٱ�
			m_vecCells[NxtIdx] = m_vecCells[CurIdx];
			bool NxtActive = m_vecCells[NxtIdx]->IsActive();

			// �̵� �迭�� ǥ���ϱ�
			m_vecCellsPosChanged[Row + AddedRow][Col] = true;

		}
	};

	return true;
}


void CBoard::CreateNewCells()
{
	Vector2 Pos;
	int  Index;
	AnimalType Type;

	for (int Col = 0; Col < m_ColCount; Col++)
	{
		for (int Row = 0; Row < m_NewCellNeeded[Col]; Row++)
		{
			// m_TopYPos
			Pos = Vector2(Col * m_SingleBlockSize.x, m_TopYPos + (Row  * m_SingleBlockSize.y));
			CCell* NewCell = new CCell;
			Type = (AnimalType)(rand() % AnimalType::END);
			Index = Row * m_ColCount + Col;
			NewCell->Init(Type);
			NewCell->SetBoard(this);
			NewCell->SetCellInitInfo(Pos, m_SingleBlockSize, Row, Col, Index);
			NewCell->SetSize(m_SingleBlockSize);
			NewCell->SetYIdx(1);
			NewCell->SetNewPos(Pos);
			// NewCell->SetMoveSpeed(m_SingleBlockSize.x);

			// NewCell ���� ��������� �Ʒ��� ������ ���̹Ƿ� m_IsMoving�� true�� ����
			NewCell->SetIsMoving(true);

			m_vecCells[Index] = NewCell;
		}
	}

	for (int i = 0; i < m_ColCount; i++)
		m_NewCellNeeded[i] = 0;
}

void CBoard::ChangeCellYIdx(int RowIndex, int ColIndex)
{
	Vector2 Pos = m_vecCells[RowIndex * m_ColCount + ColIndex]->GetPos();
	if (Pos.y >= m_MiddelYPos)
	{
		m_vecCells[RowIndex * m_ColCount + ColIndex]->SetYIdx(1);
	}
}

bool CBoard::CheckUpdateEnable()
{
	if (m_IsTwoMoving)
		return false;

	for (int Row = 0; Row < m_RowCount; Row++)
	{
		for (int Col = 0; Col < m_ColCount; Col++)
		{
			if (m_vecCells[Row * m_ColCount + Col]->IsMoving())
				return false;
		}
	}
	return true;
}

void CBoard::MoveTwoClickedCells(float DeltaTime)
{
	if (!m_ClickFirstCell || !m_ClickSecCell)
		return;

	if (!m_IsTwoMoving) // 2���� Mouse�� �̵� ���϶��� �����ϰ� �Ѵ�. 
		return;

	// ���� ��ġ, �ڽ� ��ġ ���� ����ϱ� 
	Vector2 FirstCellDf = m_ClickFirstCell->GetNewPos() - m_ClickFirstCell->GetPos();
	Vector2 SecCellDf  = m_ClickSecCell->GetNewPos() - m_ClickSecCell->GetPos();

	// Cell Idx ���� ����
	int FirstCellRow = -1, FirstCellCol = -1, SecondCellRow = -1, SecondCellCol = -1;
	int FirstCellIdx = -1, SecCellIdx = -1;

	if (FirstCellDf.Length() > 1.f)
	{
		// m_ClickFirstCell->SetPos(m_ClickFirstCell->GetPos() + m_InitFirstCellDiff * DeltaTime);
		m_ClickFirstCell->SetPos(m_ClickFirstCell->GetPos() + m_InitFirstCellDiff * DeltaTime);
	}
	if (SecCellDf.Length() > 1.f)
	{
		m_ClickSecCell->SetPos(m_ClickSecCell->GetPos() + m_InitSecCellDiff * DeltaTime);
		// m_ClickSecCell->SetPos(m_ClickSecCell->GetPos() + m_InitSecCellDiff * DeltaTime);
	}
	if (FirstCellDf.Length() <=1.f && SecCellDf.Length() <= 1.f)
	{
		// NewPos�� ������ ���� 
		m_ClickFirstCell->SetPos(m_ClickFirstCell->GetNewPos());
		m_ClickSecCell->SetPos(m_ClickSecCell->GetNewPos());

		// Dir ���� 
		m_ClickFirstCell->SetDir(Vector2(0.f, 0.f));
		m_ClickSecCell->SetDir(Vector2(0.f, 0.f));

		// IsSwapping ���� ����
		m_ClickFirstCell->SetSwapping(false);
		m_ClickSecCell->SetSwapping(false);

		FirstCellRow      = m_ClickFirstCell->GetRowIndex();
		FirstCellCol       = m_ClickFirstCell->GetColIndex();
		FirstCellIdx       = FirstCellRow * m_ColCount + FirstCellCol;

		SecondCellRow = m_ClickSecCell->GetRowIndex();
		SecondCellCol = m_ClickSecCell->GetColIndex();
		SecCellIdx = SecondCellRow * m_ColCount + SecondCellCol;

		m_ClickFirstCell->SetIdxInfos(SecondCellRow, SecondCellCol, SecCellIdx);
		m_ClickSecCell->SetIdxInfos(FirstCellRow, FirstCellCol, FirstCellIdx);

		// ���� vecCell ���� ���� ���� 
		CCell* tempCell					= m_vecCells[FirstCellIdx];
		m_vecCells[FirstCellIdx]	    = m_vecCells[SecCellIdx];
		m_vecCells[SecCellIdx] = tempCell;

		if (!m_PrevMisMatched) // ó�� �ٲٴ� ��� ( Ʋ���� �ǵ��ƿ��� ��쿡�� Match ���θ� �������� �ʴ´� )
		{
			// ���⼭ �ѹ� �˻��ϱ�
			bool IsMatch = CheckMatchCells();

			if (!IsMatch)
			{
				// ���� ���� �ʴٸ�, �ٽ� �ݴ�� �����ϱ� 
				FirstCellIdx = SecondCellRow * m_ColCount + SecondCellCol;
				SecCellIdx = FirstCellRow * m_ColCount + FirstCellCol;

				// New Pos ���� 
				// m_vecCells[FirstCellIdx] ���� SecondCellIdx ������ Cell�� ����ִ�. 
				// m_vecCells[SecCellIdx] ���� FirstCellIdx ������ Cell�� ����ִ�. 
				m_vecCells[FirstCellIdx]->SetNewPos(m_ClickFirstPos);
				m_vecCells[SecCellIdx]->SetNewPos(m_ClickSecPos);

				// Dir ���� 
				Vector2 FirstCellDir = m_ClickSecPos - m_ClickFirstPos;
				FirstCellDir.Normalize();
				Vector2 SecCellDir = m_ClickFirstPos - m_ClickSecPos;
				SecCellDir.Normalize();

				m_vecCells[FirstCellIdx]->SetDir(SecCellDir);
				m_vecCells[FirstCellIdx]->SetSwapping(true);

				m_vecCells[SecCellIdx]->SetDir(FirstCellDir);
				m_vecCells[SecCellIdx]->SetSwapping(true);

				m_ClickFirstCell = m_vecCells[FirstCellIdx];
				m_ClickSecCell = m_vecCells[SecCellIdx];

				m_InitFirstCellDiff = m_vecCells[FirstCellIdx]->GetNewPos() - m_vecCells[FirstCellIdx]->GetPos();
				m_InitSecCellDiff = m_vecCells[SecCellIdx]->GetNewPos() - m_vecCells[SecCellIdx]->GetPos();

				m_Click = 0;

				m_IsTwoMoving = true;

				m_PrevMisMatched = true;
			}
			else
			{
				// ���� �´ٸ�, Mouse + Move ���� ������ �ʱ�ȭ
				m_IsTwoMoving = false;
				m_Click = 0;
				m_ClickFirstCell = nullptr;
				m_ClickSecCell = nullptr;
				m_PrevMisMatched = false;
			}
		}
		// �ٽ� �ǵ��ƿ� ��� : ������ ���� ������ �ʱ�ȭ 
		else
		{
			m_IsTwoMoving = false;
			m_Click = 0;
			m_ClickFirstCell = nullptr;
			m_ClickSecCell = nullptr;
			m_PrevMisMatched = false;
		}
	}
}

bool CBoard::CheckMatchCells()
{
	// �˻� ����� �����ϴ� 2���� �迭 ���� (�ʱ�ȭ)
	for (int Row = 0; Row < m_RowCount; Row++)
	{
		for (int Col = 0; Col < m_ColCount; Col++)
		{
			m_vecDestroyedCells[Row][Col] = false;
		}
	}

	// ���� row, col
	// ���� ��ġ AnimalType, ���� ����, ���� x, y
	// ���� 3 �̻� �̰�, ���̻� ���� ���� ���ٸ� �Ͷ߸���
	std::queue<std::vector<int>> queue;
	int cRow = -1, cCol = -1, nRow = -1, nCol = -1, Index = -1;
	bool IsMatch = false;

	// Row �˻� ( ���� �˻� ) --> ���� �������� Real Board �� ���ؼ��� ������ ���̴�. 
	for (int RMatchLen = 3; RMatchLen <= m_ColCount; RMatchLen++)
	{
		// ��� Cell �鿡 ���ؼ� �˻��Ѵ�. ( �������� ȭ�� �� �˻��ϱ� )
		for (int Row = m_RowCount / 2; Row < m_RowCount; Row++)
		{
			for (int Col = 0; Col <= m_ColCount - RMatchLen; Col++)
			{
				bool AllSame = true;
				int   NxtCellIdx = -1;
				AnimalType InitType = m_vecCells[Row * m_ColCount + Col]->GetAnimalType();

				for (int k = 0; k < RMatchLen; k++)
				{
					NxtCellIdx = Row * m_ColCount + Col + k;
					if (m_vecCells[NxtCellIdx]->GetAnimalType() != InitType)
					{
						AllSame = false;
						break;
					}
				}
				if (AllSame)
				{
					for (int k = 0; k < RMatchLen; k++)
					{
						m_vecDestroyedCells[Row][Col + k] = true;
					}
				}
			}
		}
	}

	// Column �˻�
	for (int CMatchLen = 3; CMatchLen <= m_RowCount / 2; CMatchLen++)
	{
		// ��� Cell �鿡 ���ؼ� �˻��Ѵ�.
		for (int Col = 0; Col < m_ColCount; Col++)
		{
			// �� Colum --> ���η� �˻��� �����̴�. 
			for (int Row = m_RowCount / 2; Row <= m_RowCount - CMatchLen; Row++)
			{
				bool AllSame = true;
				int   NxtCellIdx = -1;
				AnimalType InitType = m_vecCells[Row * m_ColCount + Col]->GetAnimalType();

				for (int k = 0; k < CMatchLen; k++)
				{
					NxtCellIdx = ( Row + k ) * m_ColCount + Col;
					if (m_vecCells[NxtCellIdx]->GetAnimalType() != InitType)
					{
						AllSame = false;
						break;
					}
				}
				if (AllSame)
				{
					for (int k = 0; k < CMatchLen; k++)
					{
						m_vecDestroyedCells[Row + k][Col] = true;
					}
				}
			}
		}
	}

	// ��� �˻��� ���� , ������� Cell �鿡 ���� Block�� Empty�� �����.
	for (int Row = 0; Row < m_RowCount; Row++)
	{
		for (int Col = 0; Col < m_ColCount; Col++)
		{
			if (m_vecDestroyedCells[Row][Col])
			{
				Index = Row * m_ColCount + Col;
				m_vecBlocks[Index]->SetBlockType(BlockType::EMPTY);
				IsMatch = true;
			}
		}
	}                                                                                                                                               

	// ���� ��ġ�ϴ� Cell�� �ϳ��� ���ٸ�, false�� return �Ѵ�.
	if (!IsMatch)
		return false;

	return true;
}

Vector2 CBoard::GetOppositeDirection(int curDx, int curDy)
{
	//	int dx[4] = { -1, 1, 0, 0 };
	// int dy[4] = { 0, 0, -1, 1 };
	if (curDx == -1 && curDy == 0)
	{
		return Vector2(1.f, 0.f);
	}
	else if (curDx == 1 && curDy == 0)
	{
		return Vector2(-1.f, 0.f);
	}
	else if (curDx == 0 && curDy == 1)
	{
		return Vector2(0.f, -1.f);
	}
	else if (curDx == 0 && curDy == -1)
	{
		return Vector2(0.f, 1.f);
	}
}

bool CBoard::MakeMatchableBoard()
{
	// ������ ������ �ִٸ� �ǳʶڴ�.
	if (CheckMatchPossible())
		return true;

	// ��� Block ���� ��������� ����� == ���� �� !
	for (int row = m_RowCount / 2; row < m_RowCount; row ++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			m_vecBlocks[row * m_ColCount + col]->SetTexture(m_ShuffleTexture);
		}
	}

	/*
	float DeltaTime = CGameManager::GetInst()->GetDeltaTime();
	while(true)
	{
		m_DelayTime -= DeltaTime;
		if (m_DelayTime < 0.f)
			break;
	}
	*/

	while (true)
	{
		ShuffleCells();

		// 2. ���� CheckMatchCell �� false ��� ( ���� �´� ���� ���ٸ� �˻� ) --> ���� �´� ����� �ִٸ� �ٽ� Shuffle 
		// �ٽ� Cell ���� ���� ������ ��ģ��.
		if (CheckMatchCells())
			continue;

		// ���� ������ ������ ���Դٸ�, ���� �ٽ� ���� ���� 
		if (CheckMatchPossible())
		{
			// �ٽ� ��� Block ���� ������ �ʷ����� ����� 
			// ��� Block ���� ��������� ����� == ���� �� !
			for (int row = m_RowCount / 2; row < m_RowCount; row++)
			{
				for (int col = 0; col < m_ColCount; col++)
				{
					m_vecBlocks[row * m_ColCount + col]->SetTexture(m_BlockTexture);
				}
			}
			m_DelayTime = 100.f;
			return true;
		}
	}
}

bool CBoard::CheckMatchPossible()
{
	// 1. Board �� �� �����ͼ� �ڱ� ��ġ�� �����ϸ� �����ϴ� �ڵ�

	int MinMatchUnit = 3;

	int CurIdx = -1, NxtIdx = -1, LastIdx = -1;

	// ���� �˻� 
	for (int row = m_RowCount / 2; row < m_RowCount; row++)
	{
		for (int col = 0; col <= m_ColCount - (MinMatchUnit - 1); col++)
		{
			// ���ӵ� 2���� �¾ƾ� �Ѵ�
			CurIdx = row * m_ColCount + col;
			NxtIdx = row * m_ColCount + col + 1;
			if (m_vecCells[CurIdx]->GetAnimalType() != m_vecCells[NxtIdx]->GetAnimalType())
				continue;

			// ������ �� �Ʒ��� �����Ѵ�.
			// �� ����
			if (col + 2 < m_ColCount)
			{
				if (row - 1 >= m_RowCount / 2)
				{
					LastIdx = (row - 1) * m_ColCount + col + 2;
					if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
					{
						return true;
					}
				}

				// �Ʒ� ����
				if (row + 1 < m_RowCount)
				{
					LastIdx = (row + 1) * m_ColCount + col + 2;
					if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
						return true;
				}
			}

			// ���� ���� ��ĭ ��, �Ʒ��� �����Ѵ�
			if (col - 1 >= 0)
			{
				// ���� 
				if (row - 1 >= 0)
				{
					LastIdx = (row - 1) * m_ColCount + col - 1;
					if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
					{
						return true;
					}
				}

				// �Ʒ� ����
				if (row + 1 < m_RowCount)
				{
					LastIdx = (row + 1) * m_ColCount + col - 1;
					if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
						return true;
				}
			}


			// 2ĭ ����, 3ĭ �������� �˻��Ѵ�.
			if (col - 2 >= 0)
			{
				LastIdx = row * m_ColCount + col - 2;
				if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
					return true;
			}

			// 2ĭ ����, 2ĭ �������� �˻��Ѵ�.
			if (col + 3 < m_ColCount)
			{
				LastIdx = row * m_ColCount + col + 3;
				if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
					return true;
			}
		}
	}

	// ���� �˻�
	for (int col = 0; col <= m_ColCount - MinMatchUnit; col++)
	{
		for (int row = m_RowCount / 2; row <= m_RowCount - (MinMatchUnit - 1); row++)
		{
			// ���ӵ� 2���� �¾ƾ� �Ѵ�
			CurIdx = row * m_ColCount + col;
			NxtIdx = (row + 1) * m_ColCount + col;
			if (m_vecCells[CurIdx]->GetAnimalType() != m_vecCells[NxtIdx]->GetAnimalType())
				continue;

			// �Ʒ� ���� ���� �������� ���Ѵ�. 
			// ����
			if (row + 2 < m_RowCount)
			{
				if (col - 1 >= 0)
				{
					LastIdx = (row + 2) * m_ColCount + col - 1;
					if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
					{
						return true;
					}
				}
				// ������
				if (col + 1 < m_ColCount)
				{
					LastIdx = (row + 2) * m_ColCount + col + 1;
					if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
					{
						return true;
					}
				}
			}


			// ���� ���� ���� �������� ���Ѵ�. 
			// ����
			if (row - 1 >= m_RowCount / 2)
			{
				if (col - 1 >= 0)
				{
					LastIdx = (row - 1) * m_ColCount + col - 1;
					if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
					{
						return true;
					}
				}
				// ������
				if (col + 1 < m_ColCount)
				{
					LastIdx = (row - 1) * m_ColCount + col + 1;
					if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
					{
						return true;
					}
				}
			}

			// ���� 2ĭ, �Ʒ��� 2ĭ ¥���� �˻��Ѵ�.
			if (row - 2 >= m_RowCount / 2)
			{
				LastIdx = (row - 2) * m_ColCount + col;
				if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
					return true;
			}

			// 3ĭ ����, 2ĭ �������� �˻��Ѵ�.
			if (row + 3 < m_RowCount)
			{
				LastIdx = (row + 3) * m_ColCount + col;
				if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
					return true;
			}
		}
	}
	return false;
}



void CBoard::ShuffleCells()
{
	int Index = -1;
	AnimalType Type = (AnimalType)0;
	for (int row = m_RowCount /2; row < m_RowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			Index = row * m_ColCount + col;
			Type = (AnimalType)(rand() % AnimalType::END);
			m_vecCells[Index]->SetAnimalType(Type);

			// Cell �̵��� ���õ� �����鵵 ��� �ʱ�ȭ ���ش�
			SetCellPosChangedComplete(row, col);
		}
	}

	m_IsTwoMoving = false;
}

void CBoard::SortRenderObject(int Left, int Right, std::vector<CSharedPtr<CGameObject>>& RenderObjects)
{
	if (Left < Right)
	{
		int Pivot = SortPartition(Left, Right, RenderObjects);
		SortRenderObject(Left, Pivot - 1, RenderObjects);
		SortRenderObject(Pivot + 1, Right, RenderObjects);
	}
}

int CBoard::SortPartition(int Left, int Right, std::vector<CSharedPtr<CGameObject>>& RenderObjects)
{
	// ���ʺ��ʹ�, ū ��
	// �����ʺ��ʹ� ���� ��
	// ������ 2�� ��ü 
	int Low = Left;
	int High = Right + 1;
	do
	{
		do
		{
			Low++;
		} while (Low <= Right && RenderObjects[Low]->GetYIdx() < RenderObjects[Left]->GetYIdx());

		do
		{
			High--;
		} while (High >= Left && RenderObjects[High]->GetYIdx() > RenderObjects[Left]->GetYIdx());

		if (Low < High)
		{
			// �ٲ��ֱ� 
			CSharedPtr<CGameObject> temp = RenderObjects[Low];
			RenderObjects[Low] = RenderObjects[High];
			RenderObjects[High] = temp;
		}

	} while (Low < High);

	// �ٲ��ֱ� 
	CSharedPtr<CGameObject> temp = RenderObjects[Left];
	RenderObjects[Left] = RenderObjects[High];
	RenderObjects[High] = temp;
	return High;
}
