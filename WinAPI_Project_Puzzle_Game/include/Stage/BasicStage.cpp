#include "BasicStage.h"
#include "../Object/Board.h"
#include "../UI/UIText.h"
#include "../UI/UIButton.h"
#include "../UI/UINumberWidget.h"
#include "StageManager.h"
#include "StartStage.h"

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

	Button = Window->CreateWidget<CUIButton>("BackButton");
	// Button->SetTexture("ExitButton", TEXT("ButtonBack.bmp"), TEXTURE_PATH);
	Button->SetTexture("BackButton", TEXT("ButtonBack.bmp"), TEXTURE_PATH);
	Button->SetPos(Vector2((Button->GetSize().x / 8.f), Button->GetSize().y * 4.f));
	Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(200.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Click, Vector2(400.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(600.f, 0.f), Vector2(200.f, 100.f));
	Button->SetButtonClickCallback(this, &CBasicStage::GoBackToIntro);
	return true;
}

void CBasicStage::LoadAnimationSequence()
{
	SetCharactersAnimation();
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
}
