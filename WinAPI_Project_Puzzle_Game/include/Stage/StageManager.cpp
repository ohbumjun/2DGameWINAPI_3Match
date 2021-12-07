#include "StageManager.h"
#include "StageBuilder.h"
#include "Stage.h"

CStageManager::CStageManager() :
    m_Init(false)
{
}

CStageManager::~CStageManager()
{
}

void CStageManager::Start()
{
}

bool CStageManager::Init()
{
    if (m_Init)
        return true;
    m_Init = true;
    return true;
}

bool CStageManager::Update(float DeltaTime)
{
    m_Stage->Update(DeltaTime);
    return true ;
}

bool CStageManager::PostUpdate(float DeltaTime)
{
    m_Stage->PostUpdate(DeltaTime);
    return true;
}

bool CStageManager::Render(HDC hDC)
{
    m_Stage->Render(hDC);
    return true;
}

void CStageManager::BuildStage()
{
    m_Stage = m_StageBuilder->BuildStage(0, 9, 9);
}
