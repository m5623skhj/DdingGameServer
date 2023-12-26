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

public:
	virtual void OnEnterWorld() = 0;
	virtual void OnLeaveWorld() = 0;
	virtual void OnDamaged() = 0;
	virtual void OnDied() = 0;
	virtual void OnRevived() = 0;

protected:
	int maxHP;
	int nowHP;

	bool isAlive;
};