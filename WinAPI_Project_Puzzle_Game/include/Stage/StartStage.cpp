#include "StartStage.h"
#include "Stage.h"
#include "../UI/UIWindow.h"
#include "../UI/UIButton.h"
#include "../UI/UIText.h"
#include "../GameManager.h"

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

    Resolution RS = CGameManager::GetInst()->GetResolution();

    // Window
    CUIWindow* Window = CreateUIWindow<CUIWindow>("Window");

    // 3 Buttons
    CUIButton* Button = Window->CreateWidget<CUIButton>("MiniStageButton");
    Button->SetButtonClickCallback(this, &CStartStage::SelectMiniStage);
    Button->SetPos(Vector2(RS.Width / 4.f, RS.Height / 2.f));
    CUIText* Text = Window->CreateWidget<CUIText>("MiniStageText");

    Button = Window->CreateWidget<CUIButton>("MediumStageButton");
    Button->SetButtonClickCallback(this, &CStartStage::SelectMediumStage);
    Button->SetPos(Vector2((RS.Width / 4.f) * 2, RS.Height / 2.f));
    Text = Window->CreateWidget<CUIText>("MediumStageText");

    Button = Window->CreateWidget<CUIButton>("MaxStageButton");
    Button->SetButtonClickCallback(this, &CStartStage::SelectMaxStage);
    Button->SetPos(Vector2((RS.Width / 4.f) * 3, RS.Height / 2.f));
    Text = Window->CreateWidget<CUIText>("MediumStageText");


    return true;
}

void CStartStage::LoadAnimationSequence()
{
}

void CStartStage::SelectMiniStage()
{
    CStage::m_StaticColCount = 5;
    CStage::m_StaticRowCount = 5;
}

void CStartStage::SelectMediumStage()
{
    CStage::m_StaticColCount = 7;
    CStage::m_StaticRowCount = 7;
}

void CStartStage::SelectMaxStage()
{
    CStage::m_StaticColCount = 9;
    CStage::m_StaticRowCount = 9;
}
