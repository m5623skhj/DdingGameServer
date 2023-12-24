#include "PreCompile.h"
#include "GameObjectManager.h"
#include "GameObject.h"

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