#include "Input.h"

CInput* CInput::m_Inst = nullptr;
CInput::CInput() :
	m_MouseDown(false),
	m_MousePush(false),
	m_MouseUp(false),
	m_hWnd(0)
{
	m_vecKeyState.resize(KEYCOUNT_MAX);

	for (int i = 0; i < KEYCOUNT_MAX; i++)
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
	Info->State.Key = Key;
	Info->Name = Name;

	m_mapInfo.insert(std::make_pair(Name, Info));

	bool Add = true;
	for (int i = 0; i < m_vecAddKey.size(); i++)
	{
		if (m_vecAddKey[i] == Key)
		{
			Add = false;
			break;
		}
	}

	if (Add)
		m_vecAddKey.push_back(Key);
	
	return true;
}

bool CInput::SetCtrlKey(const std::string& Name, bool State)
{
	KeyInfo* Info = FindKeyInfo(Name);
	if (!Info)
		return false;
	Info->Ctrl = true;
	return true;
}

bool CInput::SetAltKey(const std::string& Name, bool State)
{
	KeyInfo* Info = FindKeyInfo(Name);
	if (!Info)
		return false;
	Info->Alt = true;
	return true;
}

bool CInput::SetShiftKey(const std::string& Name, bool State)
{
	KeyInfo* Info = FindKeyInfo(Name);
	if (!Info)
		return false;
	Info->Shift = true;
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

	// KeySetUp
	CreateKey("MouseUp", 'W');

	ShowCursor(false);



	return true;
}

void CInput::Update(float DeltaTime)
{
	// Title �ٿ����� ���콺 Ŀ�� ���̰� �ϱ�
	RECT rc;
	GetClientRect(m_hWnd, &rc); // Title �� ����

	POINT ptMouse;
	// Screen ��ǥ (�Ϲ� ��ǥ) ���´�
	GetCursorPos(&ptMouse);

	// ���콺�� Ŭ���̾�Ʈ ���� ���� (���� ��� ) ����
	ScreenToClient(m_hWnd, &ptMouse);

	// �ȿ� ������, �Ⱥ��̰�
	// �ȿ� �ȵ����� ���̰�
	// �̷��� ���ִ� ������, title �ٴ�
	// rc ���� (GetClientRect : Title�� ����) �ۿ� �ִ�
	// ����, rc ���� �ۿ� ���� ����
	// animation�� ���� ������ ���콺 Ŀ���� ���̰�
	// rc ���� �ȿ� ���� ����, ������ ���콺 Ŀ�� �Ⱥ��̰�
	if (rc.left <= ptMouse.x && ptMouse.x <= rc.right && rc.top <= ptMouse.y && ptMouse.y <= rc.bottom)
	{
		if (IsShowCursor())
		{
			SetShowCursor(false);
			ShowCursor(FALSE);
		}
	}
	else
	{
		if (!IsShowCursor())
		{
			SetShowCursor(true);
			ShowCursor(TRUE);
		}
	}

	UpdateKeyState();
	UpdateMouse(DeltaTime);
	UpdateKeyInfo(DeltaTime);

	m_vecMouseImage[m_MouseType]->Update(DeltaTime);

}

void CInput::Render(HDC hDC)
{
	m_vecMouseImage[m_MouseType]->Render(m_MousePos, hDC);
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
	else
	{
		if (m_MousePush)
		{
			m_MouseDown = false;
			m_MousePush = false;
			m_MouseUp = true;
		}
		else if(m_MouseUp)
		{
			m_MouseUp = true;
		}
	}
}

void CInput::UpdateKeyState()
{
	size_t Size = m_vecAddKey.size();

	for (size_t i = 0; i < Size; i++)
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
		else
		{
			if (m_vecKeyState[Key].State[KeyState_Push])
			{
				m_vecKeyState[Key].State[KeyState_Down] = false;
				m_vecKeyState[Key].State[KeyState_Push] = false;
				m_vecKeyState[Key].State[KeyState_Up] = false;
			}
			else
			{
				m_vecKeyState[Key].State[KeyState_Up] = false;
			}
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
	auto iter = m_mapInfo.begin();
	auto iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		int Key = iter->second->State.Key;

		// KeyState_Down
		if (m_vecKeyState[Key].State[KeyState_Down] &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			if (iter->second->Callback[KeyState_Down])
				iter->second->Callback[KeyState_Down](DeltaTime);
		}

		// KeyState_Push
		if (m_vecKeyState[Key].State[KeyState_Push] &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			if (iter->second->Callback[KeyState_Push])
				iter->second->Callback[KeyState_Push](DeltaTime);
		}

		// KeyState_Up
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
		for (int i = 0; i < KeyState_Max; i++)
		{
			iter->second->Callback[i] = nullptr;
		}
	}
}
