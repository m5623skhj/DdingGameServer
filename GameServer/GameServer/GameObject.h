#pragma once
#include "GameServerDefine.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject() = default;

public:
	Position GetPosition() const { return position; }
	void SetPosition(Position&& newPosition) { position = std::move(newPosition); }
	void SetMoveToPosition(Position&& targetPosition) { moveToPosition = std::move(targetPosition); }

	GameObjectId GetGameObjectId() { return gameObjectId; }

public:
	[[nodiscard]]
	ZoneDataId GetZoneId() const { return zoneId; }
	void SetZoneId(const ZoneDataId inZoneId) { zoneId = inZoneId; }

protected:
	Position position;
	Position moveToPosition;

	GameObjectId gameObjectId = INVLAID_GAME_OBJECT_ID;
	ZoneDataId zoneId = INVALID_ZONE_ID;
};