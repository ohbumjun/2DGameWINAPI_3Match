#pragma once

#include "Texture.h"
#include "AnimationSequence.h"
#include "Sound.h"
#include "Font.h"

class CResourceManager
{
private:
	CResourceManager();
	~CResourceManager();
private :
	FMOD::System* m_System;
	FMOD::ChannelGroup* m_MasterGroup;
	std::unordered_map<std::string, CSharedPtr<CTexture>> m_mapTexture;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence>> m_mapAnimationSequence;
	std::unordered_map<std::string, CSharedPtr<CSound>> m_mapSound;
	std::unordered_map<std::string, FMOD::ChannelGroup*> m_mapChannelGroup;
	std::list<std::wstring> m_FontLoadList;
	std::unordered_map<std::string, CSharedPtr<CFont>> m_mapFont;
public :
	bool Init();
	void Update();
public :
	// Texture -------------------------------------------------------------------------------------------------------------------------------------------------
	bool LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName =  TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName =  TEXTURE_PATH);
	void SetTextureColorKey(const std::string& Name,
		const unsigned char r, const unsigned char g,
		const unsigned char b, int Index = 0);
	void SetTextureColorKey(const std::string& Name,
		unsigned int ColorKey, int Index = 0);
	void ReleaseTexture(const std::string& Name);
	CTexture* FindTexture(const std::string& Name);

	// AnimationSequence -------------------------------------------------------------------------------------------------------------------------------------------------
	bool CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName);
	bool CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	bool CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName, const std::vector<std::wstring>& vecFileName, const std::string& PathName = TEXTURE_PATH);
	void ReleaseAnimationSequence(const std::string& Name);
	void AddAnimationFrameData(const std::string& SequenceName, const Vector2& StartPos, const Vector2& Size);
	void AddAnimationFrameData(const std::string& SequenceName, float StartX, float StartY, float SizeX, float SizeY);
	CAnimationSequence* FindAnimationSequence(const std::string& Name);

	// Sound -------------------------------------------------------------------------------------------------------------------------------------------------
	bool CreateChannelGroup(const std::string& GroupName);
	bool CreateSound(const std::string& SoundName, const std::string& GroupName,
	bool Loop, const char* FileName, const std::string& PathName = SOUND_PATH);
	void SoundPlay(const std::string& SoundName);
	void SoundResume(const std::string& SoundName);
	void SoundStop(const std::string& SoundName);
	void SoundPause(const std::string& SoundName);
	void SetVolume(const std::string& SoundName, int Volume);

	CSound* FindSound(const std::string& SoundName);
	FMOD::ChannelGroup* FindChannelGroup(const std::string& GroupName);

	// Font -------------------------------------------------------------------------------------------------------------------------------------------------
	bool LoadOtheFont(const TCHAR* FileName, const std::string& PathName = FONT_PATH);
	bool LoadFont(const std::string& Name, const TCHAR* FontName, int Width = 0, int Height = 0);
	CFont* FindFont(const std::string& FontName);

private :
	static CResourceManager* m_Inst;
public :
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
