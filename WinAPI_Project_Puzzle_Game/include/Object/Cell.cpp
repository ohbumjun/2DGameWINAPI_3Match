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
	m_AnimalNames{"Panda","Bear","Parrot","Elephant","Penguin","Duck"},
	m_IsMoving(false),
	m_NewPos{},
	m_IsSwapping(false)
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
	}

	if (m_IsSwapping) // 바뀌는 중이라면, 
		return;
	else // 바뀌는 중이 아니라면, 
		m_IsMoving = false;
	
	// 멈추게 끔 세팅해줘야 한다. 
	// 분기처리를 어떻게 해줘야 하는가 
	// 현재 중요한 것은, 마우스 클릭으로 이동하는 애들, 그냥 아래가 비어서
	// 떨이지는 애들을 분리해서 작업해야 한다.
	// 먼저 , 클릭하면 없애줘서, 애들을 내려보자. 
	// 먼저, 현재 Dir가 0,0 이면 멈춘 상태 
	
	// 아래 이동 
	if (m_Pos.y < m_NewPos.y)
	{
		Move(Vector2(0.f, 1.f));
		m_IsMoving = true;
	}
	else
	{
		// 자기 다음 이동 여부 체크 --> 이동 불가능하다면, 자기가 속한 Block 의 상태를 Basic으로 바꿔준다.
		m_Board->GetBlock(m_RowIndex, m_ColIndex)->SetBlockType(BlockType::BASIC);
		m_IsMoving = false;
		// m_Dir = Vector2(0.f, 0.f);
	}
}

void CCell::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

}

void CCell::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CCell* CCell::Clone()
{
	return new CCell(*this);
}

bool CCell::Move(const Vector2& Dir)
{
	// if (Dir.x == 0.f && Dir.y == 0.f)
	//		return false;
		
	Vector2 CurrentMove = Dir * m_MoveSpeed * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale;
	m_Pos += CurrentMove;
	return true;
}

bool CCell::Move(const Vector2& Dir, float Speed)
{
	// if (Dir.x == 0.f && Dir.y == 0.f)
	//		return false;
	Vector2 CurrentMove = Dir * Speed * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale;
	m_Pos += CurrentMove;
	return true;
}
