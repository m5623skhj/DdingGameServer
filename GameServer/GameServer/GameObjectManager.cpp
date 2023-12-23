#include "PreCompile.h"
#include "GameObjectManager.h"
#include "GameObject.h"

GameObjectManager& GameObjectManager::GetInst()
{
	static GameObjectManager inst;
	return inst;
}

void GameObjectManager::AddGameObject(std::shared_ptr<GameObject> object)
{
	objectMap.insert({ object->GetGameObjectId(), object });
}