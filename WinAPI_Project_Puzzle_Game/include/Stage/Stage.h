#pragma once
#include "../GameInfo.h"
#include "../Object/GameObject.h"
#include "../Object/Board.h"
#include "../UI/UIWindow.h"

class CStage
{
public :
	CStage(int row = 5, int col = 5);
	CStage(const CStage& Stage);
	virtual ~CStage();
protected :
	class CBoard* m_Board;
	int m_Row;
	int m_Col;
	int m_StageLevel;
protected:
	class CUIWindow** m_UIArray;
	int m_UICapacity;
	int m_UICount;
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
	void SetCharactersAnimation();
	CGameObject* FindObject(const std::string& Name);
public:
	virtual bool Init();
	virtual bool Update(float DeltaTime);
	virtual bool PostUpdate(float DeltaTime);
	virtual bool Render(HDC hDC);
public :
	static int SortZOrder(const void* Src, const void* Dest);
public :
	template<typename T>
	T* FindUIWindow(const std::string& Name)
	{
		for (int i = 0; i < m_UICount; i++)
		{
			if (m_UIArray[i]->GetName() == Name)
				return (T*)m_UIArray[i];
		}
		return nullptr;
	}
	template<typename T>
	T* CreateUIWindow(const std::string& Name)
	{
		T* Window = new T;
		Window->SetStage(this);
		Window->SetName(Name);
		if (!Window->Init())
		{
			SAFE_DELETE(Window);
			return nullptr;
		}
		if (m_UICapacity <= m_UICount)
		{
			m_UICapacity = m_UICapacity * 2;
			CUIWindow** Array = new CUIWindow * [m_UICapacity];
			memcpy(Array, m_UIArray, sizeof(CUIWindow*) * m_UICount);
			SAFE_DELETE_ARRAY(m_UIArray);
			m_UIArray = Array;
		}
		m_UIArray[m_UICount++] = Window;
		return Window;
	}
};

