#pragma once
#include "../GameInfo.h"
#include "../Object/Board.h"

class CStage
{
	friend class CStageBuilder;
public :
	CStage(int row = 5, int col = 5);
	~CStage();
private :
	class CBoard* m_Board;
	int m_Row;
	int m_Col;
	int m_StageLevel;
public:
	virtual bool Init();
	virtual bool Update(float DeltaTime);
	virtual bool PostUpdate(float DeltaTime);
	virtual bool Render(HDC hDC);
public :
	int GetRow() const
	{
		// return m_Board->GetRow();
		return m_Row;
	}
	int GetCol() const
	{
		// return m_Board->GetCol();
		return m_Col;
	}
	CBoard* GetBoard() const
	{
		return m_Board;
	}
public :
	// 실제 위치 세팅 
	// void ComposeStage();
};

