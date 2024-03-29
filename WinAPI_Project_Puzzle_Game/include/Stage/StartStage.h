#pragma once
#include "Stage.h"
class CStartStage :
    public CStage
{
public :
    CStartStage();
    virtual ~CStartStage() override;
private :
    int m_Row;
    int m_Col;
public :
    virtual bool Init() override;
    void LoadAnimationSequence();
    void LoadSound();
public :
    void SelectMiniStage();
    void SelectMediumStage();
    void SelectMaxStage();
    
};
