#include "UIWindow.h"

CUIWindow::CUIWindow():
m_Visibility(true),
m_ZOrder(0),
m_WidgetCount(0),
m_WidgetCapacity(10),
m_Stage(nullptr),
m_WidgetArray(nullptr)
{
	m_WidgetArray = new CUIWidget * [m_WidgetCapacity];
}

CUIWindow::CUIWindow(const CUIWindow& Window)
{
	*this = Window;

	m_WidgetCount = Window.m_WidgetCount;
	m_WidgetArray = new CUIWidget * [m_WidgetCount];

	for (int i = 0; i < Window.m_WidgetCount; i++)
	{
		m_WidgetArray[m_WidgetCount++] = Window.m_WidgetArray[i]->Clone();
	}
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
	for (int i = 0; i < m_WidgetCount;)
	{
		if (!m_WidgetArray[i]->IsActive())
		{
			SAFE_DELETE(m_WidgetArray[i]);
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
		++i;
	}
}

void CUIWindow::PostUpdate(float DeltaTime)
{
	for (int i = 0; i < m_WidgetCount;)
	{
		if (!m_WidgetArray[i]->IsActive())
		{
			SAFE_DELETE(m_WidgetArray[i]);
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
			SAFE_DELETE(m_WidgetArray[i]);
			for (int j = i; j < m_WidgetCount - 1; j++)
			{
				m_WidgetArray[j] = m_WidgetArray[j + 1];
			}
			--m_WidgetCount;
			continue;
		}
		++i;
	}
	for (int i = m_WidgetCount - 1; i >= 0; i--)
	{
		if (!m_WidgetArray[i]->GetVisibility())
			continue;
		m_WidgetArray[i]->Render(hDC);
	}
}

void CUIWindow::Collision(float DeltaTime)
{
	if (m_WidgetCount > 1)
	{
		qsort(m_WidgetArray, (size_t)m_WidgetCount, sizeof(CUIWidget*), CUIWindow::SortZ);
	}
}

int CUIWindow::SortZ(const void* Src, const void* Dest)
{
	CUIWidget* SrcWidget   = *(CUIWidget**)Src;
	CUIWidget* DestWidget = *(CUIWidget**)Dest;
	if (SrcWidget->GetZOrder() < DestWidget->GetZOrder())
		return -1;
	else if (SrcWidget->GetZOrder() > DestWidget->GetZOrder())
		return 1;
	return 0;
}
