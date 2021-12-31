#pragma once
#include "UIImage.h"

class CUINumberWidget :
    public CUIImage
{
public:
    CUINumberWidget();
    CUINumberWidget(const CUINumberWidget& Widget);
    virtual ~CUINumberWidget();
private:
    int m_Number;
    std::vector<int> m_vecNumber;
    std::stack<int> m_TempNumber;
    bool m_RenderEnable;
public:
    void SetNumber(int Number)
    {
        m_Number = Number;
        m_RenderEnable = true;
    }
    void SetRenderEnable(bool State)
    {
        m_RenderEnable = State;
    }
    void AddNumber(int Number)
    {
        m_Number += Number;
    }
    void ClearNumber()
    {
        m_vecNumber.clear();
    }
public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render(HDC hDC);
    virtual void Render(const Vector2& Pos, HDC hDC);
    virtual CUINumberWidget* Clone();
};

