#include "Options.h"

#include <fstream>
#include <string>
#include <iostream>

int Options::control = 1;
int Options::debug = 0;
int Options::sfx = 7;
int Options::music = 0;

void Options::load()
{
    std::ifstream of("options.txt");
    
    std::string l;

    while (std::getline(of, l))
    {
        if (l[0] == '#') continue;
        int eq = l.find_first_of('=');
        if (eq < 0) continue;
        std::string name = l.substr(0, eq);
        std::string value = l.substr(eq + 1, l.length());
        if (name == "control")
        {
            setControl(std::atoi(value.c_str()));
        }
        else if (name == "debug")
        {
            setDebug(std::atoi(value.c_str()));
        }
        else if (name == "sfx")
        {
            setSFX(std::atoi(value.c_str()));
        }
        else if (name == "music")
        {
            setMusic(std::atoi(value.c_str()));
        }
    }

    of.close();
}

void Options::save()
{
    std::ofstream of("options.txt");

    of << "control=" << getControl() << "\n";
    of << "debug=" << getDebug() << "\n";
    of << "sfx=" << getDebug() << "\n";
    of << "music=" << getMusic() << "\n";
    of.close();
}

void Options::setControl(int v)
{
    if (v <= 0) v = 0;
    else v = 1;
    control = v;
}

void Options::setDebug(int v)
{
    if (v <= 0) v = 0;
    else v = 1;
    debug = v;
}

void Options::setSFX(int v)
{
    if (v >= 10) v = 10;
    if (v <= 0) v = 0;
    sfx = v;
}

void Options::setMusic(int v)
{
    if (v >= 10) v = 10;
    if (v <= 0) v = 0;
    music = v;
}

int Options::getControl()
{
    return control;
}

int Options::getDebug()
{
    return debug;
}

int Options::getMusic()
{
    return music;
}

int Options::getSFX()
{
    return sfx;
}
