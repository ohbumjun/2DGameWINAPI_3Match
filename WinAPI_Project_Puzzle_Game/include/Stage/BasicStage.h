#pragma once

#include "Stage.h"

class CBasicStage :
    public CStage
{
    friend class CStageManager;
private :
    CBasicStage();
    ~CBasicStage();
private :
    class CUINumberWidget* m_NumberWidget;
public :
    virtual bool Init() override;
private :
    void LoadAnimationSequence();
public :
    virtual bool Update(float DeltaTime) override;
};

