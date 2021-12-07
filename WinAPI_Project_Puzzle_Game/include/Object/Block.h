#pragma once
#include "../GameInfo.h"

class CBlock
{
public :
	CBlock();
	~CBlock();
private :
	BlockType m_BlockType;
public :
	void SetBlockType(BlockType Type)
	{
		m_BlockType = Type;
	}
public :
	BlockType GetBlockType() const
	{
		return m_BlockType;
	}
};

