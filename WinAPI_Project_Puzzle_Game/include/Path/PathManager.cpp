#include "PathManager.h"

CPathManager* CPathManager::m_Inst = nullptr;

CPathManager::CPathManager() :
	m_mapPath{}
{
}

CPathManager::~CPathManager()
{
	auto iter = m_mapPath.begin();
	auto iterEnd = m_mapPath.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CPathManager::Init()
{
	TCHAR Path[MAX_PATH];
	GetModuleFileName(0, Path, MAX_PATH);

	int Length = lstrlen(Path);

	for (int i = Length - 1; i >= 0; i--)
	{
		if (Path[i] == '\\')
		{
			memset(&Path[i+1], 0, Length - i - 1);
			break;
		}
	}

	PathInfo* Info = new PathInfo;
	lstrcpy(Info->Path, Path);

#ifdef UNICODE
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, Path, -1, nullptr, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, Path, -1, Info->PathMultibyte, ConvertLength, 0,0);
#else
	strcpy_s(Info->PathMultibyte, Path);
#endif // UNICODE

	m_mapPath.insert(std::make_pair(ROOT_PATH, Info));

	AddPath(TEXTURE_PATH, TEXT("Texture\\"));
	AddPath(SOUND_PATH, TEXT("Sound\\"));
	AddPath(FONT_PATH, TEXT("Font\\"));
	AddPath(MAP_PATH, TEXT("Map\\"));


	return true;
}

bool CPathManager::AddPath(const std::string& Name, const TCHAR* Path, const std::string& BaseName)
{
	if (FindPath(Name))
		return true;

	const PathInfo* BasePath = FindPath(BaseName);

	if (!BasePath)
		return false;

	PathInfo* Info = new PathInfo;
	lstrcpy(Info->Path, BasePath->Path);
	lstrcat(Info->Path, Path);

#ifdef UNICODE
	int ConvertLength = WideCharToMultiByte(
		CP_ACP, // cost page 
		0,  // flag 
		Info->Path,  // ��ȯ'��'�� ���ڿ� �ּ� 
		-1,  // ���ڿ� ���� --> ��� 
		nullptr,  // ���� �ּ� 
		0,  // ���ڿ� ���� 
		0,  // ����Ʈ ĳ����
		0); // �÷��׸� �ݳ��ϴ� �ּ� 

	WideCharToMultiByte(CP_ACP, 0, Info->Path, -1, 
		Info->PathMultibyte, ConvertLength, 0,0);
#else
	strcpy_s(Info->PathMultibyte, Info->Path);
#endif // UNICODE

	m_mapPath.insert(std::make_pair(Name, Info));
	
	return false;
}

const PathInfo* CPathManager::FindPath(const std::string& Name)
{
	auto iter = m_mapPath.begin();
	auto iterEnd = m_mapPath.end();

	for (; iter != iterEnd; ++iter)
	{
		if (iter->first == Name)
			return iter->second;
	}
	return nullptr;
}
