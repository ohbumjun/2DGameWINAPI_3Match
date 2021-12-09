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
	// Z Order�� ���� Widget�� �����Ѵ�.
	if (m_WidgetCount > 2)
	{
		qsort(m_WidgetArray, (size_t)m_WidgetCount, sizeof(CUIWidget*), CUIWindow::SortZOrder);
	}
}

void CUIWindow::Render(HDC hDC)
{
	// IsActive�� true �� �ֵ鸸 �����ϱ�
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

// Z ����, ���� ������ ���� ���� �׷����� �Ѵ�.
//�� ���� �ｼ, Z ���� ���� Widget�� ���� ���߿� �׸��ٴ� ��
// ���� �迭��, Z �� ������������ ����
// ������, �ڿ������� �Ͽ� Render �ϸ�
// Z ���� ���� ���� Widget�� ���� ���߿� �׷����� �� ���̴�. 
int CUIWindow::SortZOrder(const void* Src, const void* Dest)
{
	// ������ Src�� ���� ������
	// m_WidgetArray ���� ���� �����ϱ� ���� ������ �ϳ� (*) --> �׷��� ���� ���� Widgte�� �ּҰ��� ���ϴ� ������. * �� �ѹ� �� �ٿ��� Widget Object ��ü�� ���� 
	// �׷��� Src�� �ּҰ��� ���ϴ� CUIWidget* Ÿ���� ������ ���̴�
	CUIWidget* SrcObj = *(CUIWidget**)Src;
	CUIWidget* DestObj = *(CUIWidget**)Dest;

	// Z ���� ���Ѵ�
	int SrcZ = SrcObj->GetZOrder();
	int DestZ = DestObj->GetZOrder();
	
	if (SrcZ > DestZ)
		return -1;
	else if (SrcZ < DestZ)
		return 1;

	return 0;
}
