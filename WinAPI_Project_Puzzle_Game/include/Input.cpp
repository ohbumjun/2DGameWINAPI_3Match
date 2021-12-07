#include "Input.h"
#include "Resource/ResourceManager.h"

CInput* CInput::m_Inst = nullptr;

CInput::CInput() : m_MouseDown(false),
m_MousePush(false),
m_MouseUp(false),
m_hWnd(0)
{
	m_vecKeyState.resize(KEYCOUNT_MAX);

	for (int i = 0; i < KEYCOUNT_MAX; ++i)
	{
		m_vecKeyState[i].Key = i;
	}

	m_Ctrl = false;
	m_Alt = false;
	m_Shift = false;

	m_ShowCursor = false;

	m_MouseType = Mouse_Default;
}

CInput::~CInput()
{
	std::unordered_map<std::string, KeyInfo*>::iterator iter = m_mapInfo.begin();
	std::unordered_map<std::string, KeyInfo*>::iterator iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CInput::CreateKey(const std::string& Name, int Key)
{
	KeyInfo* Info = FindKeyInfo(Name);

	if (Info)
		return false;

	Info = new KeyInfo;

	Info->Name = Name;
	Info->State.Key = Key;

	m_mapInfo.insert(std::make_pair(Name, Info));

	bool Add = false;

	size_t Size = m_vecAddKey.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecAddKey[i] == Key)
		{
			Add = true;
			break;
		}
	}

	if (!Add)
		m_vecAddKey.push_back(Key);

	return true;
}

bool CInput::SetCtrlKey(const std::string& Name, bool State)
{
	KeyInfo* Info = FindKeyInfo(Name);

	if (!Info)
		return false;

	Info->Ctrl = State;

	return true;
}

bool CInput::SetAltKey(const std::string& Name, bool State)
{
	KeyInfo* Info = FindKeyInfo(Name);

	if (!Info)
		return false;

	Info->Alt = State;

	return true;
}

bool CInput::SetShiftKey(const std::string& Name, bool State)
{
	KeyInfo* Info = FindKeyInfo(Name);

	if (!Info)
		return false;

	Info->Shift = State;

	return true;
}

KeyInfo* CInput::FindKeyInfo(const std::string& Name)
{
	std::unordered_map<std::string, KeyInfo*>::iterator iter = m_mapInfo.find(Name);

	if (iter == m_mapInfo.end())
		return nullptr;

	return iter->second;
}

bool CInput::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	m_MouseDown = false;
	m_MousePush = false;
	m_MouseUp = false;

	// Move
	CreateKey("MoveUp", 'W');
	CreateKey("MoveDown", 'S');
	CreateKey("MoveLeft", 'A');
	CreateKey("MoveRight", 'D');

	// Jump
	CreateKey("Jump", VK_SPACE);

	// Pause, Resume
	CreateKey("Pause", 'P');
	SetCtrlKey("Pause", true);
	CreateKey("Resume", 'R');
	SetCtrlKey("Resume", true);

	// Skill
	CreateKey("ActivateSkill", 'K');
	// CreateKey("SkillDestoryAll", 'N');

	// Run
	CreateKey("RunUp", 'W');
	SetShiftKey("RunUp", true);
	CreateKey("RunDown", 'S');
	SetShiftKey("RunDown", true);
	CreateKey("RunLeft", 'A');
	SetShiftKey("RunLeft", true);
	CreateKey("RunRight", 'D');
	SetShiftKey("RunRight", true);

	// Dash
	CreateKey("DashUp", 'W');
	SetCtrlKey("DashUp", true);
	CreateKey("DashDown", 'S');
	SetCtrlKey("DashDown", true);
	CreateKey("DashLeft", 'A');
	SetCtrlKey("DashLeft", true);
	CreateKey("DashRight", 'D');
	SetCtrlKey("DashRight", true);

	// Teleport
	CreateKey("Teleport", 'T');
	CreateKey("TeleportPositionSet", VK_RBUTTON);
	SetCtrlKey("TeleportPositionSet", true);

	// Zoom
	CreateKey("TargetPos", VK_LBUTTON);
	SetCtrlKey("TargetPos", true);
	CreateKey("TargetFire", VK_SPACE);
	SetCtrlKey("TargetFire", true);

	// Item 
	CreateKey("GetItem", 'Z');

	// Inv 
	CreateKey("UseHpPotion", 'P');
	CreateKey("UseMpPotion", 'O');
	CreateKey("UseShield", 'I');

	// Store
	CreateKey("BuyItem", 'B');

	// Gun
	CreateKey("Reload", 'R');
	CreateKey("ChangeGunToLight", '1');
	CreateKey("ChangeGunToMedium", '2');
	CreateKey("ChangeGunToHeavy", '3');

	// Edit
	CreateKey("Editor", VK_F1);
	CreateKey("MouseLButton", VK_LBUTTON);
	CreateKey("MouseRButton", VK_RBUTTON);


	ShowCursor(FALSE);



	/*
	// CUIImage* MouseDefault = new CUIImage;
	// MouseDefault->SetSize(32.f, 31.f);
	std::vector<std::wstring> vecFileName;

	for (int i = 0; i <= 10; ++i)
	{
		TCHAR FileName[MAX_PATH] = {};
		wsprintf(FileName, TEXT("Mouse/%d.bmp"), i);

		vecFileName.push_back(FileName);
	}

	CResourceManager::GetInst()->LoadTexture("MouseDefault", vecFileName);

	CTexture* Texture = CResourceManager::GetInst()->FindTexture("MouseDefault");

	for (int i = 0; i <= 10; ++i)
	{
		Texture->SetColorKey(255, 0, 255, i);
	}

	MouseDefault->SetTexture(Texture);

	for (int i = 0; i <= 10; ++i)
	{
		MouseDefault->AddFrameData(Vector2(0.f, 0.f), Vector2(32.f, 31.f));
	}
	m_vecMouseImage.push_back(MouseDefault);
	*/

	return true;
}

void CInput::Update(float DeltaTime)
{
	// Title 바에서도 마우스 커서 보이게 하기
	RECT rc;
	GetClientRect(m_hWnd, &rc); // Title바 제외
	// GetWindowRect() : Title 바 포함

	POINT ptMouse;
	// Screen 좌표 (일반 좌표 )로 나온다
	GetCursorPos(&ptMouse);
	// 마우스도 클라이언트 영역(왼쪽 상단이 원점) 것으로
	ScreenToClient(m_hWnd, &ptMouse);

	// 안에 들어오면, 안보이게
	// 안에 안들어오면 보이게
	// 이렇게 해주는 이유는, title 바는
	// rc 범위 (GetClientRect : Title바 제외) 밖에 있다
	// 따라서, rc 범위 밖에 있을 때는
	// animation이 없는 원래의 마우스 커서가 보이게
	// rc 범위 안에 있을 때는, 원래의 마우스 커서 안보이게
	if (rc.left <= ptMouse.x && ptMouse.x <= rc.right &&
		rc.top <= ptMouse.y &&
		ptMouse.y <= rc.bottom)
	{
		// 들어왔을 때는,
		// 보이는 경우, 안보이게 해야 한다
		if (IsShowCursor())
		{
			SetShowCursor(false);
			ShowCursor(FALSE);
		}
	}
	else
	{
		// 안에 "안"들어왔는데 안보이고 있다면
		if (!IsShowCursor())
		{
			// 보이게 처리하기
			SetShowCursor(true);
			ShowCursor(TRUE);
		}
	}

	UpdateKeyState();
	UpdateMouse(DeltaTime);
	UpdateKeyInfo(DeltaTime);

	// m_vecMouseImage[m_MouseType]->Update(DeltaTime);
}

void CInput::Render(HDC hDC)
{
	// m_vecMouseImage[m_MouseType]->Render(m_MousePos, hDC);
}

void CInput::UpdateMouse(float DeltaTime)
{
	POINT ptMouse;

	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	Vector2 Pos;
	Pos.x = (float)ptMouse.x;
	Pos.y = (float)ptMouse.y;

	m_MouseMove = Pos - m_MousePos;
	m_MousePos = Pos;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (!m_MouseDown && !m_MousePush)
		{
			m_MouseDown = true;
			m_MousePush = true;
		}

		else
		{
			m_MouseDown = false;
		}
	}

	else if (m_MousePush)
	{
		m_MouseDown = false;
		m_MousePush = false;
		m_MouseUp = true;
	}

	else if (m_MouseUp)
	{
		m_MouseUp = false;
	}
}

void CInput::UpdateKeyState()
{
	size_t Size = m_vecAddKey.size();

	for (size_t i = 0; i < Size; ++i)
	{
		int Key = m_vecAddKey[i];

		if (GetAsyncKeyState(Key) & 0x8000)
		{
			if (!m_vecKeyState[Key].State[KeyState_Down] &&
				!m_vecKeyState[Key].State[KeyState_Push])
			{
				m_vecKeyState[Key].State[KeyState_Down] = true;
				m_vecKeyState[Key].State[KeyState_Push] = true;
			}

			else
			{
				m_vecKeyState[Key].State[KeyState_Down] = false;
			}
		}

		else if (m_vecKeyState[Key].State[KeyState_Push])
		{
			m_vecKeyState[Key].State[KeyState_Push] = false;
			m_vecKeyState[Key].State[KeyState_Down] = false;
			m_vecKeyState[Key].State[KeyState_Up] = true;
		}

		else if (m_vecKeyState[Key].State[KeyState_Up])
		{
			m_vecKeyState[Key].State[KeyState_Up] = false;
		}
	}

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		m_Ctrl = true;
	}

	else
		m_Ctrl = false;

	if (GetAsyncKeyState(VK_MENU) & 0x8000)
	{
		m_Alt = true;
	}

	else
		m_Alt = false;

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		m_Shift = true;
	}

	else
		m_Shift = false;
}

void CInput::UpdateKeyInfo(float DeltaTime)
{
	std::unordered_map<std::string, KeyInfo*>::iterator iter = m_mapInfo.begin();
	std::unordered_map<std::string, KeyInfo*>::iterator iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		int Key = iter->second->State.Key;

		if (m_vecKeyState[Key].State[KeyState_Down] &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			if (iter->second->Callback[KeyState_Down])
				iter->second->Callback[KeyState_Down](DeltaTime);
		}

		if (m_vecKeyState[Key].State[KeyState_Push] &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			if (iter->second->Callback[KeyState_Push])
				iter->second->Callback[KeyState_Push](DeltaTime);
		}

		if (m_vecKeyState[Key].State[KeyState_Up] &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			if (iter->second->Callback[KeyState_Up])
				iter->second->Callback[KeyState_Up](DeltaTime);
		}
	}
}

void CInput::ClearCallback()
{
	auto iter = m_mapInfo.begin();
	auto iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		for (int i = 0; i < KeyState_Max; ++i)
		{
			iter->second->Callback[i] = nullptr;
		}
	}
}
