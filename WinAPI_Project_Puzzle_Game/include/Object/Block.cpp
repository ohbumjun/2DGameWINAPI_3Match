#include "Block.h"

CBlock::CBlock() :
	m_BlockType(BlockType::BASIC)
{
}

CBlock::~CBlock()
{
}

void CBlock::Start()
{
}

bool CBlock::Init()
{
	return true;
}

void CBlock::Update(float DeltaTime)
{
}

void CBlock::PostUpdate(float DeltaTime)
{
}

void CBlock::Render(HDC hDC)
{
}


CBlock* CBlock::Clone()
{
	return new CBlock(*this);
}
