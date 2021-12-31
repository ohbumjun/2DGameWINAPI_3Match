#include "StartStage.h"
#include "../UI/UIWindow.h"
#include "../UI/UIButton.h"
#include "../UI/UIText.h"

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

    // Window


    return true;
}

void CStartStage::LoadAnimationSequence()
{
}
