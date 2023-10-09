#pragma once

enum ChallengeType
{
	NONE,
	POTATOES
};

class World;
class Challenge
{
public:
	virtual void onStart(World*);

	ChallengeType type;
};