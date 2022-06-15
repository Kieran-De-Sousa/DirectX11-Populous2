// Created by Ben 10/03/2022

#include "pch.h"

#include "AudioHandler.h"

Engine::AudioHandler::AudioHandler(GameData* _game_data)
{
    game_data = _game_data;

    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags = eflags | AudioEngine_Debug;
#endif
    audio_engine = std::make_unique<AudioEngine>(eflags);
#pragma endregion
}

void Engine::AudioHandler::update(const DX::StepTimer& timer)
{
    // This will update the audio engine but give us chance to do somehting else if that isn't working
    if (!audio_engine->Update())
    {
        if (audio_engine->IsCriticalError())
        {
            // We lost the audio device!
        }
    }
    else
    {
        // Update sounds playing
        auto it = sounds.begin();
        while (it != sounds.end())
        {
            Sound* sound = (*it);
            sound->duration_timer += timer.GetElapsedSeconds();

            if (sound->duration_timer > sound->duration)
            {
                delete *it;
                it = sounds.erase(it);
            }
            else
            {
                (*it)->Tick(game_data);
                ++it;
            }
        }
    }
}

void Engine::AudioHandler::playSound(const SoundData& sound_data)
{
    Sound* sound = new Sound(audio_engine.get(), sound_data.sound_name);
    sound->SetVolume(sound_data.volume);

    sounds.push_back(sound);
    sound->Play();
}
