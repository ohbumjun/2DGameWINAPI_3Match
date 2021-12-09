#pragma once

#include "../Ref.h"
#include "UIWidget.h"

class CUIWindow : public CRef
{
	friend class CStage;

protected :
	CUIWindow();
	virtual ~CUIWindow();
protected :
	int m_ZOrder;
	CUIWidget** m_WidgetArray;
	int m_WidgetCount;
	int m_WidgetCapacity;

	bool m_Visibility;
	Vector2 m_Pos;
	Vector2 m_Size;
public :
	CUIWidget* GetWidget(int Index)
	{
		return m_WidgetArray[Index];
	}
	int GetWidgetCount() const
	{
		return m_WidgetCount;
	}
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
	bool GetVisibility()    const
	{
		return m_Visibility;
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
	void SetZOrder(int ZOrder)
	{
		m_ZOrder = ZOrder;
	}
	void SetVisibility(bool Visiblity)
	{
		m_Visibility = Visiblity;
	}
public :
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
public :
	static int SortZOrder(const void* Src, const void* Dest);
public :
	template<typename T>
	T* FindWidget(const std::string& Name)
	{
		for (int i = 0; i < m_WidgetCount; i++)
		{
			if (m_WidgetArray[i]->GetName() == Name)
				return (T*)(m_WidgetArray[i]);
		}
		return nullptr;
	}
public :
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

		if (m_WidgetCount == m_WidgetCapacity)
		{
			m_WidgetCapacity *= 2;
			CUIWidget** Array = new CUIWidget * [m_WidgetCapacity];
			memcpy(Array, m_WidgetArray, sizeof(CUIWidget*) * m_WidgetCount);
			SAFE_DELETE_ARRAY(m_WidgetArray);
			m_WidgetArray = Array;
		}

		Widget->AddRef();
		m_WidgetArray[m_WidgetCount] = Widget;
		++m_WidgetCount;

		return (T*)Widget;
	}
};