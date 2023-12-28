#include "PreCompile.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include <type_traits>

GameObjectManager& GameObjectManager::GetInst()
{
	static GameObjectManager inst;
	return inst;
}

void GameObjectManager::InsertGameObject(std::shared_ptr<GameObject> object)
{
	std::lock_guard lock(objectMapLock);
	objectMap.insert({ object->GetGameObjectId(), object });
}

void GameObjectManager::EraseGameObject(GameObjectId gameObjectId)
{
	std::lock_guard lock(objectMapLock);
	objectMap.erase(gameObjectId);
}

std::shared_ptr<GameObject> GameObjectManager::FindObject(GameObjectId gameObjectId)
{
	std::lock_guard lock(objectMapLock);
	auto findIter = objectMap.find(gameObjectId);
	if (findIter == objectMap.end())
	{
		return nullptr;
	}

	return findIter->second;
}

template<typename T>
std::shared_ptr<T> GameObjectManager::FindObject(GameObjectId gameObjectId)
{
	std::lock_guard lock(objectMapLock);
	auto findIter = objectMap.find(gameObjectId);
	if (findIter == objectMap.end())
	{
		return nullptr;
	}

	return dynamic_cast<std::shared_ptr<T>>(findIter->second);
}