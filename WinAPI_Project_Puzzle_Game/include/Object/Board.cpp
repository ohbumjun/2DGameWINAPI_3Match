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

	// 기존 Board 제거 
	// m_vecCells.clear();
	// m_vecBlocks.clear();

	// 변수 초기화 
	m_RowCount = RowCount * 2; // Virtual 까지 합쳐서 
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

	// 크기 재할당 
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

	// 여기에서, Random 하게 섞일 수 있는 idx 들을 마련하는 함수를 호출
	// while 문을 돌려서, 
	// 1) Set가 없을 때까지, 찾은 다음 , 찾으면 return 
	// 2) (심화) 적어도 1개는 세팅될 수 있도록 세팅하기 
	// 그 다음 구한 idx 들을 차례대로 Cell에 세팅해서 해당 idx에 맞는 Cell Type --> Texture 세팅하기 


	// Cell, Block 세팅 
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
			// Vector (x,y) : x는 열, y는 행 
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

				// 화면에 보여지는 Board 에서의 NewCell YIdx 
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

	// Tile 선택
	if (MousePos.x < 0.f || MousePos.x > m_Size.x || MousePos.y < 0.f || MousePos.y > m_Size.y)
		return;


	// Tile 선택이 안되면, return 처리 
	if (m_Click == 0)
	{
		m_ClickFirstPos = MousePos;

		// 선택된 Block 의 Idx 
		m_ClickFirstIdxX = (int)(MousePos.x / m_SingleBlockSize.x); // 열 
		m_ClickFirstIdxY = (int)(MousePos.y / m_SingleBlockSize.y) + (m_RowCount / 2); // 행 
		
		m_Click = 1;
	}
	else if (m_Click == 1)
	{
		m_ClickSecPos = MousePos;

		// 선택된 Block 의 Idx 
		m_ClickSecIdxX = (int)(MousePos.x / m_SingleBlockSize.x); // 열 
		m_ClickSecIdxY = (int)(MousePos.y / m_SingleBlockSize.y) + (m_RowCount / 2); // 행 
	
		if ((abs(m_ClickFirstIdxX - m_ClickSecIdxX) <= 1) && (abs(m_ClickFirstIdxY - m_ClickSecIdxY) <= 1))
		{
			// 선택된 녀석 Block 상태 바꾸기 
			int FirstCellIdx = m_ClickFirstIdxY * m_ColCount + m_ClickFirstIdxX;
			int SecCellIdx  = m_ClickSecIdxY * m_ColCount + m_ClickSecIdxX;
	
			// New Pos 세팅 
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
		// 1) 사라질 Block 표시하기 ( 알고리즘 적용하기 )
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
					// 메모리 해제
					SAFE_DELETE(m_vecCells[Index]);

					// 해당 열 삭제 표시
					// m_ColsDestroyed[C] = true;

					// 해당 열 삭제 개수 증가
					// m_NumColsDestroyed[C] += 1;

					// 제거된 가장 낮은 Row 세팅
					// m_ColDestroyedMinRow[C] = R;
					ChangeUpperBlockStates(R, C);
					continue;
				}
				// m_vecCells[Index]->Update(DeltaTime);
			}
		}
		*/

		// Cells Idx 변화 정보 초기화 
		for (int row = 0; row < m_RowCount; row++)
		{
			for (int col = 0; col < m_ColCount; col++)
			{
				m_ChangedCellRowInfo[row][col] = 0;
			}
		}

		// 2) 실제 사라지게 하기 + New Pos 세팅 
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
					// 메모리 해제
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

		// 3) 새로운 Idx 정보 세팅
		ChangeCellsIdx();

		// 4) 새로운 Cell 생성
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
	// 모든 Block, Cell들이 채워질 때만 Render
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

		// Pos 정보 바꾸기 
		Vector2 PrevPos = m_vecCells[CurIdx]->GetPos();
		m_vecCells[CurIdx]->SetNewPos(PrevPos.x, PrevPos.y + m_SingleBlockSize.y);

	}

	return true;
}

bool CBoard::ChangeUpperCellIdxInfo(int RowIndex, int ColIndex)
{
	int NxtIdx = -1, CurIdx = -1;

	// 새로 생성해야할 Cell 개수 증가 
	m_NewCellNeeded[ColIndex] += 1;

	// Cell의 차후 Idx 정보를 세팅해준다.--> ChangeCellsIdx 에서 해당 정보에 맞게 실제 변경
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

			// 새로운 Idx 구하기 
			AddedRow = m_ChangedCellRowInfo[Row][Col];
			NxtIdx = (Row + AddedRow) * m_ColCount + Col;

			// Idx 정보 다시 세팅해주기 
			m_vecCells[CurIdx]->SetIdxInfos(Row + AddedRow, Col, NxtIdx);
			// 배열 내 Cell 정보 바꾸기
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
	// 왼쪽부터는, 큰 것
	// 오른쪽부터는 작은 것
	// 만나면 2개 교체 
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
			// 바꿔주기 
			CSharedPtr<CGameObject> temp = RenderObjects[Low];
			RenderObjects[Low] = RenderObjects[High];
			RenderObjects[High] = temp;
		}

	} while (Low < High);

	// 바꿔주기 
	CSharedPtr<CGameObject> temp = RenderObjects[Left];
	RenderObjects[Left] = RenderObjects[High];
	RenderObjects[High] = temp;
	return High;
}
