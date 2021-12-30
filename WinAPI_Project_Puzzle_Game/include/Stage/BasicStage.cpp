#include "BasicStage.h"
#include "../Object/Board.h"
#include "../UI/UIText.h"
#include "../UI/UIButton.h"
#include "../UI/UINumberWidget.h"

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
	Button->SetPos(Vector2(Vector2(Button->GetSize().x / 4.f, Button->GetSize().y / 2.f)));
	Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(200.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Click, Vector2(400.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(600.f, 0.f), Vector2(200.f, 100.f));
	Button->SetButtonClickCallback(Window, &CUIWindow::Exit);

	CUIText* Text = Window->CreateWidget<CUIText>("ExitText"); //
	Text->SetPos(Button->GetPos());
	Text->SetText(TEXT("EXIT"));
	Text->SetTextColor(255.f, 255.f, 255.f);
	Text->SetZOrder(1);

	/*
	m_NumberWidget = Window->CreateWidget<CUINumberWidget>("ScoreNumber");
	m_NumberWidget->SetPos(ButtonPos * 2);
	*/

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
