#include "Board.h"
#include "../Input.h"
#include "../Resource/ResourceManager.h"
#include "../GameManager.h"
#include "GameObject.h"

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
	m_BlockCapacity(50)
{
	m_vecCells		   = new CCell * [m_BlockCapacity];
	m_vecBlocks		   = new CBlock * [m_BlockCapacity];
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
	m_RowCount = RowCount * 2 ; // Virtual ���� ���ļ� 
	m_ColCount = ColCount;
	m_SquareSize = SquareSize;

	// ���� Board ���� 
	// m_vecCells.clear();
	// m_vecBlocks.clear();

	m_Size = m_SquareSize * Vector2((float)m_ColCount, (float)m_RowCount);
	m_BlockCount = (m_RowCount) * m_ColCount;

	m_ColsDestroyed.reserve(m_ColCount);
	m_NumColsDestroyed.reserve(m_ColCount);
	m_ColDestroyedMinRow.reserve(m_ColCount);

	for (int i = 0; i < m_ColCount; i++)
	{
		m_ColsDestroyed.push_back(false);
		m_NumColsDestroyed.push_back(0);
		m_ColDestroyedMinRow.push_back(-1);
	}
	

	// ũ�� ���Ҵ� 
	if (m_BlockCount > m_BlockCapacity)
	{
		m_BlockCapacity *= 2;

		CCell** n_vecCells							= new CCell * [m_BlockCapacity];
		CBlock** n_vecBlocks						= new CBlock * [m_BlockCapacity];

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
	Vector2 StartOffset = Vector2(0, -m_Size.y / 2);
	m_TopYPos = StartOffset.y;
	int Index = -1;
	int ObjectNums = 0;

	for (int r = 0; r < m_RowCount; r++)
	{
		for (int c = 0; c < m_ColCount; c++)
		{
			// Vector (x,y) : x�� ��, y�� �� 
			Vector2 Pos = Vector2((float)c, (float)r) * m_SquareSize + StartOffset;
			// Vector2 Pos = Vector2((float)c, (float)r) * m_SquareSize + 0;

			// AnimType
			AnimalType Type = (AnimalType)(rand() % AnimalType::END);

			Index = r * m_ColCount + c;

			// Upper Virtual Block
			CBlock* NewBlock = new CBlock;
			NewBlock->Init(); // SetBlockInitInfo(const Vector2 Pos, const Vector2& Size, int RowIndex, int ColIndex, int Index, class CTexture* Texture)
			NewBlock->SetBoard(this);
			NewBlock->SetSize(m_SquareSize);
			NewBlock->SetYIdx(2);
			NewBlock->AddRef();
			m_vecBlocks[Index] = NewBlock;
			NewBlock->SetBlockInitInfo(Pos, m_SquareSize, r, c, Index, m_WhiteTexture);

			// Cell
			CCell* NewCell = new CCell;
			NewCell->Init(Type);
			NewCell->SetBoard(this);
			// SetCellInitInfo(const Vector2 Pos, const Vector2& Size, int RowIndex, int ColIndex, int Index)
			NewCell->SetCellInitInfo(Pos, m_SquareSize, r, c, Index);
			NewCell->SetSize(m_SquareSize);
			// NewCell->SetYIdx(-1);
			NewCell->SetYIdx(1);
			NewCell->AddRef();
			NewCell->SetNewYPos(Pos.x, Pos.y);
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

	// Board ���� �߰����� Board ����� ���� 
	
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
	int BlockIdxY = (int)(MousePos.y / m_SquareSize.y) + ( m_RowCount / 2 ); // �� 

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
		m_ClickFirst = false;
	}

	// ���õ� �༮ Block ���� �ٲٱ� 
	int SIndex = BlockIdxY * m_ColCount + BlockIdxX ;
	m_vecBlocks[SIndex]->SetBlockType(BlockType::EMPTY);
	// m_vecBlocks[SIndex]->SetMoveEnable(true);
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
	CResourceManager::GetInst()->LoadTexture("WhiteTexture", TEXT("white.bmp"));
	m_BlockTexture = CResourceManager::GetInst()->FindTexture("BlockTexture");
	m_WhiteTexture = CResourceManager::GetInst()->FindTexture("WhiteTexture");

	Vector2 TextureSize = Vector2((float)m_BlockTexture->GetWidth(), (float)m_BlockTexture->GetHeight());
	CreateBoard(5,5, TextureSize);

	return true;
}

bool CBoard::Update(float DeltaTime)
{
	Start();

	if (m_BlockCount > 0)
	{
		int Index = -1;

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
				m_vecCells[Index]->Update(DeltaTime);
			}
		}

		for (size_t i = 0; i < m_BlockCount; i++)
		{
			m_vecBlocks[i]->Update(DeltaTime);
		}
	}
	
	// CompareClicks();
	// ReceiveClicks();

	return true;
}

bool CBoard::PostUpdate(float DeltaTime)
{
	if (m_BlockCount > 0)
	{
		int Index = -1;

		// ���ŵ� Cell�� �ִ� Column Ȯ�� + ���� �����ϱ� 
		/*
		for (int C = 0; C < m_ColCount; C++)
		{
			if (!m_ColsDestroyed[C])
				continue;
			int LowestRow = m_ColDestroyedMinRow[C];
			if (ChangeUpperBlockStates(LowestRow, C))
			{
				CreateNewCells(C, m_NumColsDestroyed[C]);
			}
		}
		*/

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
				RenderObjects.push_back(m_vecBlocks[i]);
			if (m_vecCells[i])
				RenderObjects.push_back(m_vecCells[i]);
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

bool CBoard::ChangeUpperBlockStates(int RowIndex, int ColIndex)
{
	int NxtIdx = -1;
	int CurIdx = -1;
	for (int Row = RowIndex - 1; Row >= 0; Row--)
	{
		CurIdx = Row * m_ColCount + ColIndex;
		NxtIdx = (Row+1) * m_ColCount + ColIndex;

		// m_vecBlocks[i * m_ColCount + ColIndex]->SetMoveEnable(true);
		// Pos ���� �ٲٱ� 
		Vector2 PrevPos = m_vecCells[CurIdx]->GetPos();
		m_vecCells[CurIdx]->SetNewYPos(PrevPos.x, PrevPos.y + m_SquareSize.y);
		// Idx ���� �ٽ� �������ֱ� 
		m_vecCells[CurIdx]->SetIdxInfos(Row + 1, ColIndex, NxtIdx);
		// �迭 �� Cell ���� �ٲٱ�
		m_vecCells[NxtIdx] = m_vecCells[CurIdx];
	}
	return true;
}

void CBoard::CreateNewCells(int ColIdx, int NumCells)
{
	Vector2 Pos;
	int  Index;
	AnimalType Type;

	// �� Real Board ������ MoveEnable�� true�� Setting�Ǿ� �ִ� ��� ���� 
	/*
	for (int R = 0; R < NumCells; R++)
	{
		// m_TopYPos
		Pos = Vector2(R * m_SquareSize.x, m_TopYPos + R * m_SquareSize.y);
		CCell* NewCell = new CCell;
		Type = (AnimalType)(rand() % AnimalType::END);
		Index = R * m_ColCount + ColIdx;

		NewCell->Init(Type);
		NewCell->SetBoard(this);
		// SetCellInitInfo(const Vector2 Pos, const Vector2& Size, int RowIndex, int ColIndex, int Index)
		NewCell->SetCellInitInfo(Pos, m_SquareSize, R, ColIdx, Index);
		NewCell->SetSize(m_SquareSize);
		// NewCell->SetYIdx(-1);
		NewCell->SetYIdx(1);
		NewCell->AddRef();
		NewCell->SetNewlyCreated(true);

		// m_vecCells[Index] = NewCell;
	}
	*/
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
		} while (Low <= Right  && RenderObjects[Low]->GetYIdx() < RenderObjects[Left]->GetYIdx());

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
