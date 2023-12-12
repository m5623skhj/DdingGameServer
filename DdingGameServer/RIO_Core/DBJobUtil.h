#pragma once
#include <memory>
#include "DBJob.h"

template<
	typename T, typename = std::enable_if_t<std::is_base_of_v<DBJob, T>>,
	typename U, typename = std::enable_if_t<std::is_base_of_v<IGameAndDBPacket, U>>>
std::shared_ptr<T> MakeDBJob(RIOSession& inOwner, U& packet, U& forRollback)
{
	return std::make_shared<T>(inOwner, packet, forRollback);
}

std::shared_ptr<BatchedDBJob> MakeBatchedDBJob(RIOSession& inOwner);