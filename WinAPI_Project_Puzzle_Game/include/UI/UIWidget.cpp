#include "UIWidget.h"

CUIWidget::CUIWidget():
m_MouseHovered(false),
m_ZOrder(0),
m_Visibility(true)
{}

CUIWidget::CUIWidget(const CUIWidget& Widget)
{
	*this = Widget;
	m_MouseHovered = false;
}

CUIWidget::~CUIWidget()
{}

inline bool CUIWidget::Init()
{
	return true;
}

inline void CUIWidget::Update(float DeltaTime)
{}

inline void CUIWidget::PostUpdate(float DeltaTIme)
{}

inline void CUIWidget::Render(HDC hDC)
{}

inline bool CUIWidget::Collision(const Vector2& MousePos, float DeltaTime)
{
	if (m_Pos.x <= MousePos.x && MousePos.x < m_Pos.x + m_Size.x &&
		m_Pos.y <= MousePos.y && MousePos.y < m_Pos.y + m_Size.y)
	{
		if (!m_MouseHovered)
		{
			m_MouseHovered = true;
			CollisionMouseHoveredCallback(DeltaTime);
		}
	}
	else
	{
		if (m_MouseHovered)
		{
			CallMouseReleaseCallback(DeltaTime);
			m_MouseHovered = false;
		}
	}
}

CUIWidget* CUIWidget::Clone()
{
	return new CUIWidget(*this);
}

inline void CUIWidget::CollisionMouseHoveredCallback(float DeltaTime)
{
	m_MouseHovered = true;
}

inline void CUIWidget::CallMouseReleaseCallback(float DeltaTime)
{
	m_MouseHovered = false;
}