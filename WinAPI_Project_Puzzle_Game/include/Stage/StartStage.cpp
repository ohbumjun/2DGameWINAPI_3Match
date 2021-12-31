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
    Button->SetPos(Vector2((RS.Width / 4.f) * 3 - 100.f, RS.Height / 5.f));
    Button->SetTexture("StartButton", TEXT("ButtonBack.bmp"));

    Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(200.f, 100.f));
    Button->SetFrameData(EButton_State::MouseOn, Vector2(200.f, 0.f), Vector2(200.f, 100.f));
    Button->SetFrameData(EButton_State::Click, Vector2(400.f, 0.f), Vector2(200.f, 100.f));
    Button->SetFrameData(EButton_State::Disable, Vector2(600.f, 0.f), Vector2(200.f, 100.f));


    CUIText* Text = Window->CreateWidget<CUIText>("MiniStageText");
    Text->SetPos(Button->GetPos() + Vector2(Button->GetSize().x / 8.f, Button->GetSize().y / 2.f - 10.f));
    Text->SetZOrder(1);
    Text->SetText(TEXT("5 * 5"));

    // 
    Button = Window->CreateWidget<CUIButton>("MediumStageButton");
    Button->SetPos(Vector2((RS.Width / 4.f) * 3 - 100.f, (RS.Height / 5.f) * 2));
    Button->SetTexture("StartButton", TEXT("ButtonBack.bmp"));

    Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(200.f, 100.f));
    Button->SetFrameData(EButton_State::MouseOn, Vector2(200.f, 0.f), Vector2(200.f, 100.f));
    Button->SetFrameData(EButton_State::Click, Vector2(400.f, 0.f), Vector2(200.f, 100.f));
    Button->SetFrameData(EButton_State::Disable, Vector2(600.f, 0.f), Vector2(200.f, 100.f));


    Text = Window->CreateWidget<CUIText>("MediumStageText");
    Text->SetPos(Button->GetPos() + Vector2(Button->GetSize().x / 8.f, Button->GetSize().y / 2.f - 10.f));
    Text->SetZOrder(1);
    Text->SetText(TEXT("5 * 5"));

    //
    Button = Window->CreateWidget<CUIButton>("MaxStageButton");
    Button->SetPos(Vector2((RS.Width / 4.f) * 3 - 100.f, (RS.Height / 5.f) * 3));
    Button->SetTexture("StartButton", TEXT("ButtonBack.bmp"));

    Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(200.f, 100.f));
    Button->SetFrameData(EButton_State::MouseOn, Vector2(200.f, 0.f), Vector2(200.f, 100.f));
    Button->SetFrameData(EButton_State::Click, Vector2(400.f, 0.f), Vector2(200.f, 100.f));
    Button->SetFrameData(EButton_State::Disable, Vector2(600.f, 0.f), Vector2(200.f, 100.f));


    Text = Window->CreateWidget<CUIText>("MediumStageText");
    Text->SetPos(Button->GetPos() + Vector2(Button->GetSize().x / 8.f, Button->GetSize().y / 2.f - 10.f));
    Text->SetZOrder(1);
    Text->SetText(TEXT("5 * 5"));

    return true;
}

void CStartStage::LoadAnimationSequence()
{
}

void CStartStage::InitButtonUI(CUIButton*& Button, int Num)
{
    
}

void CStartStage::SelectMiniStage()
{
    m_Row = 5;
    m_Col   = 5;
}

void CStartStage::SelectMediumStage()
{
    m_Row = 7;
    m_Col   = 7;
}

void CStartStage::SelectMaxStage()
{
    m_Row = 9;
    m_Col   = 9;
}
