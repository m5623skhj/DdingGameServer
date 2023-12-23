#pragma once
#include "GameServerDefine.h"

struct Position
{
	UINT x = 0;
	UINT y = 0;
};

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

protected:
	Position position;
	Position moveToPosition;

	GameObjectId gameObjectId = INVLAID_GAME_OBJECT_ID;
};