#pragma once

#define INVALID_SESSION_ID 0
#define IP_SIZE 16

#define IO_COUNT_RELEASE_VALUE 0x100000
#define MAX_RIO_RESULT 256
#define MAX_SEND_BUFFER_SIZE 16384

#define MAXIMUM_BATCHED_DB_JOB_SIZE 16

#define ONE_FRAME 33

#define INVALID_DB_JOB_KEY UINT64_MAX

using SessionId = UINT64;
using DBJobKey = UINT64;