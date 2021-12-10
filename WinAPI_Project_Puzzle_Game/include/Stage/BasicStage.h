#pragma once

#include "Stage.h"

class CBasicStage :
    public CStage
{
    friend class CStageManager;
private :
    CBasicStage();
    ~CBasicStage();
public :
    virtual bool Init() override;
private :
    void LoadAnimationSequence();
};

