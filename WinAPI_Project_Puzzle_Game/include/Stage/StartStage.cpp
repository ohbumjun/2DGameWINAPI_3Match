#include "StartStage.h"

CStartStage::CStartStage()
{
}

CStartStage::~CStartStage()
{
}

bool CStartStage::Init()
{
    if (!CStage::Init())
        return false;
    return true;
}

void CStartStage::LoadAnimationSequence()
{
}
