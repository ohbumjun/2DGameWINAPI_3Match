#include "StageManager.h"
#include "../Input.h"
#include "Stage.h"

CStageManager* CStageManager::m_Inst = nullptr;

CStageManager::CStageManager() :
    m_Stage(nullptr),
    m_NextStage(nullptr)
{
}

CStageManager::~CStageManager()
{
    SAFE_DELETE(m_Stage);
    SAFE_DELETE(m_NextStage);
}

void CStageManager::Start()
{
}

bool CStageManager::Init()
{
    m_Stage = new CStage;
    m_Stage->Init();
    return true;
}

bool CStageManager::Update(float DeltaTime)
{
    m_Stage->Update(DeltaTime);
    return ChangeStage() ;
}

bool CStageManager::PostUpdate(float DeltaTime)
{
    m_Stage->PostUpdate(DeltaTime);
    return ChangeStage();
}

bool CStageManager::Render(HDC hDC)
{
    m_Stage->Render(hDC);
    return ChangeStage();
}

bool CStageManager::ChangeStage()
{
    if (m_NextStage)
    {
        SAFE_DELETE(m_Stage);
        CInput::GetInst()->ClearCallback();

        m_NextStage->Init();
        m_Stage = m_NextStage;
        m_NextStage = nullptr;
    }
    return false;
}

/*
void CStageManager::BuildStage()
{
    // 생성한 Stage 정보를 이용해서 씬을 구성한다
    m_Stage->ComposeStage();
}
*/