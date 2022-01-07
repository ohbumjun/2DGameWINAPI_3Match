#pragma once

#include "../Ref.h"

class CSound : public CRef {
	friend class CResourceManager;
protected:
	CSound();
	CSound(const CSound& Sound);
	virtual ~CSound() override;
private :
	std::string m_Name;
	FMOD::ChannelGroup* m_Group;
	FMOD::Channel* m_Channel;
	FMOD::Sound* m_Sound;
	FMOD::System* m_System;
	bool m_Loop;
	bool m_Play;
public :
	bool LoadSound(const std::string& SoundName, FMOD::ChannelGroup* Group, FMOD::System* System, bool Loop,
		const char* FileName, const std::string& PathName = SOUND_PATH);
	void Play();
	void Stop();
	void Resume();
	void Pause();
};