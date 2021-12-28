#pragma once

#include "../Ref.h"
#include "UIWidget.h"

class CUIWindow : public CRef
{
	friend class CStage;
public :
	CUIWindow();
	virtual ~CUIWindow();
private :
	int m_ZOrder;
	Vector2 m_Size;
	Vector2 m_Pos;
	class CUIWidget** m_WidgetArray;
	int m_WidgetCount;
	int m_WidgetCapacity;
public :
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
public :
	static int SortZ(const void* Src, const void* Dest);
};



