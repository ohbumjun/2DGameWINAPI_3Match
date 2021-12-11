#include "BasicStage.h"


CBasicStage::CBasicStage()
{
}

CBasicStage::~CBasicStage()
{
}

bool CBasicStage::Init()
{
	CStage::Init();
	LoadAnimationSequence();

	return true;
}

void CBasicStage::LoadAnimationSequence()
{
	SetCharactersAnimation();
}
