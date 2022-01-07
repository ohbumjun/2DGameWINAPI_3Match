#include "Sound.h"
#include "../Path/PathManager.h"

CSound::CSound() :
	m_Group(nullptr),
	m_Channel(nullptr),
	m_Sound(nullptr),
	m_System(nullptr),
	m_Loop(false),
	m_Play(false)
{}

CSound::CSound(const CSound& Sound)
{
	m_Group = Sound.m_Group;
	m_Channel = Sound.m_Channel;
	m_Sound = Sound.m_Sound;
	m_System = Sound.m_System;
}

CSound::~CSound()
{}

bool CSound::LoadSound(const std::string& SoundName, FMOD::ChannelGroup*  Group, FMOD::System* System, bool Loop, const char* FileName,
	const std::string& PathName)
{
	m_System = System;
	m_Group = Group;
	m_Name = SoundName;
	m_Loop = Loop;

	FMOD_MODE Mode = FMOD_DEFAULT;
	if (m_Loop)
		Mode = FMOD_LOOP_NORMAL;

	char FullPath[MAX_PATH] = {};

	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	if (Path)
		strcpy_s(FullPath, Path->PathMultibyte);
	strcat_s(FullPath, FileName);

	if (m_System->createSound(FullPath, Mode, nullptr, &m_Sound) != FMOD_OK)
		return false;

	return true;
}

void CSound::Play()
{
	m_System->playSound(m_Sound, m_Group, false, &m_Channel);
	m_Play = true;
}

void CSound::Stop()
{
	if (!m_Channel)
		return;

	bool Playing = false;
	m_Channel->isPlaying(&Playing);

	if (Playing)
	{
		m_Channel->stop();
		m_Channel = nullptr;
		m_Play = false;
	}
}

void CSound::Resume()
{
	if (!m_Channel)
		return;

	bool Playing = false;
	m_Channel->isPlaying(&Playing);

	if (!Playing)
	{
		m_Channel->setPaused(false);
		m_Play = true;
	}
}

void CSound::Pause()
{
	if (!m_Channel)
		return;

	bool Playing = false;
	m_Channel->isPlaying(&Playing);

	if (Playing)
	{
		m_Channel->setPaused(true);
		m_Play = false;
	}
}
