#include "UIWidget.h"

CUIWidget::CUIWidget() :
	m_ZOrder(0),
	m_Visibility(true),
	m_MouseHovered(false)
{
}

CUIWidget::CUIWidget(const CUIWidget& widget)
{
	*this = widget;
	m_RefCount = 0;
}

CUIWidget::~CUIWidget()
{
}

bool CUIWidget::Init()
{
	return true;
}

void CUIWidget::Update(float DeltaTime)
{
}

void CUIWidget::PostUpdate(float DeltaTime)
{
}

void CUIWidget::Render(HDC hDC)
{
}

void CUIWidget::Render(const Vector2& Pos, HDC hDC)
{
}

CUIWidget* CUIWidget::Clone()
{
	return new CUIWidget(*this);
}

bool CUIWidget::CollisionMouse(const Vector2& MousePos, float DeltaTime)
{
	if (m_Pos.x <= MousePos.x && MousePos.x <= m_Pos.x + m_Size.x &&
		m_Pos.y <= MousePos.y && MousePos.y <= m_Pos.y + m_Size.y)
	{
		if (!m_MouseHovered)
			CollisionMouseHoveredCallback(DeltaTime);
		return true;
	}
	return false;
}

void CUIWidget::Move(Vector2& Dir, float Speed)
{
	Dir.Normalize();
	Vector2 CurrentMove = Dir * Speed;
	m_Pos += CurrentMove;
}

void CUIWidget::CollisionMouseHoveredCallback(float DeltaTime)
{
	m_MouseHovered = true;
}

void CUIWidget::CollisionMouseReleaseCallback(float DeltaTime)
{
	m_MouseHovered = false;
}
