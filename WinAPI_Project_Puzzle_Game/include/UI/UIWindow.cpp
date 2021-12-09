#include "UIWindow.h"
#include "UIWidget.h"

CUIWindow::CUIWindow() :
	m_Visibility(true),
	m_WidgetArray(nullptr),
	m_ZOrder(0),
	m_WidgetCapacity(4),
	m_WidgetCount(0)
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

			for (int j = i; j < m_WidgetCapacity - 1; ++j)
			{
				m_WidgetArray[j] = m_WidgetArray[j + 1];
			}

			--m_WidgetCount;
			continue;
		}
		else if (!m_WidgetArray[i]->GetVisibility())
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
			SAFE_RELEASE(m_WidgetArray[i]);

			for (int j = i; j < m_WidgetCount - 1; ++j)
			{
				m_WidgetArray[j] = m_WidgetArray[j + 1];
			}

			--m_WidgetCount;

			continue;
		}

		else if (!m_WidgetArray[i]->GetVisibility())
		{
			++i;
			continue;
		}

		m_WidgetArray[i]->PostUpdate(DeltaTime);
		++i;
	}
}

void CUIWindow::Collision(float DeltaTime)
{
	// Z Order에 따라 Widget을 정렬한다.
	if (m_WidgetCount > 2)
	{
		qsort(m_WidgetArray, (size_t)m_WidgetCount, sizeof(CUIWidget*), CUIWindow::SortZOrder);
	}
}

void CUIWindow::Render(HDC hDC)
{
	// IsActive가 true 인 애들만 선별하기
	for (int i = 0; i < m_WidgetCount;)
	{
		if (!m_WidgetArray[i]->IsActive())
		{
			SAFE_RELEASE(m_WidgetArray[i]);
			for (int j = i; j < m_WidgetCount - 1; ++j)
			{
				m_WidgetArray[j] = m_WidgetArray[j + 1];
			}
			--m_WidgetCount;
			continue;
		}
		++i;
	}

	for (int i = m_WidgetCount; i >= 0; --i)
	{
		if (!m_WidgetArray[i]->GetVisibility())
			continue;
		m_WidgetArray[i]->Render(hDC);
	}
}

// Z 값은, 값이 작으면 제일 위에 그려지게 한다.
//그 말은 즉슨, Z 값이 낮은 Widget은 가장 나중에 그린다는 것
// 따라서 배열은, Z 의 오름차순으로 정렬
// 접근은, 뒤에서부터 하여 Render 하면
// Z 값이 가장 작은 Widget이 가장 나중에 그려지게 될 것이다. 
int CUIWindow::SortZOrder(const void* Src, const void* Dest)
{
	// 들어오는 Src는 이중 포인터
	// m_WidgetArray 안의 값에 접근하기 위한 포인터 하나 (*) --> 그렇게 얻은 값이 Widgte의 주소값을 지니는 포인터. * 를 한번 더 붙여서 Widget Object 자체에 접근 
	// 그러한 Src의 주소값을 지니는 CUIWidget* 타입을 가지는 것이다
	CUIWidget* SrcObj = *(CUIWidget**)Src;
	CUIWidget* DestObj = *(CUIWidget**)Dest;

	// Z 값을 구한다
	int SrcZ = SrcObj->GetZOrder();
	int DestZ = DestObj->GetZOrder();
	
	if (SrcZ > DestZ)
		return -1;
	else if (SrcZ < DestZ)
		return 1;

	return 0;
}
