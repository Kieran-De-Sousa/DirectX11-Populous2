#include "pch.h"
#include "Sound.h"

Sound::Sound(AudioEngine* _audEngine, string _filename)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	string fullpath = /*"../Sounds/" +*/ _filename + ".wav";
	std::wstring wFilename = converter.from_bytes(fullpath.c_str());

	m_sfx = std::make_unique<SoundEffect>(_audEngine, wFilename.c_str());
	duration = m_sfx->GetSampleDurationMS() / 1000 + 1; // / 1000 = ms -> seconds, + 1 so the buffer has definitely finished before it is deleted in AudioHandler
}

Sound::~Sound()
{
	m_sfx.reset();
}

void Sound::Play()
{
	if (m_sfx)
	{
		m_sfx->Play(m_volume, m_pitch, m_pan);
	}
}
