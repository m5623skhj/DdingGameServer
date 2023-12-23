#include "PreCompile.h"
#include "GameObject.h"
#include "GameObjectManager.h"

GameObject::GameObject()
{
	gameObjectId = GameObjectManager::GetInst().GenerateObjectId();
}