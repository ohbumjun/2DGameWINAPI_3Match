#include "BasicStage.h"


CBasicStage::CBasicStage()
{
}

CBasicStage::~CBasicStage()
{
}

bool CBasicStage::Init()
{
	LoadAnimationSequence();

	return false;
}

void CBasicStage::LoadAnimationSequence()
{
	SetCharactersAnimation();
}
