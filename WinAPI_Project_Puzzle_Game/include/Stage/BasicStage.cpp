#include "BasicStage.h"
#include "../Object/Board.h"
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
	CUIWindow* Window = CreateUIWindow<CUIWindow>("Window");
	Window->SetPos(100.f, 0.f);
	CUIButton* Button = Window->CreateWidget<CUIButton>("ExitButton");

	// Button->SetTexture("ExitButton", TEXT("ButtonBack.bmp"), TEXTURE_PATH);
	Button->SetTexture("ExitButton", TEXT("ButtonBack.bmp"), TEXTURE_PATH);
	Vector2 StartPos = GetBoard()->GetPos();
	Vector2 StartSize = GetBoard()->GetRealBoardSize();
	Button->SetPos(StartPos + Vector2(StartSize.x, 0.f));

	return true;
}

void CBasicStage::LoadAnimationSequence()
{
	SetCharactersAnimation();
}
