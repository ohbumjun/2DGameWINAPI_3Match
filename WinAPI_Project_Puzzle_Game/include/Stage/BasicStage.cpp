#include "BasicStage.h"
#include "../Object/Board.h"
#include "../UI/UIText.h"
#include "../UI/UIButton.h"

CBasicStage::CBasicStage()
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
	CUIWindow* ButtonWindow = CreateUIWindow<CUIWindow>("Window");
	ButtonWindow->SetPos(100.f, 0.f);

	CUIButton* Button = ButtonWindow->CreateWidget<CUIButton>("ExitButton");
	// Button->SetTexture("ExitButton", TEXT("ButtonBack.bmp"), TEXTURE_PATH);
	Button->SetTexture("ExitButton", TEXT("ButtonBack.bmp"), TEXTURE_PATH);
	Vector2 ButtonPos = Vector2(GetBoard()->GetRealBoardSize().x, 0.f);
	Button->SetPos(GetBoard()->GetPos() + ButtonPos);
	Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(200.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Click, Vector2(400.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(600.f, 0.f), Vector2(200.f, 100.f));

	CUIText* Text = ButtonWindow->CreateWidget<CUIText>("ExitText");
	Vector2 TextPos = ButtonPos + Vector2(Button->GetSize().x / 4.f - 10.f , Button->GetSize().y / 2.f - 10.f);
	Text->SetPos(TextPos);
	Text->SetText(TEXT("EXIT"));
	Text->SetTextColor(255.f, 255.f, 255.f);
	Text->SetZOrder(1);

	return true;
}

void CBasicStage::LoadAnimationSequence()
{
	SetCharactersAnimation();
}
