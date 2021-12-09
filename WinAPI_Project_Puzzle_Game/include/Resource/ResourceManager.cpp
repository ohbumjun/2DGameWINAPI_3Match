
#include "ResourceManager.h"
#include "../Path/PathManager.h"

CResourceManager* CResourceManager::m_Inst = nullptr;

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
	m_mapTexture.clear();
}

bool CResourceManager::Init()
{
	return true;
}

void CResourceManager::Update()
{
}

bool CResourceManager::LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);
	if (Texture)
		return true;

	Texture = new CTexture;

	if (!Texture->LoadTexture(Name, FileName, PathName))
	{
		SAFE_RELEASE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CResourceManager::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	CTexture* Texture = FindTexture(Name);
	if (Texture)
		return true;

	Texture = new CTexture;

	if (!Texture->LoadTextureFullPath(Name, FullPath))
	{
		SAFE_RELEASE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CResourceManager::LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);
	if (Texture)
		return true;

	Texture = new CTexture;

	if (!Texture->LoadTexture(Name, vecFileName, PathName))
	{
		SAFE_RELEASE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

void CResourceManager::SetTextureColorKey(const std::string& Name, const unsigned char r, const unsigned char g, const unsigned char b, int Index)
{
	CTexture* Texture = FindTexture(Name);
	if (!Texture)
		return;

	Texture->SetColorKey(r, g, b, Index);
}

void CResourceManager::SetTextureColorKey(const std::string& Name, unsigned int ColorKey, int Index)
{
	CTexture* Texture = FindTexture(Name);
	if (!Texture)
		return;

	Texture->SetColorKey(ColorKey, Index);
}

void CResourceManager::ReleaseTexture(const std::string& Name)
{
	auto iter = m_mapTexture.find(Name);

	if (iter->second->GetRefCount() == 1)
		m_mapTexture.erase(iter);

}

CTexture* CResourceManager::FindTexture(const std::string& Name)
{
	auto iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
		return nullptr;
	return iter->second;
}
