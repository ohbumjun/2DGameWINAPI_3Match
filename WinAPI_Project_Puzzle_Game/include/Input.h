#pragma once

#include "GameInfo.h"
#include "UI/UIImage.h"

enum EMouse_Type
{
	Mouse_Default,
	Mouse_Battle,
	Mouse_End,
};

struct Key_State
{
	// 유니코드 ?
	int Key;
	bool State[KeyState_Max];

	Key_State()  :
		Key(0),
		State{}
	{
	}
};

struct KeyInfo
{
	std::string Name;
	Key_State State;
	std::function<void(float)> Callback[KeyState_Max];

	bool Ctrl;
	bool Alt;
	bool Shift;

	KeyInfo() :
		Ctrl(false),
		Alt(false),
		Shift(false){}
};

class CInput
{
private :
	CInput();
	~CInput();
private :
	std::unordered_map<std::string, KeyInfo*> m_mapInfo;
	std::vector<Key_State> m_vecKeyState; // 각 아스키 코드 State 에 대한 상태 값을 저장 
	std::vector<int> m_vecAddKey; // 실제 저장된 Key 정보 목록들 
	bool m_Ctrl;
	bool m_Alt;
	bool m_Shift;

	Vector2 m_MousePos;
	Vector2 m_MouseMove;
	HWND m_hWnd;

	bool m_MouseDown;
	bool m_MousePush;
	bool m_MouseUp;

	bool m_ShowCursor; // 현재 마우스 커서가 보이는지 안보이는지

	std::vector<CSharedPtr<CUIImage>> m_vecMouseImage;
	EMouse_Type m_MouseType;

public :
	bool IsShowCursor() const
	{
		return m_ShowCursor;
	}
	bool GetMouseDown() const
	{
		return m_MouseDown;
	}
	bool GetMousePush() const
	{
		return m_MousePush;
	}
	bool GetMouseUp() const
	{
		return m_MouseUp;
	}
	Vector2 GetMouseMove() const
	{
		return m_MouseMove;
	}
public :
	void ChangeMouse(EMouse_Type Type)
	{
		m_MouseType = Type;
	}
	void SetShowCursor(bool Cursor)
	{
		m_ShowCursor = Cursor;
	}
public :
	bool CreateKey(const std::string& Name, int Key);
	bool SetCtrlKey(const std::string& Name, bool State);
	bool SetAltKey(const std::string& Name, bool State);
	bool SetShiftKey(const std::string& Name, bool State);
private :
	KeyInfo* FindKeyInfo(const std::string& Name);

public :
	bool Init(HWND hWnd);
	void Update(float DeltaTime);
	void Render(HDC hDC);
private :
	void UpdateMouse(float DeltaTime);
	void UpdateKeyState();
	void UpdateKeyInfo(float DeltaTime);
public :
	void ClearCallback();
public :
	template<typename T>
	void SetCallback(const std::string& Name, EKeyState State, T* Obj, void(T::* Func)(float))
	{
		KeyInfo* Info = FindKeyInfo(Name);
		Info->Callback[State] = std::bind(Func, Obj, std::placeholders::_1);
	}
private :
	static CInput* m_Inst;
public:
	static CInput* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CInput;
		return m_Inst;
	}
	static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}
};