#pragma once

#include "UIWidget.h"
#include "../Ref.h"

class CUIWindow : public CRef {
	friend class CStage;
public :
	CUIWindow();
	virtual ~CUIWindow();
private :
	class CStage* m_Stage;
	int m_WidgetCount;
	int m_WidgetCapacity;
	int m_ZOrder;
	bool m_Visibility;
	class CUIWidget** m_WidgetArray;
	Vector2 m_Size;
	Vector2 m_Pos;
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
	void SetStage(class CStage* Stage)
{
		m_Stage = Stage;
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
	virtual void Collision(float DeltaTime);
public :
	void Exit();
public :
	// 내림 차순 
	static int SortZOrder(const void* Src, const void* Dest);
public :
	template<typename T>
	T* FindWidget(const std::string& Name)
{
	for (int i = 0; i < m_WidgetCount; i++)
	{
		if (m_WidgetArray[i]->GetName() == Name)
			return (T*)m_WidgetArray[i];
	}
	return nullptr;
}
	template<typename T>
	T* CreateWidget(const std::string& Name)
{
		T* Widget = new T;
		Widget->SetName(Name);
		Widget->SetOwner(this);
	if (!Widget->Init())
	{
		SAFE_DELETE(Widget);
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
	m_WidgetArray[m_WidgetCount++] = Widget;
	return (T*)Widget;
}
};