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
	class CStageBuilder* m_Builder;
public:
	virtual bool Init();
	virtual bool Update(float DeltaTime);
	virtual bool PostUpdate(float DeltaTime);
	virtual bool Render(HDC hDC);
public :
	int GetRow() const
	{
		return m_Row;
	}
	int GetCol() const
	{
		return m_Col;
	}
	CBoard* GetBoard() const
	{
		return m_Board;
	}
public :
	void SetBuilder(class CStageBuilder* Builder)
	{
		m_Builder = Builder;
	}
public :
	// 실제 위치 세팅 
	void ComposeStage();
};

