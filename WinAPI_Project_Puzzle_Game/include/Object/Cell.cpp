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

bool CCell::Init()
{
	return true;
}

bool CCell::Update(float DeltaTime)
{
	return false;
}

bool CCell::PostUpdate(float DeltaTime)
{
	return false;
}

bool CCell::Render(HDC hDC)
{
	return false;
}
