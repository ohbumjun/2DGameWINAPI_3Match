#pragma once
#include "../GameInfo.h"
#include "../Object/Board.h"

class CStage
{
	friend class CStageBuilder;
public :
	CStage(class CStageBuilder* builder, int row, int col);
	~CStage();
private :
	int m_Row;
	int m_Col;
	class CBoard* m_Board;
public:
	virtual bool Init();
	virtual bool Update(float DeltaTime);
	virtual bool PostUpdate(float DeltaTime);
	virtual bool Render(HDC hDC);
public :
	int GetRow() const
	{
		return m_Board->GetRow();
	}
	int GetCol() const
	{
		return m_Board->GetCol();
	}
	CBoard* GetBoard() const
	{
		return m_Board;
	}
public :
	// ���� ��ġ ���� 
	void ComposeStage();
};

