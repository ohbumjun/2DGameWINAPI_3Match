#include "Sound.h"
#include "../Path/PathManager.h"

CSound::CSound() :
	m_Play(false)
{}

CSound::~CSound()
{}

bool CSound::LoadSound(FMOD::System* System, FMOD::ChannelGroup* ChannelGroup, const std::string& SoundName, bool Loop,
	const char* FileName, const std::string& PathName)
{
	m_System = System;
	m_ChannelGroup = ChannelGroup;
	m_Name = SoundName;
	m_Loop = Loop;

	char FullPath[MAX_PATH] = {};

	const PathInfo* PathInfo = CPathManager::GetInst()->FindPath(PathName);
	if (PathInfo)
		strcpy_s(FullPath, PathInfo->PathMultibyte);

	strcat_s(FullPath, FileName);

	FMOD_MODE Mode = FMOD_DEFAULT;
	if (Loop)
		Mode = FMOD_LOOP_NORMAL;

	if (!m_System->createSound(FullPath, Mode, nullptr, &m_Sound))
		return false;

	return true;
}

void CSound::Play()
{
	m_System->playSound(m_Sound, m_ChannelGroup, false, &m_Channel);
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
	}
	m_Play = false;
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
	}
	m_Play = true;
}

void CSound::Pause()
{
	if (!m_Channel)
		return;
	bool Playing = false;
	m_Channel->isPlaying(&Playing);
	if (Playing)
		m_Channel->setPaused(true);
	m_Play = false;
}
