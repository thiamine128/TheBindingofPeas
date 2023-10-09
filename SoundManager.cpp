#include "SoundManager.h"

#include "util/Options.h"

#include <iostream>

FMOD_SYSTEM* SoundManager::system = nullptr;
FMOD_CHANNEL* SoundManager::channel = 0;
FMOD_CHANNEL* SoundManager::loopChannel = 0;
std::unordered_map<std::string, FMOD_SOUND*> SoundManager::entires;

//初始化FMOD
void SoundManager::init()
{
    FMOD_RESULT result;
    result = FMOD_System_Create(&system, FMOD_VERSION);
    void* extraDirverData = 0;
    if (result != FMOD_OK)
    {
        std::cout << "Failed to create fmod system." << std::endl;
    }
    FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, extraDirverData);
    if (result != FMOD_OK)
    {
        std::cout << "Failed to create fmod system." << std::endl;
    }
}

//释放内存
void SoundManager::terminate()
{
    FMOD_System_Close(system);
    FMOD_System_Release(system);
}

//播放音效
void SoundManager::play(std::string const& s)
{
    FMOD_SOUND* sound;
    if (entires.count(s) == 0)
    {
        FMOD_RESULT result;
        result = FMOD_System_CreateSound(system, s.c_str(), FMOD_DEFAULT, 0, &sound);
        if (result != FMOD_OK)
        {
            std::cout << "Failed to load sound " << s << std::endl;
        }
    } else
    {
        sound = entires[s];
    }
    if (Options::getSFX())
    {
        FMOD_Sound_SetMode(sound, FMOD_LOOP_OFF);
        FMOD_System_PlaySound(system, sound, 0, false, &channel);
        float sfx = Options::getSFX() / 10.0f;
        FMOD_Channel_SetVolume(channel, sfx);
    }
}

//循环播放音效
void SoundManager::playLoop(std::string const& s)
{
    FMOD_SOUND* sound;
    if (entires.count(s) == 0)
    {
        FMOD_RESULT result;
        result = FMOD_System_CreateSound(system, s.c_str(), FMOD_DEFAULT, 0, &sound);
        if (result != FMOD_OK)
        {
            std::cout << "Failed to load sound " << s << std::endl;
        }
    }
    else
    {
        sound = entires[s];
    }
    FMOD_Sound_SetMode(sound, FMOD_LOOP_NORMAL);
    FMOD_System_PlaySound(system, sound, 0, false, &loopChannel);
    float music = Options::getMusic() / 40.0f;
    FMOD_Channel_SetVolume(loopChannel, music);
}

//停止循环播放的音效
void SoundManager::stopLoop()
{
    FMOD_Channel_Stop(loopChannel);
    loopChannel = 0;
}

//FMOD更新
void SoundManager::update()
{
    FMOD_System_Update(system);
    if (loopChannel)
    {
        float music = Options::getMusic() / 40.0f;
        FMOD_Channel_SetVolume(loopChannel, music);
    }
}