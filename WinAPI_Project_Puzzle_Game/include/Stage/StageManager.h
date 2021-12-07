#pragma once
#include "../GameInfo.h"

class CStageManager
{
private :
	CStageManager();
	~CStageManager();
private :
	bool m_Init;
	class CStage* m_Stage;
	class CStageBuilder* m_StageBuilder;
public :
	void Start();
	bool Init();
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Render(HDC hDC);
public :
	void BuildStage();
private :
	static CStageManager* m_Inst;
public :
	static CStageManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CStageManager;
		return m_Inst;
	}
	static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}
};

