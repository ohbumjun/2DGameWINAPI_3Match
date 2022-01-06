#include "Sound.h"
#include "../Path/PathManager.h"

CSound::CSound() :
m_Play(false)
{}

CSound::CSound(const CSound& Sound)
{
	m_Name     = Sound.m_Name;
	m_Group    = Sound.m_Group;
	m_Channel = Sound.m_Channel;
	m_System  = Sound.m_System;
	m_Sound   = Sound.m_Sound;
	m_Play      = Sound.m_Play;
	m_Loop     = Sound.m_Loop;
}

CSound::~CSound()
{}

bool CSound::LoadSound(FMOD::System* System, FMOD::ChannelGroup* Group, const std::string& Name, bool Loop,
	const char* FileName, const std::string& PathName)
{
	m_System = System;
	m_Group = Group;
	m_Name = Name;
	m_Loop = Loop;

	char FullPath[MAX_PATH] = {};

	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);
	if (Path->PathMultibyte)
		strcpy_s(FullPath, Path->PathMultibyte);
	strcat_s(FullPath, FileName);


	FMOD_MODE Mode = FMOD_DEFAULT;
	if (Loop)
		Mode = FMOD_LOOP_NORMAL;

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
	{
		m_Channel->setPaused(true);
	}
	m_Play = false;
}
