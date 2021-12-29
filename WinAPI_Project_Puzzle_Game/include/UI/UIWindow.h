#pragma  once

#include "../Ref.h"
#include "UIWidget.h"

class CUIWindow : public CRef {
public :
	CUIWindow();
	CUIWindow(const CUIWindow& Window);
	virtual ~CUIWindow();
private :
	class CStage* m_Stage;
	int m_ZOrder;
	bool m_Visibility;
	Vector2 m_Size;
	Vector2 m_Pos;
	int m_WidgetCount;
	int m_WidgetCapacity;
	class CUIWidget** m_WidgetArray;
public :
	Vector2 GetSize() const
{
		return m_Size;
}
	Vector2 GetPos() const
{
		return m_Pos;
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
	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}
	void SetPost(const Vector2& Pos)
	{
		m_Pos = Pos;
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
	virtual void Render(HDC hDC);
	virtual void Collision(float DeltaTime);
public :
	static int SortZ(const void* Src, const void* Dest);
public :
	template<typename T>
	T* FindWidget(const std::string& Name)
{
	for (int i = 0; i < m_WidgetCount; i++)
	{
		if (m_WidgetArray[i]->GetName() == Name)
			return (T*)m_WidgetArray[i];
	}
}
	template<typename T>
	T* CreateWidget(const std::string& Name)
{
		T* Widget = new T;
		Widget->SetOwner(this);
		Widget->SetStage(m_Stage);
	if (!Widget->Init())
	{
		SAFE_DELETE(Widget)
			return nullptr;
	}
	if (m_WidgetCount > m_WidgetCapacity)
	{
		m_WidgetCapacity = m_WidgetCount * 2;
		CUIWidget** Array = new CUIWidget * [m_WidgetCapacity];
		memcpy(Array, m_WidgetArray, sizeof(CUIWidget*) * m_WidgetCount);
		delete[] m_WidgetArray;
		m_WidgetArray = Array;
	}

	Widget->AddRef();
	m_WidgetArray[m_WidgetCount++] = Widget;
	return Widget;
}

};