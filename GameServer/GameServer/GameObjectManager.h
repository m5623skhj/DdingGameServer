#pragma once
#include <unordered_map>
#include "GameServerDefine.h"
#include <memory>

class GameObject;

class GameObjectManager
{
private:
	GameObjectManager() = default;
	~GameObjectManager() = default;

public:
	static GameObjectManager& GetInst();

public:
	void AddGameObject(std::shared_ptr<GameObject> object);
	GameObjectId GenerateObjectId() { return ++objectIdGenerator; }

private:
	std::atomic<GameObjectId> objectIdGenerator = 1;
	std::unordered_map<GameObjectId, std::shared_ptr<GameObject>> objectMap;
};