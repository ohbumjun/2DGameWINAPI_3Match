#include "UIWindow.h"
#include "../Input.h"
#include "../GameManager.h"

CUIWindow::CUIWindow():
m_Stage(nullptr),
m_ZOrder(0),
m_Visibility(true),
m_WidgetCount(0),
m_WidgetCapacity(10)
{
	m_WidgetArray = new CUIWidget * [m_WidgetCapacity];
}


CUIWindow::~CUIWindow()
{
	for (int i = 0; i < m_WidgetCount; i++)
	{
		SAFE_RELEASE(m_WidgetArray[i]);
	}
	SAFE_DELETE_ARRAY(m_WidgetArray);
}

bool CUIWindow::Init()
{
	return true;
}

void CUIWindow::Update(float DeltaTime)
{
	Vector2 MousePos = CInput::GetInst()->GetMousePos();

	for (int i = 0; i< m_WidgetCount;)
	{
		if (!m_WidgetArray[i]->IsActive())
		{
			SAFE_RELEASE(m_WidgetArray[i]);
			for (int j = i; j < m_WidgetCount - 1; j++)
			{
				m_WidgetArray[j] = m_WidgetArray[j + 1];
			}
			--m_WidgetCount;
			continue;
		}
		if (!m_WidgetArray[i]->GetVisibility())
		{
			++i;
			continue;
		}
		m_WidgetArray[i]->Update(DeltaTime);
		m_WidgetArray[i]->Collision(DeltaTime, MousePos);
		++i;
	}
}

void CUIWindow::PostUpdate(float DeltaTime)
{
	for (int i = 0; i < m_WidgetCount;)
	{
		if (!m_WidgetArray[i]->IsActive())
		{
			SAFE_RELEASE(m_WidgetArray[i]);
			for (int j = i; j < m_WidgetCount - 1; j++)
			{
				m_WidgetArray[j] = m_WidgetArray[j + 1];
			}
			--m_WidgetCount;
			continue;
		}
		if (!m_WidgetArray[i]->GetVisibility())
		{
			++i;
			continue;
		}
		m_WidgetArray[i]->PostUpdate(DeltaTime);
		++i;
	}
}

void CUIWindow::Render(HDC hDC)
{
	for (int i = 0; i < m_WidgetCount;)
	{
		if (!m_WidgetArray[i]->IsActive())
		{
			SAFE_RELEASE(m_WidgetArray[i]);
			for (int j = i; j < m_WidgetCount  - 1; j++)
			{
				m_WidgetArray[j] = m_WidgetArray[j + 1];
			}
			--m_WidgetCount;
			continue;
		}
		++i;
	}

	if (m_WidgetCount > 1)
		qsort(m_WidgetArray, (size_t)m_WidgetCount, sizeof(CUIWidget*), CUIWindow::SortZOrder);

	// z 값 기준 내림차순 정렬 --> 낮은 애부터 그린다 --> 높은 애들이 나중에 그려지게 
	for (int i = m_WidgetCount - 1; i >= 0; i--)
	{
		if (!m_WidgetArray[i]->GetVisibility())
			continue;
		m_WidgetArray[i]->Render(hDC);
	}
}

void CUIWindow::Render(HDC hDC, const Vector2& Pos)
{}

void CUIWindow::Collision(float DeltaTime)
{
	if (m_WidgetCount > 1)
	{
		qsort(m_WidgetArray, (size_t)m_WidgetCount, sizeof(CUIWidget*), CUIWindow::SortZOrder);
	}
}

void CUIWindow::Exit()
{
	CGameManager::GetInst()->Exit();
}

// 내림차순 정렬 
int CUIWindow::SortZOrder(const void* Src, const void* Dest)
{
	CUIWidget* SrcWidget  = *(CUIWidget**)Src;
	CUIWidget* DestWidget = *(CUIWidget**)Dest;

	// 내림차순 정렬
	if (SrcWidget->GetZOrder() < DestWidget->GetZOrder())
		return 1;
	else if (SrcWidget->GetZOrder() > DestWidget->GetZOrder())
		return -1;
	return 0;
}
