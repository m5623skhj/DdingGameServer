#pragma once
#include "Creature.h"
#include "GameServerDefine.h"
#include <memory>
#include "DefineType.h"

class NPC : public Creature
{
public:
	NPC() = default;
	explicit NPC(GameObjectId inNPCObjectId);

public:
	virtual void OnEnterWorld() override;
	virtual void OnLeaveWorld() override;
	virtual void OnDamaged() override;
	virtual void OnDied() override;
	virtual void OnRevived() override;

private:
	UINT64 npdDataId = INVALID_DATA_ID;
};