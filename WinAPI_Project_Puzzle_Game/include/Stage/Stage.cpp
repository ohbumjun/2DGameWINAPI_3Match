#include "Stage.h"
#include "../Object/Board.h"
#include "../Resource/ResourceManager.h"

int CStage::s_StaticRow         = 0;
int CStage::s_StaticCol          = 0;
bool CStage::m_StageEnable = false;

CStage::CStage() :
	m_Board(nullptr),
	m_StageLevel(0),
	m_UIArray(nullptr),
	m_UICapacity(10),
	m_UICount(0)
{
	m_UIArray = new CUIWindow * [m_UICapacity];
}

CStage::CStage(const CStage& Stage)
{
	if (Stage.m_Board)
		m_Board = Stage.m_Board;
}

CStage::~CStage()
{
	SAFE_DELETE(m_Board); //

	for (int i = 0; i < m_UICount; i++)
	{
		SAFE_DELETE(m_UIArray[i]);
	}
	SAFE_DELETE_ARRAY(m_UIArray);
}

CGameObject* CStage::FindObject(const std::string& Name)
{
	return nullptr;
}

bool CStage::Init()
{
	if (m_StageEnable)
	{
		m_Board = new CBoard;

		if (!m_Board->Init(s_StaticRow, s_StaticCol))
			return false;

		SetCharactersAnimation();
	}

	return true;
}

bool CStage::Update(float DeltaTime)
{
	if (m_Board)
		m_Board->Update(DeltaTime);

	// UI
	for (int i = 0; i < m_UICount;)
	{
		if (!m_UIArray[i]->IsActive())
		{
			SAFE_DELETE(m_UIArray[i]);
			i++;
			continue;
		}
		if (!m_UIArray[i]->GetVisibility())
		{
			++i;
			continue;
		}
		m_UIArray[i]->Update(DeltaTime);
		++i;
	}

	return true;
}

bool CStage::PostUpdate(float DeltaTime)
{
	if (m_Board)
		m_Board->PostUpdate(DeltaTime);

	// UI
	for (int i = 0; i < m_UICount;)
	{
		if (!m_UIArray[i]->IsActive())
		{
			SAFE_DELETE(m_UIArray[i]);
			i++;
			continue;
		}
		if (!m_UIArray[i]->GetVisibility())
		{
			++i;
			continue;
		}
		m_UIArray[i]->PostUpdate(DeltaTime);
		++i;
	}

	return true;
}

bool CStage::Render(HDC hDC)
{
	if (m_Board)
		m_Board->Render(hDC);

	for (int i = 0; i < m_UICount;)
	{
		if (!m_UIArray[i]->IsActive())
		{
			for (int j = i; j < m_UICount - 1; j++)
			{
				m_UIArray[j] = m_UIArray[j + 1];
			}
			--m_UICount;
			continue;
		}
		++i;
	}

	if (m_UICount > 1)
	{
		qsort(m_UIArray, (size_t)m_UICount, sizeof(CUIWindow*), CStage::SortZOrder);
	}

	// 오름 차순 정렬 --> 앞에서부터 그리기 --> Z Order 높은 애들이 앞에 그려질 수 있도록 
	for (int i = 0; i < m_UICount; i ++)
	{
		if (!m_UIArray[i]->GetVisibility())
			continue;
		m_UIArray[i]->Render(hDC);
	}

	return true;
}

int CStage::SortZOrder(const void* Src, const void* Dest)
{
	CUIWindow* SrcWindow   = *(CUIWindow**)Src;
	CUIWindow* DestWindow = *(CUIWindow**)Dest;

	int SrcZOrder = SrcWindow->GetZOrder();
	int DestZOrder = DestWindow->GetZOrder();

	if (SrcZOrder < DestZOrder)
		return 1;
	else if (SrcZOrder > DestZOrder)
		return -1;
	return 0;
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
			CResourceManager::GetInst()->AddAnimationFrameData("Bear", 0.f, i * 80.f, 80.f, 80.f);
	}

	Exist = CResourceManager::GetInst()->CreateAnimationSequence("Duck", "Duck", TEXT("duck.bmp"));
	if (Exist)
	{
		CResourceManager::GetInst()->SetTextureColorKey("Duck", 255, 255, 255);
		for (int i = 0; i < 1; ++i)
			CResourceManager::GetInst()->AddAnimationFrameData("Duck", 0.f, i * 80.f, 80.f, 80.f);
	}

	Exist = CResourceManager::GetInst()->CreateAnimationSequence("Elephant", "Elephant", TEXT("elephant.bmp"));
	if (Exist)
	{
		CResourceManager::GetInst()->SetTextureColorKey("Elephant", 255, 255, 255);
		for (int i = 0; i < 1; ++i)
			CResourceManager::GetInst()->AddAnimationFrameData("Elephant", 0.f, i * 80.f, 80.f, 80.f);
	}

	Exist = CResourceManager::GetInst()->CreateAnimationSequence("Panda", "Panda", TEXT("panda.bmp"));
	if (Exist)
	{
		CResourceManager::GetInst()->SetTextureColorKey("Panda", 255, 255, 255);
		for (int i = 0; i < 1; ++i)
			CResourceManager::GetInst()->AddAnimationFrameData("Panda", 0.f, i * 80.f, 80.f, 80.f);
	}

	Exist = CResourceManager::GetInst()->CreateAnimationSequence("Parrot", "Parrot", TEXT("parrot.bmp"));
	if (Exist)
	{
		CResourceManager::GetInst()->SetTextureColorKey("Parrot", 255, 255, 255);
		for (int i = 0; i < 1; ++i)
			CResourceManager::GetInst()->AddAnimationFrameData("Parrot", 0.f, i * 80.f, 80.f, 80.f);
	}

	Exist = CResourceManager::GetInst()->CreateAnimationSequence("Penguin", "Penguin", TEXT("penguin.bmp"));
	if (Exist)
	{
		CResourceManager::GetInst()->SetTextureColorKey("Penguin", 255, 255, 255);
		for (int i = 0; i < 1; ++i)
			CResourceManager::GetInst()->AddAnimationFrameData("Penguin", 0.f, i * 80.f, 80.f, 80.f);
	}
}

/*
void CStage::ComposeStage()
{
	m_Board->ComposeStage();
}
*/