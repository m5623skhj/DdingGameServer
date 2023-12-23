#pragma once
#include "GameObject.h"

class Creature : public GameObject
{
public:
	Creature() = default;
	virtual ~Creature() = default;

public:
	void SetHP(int newHP);
	void AddHP(int addValue);
	void SubtractHP(int subtractHP);
	void Revive(int newHP);

protected:
	int maxHP;
	int nowHP;

	bool isAlive;
};