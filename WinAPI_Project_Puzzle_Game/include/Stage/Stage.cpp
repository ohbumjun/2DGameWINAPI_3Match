#include "Stage.h"
#include "../Object/Board.h"
#include "../Resource/ResourceManager.h"

CStage::CStage(int row, int col) :
	m_Row(row),
	m_Col(col),
	m_Board(nullptr),
	m_StageLevel(0)
{
}

CStage::CStage(const CStage& Stage)
{
	m_Row = Stage.m_Row;
	m_Col = Stage.m_Col;;
	if (Stage.m_Board)
		m_Board = Stage.m_Board;
}

CStage::~CStage()
{
	SAFE_DELETE(m_Board);
}

CGameObject* CStage::FindObject(const std::string& Name)
{
	return nullptr;
}

bool CStage::Init()
{
	m_Board = new CBoard;

	if (!m_Board->Init())
		return false;

	SetCharactersAnimation();

	return true;
}

bool CStage::Update(float DeltaTime)
{
	m_Board->Update(DeltaTime);
	return true;
}

bool CStage::PostUpdate(float DeltaTime)
{
	m_Board->PostUpdate(DeltaTime);
	return true;
}

bool CStage::Render(HDC hDC)
{
	m_Board->Render(hDC);
	return true;
}

void CStage::SetCharactersAnimation()
{
	bool Exist = false;
	// Bear, Duck, Elephant, Panda, Parrot, Penguin
	Exist = CResourceManager::GetInst()->CreateAnimationSequence("Bear", "Bear", TEXT("bear.bmp"));
	if (Exist)
	{
		CResourceManager::GetInst()->SetTextureColorKey("Bear", 255, 255, 255);
		for (int i = 0; i < 1; ++i)
			CResourceManager::GetInst()->AddAnimationFrameData("Bear", 0.f, i * 128.f, 128.f, 128.f);
	}

	Exist = CResourceManager::GetInst()->CreateAnimationSequence("Duck", "Duck", TEXT("duck.bmp"));
	if (Exist)
	{
		CResourceManager::GetInst()->SetTextureColorKey("Duck", 255, 255, 255);
		for (int i = 0; i < 1; ++i)
			CResourceManager::GetInst()->AddAnimationFrameData("Duck", 0.f, i * 128.f, 128.f, 128.f);
	}

	Exist = CResourceManager::GetInst()->CreateAnimationSequence("Elephant", "Elephant", TEXT("elephant.bmp"));
	if (Exist)
	{
		CResourceManager::GetInst()->SetTextureColorKey("Elephant", 255, 255, 255);
		for (int i = 0; i < 1; ++i)
			CResourceManager::GetInst()->AddAnimationFrameData("Elephant", 0.f, i * 128.f, 128.f, 128.f);
	}

	Exist = CResourceManager::GetInst()->CreateAnimationSequence("Panda", "Panda", TEXT("panda.bmp"));
	if (Exist)
	{
		CResourceManager::GetInst()->SetTextureColorKey("Panda", 255, 255, 255);
		for (int i = 0; i < 1; ++i)
			CResourceManager::GetInst()->AddAnimationFrameData("Panda", 0.f, i * 128.f, 128.f, 128.f);
	}

	Exist = CResourceManager::GetInst()->CreateAnimationSequence("Parrot", "Parrot", TEXT("parrot.bmp"));
	if (Exist)
	{
		CResourceManager::GetInst()->SetTextureColorKey("Parrot", 255, 255, 255);
		for (int i = 0; i < 1; ++i)
			CResourceManager::GetInst()->AddAnimationFrameData("Parrot", 0.f, i * 128.f, 128.f, 128.f);
	}

	Exist = CResourceManager::GetInst()->CreateAnimationSequence("Penguin", "Penguin", TEXT("penguin.bmp"));
	if (Exist)
	{
		CResourceManager::GetInst()->SetTextureColorKey("Penguin", 255, 255, 255);
		for (int i = 0; i < 1; ++i)
			CResourceManager::GetInst()->AddAnimationFrameData("Penguin", 0.f, i * 128.f, 128.f, 128.f);
	}
}

/*
void CStage::ComposeStage()
{
	m_Board->ComposeStage();
}
*/