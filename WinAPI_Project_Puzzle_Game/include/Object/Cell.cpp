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
	SetCurrentAnimation(m_AnimalNames[m_AnimalType]);
	
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
