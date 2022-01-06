#pragma once

#include "../Ref.h"

class CUIWidget: public CRef {
public :
	CUIWidget();
	CUIWidget(const CUIWidget& Widget);
	virtual ~CUIWidget();
protected :
	class CStage* m_Stage;
	class CUIWindow* m_Owner;
	Vector2 m_Size;
	Vector2 m_Pos;
	bool m_Visibility;
	int m_ZOrder;
	bool m_MouseHovered;
public :
	bool GetVisibility() const
{
		return m_Visibility;
}
	int GetZOrder() const
{
		return m_ZOrder;
}
	Vector2 GetSize() const
{
		return m_Size;
}
	Vector2 GetPos() const
{
		return m_Pos;
}
public :
	void SetStage(class CStage* Stage)
{
		m_Stage = Stage;
}
	void SetOwner(class CUIWindow* Owner)
{
		m_Owner = Owner;
}
	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}
	void SetSize(float x, float y)
	{
		m_Size = Vector2(x, y);
	}
	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}
	void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}
	void SetZOrder(int ZOrder)
	{
		m_ZOrder = ZOrder;
	}
	void SetVisibility(bool Visibility)
	{
		m_Visibility = Visibility;
	}
public :
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(HDC hDC, const Vector2& Pos);
	virtual void Collision(float DeltaTime, const Vector2& MousePos);
public :
	virtual void CallMouseHoveredCallback(float DeltaTime);
	virtual void CallMouseReleaseCallback(float DeltaTime);
};