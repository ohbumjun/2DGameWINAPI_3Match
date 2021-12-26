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
	// 기존 Board 제거 
	// m_vecCells.clear();
	// m_vecBlocks.clear();

	// 변수 초기화 
	m_RowCount = RowCount * 2; // Virtual 까지 합쳐서 
	m_ColCount = ColCount;
	m_SingleBlockSize = SquareSize;

	m_Size = m_SingleBlockSize * Vector2((float)m_ColCount, (float)m_RowCount);
	m_BlockCount = (m_RowCount)*m_ColCount;

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
			//  NewCell->SetMoveSpeed(m_SingleBlockSize.x * 2);
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
	bool UpdateEnable = CheckUpdateEnable();
	if (!UpdateEnable)
		return;
	Vector2 MousePos = CInput::GetInst()->GetMousePos();

	// Tile 선택
	if (MousePos.x < 0.f || MousePos.x > m_Size.x || MousePos.y < 0.f || MousePos.y > m_Size.y / 2)
		return;

	// Cell 을 없애보는 Test Code 
	// int Col   = (int)(MousePos.x / m_SingleBlockSize.x); // 열 
	// int Row = (int)(MousePos.y / m_SingleBlockSize.y) + (m_RowCount / 2); // 행 
	// m_vecBlocks[Row * m_ColCount + Col]->SetBlockType(BlockType::EMPTY);

	// Tile 선택이 안되면, return 처리 
	if (m_Click == 0)
	{
		// 선택된 Block 의 Idx 
		m_ClickFirstIdxX = (int)(MousePos.x / m_SingleBlockSize.x); // 열 
		m_ClickFirstIdxY = (int)(MousePos.y / m_SingleBlockSize.y) + (m_RowCount / 2); // 행 
		
		m_ClickFirstPos = m_vecBlocks[m_ClickFirstIdxY * m_ColCount + m_ClickFirstIdxX]->GetPos();

		m_Click = 1;
	}
	else if (m_Click == 1)
	{
		// 선택된 Block 의 Idx 
		m_ClickSecIdxX = (int)(MousePos.x / m_SingleBlockSize.x); // 열 
		m_ClickSecIdxY = (int)(MousePos.y / m_SingleBlockSize.y) + (m_RowCount / 2); // 행 

		/*
		{
			// Debug 용 코드 --> 동일한 애를 클릭할 때마다 새로운 Animal Type으로 바꿔주기
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
			// Move 처리
			m_IsTwoMoving = true;

			// 선택된 녀석 Block 상태 바꾸기 
			int FirstCellIdx = m_ClickFirstIdxY * m_ColCount + m_ClickFirstIdxX;
			int SecCellIdx  = m_ClickSecIdxY * m_ColCount + m_ClickSecIdxX;
	
			// New Pos 세팅 
			m_vecCells[FirstCellIdx]->SetNewPos(m_ClickSecPos);
			m_vecCells[SecCellIdx]->SetNewPos(m_ClickFirstPos);
			
			// Dir 세팅 
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
			// 범위 밖의 Cell 클릭 
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

	// Board 생성하기 
	Vector2 TextureSize = Vector2((float)m_BlockTexture->GetWidth(), (float)m_BlockTexture->GetHeight());
	CreateBoard(4, 4, TextureSize);

	// 초기화
	m_ChangedCellRowInfo.reserve(m_RowCount);
	for (int i = 0; i < m_RowCount; i++)
	{
		std::vector<int> vec(m_ColCount, 0);
		m_ChangedCellRowInfo.push_back(vec);
	}

	// 새로 생성될 Cell 개수 정보
	m_NewCellNeeded.resize(m_ColCount);

	// Destroyed Cells
	m_vecDestroyedCells.reserve(m_RowCount);
	for (int i = 0; i < m_RowCount; i++)
	{
		std::vector<bool> vec(m_ColCount, false);
		m_vecDestroyedCells.push_back(vec);
	}

	// 위치 바뀐 Cell 들 
	m_vecCellsPosChanged.reserve(m_RowCount);
	for (int i = 0; i < m_RowCount; i++)
	{
		std::vector<bool> vec(m_ColCount, false);
		m_vecCellsPosChanged.push_back(vec);
	}

	// 위치 바뀐 Cell 들 종료 여부 
	m_vecCellsPosChangedEnd.reserve(m_RowCount);
	for (int i = 0; i < m_RowCount; i++)
	{
		std::vector<bool> vec(m_ColCount, false);
		m_vecCellsPosChangedEnd.push_back(vec);
	}

	// 혹시 모르니, 초기 Board는 Matchable한 Board가 되도록 세팅한다.
	MakeMatchableBoard();

	return true;
}

bool CBoard::Update(float DeltaTime)
{
	Start();
	int Index = -1;

	if (m_BlockCount > 0)
	{
		// 1) 사라질 Block 표시하기 ( 알고리즘 적용하기 )
		bool UpdateEnable = CheckRemoveEnable();

		// 모든 Cell 들이 내려왔을 때, 그 순간에만 Cell 들을 Match 하는지 검사 + 제거
		bool IsMatch = true;
		if (UpdateEnable)
		{
			// Match 되는 Cell 들이 있는지 체크한다. -> Match 되는Cell 들을 제거 표시해둔다.
			bool IsMatch = CheckMatchCells();

			// Match 되는 애가 없다면 --> 해당 판이 가능한 판인지 체크한다.
			// 해당 함수 안에서, Match 가능한 판이 존재하도록 판을 마련할 것이다.
			// 1) 먼저, Match 가능한 Cell 들은 없어야 한다
			// 2) 그러면서, Match 가능한 Cell 들은 있되
			// 3) 실제 Match 되는 Cell 들은 없을 때까지 Shuffle을 돌린다.
			if (!IsMatch)
				MakeMatchableBoard();
		}


		// Cells Idx 변화 정보 초기화 
		for (int row = 0; row < m_RowCount; row++)
		{
			for (int col = 0; col < m_ColCount; col++)
			{
				m_ChangedCellRowInfo[row][col] = 0;
			}
		}

		// 2) 실제 사라지게 하기 + New Pos 세팅 + 보여지는 Real Board만 검사 
		RemoveCells();

		// 실제 Update
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

		// 3) 새로운 Idx , 방향 정보 세팅
		ChangeCellsInfos();

		// 4) 새로운 Cell 생성
		CreateNewCells();

		// 5) 클릭한 2개 Cell 들 서로 이동
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
	// 모든 Block, Cell들이 채워질 때만 Render
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

			// Idx 정보가 바뀐 이후에도
			// 새롭게 세팅된 Cell 들의 Active가 False로 세팅되어버린다

			if (!m_vecCells[Index]->IsActive())
			{
				// 메모리 해제
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

	// 위치가 변한 Cell이 하나도 없다면,
	// 애초에, Remove를 해줄 필요가 없다.
	if (N == 0)
		return false;

	// 이동 완료 여부 검사 
	for (int r = 0; r < m_RowCount; r++)
	{
		for (int c = 0; c < m_ColCount; c++)
		{
			// 변함
			if (m_vecCellsPosChanged[r][c])
			{
				// 이동 완료 x
				if (!m_vecCellsPosChangedEnd[r][c])
				{
					return false;
				}
			}
		}
	}

	// 여기까지 내려오면 변한 모든 Cell 들 이동 완료 --> 초기화
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

		// Pos 정보 바꾸기 
		// Vector2 PrevPos = m_vecCells[CurIdx]->GetPos(); X --> 실시간 New Pos 근거 Update
		Vector2 PrevPos = m_vecCells[CurIdx]->GetNewPos();
		float NewYPos = PrevPos.y + m_SingleBlockSize.y;
		m_vecCells[CurIdx]->SetNewPos(PrevPos.x, NewYPos);

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

			// 방향 아래로 내려가게 세팅 
			// m_vecCells[CurIdx]->SetDir(Vector2(0.f, 1.f));

			// 새로운 Idx 구하기 
			AddedRow = m_ChangedCellRowInfo[Row][Col];
			NxtIdx = (Row + AddedRow) * m_ColCount + Col;

			// Idx 정보 다시 세팅해주기 
			m_vecCells[CurIdx]->SetIdxInfos(Row + AddedRow, Col, NxtIdx);

			bool Active = m_vecCells[CurIdx]->IsActive();

			// 배열 내 Cell 정보 바꾸기
			m_vecCells[NxtIdx] = m_vecCells[CurIdx];
			bool NxtActive = m_vecCells[NxtIdx]->IsActive();

			// 이동 배열로 표시하기
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

			// NewCell 들은 결과적으로 아래로 내려갈 것이므로 m_IsMoving을 true로 세팅
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

	if (!m_IsTwoMoving) // 2개의 Mouse가 이동 중일때만 동작하게 한다. 
		return;

	// 현재 위치, 자신 위치 차이 계산하기 
	Vector2 FirstCellDf = m_ClickFirstCell->GetNewPos() - m_ClickFirstCell->GetPos();
	Vector2 SecCellDf  = m_ClickSecCell->GetNewPos() - m_ClickSecCell->GetPos();

	// Cell Idx 정보 세팅
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
		// NewPos로 완전히 세팅 
		m_ClickFirstCell->SetPos(m_ClickFirstCell->GetNewPos());
		m_ClickSecCell->SetPos(m_ClickSecCell->GetNewPos());

		// Dir 세팅 
		m_ClickFirstCell->SetDir(Vector2(0.f, 0.f));
		m_ClickSecCell->SetDir(Vector2(0.f, 0.f));

		// IsSwapping 조건 세팅
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

		// 실제 vecCell 내의 정보 세팅 
		CCell* tempCell					= m_vecCells[FirstCellIdx];
		m_vecCells[FirstCellIdx]	    = m_vecCells[SecCellIdx];
		m_vecCells[SecCellIdx] = tempCell;

		if (!m_PrevMisMatched) // 처음 바꾸는 경우 ( 틀려서 되돌아오는 경우에는 Match 여부를 조사하지 않는다 )
		{
			// 여기서 한번 검사하기
			bool IsMatch = CheckMatchCells();

			if (!IsMatch)
			{
				// 만약 맞지 않다면, 다시 반대로 시행하기 
				FirstCellIdx = SecondCellRow * m_ColCount + SecondCellCol;
				SecCellIdx = FirstCellRow * m_ColCount + FirstCellCol;

				// New Pos 세팅 
				// m_vecCells[FirstCellIdx] 에는 SecondCellIdx 내용의 Cell이 들어있다. 
				// m_vecCells[SecCellIdx] 에는 FirstCellIdx 내용의 Cell이 들어있다. 
				m_vecCells[FirstCellIdx]->SetNewPos(m_ClickFirstPos);
				m_vecCells[SecCellIdx]->SetNewPos(m_ClickSecPos);

				// Dir 세팅 
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
				// 만약 맞다면, Mouse + Move 관련 변수들 초기화
				m_IsTwoMoving = false;
				m_Click = 0;
				m_ClickFirstCell = nullptr;
				m_ClickSecCell = nullptr;
				m_PrevMisMatched = false;
			}
		}
		// 다시 되돌아온 경우 : 무조건 관련 변수들 초기화 
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
	// 검사 결과를 저장하는 2차원 배열 세팅 (초기화)
	for (int Row = 0; Row < m_RowCount; Row++)
	{
		for (int Col = 0; Col < m_ColCount; Col++)
		{
			m_vecDestroyedCells[Row][Col] = false;
		}
	}

	// 현재 row, col
	// 이전 위치 AnimalType, 같은 개수, 방향 x, y
	// 만약 3 이상 이고, 더이상 같은 것이 없다면 터뜨리기
	std::queue<std::vector<int>> queue;
	int cRow = -1, cCol = -1, nRow = -1, nCol = -1, Index = -1;
	bool IsMatch = false;

	// Row 검사 ( 가로 검사 ) --> 실제 보여지는 Real Board 에 대해서만 진행할 것이다. 
	for (int RMatchLen = 3; RMatchLen <= m_ColCount; RMatchLen++)
	{
		// 모든 Cell 들에 대해서 검사한다. ( 보여지는 화면 만 검사하기 )
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

	// Column 검사
	for (int CMatchLen = 3; CMatchLen <= m_RowCount / 2; CMatchLen++)
	{
		// 모든 Cell 들에 대해서 검사한다.
		for (int Col = 0; Col < m_ColCount; Col++)
		{
			// 각 Colum --> 세로로 검사할 예정이다. 
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

	// 모두 검사한 다음 , 사라지는 Cell 들에 대해 Block을 Empty로 만든다.
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

	// 만약 일치하는 Cell이 하나도 없다면, false를 return 한다.
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
	// 가능한 조합이 있다면 건너뛴다.
	if (CheckMatchPossible())
		return true;

	// 모든 Block 들을 노란색으로 만든다 == 섞는 중 !
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

		// 2. 만약 CheckMatchCell 이 false 라면 ( 현재 맞는 것이 없다면 검사 ) --> 현재 맞는 놈들이 있다면 다시 Shuffle 
		// 다시 Cell 들을 섞는 과정을 거친다.
		if (CheckMatchCells())
			continue;

		// 만약 가능한 조합이 나왔다면, 이제 다시 원상 복구 
		if (CheckMatchPossible())
		{
			// 다시 모든 Block 들의 색상을 초록으로 만든다 
			// 모든 Block 들을 노란색으로 만든다 == 섞는 중 !
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
	// 1. Board 가 다 내려와서 자기 위치에 도달하면 조사하는 코드

	int MinMatchUnit = 3;

	int CurIdx = -1, NxtIdx = -1, LastIdx = -1;

	// 가로 검사 
	for (int row = m_RowCount / 2; row < m_RowCount; row++)
	{
		for (int col = 0; col <= m_ColCount - (MinMatchUnit - 1); col++)
		{
			// 연속된 2개는 맞아야 한다
			CurIdx = row * m_ColCount + col;
			NxtIdx = row * m_ColCount + col + 1;
			if (m_vecCells[CurIdx]->GetAnimalType() != m_vecCells[NxtIdx]->GetAnimalType())
				continue;

			// 오른쪽 위 아래를 조사한다.
			// 위 조사
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

				// 아래 조사
				if (row + 1 < m_RowCount)
				{
					LastIdx = (row + 1) * m_ColCount + col + 2;
					if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
						return true;
				}
			}

			// 왼쪽 방향 한칸 위, 아래를 조사한다
			if (col - 1 >= 0)
			{
				// 위쪽 
				if (row - 1 >= 0)
				{
					LastIdx = (row - 1) * m_ColCount + col - 1;
					if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
					{
						return true;
					}
				}

				// 아래 조사
				if (row + 1 < m_RowCount)
				{
					LastIdx = (row + 1) * m_ColCount + col - 1;
					if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
						return true;
				}
			}


			// 2칸 왼쪽, 3칸 오른쪽을 검사한다.
			if (col - 2 >= 0)
			{
				LastIdx = row * m_ColCount + col - 2;
				if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
					return true;
			}

			// 2칸 왼쪽, 2칸 오른쪽을 검사한다.
			if (col + 3 < m_ColCount)
			{
				LastIdx = row * m_ColCount + col + 3;
				if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
					return true;
			}
		}
	}

	// 세로 검사
	for (int col = 0; col <= m_ColCount - MinMatchUnit; col++)
	{
		for (int row = m_RowCount / 2; row <= m_RowCount - (MinMatchUnit - 1); row++)
		{
			// 연속된 2개는 맞아야 한다
			CurIdx = row * m_ColCount + col;
			NxtIdx = (row + 1) * m_ColCount + col;
			if (m_vecCells[CurIdx]->GetAnimalType() != m_vecCells[NxtIdx]->GetAnimalType())
				continue;

			// 아래 방향 왼쪽 오른쪽을 비교한다. 
			// 왼쪽
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
				// 오른쪽
				if (col + 1 < m_ColCount)
				{
					LastIdx = (row + 2) * m_ColCount + col + 1;
					if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
					{
						return true;
					}
				}
			}


			// 위의 방향 왼쪽 오른쪽을 비교한다. 
			// 왼쪽
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
				// 오른쪽
				if (col + 1 < m_ColCount)
				{
					LastIdx = (row - 1) * m_ColCount + col + 1;
					if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
					{
						return true;
					}
				}
			}

			// 위로 2칸, 아래로 2칸 짜리를 검사한다.
			if (row - 2 >= m_RowCount / 2)
			{
				LastIdx = (row - 2) * m_ColCount + col;
				if (m_vecCells[CurIdx]->GetAnimalType() == m_vecCells[LastIdx]->GetAnimalType())
					return true;
			}

			// 3칸 왼쪽, 2칸 오른쪽을 검사한다.
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

			// Cell 이동과 관련된 변수들도 모두 초기화 해준다
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
