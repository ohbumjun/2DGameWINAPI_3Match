#include "StartStage.h"
#include "BasicStage.h"
#include "Stage.h"
#include "../UI/UIWindow.h"
#include "../UI/UIButton.h"
#include "../UI/UIText.h"
#include "../GameManager.h"
#include "../UI/UIImage.h"
#include "StageManager.h"
#include "../Resource/ResourceManager.h"

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

    // Sound
    LoadSound();

    Resolution RS = CGameManager::GetInst()->GetResolution();

    // Window
    CUIWindow* Window = CreateUIWindow<CUIWindow>("Window");
    CUIImage* Image = Window->CreateWidget<CUIImage>("3MatchTextImage");
    Image->SetTexture("3MatchTextImage", TEXT("3match.bmp"));
    Image->SetPos(Vector2(140.f, 0.f));
        
    // 3 Buttons
    CUIButton* Button = Window->CreateWidget<CUIButton>("MiniStageButton");

    Button->SetButtonClickCallback(this, &CStartStage::SelectMiniStage);
    Button->SetPos(Vector2((RS.Width / 4.f) * 3 - 100.f, RS.Height / 5.f));
    Button->SetTexture("StartButton", TEXT("ButtonBack.bmp"));

    Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(200.f, 100.f));
    Button->SetFrameData(EButton_State::MouseOn, Vector2(200.f, 0.f), Vector2(200.f, 100.f));
    Button->SetFrameData(EButton_State::Click, Vector2(400.f, 0.f), Vector2(200.f, 100.f));
    Button->SetFrameData(EButton_State::Disable, Vector2(600.f, 0.f), Vector2(200.f, 100.f));
    Button->SetMouseHoverSound("ButtonHover");


    CUIText* Text = Window->CreateWidget<CUIText>("MiniStageText");
    Text->SetPos(Button->GetPos() + Vector2(Button->GetSize().x / 8.f - 30.f, Button->GetSize().y / 2.f - 10.f));
    Text->SetZOrder(1);
    Text->SetText(TEXT("5 * 5 Cells"));
    Text->SetTextColor(0, 0, 0);

    // 
    Button = Window->CreateWidget<CUIButton>("MediumStageButton");
    Button->SetPos(Vector2((RS.Width / 4.f) * 3 - 100.f, (RS.Height / 5.f) * 2));
    Button->SetTexture("StartButton", TEXT("ButtonBack.bmp"));

    Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(200.f, 100.f));
    Button->SetFrameData(EButton_State::MouseOn, Vector2(200.f, 0.f), Vector2(200.f, 100.f));
    Button->SetFrameData(EButton_State::Click, Vector2(400.f, 0.f), Vector2(200.f, 100.f));
    Button->SetFrameData(EButton_State::Disable, Vector2(600.f, 0.f), Vector2(200.f, 100.f));
    Button->SetButtonClickCallback(this, &CStartStage::SelectMediumStage);
    Button->SetMouseHoverSound("ButtonHover");

    Text = Window->CreateWidget<CUIText>("MediumStageText");
    Text->SetPos(Button->GetPos() + Vector2(Button->GetSize().x / 8.f - 30.f, Button->GetSize().y / 2.f - 10.f));
    Text->SetZOrder(1);
    Text->SetText(TEXT("7 * 7 Cells"));
    Text->SetTextColor(0, 0, 0);

    Button = Window->CreateWidget<CUIButton>("MaxStageButton");
    Button->SetPos(Vector2((RS.Width / 4.f) * 3 - 100.f, (RS.Height / 5.f) * 3));
    Button->SetTexture("StartButton", TEXT("ButtonBack.bmp"));

    Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(200.f, 100.f));
    Button->SetFrameData(EButton_State::MouseOn, Vector2(200.f, 0.f), Vector2(200.f, 100.f));
    Button->SetFrameData(EButton_State::Click, Vector2(400.f, 0.f), Vector2(200.f, 100.f));
    Button->SetFrameData(EButton_State::Disable, Vector2(600.f, 0.f), Vector2(200.f, 100.f));
    Button->SetButtonClickCallback(this, &CStartStage::SelectMaxStage);
    Button->SetMouseHoverSound("ButtonHover");

    Text = Window->CreateWidget<CUIText>("MediumStageText");
    Text->SetPos(Button->GetPos() + Vector2(Button->GetSize().x / 8.f - 30.f, Button->GetSize().y / 2.f - 10.f));
    Text->SetZOrder(1);
    Text->SetText(TEXT("9 * 9 Cells"));
    Text->SetTextColor(0, 0, 0);

    return true;
}

void CStartStage::LoadAnimationSequence()
{
}

void CStartStage::LoadSound()
{
    CResourceManager::GetInst()->CreateSound("BGMBasic", "BGM", true, "HomeBgm.wav");
    CResourceManager::GetInst()->SoundPlay("BGMBasic");
    m_vecSoundNames.push_back("BGMBasic");

    CResourceManager::GetInst()->CreateSound("ButtonHover", "Effect", false, "UI_Hover.wav");
    m_vecSoundNames.push_back("ButtonHover");
}


void CStartStage::SelectMiniStage()
{
    CStage::s_StaticRow = 5;
    CStage::s_StaticCol = 5;
    CStage::m_StageEnable = true;
    CStageManager::GetInst()->CreateStage<CBasicStage>();
}

void CStartStage::SelectMediumStage()
{
    CStage::s_StaticRow = 7;
    CStage::s_StaticCol = 7;
    CStage::m_StageEnable = true;
    CStageManager::GetInst()->CreateStage<CBasicStage>();
}

void CStartStage::SelectMaxStage()
{
    CStage::s_StaticRow = 9;
    CStage::s_StaticCol = 9;
    CStage::m_StageEnable = true;
    CStageManager::GetInst()->CreateStage<CBasicStage>();
}
