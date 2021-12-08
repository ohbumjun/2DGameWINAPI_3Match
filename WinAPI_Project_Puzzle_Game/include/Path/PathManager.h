#pragma once

#include "../GameInfo.h"

struct PathInfo
{
	// TCHAR : 유니코드, 멀티바이트 문자열 둘다 지원
	TCHAR Path[MAX_PATH];
	char PathMultibyte[MAX_PATH];

	PathInfo() :
		Path{},
		PathMultibyte{}
	{}
};

class CPathManager
{
private :
	CPathManager();
	~CPathManager();
private :
	std::unordered_map<std::string, PathInfo*> m_mapPath;
public :
	bool Init();
	bool AddPath(const std::string& Name, const TCHAR* Path, const std::string& BaseName = ROOT_PATH);
	const PathInfo* FindPath(const std::string& Name);
private :
	static CPathManager* m_Inst;
public :
	static CPathManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CPathManager;
		return m_Inst;
	}
	static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}
};