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
	Vector2 StartPos = GetBoard()->GetPos();
	Vector2 StartSize = GetBoard()->GetRealBoardSize();
	Vector2 ButtonPos = StartPos + Vector2(StartSize.x, 0.f);
	Button->SetPos(ButtonPos);

	CUIText* Text = ButtonWindow->CreateWidget<CUIText>("ExitText");
	Text->SetPos(ButtonPos + ButtonWindow->GetPos() - Vector2(10.f, 0.f));
	Text->SetText(TEXT("EXIT"));
	Text->SetTextColor(255.f, 255.f, 255.f);
	Text->SetZOrder(1);

	return true;
}

void CBasicStage::LoadAnimationSequence()
{
	SetCharactersAnimation();
}
