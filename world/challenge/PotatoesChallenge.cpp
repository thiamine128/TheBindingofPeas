#include "PotatoesChallenge.h"

#include "world/World.h"

PotatoesChallenge::PotatoesChallenge()
{
	this->type = ChallengeType::POTATOES;
}

void PotatoesChallenge::onStart(World* world)
{
	world->disableShoot();
	world->getPlayer()->gainItem(Item::potatoseedpack);
}
