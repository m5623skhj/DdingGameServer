#pragma once

#include <synchapi.h>

class ScopeSRWLock
{
public:
	ScopeSRWLock() = delete;
	explicit ScopeSRWLock(OUT SRWLOCK& lock, bool exclusive);
	~ScopeSRWLock();

private:
	SRWLOCK& lock;
	bool exclusive;
};

#define SCOPE_READ_LOCK(lock) ScopeSRWLock scopeLock_##__LINE__(lock, false)
#define SCOPE_WRITE_LOCK(lock) ScopeSRWLock scopeLock_##__LINE__(lock, true)

class ScopeCriticalSection
{
public:
	ScopeCriticalSection() = delete;
	explicit ScopeCriticalSection(OUT CRITICAL_SECTION& lock);
	~ScopeCriticalSection();

private:
	CRITICAL_SECTION& lock;
};

#define SCOPE_LOCK(lock) ScopeCriticalSection scopeLock_##__LINE__(lock)

#define SCOPE_MUTEX(lock) std::lock_guard scopeLock_##__LINE__(lock);