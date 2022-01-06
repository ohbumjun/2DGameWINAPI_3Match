#include "BasicStage.h"
#include "../Object/Board.h"
#include "../UI/UIText.h"
#include "../UI/UIButton.h"
#include "../UI/UINumberWidget.h"
#include "StageManager.h"
#include "StartStage.h"
#include "../Resource/ResourceManager.h"

CBasicStage::CBasicStage() :
	m_NumberWidget(nullptr)
{
}

CBasicStage::~CBasicStage()
{
}

bool CBasicStage::Init()
{
	CStage::Init();
	LoadAnimationSequence();

	// Sound
	LoadSound();

	// UI
	CUIWindow* Window = CreateUIWindow<CUIWindow>("Window");
	Window->SetPos(Vector2(GetBoard()->GetRealBoardSize().x, 0.f));

	CUIButton* Button = Window->CreateWidget<CUIButton>("ExitButton");
	// Button->SetTexture("ExitButton", TEXT("ButtonBack.bmp"), TEXTURE_PATH);
	Button->SetTexture("ExitButton", TEXT("ButtonBack.bmp"), TEXTURE_PATH);
	Button->SetPos(Vector2(Button->GetSize().x / 8.f, Button->GetSize().y / 2.f));
	Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(200.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Click, Vector2(400.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(600.f, 0.f), Vector2(200.f, 100.f));
	Button->SetButtonClickCallback(Window, &CUIWindow::Exit);
	Button->SetMouseHoverSound("ButtonHover");

	CUIText* Text = Window->CreateWidget<CUIText>("ExitText"); 
	Text->SetPos(Button->GetPos() + Vector2(Button->GetSize().x / 8.f - 10.f, Button->GetSize().y / 2.f - 10.f));
	Text->SetText(TEXT("EXIT"));
	Text->SetTextColor(0, 0, 0);
	Text->SetZOrder(1);

	Text = Window->CreateWidget<CUIText>("ScoreText");
	Text->SetPos(Vector2(Button->GetPos().x, Button->GetPos().y * 4));
	Text->SetText(TEXT("Score"));
	Text->SetTextColor(0, 0, 0);
	Text->SetZOrder(1);

	m_NumberWidget = Window->CreateWidget<CUINumberWidget>("ScoreNumber");
	m_NumberWidget->SetPos(Vector2(Button->GetPos().x * 2.f, Button->GetPos().y * 3.7f));

	// Go Back To Intro Scene
	Button = Window->CreateWidget<CUIButton>("BackButton");
	Button->SetTexture("BackButton", TEXT("ButtonBack.bmp"), TEXTURE_PATH);
	Button->SetPos(Vector2((Button->GetSize().x / 8.f), Button->GetSize().y * 4.f));
	Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(200.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Click, Vector2(400.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(600.f, 0.f), Vector2(200.f, 100.f));
	Button->SetButtonClickCallback(this, &CBasicStage::GoBackToIntro);
	Button->SetMouseHoverSound("ButtonHover");

	Text = Window->CreateWidget<CUIText>("Intro");
	Text->SetPos(Vector2(Button->GetPos().x + Button->GetSize().x / 8.f - 10.f, Button->GetPos().y + Button->GetSize().y / 2.f - 10.f));
	Text->SetText(TEXT("Intro"));
	Text->SetTextColor(0, 0, 0);
	Text->SetZOrder(1);

	return true;
}

void CBasicStage::LoadAnimationSequence()
{
	SetCharactersAnimation();
}

void CBasicStage::LoadSound()
{
	CResourceManager::GetInst()->CreateSound("BGMBasic", "BGM", true, "HomeBgm.wav");
	CResourceManager::GetInst()->SoundPlay("BGMBasic");
	m_vecSoundNames.push_back("BGMBasic");

	CResourceManager::GetInst()->CreateSound("ButtonHover", "Effect", false, "UI_Hover.wav");
	m_vecSoundNames.push_back("ButtonHover");

	CResourceManager::GetInst()->CreateSound("CellMove", "Effect", false, "grass-step-1.ogg");
}

bool CBasicStage::Update(float DeltaTime)
{
	if (m_NumberWidget && m_Board)
		m_NumberWidget->SetNumber(m_Board->GetMatchCount());

	CStage::Update(DeltaTime);

	return true;
}

void CBasicStage::GoBackToIntro()
{
	CStageManager::GetInst()->CreateStage<CStartStage>();
	CStage::m_StageEnable = false;
}
