#include "StageResource.h"
#include "../Resource/ResourceManager.h"

CStageResource::CStageResource()
{
}

CStageResource::~CStageResource()
{
	{
		auto	iter = m_mapAnimationSequence.begin();
		auto	iterEnd = m_mapAnimationSequence.end();

		for (; iter != iterEnd;)
		{
			std::string	Name = iter->first;

			// erase를 하면 SharedPtr이므로 자동으로 객체가 제거되며 카운트가
			// 1 감소한다. erase는 지운 다음 iterator를 반환하므로 ++을 안해줘도
			// 된다.
			iter = m_mapAnimationSequence.erase(iter);

			CResourceManager::GetInst()->ReleaseAnimationSequence(Name);
		}
	}

	{
		auto	iter = m_mapTexture.begin();
		auto	iterEnd = m_mapTexture.end();

		for (; iter != iterEnd;)
		{
			std::string	Name = iter->first;

			// erase를 하면 SharedPtr이므로 자동으로 객체가 제거되며 카운트가
			// 1 감소한다. erase는 지운 다음 iterator를 반환하므로 ++을 안해줘도
			// 된다.
			iter = m_mapTexture.erase(iter);

			CResourceManager::GetInst()->ReleaseTexture(Name);
		}
	}
}

bool CStageResource::CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName)
{
    return false;
}

bool CStageResource::CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName, const TCHAR* FileName, const std::string& PathName)
{
    return false;
}

bool CStageResource::CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
    return false;
}

void CStageResource::AddAnimationFrameData(const std::string& SequenceName, const Vector2& StartPos, const Vector2& Size)
{
}

void CStageResource::AddAnimationFrameData(const std::string& SequenceName, float PosX, float PosY, float SizeX, float SizeY)
{
}

CAnimationSequence* CStageResource::FindAnimationSequence(const std::string& Name)
{
    return nullptr;
}


bool CStageResource::LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
		return false;

	CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

	m_mapTexture.insert(std::make_pair(Name, Texture));

    return true;
}

bool CStageResource::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
		return false;

	CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CStageResource::LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName))
		return false;

	CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

void CStageResource::SetTextureColorKey(const std::string& Name, const unsigned char r, const unsigned char g, const unsigned char b, int Index)
{
	CResourceManager::GetInst()->SetTextureColorKey(Name, r, g, b, Index);
}

void CStageResource::SetTextureColorKey(const std::string& Name, unsigned int ColorKey, int Index)
{
}

CTexture* CStageResource::FindTexture(const std::string& Name)
{
    return nullptr;
}
