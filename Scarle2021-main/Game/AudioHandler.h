// Created by Ben 10/03/2022

#pragma once

#include <list>

#include "StepTimer.h"

#include "GameData.h"
#include "Audio.h"
#include "Sound.h"

namespace Engine
{
	class AudioHandler
	{
	public:
		AudioHandler(GameData* _game_data);
		~AudioHandler() = default;

		void update(const DX::StepTimer& timer);

		struct SoundData
		{
			SoundData() = default;
			~SoundData() = default;

			std::string sound_name = "";
			float volume = 1.0F;
		};
		void playSound(const SoundData& sound_data);

	private:
		std::unique_ptr<DirectX::AudioEngine> audio_engine;
		std::list<Sound*>sounds;

		GameData* game_data;
	};
}
