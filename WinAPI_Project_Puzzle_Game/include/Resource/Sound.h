#pragma once

#include "../Ref.h"

class CSound : public CRef {
	friend class CResourceManager;
protected :
	CSound();
	CSound(const CSound& Sound);
	~CSound();
private :
	std::string m_Name;
	FMOD::ChannelGroup* m_Group;
	FMOD::Channel* m_Channel;
	FMOD::System* m_System;
	FMOD::Sound* m_Sound;
	bool m_Play;
	bool m_Loop;
public :
	bool LoadSound(FMOD::System* System, FMOD::ChannelGroup* Group, const std::string& Name,
		bool Loop, const char* FileName, const std::string& PathName = SOUND_PATH);
	void Play();
	void Stop();
	void Resume();
	void Pause();
};