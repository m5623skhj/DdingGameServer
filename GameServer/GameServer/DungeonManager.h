#pragma once
#include <unordered_map>

class Dungeon;

class DungeonManager
{
private:
	DungeonManager() = default;
	~DungeonManager() = default;

public:
	static DungeonManager& GetInst();

private:
	std::unordered_map<DungeonDataId, std::shared_ptr<Dungeon>> DungeonMap;
};