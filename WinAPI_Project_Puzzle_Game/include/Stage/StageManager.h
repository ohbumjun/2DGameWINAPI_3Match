#pragma once
#include "../GameInfo.h"

class CStageManager
{
private :
	CStageManager();
	~CStageManager();
private :
	class CStage* m_Stage;
	class CStage* m_NextStage;
public :
	void Start();
	bool Init();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void Render(HDC hDC);
private :
	bool ChangeStage();
public :
	template<typename T>
	bool CreateStage()
	{
		m_NextStage = new T;
		return true;
	}
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

