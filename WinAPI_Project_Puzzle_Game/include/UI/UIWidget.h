#pragma once

#include "../Ref.h"

class CUIWidget : public CRef
{
public:
	CUIWidget();
	CUIWidget(const  CUIWidget& widget);
	virtual ~CUIWidget();
protected :
	int m_ZOrder;
	bool m_Visibility;
	class CUIWindow* m_Owner;
	Vector2 m_Pos;
	Vector2 m_Size;
	bool m_MouseHovered;

public :
	Vector2 GetPos() const
	{
		return m_Pos;
	}
	Vector2 GetSize() const
	{
		return m_Size;
	}
	bool GetVisibility() const
	{
		return m_Visibility;
	}
	int GetZOrder() const
	{
		return m_ZOrder;
	}

public :
	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}
	void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}
	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}
	void SetSize(float x, float y)
	{
		m_Size = Vector2(x, y);
	}
	void SetOwner(class CUIWindow* Owner)
	{
		m_Owner = Owner;
	}
	void SetVisibility(bool Visibility)
	{
		m_Visibility = Visibility;
	}
	void SetZOrder(int ZOrder)
	{
		m_ZOrder = ZOrder;
	}
public :
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& Pos, HDC hDC);
	virtual CUIWidget* Clone();
public :
	bool CollisionMouse(const Vector2& MousePos, float DeltaTime);
	void Move(Vector2& Dir, float Speed);
public :
	virtual void CollisionMouseHoveredCallback(float DeltaTime);
	virtual void CollisionMouseReleaseCallback(float DeltaTime);
};