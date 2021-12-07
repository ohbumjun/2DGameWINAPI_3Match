#include "StageManager.h"
#include "StageBuilder.h"
#include "Stage.h"

CStageManager* CStageManager::m_Inst = nullptr;

CStageManager::CStageManager() :
    m_Init(false),
    m_Stage(nullptr)
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

    BuildStage();

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
    m_Stage = CStageBuilder::BuildStage(0, 9, 9);
}
