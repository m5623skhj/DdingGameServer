#include "PreCompile.h"
#include "Creature.h"

void Creature::SetHP(int newHP)
{
	if (isAlive == false)
	{
		return;
	}

	nowHP = newHP;
	if (maxHP < nowHP)
	{
		nowHP = maxHP;
	}
	else if (nowHP < 0)
	{
		nowHP = 0;
		OnDied();
	}
}

void Creature::AddHP(int addHP)
{
	if (isAlive == false)
	{
		return;
	}

	nowHP += addHP;
	if (maxHP < nowHP)
	{
		nowHP = maxHP;
	}
}

void Creature::SubtractHP(int subtractHP)
{
	if (isAlive == false)
	{
		return;
	}

	nowHP -= subtractHP;
	if (nowHP < 0)
	{
		nowHP = 0;
		isAlive = false;
		OnDied();
	}
}

void Creature::Revive(int newHP)
{
	if (isAlive == true)
	{
		return;
	}

	nowHP = newHP;
	isAlive = true;
	OnRevived();
}