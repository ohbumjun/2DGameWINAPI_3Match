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
	m_RowCount = RowCount * 2 ; // Virtual 까지 합쳐서 
	m_ColCount = ColCount;
	m_SquareSize = SquareSize;

	// 기존 Board 제거 
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
	

	// 크기 재할당 
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

	// 여기에서, Random 하게 섞일 수 있는 idx 들을 마련하는 함수를 호출
	// while 문을 돌려서, 
	// 1) Set가 없을 때까지, 찾은 다음 , 찾으면 return 
	// 2) (심화) 적어도 1개는 세팅될 수 있도록 세팅하기 
	// 그 다음 구한 idx 들을 차례대로 Cell에 세팅해서 해당 idx에 맞는 Cell Type --> Texture 세팅하기 


	// 기본 Board Texture Loading 
	Vector2 StartOffset = Vector2(0, -m_Size.y / 2);
	m_TopYPos = StartOffset.y;
	int Index = -1;
	int ObjectNums = 0;

	for (int r = 0; r < m_RowCount; r++)
	{
		for (int c = 0; c < m_ColCount; c++)
		{
			// Vector (x,y) : x는 열, y는 행 
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
				// 화면에 보여지는 Board 에서의 NewCell YIdx 
				NewCell->SetYIdx(1);
			}
		}
	}

	// Board 위에 추가적인 Board 목록을 세팅 
	
	return true;
}

void CBoard::MouseLButton(float DeltaTime)
{
	bool MouseDown = CInput::GetInst()->GetMouseDown();
	if (!MouseDown)
		return;
	Vector2 MousePos = CInput::GetInst()->GetMousePos();

	// Tile 선택
	if (MousePos.x < 0.f || MousePos.x > m_Size.x || MousePos.y < 0.f || MousePos.y > m_Size.y)
		return;

	// 선택된 Block 의 Idx 
	int BlockIdxX = (int)(MousePos.x / m_SquareSize.x); // 열 
	int BlockIdxY = (int)(MousePos.y / m_SquareSize.y) + ( m_RowCount / 2 ); // 행 

	// Tile 선택이 안되면, return 처리 
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

	// 선택된 녀석 Block 상태 바꾸기 
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

		// 제거된 Cell이 있는 Column 확인 + 새로 생성하기 
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
	// 모든 Block, Cell들이 채워질 때만 Render
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
		// Pos 정보 바꾸기 
		Vector2 PrevPos = m_vecCells[CurIdx]->GetPos();
		m_vecCells[CurIdx]->SetNewYPos(PrevPos.x, PrevPos.y + m_SquareSize.y);
		// Idx 정보 다시 세팅해주기 
		m_vecCells[CurIdx]->SetIdxInfos(Row + 1, ColIndex, NxtIdx);
		// 배열 내 Cell 정보 바꾸기
		m_vecCells[NxtIdx] = m_vecCells[CurIdx];
	}
	return true;
}

void CBoard::CreateNewCells(int ColIdx, int NumCells)
{
	Vector2 Pos;
	int  Index;
	AnimalType Type;

	// 각 Real Board 열마다 MoveEnable이 true로 Setting되어 있는 요소 세팅 
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
		} while (Low <= Right  && RenderObjects[Low]->GetYIdx() < RenderObjects[Left]->GetYIdx());

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
