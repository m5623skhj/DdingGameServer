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
	ZoneId GetZoneId() const { return zoneId; }
	void SetZoneId(const ZoneId inZoneId) { zoneId = inZoneId; }

protected:
	Position position;
	Position moveToPosition;

	GameObjectId gameObjectId = INVLAID_GAME_OBJECT_ID;
	ZoneId zoneId = INVALID_ZONE_ID;
};