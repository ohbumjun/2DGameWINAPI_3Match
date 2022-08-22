
#include "ResourceManager.h"
#include "../Path/PathManager.h"

CResourceManager* CResourceManager::m_Inst = nullptr;

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
	m_mapTexture.clear();
	m_mapAnimationSequence.clear();
	m_mapSound.clear();

	{
		auto iter = m_mapChannelGroup.begin();
		auto iterEnd = m_mapChannelGroup.end();
		for (; iter != iterEnd; ++iter)
		{
			iter->second->release();
		}
	}

	if (m_System)
	{
		m_System->close();
		m_System->release();
	}

	{
		auto iter = m_FontLoadList.begin();
		auto iterEnd = m_FontLoadList.end();

		for (; iter != iterEnd; ++iter)
		{
			RemoveFontResource((*iter).c_str());
		}
	}

}

bool CResourceManager::Init()
{
	HRESULT result = FMOD::System_Create(&m_System);
	if (result != FMOD_OK)
		return false;

	result = m_System->init(100, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
		return false;

	result = m_System->getMasterChannelGroup(&m_MasterGroup);
	if (result != FMOD_OK)
		return false;

	m_mapChannelGroup.insert(std::make_pair("Master", m_MasterGroup));

	// 

	LoadOtherFont(TEXT("NotoSansKR-Black.otf"));
	LoadOtherFont(TEXT("NotoSansKR-Bold.otf"));
	LoadOtherFont(TEXT("NotoSansKR-Regular.otf"));

	LoadFont("DefaultFont", TEXT("NotoSansKR-Regular"));

	// Sound
	CreateChannelGroup("Effect");
	CreateChannelGroup("UI");
	CreateChannelGroup("BGM");

	return true;
}

void CResourceManager::Update()
{
	m_System->update();
}

bool CResourceManager::LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);
	if (Texture)
		return true;

	Texture = new CTexture;

	if (!Texture->LoadTexture(Name, FileName, PathName))
	{
		SAFE_DELETE(Texture);
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

bool CResourceManager::CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName)
{
	CAnimationSequence* Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);
	if (Sequence)
		return true;
	Sequence = new CAnimationSequence;
	
	CTexture* Texture = FindTexture(TextureName);
	
	if (!Sequence->Init(SequenceName, Texture))
	{
		SAFE_DELETE(Sequence);
		return false;
	}
	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));

	return true;
}

bool CResourceManager::CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName, const TCHAR* FileName, const std::string& PathName)
{
	CAnimationSequence* Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);
	if (Sequence)
		return true;

	if (!LoadTexture(TextureName, FileName, PathName))
		return false;
	CTexture* Texture = FindTexture(TextureName);

	Sequence = new CAnimationSequence;
	if (!Sequence->Init(SequenceName, Texture))
	{
		SAFE_DELETE(Sequence);
		return false;
	}
	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));
	return true;
}

bool CResourceManager::CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	CAnimationSequence* Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);
	if (Sequence)
		return true;

	if (!LoadTexture(TextureName, vecFileName, PathName))
		return false;
	CTexture* Texture = FindTexture(TextureName);

	Sequence = new CAnimationSequence;
	if (!Sequence->Init(SequenceName, Texture))
	{
		SAFE_DELETE(Sequence);
		return false;
	}
	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));
	return true;
}

void CResourceManager::ReleaseAnimationSequence(const std::string& Name)
{
	auto iter = m_mapAnimationSequence.find(Name);
	if (iter->second->GetRefCount() == 1)
	{
		m_mapAnimationSequence.erase(iter);
	}
}

void CResourceManager::AddAnimationFrameData(const std::string& SequenceName, const Vector2& StartPos, const Vector2& Size)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);
	if (!Sequence)
		return;
	Sequence->AddFrameData(StartPos, Size);
}

void CResourceManager::AddAnimationFrameData(const std::string& SequenceName, float StartX, float StartY, float SizeX, float SizeY)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);
	if (!Sequence)
		return;
	Sequence->AddFrameData(StartX, StartY, SizeX, SizeY);
}

CAnimationSequence* CResourceManager::FindAnimationSequence(const std::string& Name)
{
	auto iter = m_mapAnimationSequence.find(Name);
	if (iter == m_mapAnimationSequence.end())
		return nullptr;
	return iter->second;
}


bool CResourceManager::CreateSound(const std::string& SoundName, const std::string& GroupName, bool Loop, const char* FileName)
{
	CSound* Sound = FindSound(SoundName);
	if (Sound)
		return true;

	FMOD::ChannelGroup* Group = FindGroup(GroupName);
	if (!Group)
		return false;

	Sound = new CSound;
	if (!Sound->LoadSound(SoundName, Group, m_System, Loop, FileName))
	{
		SAFE_DELETE(Sound);
		return false;
	}

	m_mapSound.insert(std::make_pair(SoundName, Sound));

	return true;
}

bool CResourceManager::CreateChannelGroup(const std::string& GroupName)
{
	FMOD::ChannelGroup* Group = FindGroup(GroupName);
	if (Group)
		return true;

	FMOD_RESULT result = m_System->createChannelGroup(GroupName.c_str(), &Group);

	if (result != FMOD_OK)
		return false;

	m_MasterGroup->addGroup(Group);

	m_mapChannelGroup.insert(std::make_pair(GroupName, Group));

	return true;
}

void CResourceManager::SoundPlay(const std::string& SoundName)
{
	CSound* Sound = FindSound(SoundName);
	if (!Sound)
		return;;
	Sound->Play();
}

void CResourceManager::SoundStop(const std::string& SoundName)
{
	CSound* Sound = FindSound(SoundName);
	if (!Sound)
		return;;
	Sound->Stop();
}

void CResourceManager::SoundPause(const std::string& SoundName)
{
	CSound* Sound = FindSound(SoundName);
	if (!Sound)
		return;;
	Sound->Pause();
}

void CResourceManager::SoundResume(const std::string& SoundName)
{
	CSound* Sound = FindSound(SoundName);
	if (!Sound)
		return;;
	Sound->Resume();
}

CSound* CResourceManager::FindSound(const std::string& SoundName)
{
	auto iter = m_mapSound.find(SoundName);
	if (iter == m_mapSound.end())
		return nullptr;
	return iter->second;
}

inline FMOD::ChannelGroup* CResourceManager::FindGroup(const std::string& GroupName)
{
	auto iter = m_mapChannelGroup.find(GroupName);
	if (iter == m_mapChannelGroup.end())
		return nullptr;
	return iter->second;
}

bool CResourceManager::LoadOtherFont(const TCHAR* FileName, const std::string& PathName)
{
	TCHAR FullPath[MAX_PATH] = {};

	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	if (Path)
		lstrcpy(FullPath, Path->Path);
	lstrcat(FullPath, FileName);

	AddFontResource(FullPath);

	m_FontLoadList.push_back(FullPath);

	return true;
}

bool CResourceManager::LoadFont(const std::string& Name, const TCHAR* FontName, int Width, int Height)
{
	CFont* Font = FindFont(Name);
	if (Font)
		return true;

	Font = new CFont;
	if (!Font->LoadFont(FontName, Width, Height))
	{
		SAFE_DELETE(Font);
		return false;
	}

	m_mapFont.insert(std::make_pair(Name, Font));

	return true;
}

CFont* CResourceManager::FindFont(const std::string& FontName)
{
	auto iter = m_mapFont.find(FontName);
	if (iter == m_mapFont.end())
		return nullptr;
	return iter->second;
}
