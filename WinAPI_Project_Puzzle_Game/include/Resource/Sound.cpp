#include "Sound.h"
#include "../Path/PathManager.h"

CSound::CSound() :
	m_System(nullptr),
	m_Sound(nullptr),
	m_Group(nullptr),
	m_Channel(nullptr),
	m_Play(nullptr),
	m_Loop(nullptr)
{}

CSound::~CSound()
{
	if (m_Sound)
		m_Sound->release();
}

bool CSound::LoadSound(FMOD::System* System, FMOD::ChannelGroup* Group, bool Loop, const std::string& Name,
	const char* FileName, const std::string& PathName)
{
	m_Name = Name;
	m_Group = Group;
	m_System = System;

	char FullPath[MAX_PATH] = {};
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);
	if (Path)
		strcpy_s(FullPath, Path->PathMultibyte);
	strcat_s(FullPath, Path->PathMultibyte);

	FMOD_MODE Mode = FMOD_DEFAULT;
	if (Loop)
		Mode = FMOD_LOOP_NORMAL;

	if (m_System->createSound(Name.c_str(), Mode, nullptr, &m_Sound) != FMOD_OK)
		return false;

	return true;
}

void CSound::Play()
{
	if (m_Channel)
	{
		m_System->playSound(m_Sound, m_Group, false,&m_Channel);
		m_Play = true;
	}
}

void CSound::Stop()
{
	if (m_Channel)
	{
		bool Playing = false;
		m_Channel->isPlaying(&Playing);
		if (Playing)
		{
			m_Channel->stop();
			m_Play = false;
		}
	}
}

void CSound::Pause()
{
	if (m_Channel)
	{
		bool Playing = false;
		m_Channel->isPlaying(&Playing);
		if (Playing)
		{
			m_Channel->setPaused(true);
			m_Channel = nullptr;
			m_Play = false;
		}
	}
}

void CSound::Resume()
{
	if (m_Channel)
	{
		bool Playing = false;
		m_Channel->isPlaying(&Playing);
		if (!Playing)
		{
			m_Channel->setPaused(false);
		}
		m_Play = true;
	}
}