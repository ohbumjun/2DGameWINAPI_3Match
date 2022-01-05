#pragma  once

#include "../Ref.h"

class CSound : public CRef {
public :
	CSound();
	~CSound();
private :
	std::string m_Name;
	FMOD::ChannelGroup* m_ChannelGroup;
	FMOD::Channel* m_Channel;
	FMOD::Sound* m_Sound;
	FMOD::System* m_System;
	bool m_Loop;
	bool m_Play;
public :
	bool IsPlay() const
{
		return m_Play;
}
	bool IsLoop() const
{
		return m_Loop;
}
public :
	bool LoadSound(FMOD::System* System, FMOD::ChannelGroup* Group,
		bool Loop, const std::string& Name, const char* FileName,
		const std::string& PathName = SOUND_PATH);
	void Play();
	void Stop();
	void Resume();
	void Pause();
};