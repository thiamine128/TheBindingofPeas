#pragma once
class Options
{
public:
	static void load();
	static void save();
	
	static void setControl(int);
	static void setDebug(int);
	static void setSFX(int);
	static void setMusic(int);
	static int getControl();
	static int getDebug();
	static int getMusic();
	static int getSFX();
protected:
	static int control;
	static int debug;
	static int sfx;
	static int music;
};

