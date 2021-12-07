#pragma once

#include "Texture.h"
#include "AnimationSequence.h"
// #include "Sound.h"
// #include "Font.h"

class CResourceManager
{
private:
	CResourceManager();
	~CResourceManager();

private:
	// FMOD::System* m_System;
	// FMOD::ChannelGroup* m_MasterGroup;
	// std::unordered_map<std::string, FMOD::ChannelGroup*>	m_mapChannelGroup;
	// std::unordered_map<std::string, CSharedPtr<CSound>>		m_mapSound;

private:
	std::unordered_map<std::string, CSharedPtr<CTexture>>	m_mapTexture;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence>>	m_mapAnimationSequence;
	std::list<std::wstring>	m_FontLoadList;
	// std::unordered_map<std::string, CSharedPtr<CFont>>	m_mapFont;


public:
	bool Init();
	void Update();

public:
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextureColorKey(const std::string& Name,
		const unsigned char r, const unsigned char g,
		const unsigned char b, int Index = 0);
	void SetTextureColorKey(const std::string& Name,
		unsigned int ColorKey, int Index = 0);

	void ReleaseTexture(const std::string& Name);

	CTexture* FindTexture(const std::string& Name);

public:
	bool CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName);
	bool CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName,
		const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	bool CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName,
		const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	void AddAnimationFrameData(const std::string& SequenceName, const Vector2& StartPos, const Vector2& Size);
	void AddAnimationFrameData(const std::string& SequenceName, float PosX, float PosY, float SizeX, float SizeY);

	void ReleaseAnimationSequence(const std::string& Name);

	CAnimationSequence* FindAnimationSequence(const std::string& Name);

public:
	bool LoadSound(const std::string& GroupName, bool Loop, const std::string& Name, const char* FileName,
		const std::string& PathName = SOUND_PATH);
	bool CreateSoundChannelGroup(const std::string& Name);
	bool SetVolume(int Volume);
	bool SetVolume(const std::string& GroupName, int Volume);
	bool SoundPlay(const std::string& Name);
	bool SoundStop(const std::string& Name);
	bool SoundPause(const std::string& Name);
	bool SoundResume(const std::string& Name);

	void ReleaseSound(const std::string& Name);

	// FMOD::ChannelGroup* FindSoundChannelGroup(const std::string& Name);
	// CSound* FindSound(const std::string& Name);

// public:
	// bool LoadOtherFont(const TCHAR* FileName, const std::string& PathName = FONT_PATH);
	// bool LoadFont(const std::string& Name, const TCHAR* FontName,
	//		int Width = 0, int Height = 0);

	// CFont* FindFont(const std::string& Name);

private:
	static CResourceManager* m_Inst;

public:
	static CResourceManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CResourceManager;

		return m_Inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}
};

