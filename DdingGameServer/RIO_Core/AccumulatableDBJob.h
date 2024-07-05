#pragma once
#include "DBJob.h"

class AccumulatableDBJob : public DBJob
{
public:
	AccumulatableDBJob() = delete;
	template <typename T, typename = std::enable_if_t<std::is_base_of_v<IGameAndDBPacket, T>>>
	explicit AccumulatableDBJob(RIOSession& inOwner, T& packet, T& rollbackPacket)
		: DBJob(inOwner, packet, rollbackPacket)
	{
	}
	virtual ~AccumulatableDBJob() override = default;

public:
	virtual void Clear() = 0;

public:
	virtual void operator+(AccumulatableDBJob&& other) = 0;
	virtual void operator-(AccumulatableDBJob&& ohter) = 0;
};