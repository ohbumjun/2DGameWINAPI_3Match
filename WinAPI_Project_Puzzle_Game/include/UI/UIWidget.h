#include "../Ref.h"

class CUIWidget : public CRef {
public:
	CUIWidget();
	CUIWidget(const CUIWidget& Widget);
	virtual ~CUIWidget();
protected :
	class CUIWindow* m_Owner;
	class CStage* m_Stage;
	Vector2 m_Size;
	Vector2 m_Pos;
	int m_ZOrder;
	bool m_Visibility;
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
	int GetZOrder() const
{
		return m_ZOrder;
}
	bool GetVisibility() const
{
		return m_Visibility;
}
	bool IsMouseHovered() const
{
		return m_MouseHovered;
}
public :
	void SetOwner(class CUIWindow* Window)
	{
		m_Owner = Window;
	}
	void SetStage(class CStage* const Stage)
	{
		m_Stage = Stage;
	}
	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}
	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}
	void SetZOrder(int Order)
{
		m_ZOrder = Order;
}
	void SetVisibility(bool Visibility)
{
		m_Visibility = Visibility;
}
	void SetMouseHovered(bool Hover)
{
		m_MouseHovered = Hover;
}
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTIme);
	virtual void Render(HDC hDC);
	virtual bool Collision(const Vector2& MousePos, float DeltaTime);
	virtual CUIWidget* Clone();
public :
	void CollisionMouseHoveredCallback(float DeltaTime);
	void CallMouseReleaseCallback(float DeltaTime);
};


