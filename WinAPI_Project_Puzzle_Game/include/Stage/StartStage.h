#pragma once
#include "Stage.h"
class CStartStage :
    public CStage
{
public :
    CStartStage();
    virtual ~CStartStage() override;
public :
    virtual bool Init() override;
    void LoadAnimationSequence();
    void InitButtonUI(class CUIButton* (&Button), int Num);
public :
    void SelectMiniStage();
    void SelectMediumStage();
    void SelectMaxStage();
};
