#include "Cell.h"

CCell::CCell() :
	m_CellType(CellType::BASIC),
	m_AnimalType(AnimalType::PANDA)
{
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
