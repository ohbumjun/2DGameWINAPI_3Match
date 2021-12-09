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
};