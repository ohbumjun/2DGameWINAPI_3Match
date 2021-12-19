#include "Board.h"
#include "../Input.h"
#include "../Resource/ResourceManager.h"
#include "../GameManager.h"
#include "GameObject.h"

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
	m_ClickEnable(true)
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

	m_ChangedCellRowInfo.reserve(m_RowCount);
	for (int i = 0; i < m_RowCount; i++)
	{
		std::vector<int> vec(m_ColCount, 0);
		m_ChangedCellRowInfo.push_back(vec);
	}

	m_NewCellNeeded.resize(m_ColCount);

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
	if (!CheckClickEnable())
		return;
	Vector2 MousePos = CInput::GetInst()->GetMousePos();

	// Tile ����
	if (MousePos.x < 0.f || MousePos.x > m_Size.x || MousePos.y < 0.f || MousePos.y > m_Size.y)
		return;


	// Tile ������ �ȵǸ�, return ó�� 
	if (m_Click == 0)
	{
		m_ClickFirstPos = MousePos;

		// ���õ� Block �� Idx 
		m_ClickFirstIdxX = (int)(MousePos.x / m_SingleBlockSize.x); // �� 
		m_ClickFirstIdxY = (int)(MousePos.y / m_SingleBlockSize.y) + (m_RowCount / 2); // �� 
		
		m_Click = 1;
	}
	else if (m_Click == 1)
	{
		m_ClickSecPos = MousePos;

		// ���õ� Block �� Idx 
		m_ClickSecIdxX = (int)(MousePos.x / m_SingleBlockSize.x); // �� 
		m_ClickSecIdxY = (int)(MousePos.y / m_SingleBlockSize.y) + (m_RowCount / 2); // �� 
	
		if ((abs(m_ClickFirstIdxX - m_ClickSecIdxX) <= 1) && (abs(m_ClickFirstIdxY - m_ClickSecIdxY) <= 1))
		{
			// ���õ� �༮ Block ���� �ٲٱ� 
			int FirstCellIdx = m_ClickFirstIdxY * m_ColCount + m_ClickFirstIdxX;
			int SecCellIdx  = m_ClickSecIdxY * m_ColCount + m_ClickSecIdxX;
	
			// New Pos ���� 
			m_vecCells[FirstCellIdx]->SetNewPos(m_ClickSecPos);
			m_vecCells[SecCellIdx]->SetNewPos(m_ClickFirstPos);

			m_Click = 0;
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
	m_BlockTexture = CResourceManager::GetInst()->FindTexture("BlockTexture");
	m_WhiteTexture = CResourceManager::GetInst()->FindTexture("WhiteTexture");

	Vector2 TextureSize = Vector2((float)m_BlockTexture->GetWidth(), (float)m_BlockTexture->GetHeight());
	CreateBoard(5, 5, TextureSize);

	return true;
}

bool CBoard::Update(float DeltaTime)
{
	Start();
	int Index = -1;

	if (m_BlockCount > 0)
	{
		// 1) ����� Block ǥ���ϱ� ( �˰��� �����ϱ� )
		/*
		for (int R = m_RowCount - 1; R >= 0; R--)
		{
			for (int C = m_ColCount - 1; C >= 0; C--)
			{
				Index = R * m_ColCount + C;
				if (!m_vecCells[Index])
					continue;
				if (!m_vecCells[Index]->IsActive())
				{
					// �޸� ����
					SAFE_DELETE(m_vecCells[Index]);

					// �ش� �� ���� ǥ��
					// m_ColsDestroyed[C] = true;

					// �ش� �� ���� ���� ����
					// m_NumColsDestroyed[C] += 1;

					// ���ŵ� ���� ���� Row ����
					// m_ColDestroyedMinRow[C] = R;
					ChangeUpperBlockStates(R, C);
					continue;
				}
				// m_vecCells[Index]->Update(DeltaTime);
			}
		}
		*/

		// Cells Idx ��ȭ ���� �ʱ�ȭ 
		for (int row = 0; row < m_RowCount; row++)
		{
			for (int col = 0; col < m_ColCount; col++)
			{
				m_ChangedCellRowInfo[row][col] = 0;
			}
		}

		// 2) ���� ������� �ϱ� + New Pos ���� 
		for (int R = m_RowCount - 1; R >= 0; R--)
		{
			for (int C = m_ColCount - 1; C >= 0; C--)
			{
				Index = R * m_ColCount + C;

				CCell* Cell = m_vecCells[Index];
				// if (!m_vecCells[Index])
				if (!Cell)
					continue;

				if (!m_vecCells[Index]->IsActive())
				{
					// �޸� ����
					SAFE_DELETE(m_vecCells[Index]);
					ChangeUpperCellsPos(R, C);
					ChangeUpperCellIdxInfo(R, C);
					continue;
				}

				// Update 
				m_vecCells[Index]->Update(DeltaTime);

				// Update Cell YIdx 
				ChangeCellYIdx(R, C);
			}
		}

		// 3) ���ο� Idx ���� ����
		ChangeCellsIdx();

		// 4) ���ο� Cell ����
		CreateNewCells();

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

		RenderObjects.reserve(m_BlockCapacity * 2);

		for (int i = 0; i < m_BlockCapacity; i++)
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

bool CBoard::ChangeUpperCellsPos(int RowIndex, int ColIndex)
{
	int CurIdx = -1;
	for (int Row = RowIndex - 1; Row >= 0; Row--)
	{
		CurIdx = Row * m_ColCount + ColIndex;

		// Pos ���� �ٲٱ� 
		Vector2 PrevPos = m_vecCells[CurIdx]->GetPos();
		m_vecCells[CurIdx]->SetNewPos(PrevPos.x, PrevPos.y + m_SingleBlockSize.y);

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

bool CBoard::ChangeCellsIdx()
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

			// ���ο� Idx ���ϱ� 
			AddedRow = m_ChangedCellRowInfo[Row][Col];
			NxtIdx = (Row + AddedRow) * m_ColCount + Col;

			// Idx ���� �ٽ� �������ֱ� 
			m_vecCells[CurIdx]->SetIdxInfos(Row + AddedRow, Col, NxtIdx);
			// �迭 �� Cell ���� �ٲٱ�
			m_vecCells[NxtIdx] = m_vecCells[CurIdx];
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

bool CBoard::CheckClickEnable()
{
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
