#pragma once
#include <unordered_map>
#include "GameServerDefine.h"
#include <memory>
#include <shared_mutex>

class GameObject;

class GameObjectManager
{
private:
	GameObjectManager() = default;
	~GameObjectManager() = default;

public:
	static GameObjectManager& GetInst();

public:
	void InsertGameObject(std::shared_ptr<GameObject> object);
	void EraseGameObject(GameObjectId gameObjectId);
	std::shared_ptr<GameObject> FindObject(GameObjectId gameObjectId);
	template<typename T>
	std::shared_ptr<T> FindObject(GameObjectId gameObjectId);
	GameObjectId GenerateObjectId() { return ++objectIdGenerator; }

private:
	std::atomic<GameObjectId> objectIdGenerator = 1;
	std::unordered_map<GameObjectId, std::shared_ptr<GameObject>> objectMap;
	std::shared_mutex objectMapLock;
};