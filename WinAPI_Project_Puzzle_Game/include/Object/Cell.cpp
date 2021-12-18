#include "Board.h"
#include "Cell.h"
#include "../GameManager.h"

CCell::CCell() :
	m_CellType(CellType::BASIC),
	m_BlockType(BlockType::BASIC),
	m_AnimalType(AnimalType::PANDA),
	m_MoveSpeed(10.f),
	m_Dir(Vector2(0.f, 1.f)),
	m_TimeScale(1.f),
	m_Board(nullptr),
	m_AnimalNames{"Panda","Bear","Parrot","Elephant","Penguin","Duck"}
{
}

CCell::CCell(const CCell& Cell)
{
	m_CellType = Cell.m_CellType;
	m_AnimalType = Cell.m_AnimalType;
	m_Dir = Cell.m_Dir;;
	m_TimeScale = Cell.m_TimeScale;
	m_MoveSpeed = Cell.m_MoveSpeed;	
	if (Cell.m_Board)
		m_Board = Cell.m_Board;
}

CCell::~CCell()
{
}

void CCell::Start()
{
	CGameObject::Start();

	AddAnimation(m_AnimalNames[m_AnimalType], true, 1.f);
	// SetCurrentAnimation(m_AnimalNames[m_AnimalType]);
}

bool CCell::Init(AnimalType Type)
{
	if (!CGameObject::Init())
		return false;

	m_AnimalType = Type;

	CreateAnimation();
	return true;
}

void CCell::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	
	// Check Block State
	m_BlockType = m_Board->GetBlock(m_RowIndex, m_ColIndex)->GetBlockType();
	
	if (m_BlockType == BlockType::EMPTY)
	{
		Destroy();
		m_Board->ChangeUpperBlockStates(m_RowIndex, m_ColIndex);
	}
	
	// 자기 아래 Block 상태 확인 --> Empty면 계속 내려가게 세팅한다. 
	// WM_MOUSELEAVE
	if (CanMove())
	{
		Move(Vector2(0.0f, 10.f));

		// Update Row Index
		m_RowIndex = m_Pos.y / m_Size.y;
	}

}

void CCell::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

	// 자기 다음 이동 여부 체크 --> 이동 불가능하다면, 자기가 속한 Block 의 상태를 Basic으로 바꿔준다.
	if (!CanMove())
	{
		m_Board->GetBlock(m_RowIndex, m_ColIndex)->SetBlockType(BlockType::BASIC);
		m_Board->GetBlock(m_RowIndex, m_ColIndex)->SetMoveEnable(false);
	}
}

void CCell::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CCell* CCell::Clone()
{
	return new CCell(*this);
}

void CCell::Move(const Vector2& Dir)
{
	Vector2 CurrentMove = Dir * m_MoveSpeed * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale;
	m_Pos += CurrentMove;
}

void CCell::Move(const Vector2& Dir, float Speed)
{
	Vector2 CurrentMove = Dir * Speed * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale;
	m_Pos += CurrentMove;
}

bool CCell::CanMove()
{
	int DownRow = m_RowIndex + 1;

	// 맨 마지막 Idx
	if (m_RowIndex >= (m_Board->GetSize().y / m_Size.y) - 1)
	{
		DownRow -= 1;
		m_Board->GetBlock(DownRow, m_ColIndex)->SetMoveEnable(false);
	}
	return  m_Board->GetBlock(DownRow, m_ColIndex)->GetMoveEnable();
}
