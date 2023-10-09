#pragma once

#include <fmod.h>
#include <string>
#include <unordered_map>

class SoundManager
{
public:
    static void init();
    static void terminate();
    static void play(std::string const&);
    static void playLoop(std::string const&);
    static void stopLoop();
    static void update();
protected:
    static FMOD_SYSTEM* system;
    static FMOD_CHANNEL* channel;
    static FMOD_CHANNEL* loopChannel;
    static std::unordered_map<std::string, FMOD_SOUND*> entires;
};