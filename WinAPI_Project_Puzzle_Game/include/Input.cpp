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
}

void CInput::Render(HDC hDC)
{
}

void CInput::UpdateMouse(float DeltaTime)
{
}

void CInput::UpdateKeyState()
{
}

void CInput::UpdateKeyInfo(float DeltaTime)
{
}

void CInput::ClearCallback()
{
}
