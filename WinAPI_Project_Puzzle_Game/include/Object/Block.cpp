#include "Board.h"
#include "Block.h"

CBlock::CBlock() :
	m_BlockType(BlockType::BASIC),
	m_Board(nullptr),
	m_MoveEnable(false)
{
}

CBlock::~CBlock()
{
}

void CBlock::Start()
{
	CGameObject::Start();
}

bool CBlock::Init()
{
	if (!CGameObject::Init())
		return false;
	return true;
}

void CBlock::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CBlock::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CBlock::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}


CBlock* CBlock::Clone()
{
	return new CBlock(*this);
}
