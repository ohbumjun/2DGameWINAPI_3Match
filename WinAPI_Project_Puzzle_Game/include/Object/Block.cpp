#include "Block.h"

CBlock::CBlock() :
	m_BlockType(BlockType::BASIC)
{
}

CBlock::~CBlock()
{
}

bool CBlock::Init()
{
	return true;
}

bool CBlock::Update(float DeltaTime)
{
	return false;
}

bool CBlock::PostUpdate(float DeltaTime)
{
	return false;
}

bool CBlock::Render(HDC hDC)
{
	return false;
}
