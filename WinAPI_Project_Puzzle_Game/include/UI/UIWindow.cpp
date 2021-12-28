#include "UIWindow.h"

CUIWindow::CUIWindow() :
m_ZOrder(0),
m_WidgetCount(0),
m_WidgetCapacity(4)
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
			i++;
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
}

void CUIWindow::Collision(float DeltaTime)
{
}

void CUIWindow::Render(HDC hDC)
{
	//오름 차순 정렬 -> 뒤에서 부터 그리기 -> z order 작은 애가 제일 나중에 -> 화면 제일 앞에
	if (m_WidgetCount > 1)
	{
		qsort(m_WidgetArray, (size_t)m_WidgetCount, sizeof(CUIWidget*), CUIWindow::SortZ);
	}
	for (int i = m_WidgetCount - 1; i >= 0; i--)
	{
		if (!m_WidgetArray[i]->GetVisibility())
			continue;
		m_WidgetArray[i]->Render(hDC);
	}
}

int CUIWindow::SortZ(const void* Src, const void* Dest)
{
	CUIWidget* SrcObj   = *(CUIWidget**)Src;
	CUIWidget* DestObj = *(CUIWidget**)Dest;

	int SrcZOrder = SrcObj->GetZOrder();
	int DestZOrder = DestObj->GetZOrder();

	if (SrcZOrder < DestZOrder)
		return 1;
	else
		return -1;
	return 0;
}