#include "Cell.h"

CCell::CCell() :
	m_CellType(CellType::BASIC),
	m_AnimalType(AnimalType::PANDA)
{
}

CCell::CCell(const CCell& Cell)
{
	m_CellType = Cell.m_CellType;
	m_AnimalType = Cell.m_AnimalType;
	m_Dir = Cell.m_Dir;;
	m_TimeScale = Cell.m_TimeScale;
	m_MoveSpeed = Cell.m_MoveSpeed;
}

CCell::~CCell()
{
}

void CCell::Start()
{
}

bool CCell::Init()
{
	return true;
}

void CCell::Update(float DeltaTime)
{
}

void CCell::PostUpdate(float DeltaTime)
{
}

void CCell::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CCell* CCell::Clone()
{
	return new CCell(*this);
}
