#pragma once

enum class RIO_COMPLETION_KEY_TYPE : INT8
{
	STOP = 0
	, START
};

enum class RIO_OPERATION_TYPE : INT8
{
	OP_ERROR = 0
	, OP_RECV
	, OP_SEND
	, OP_SEND_REQUEST
};

enum class IO_MODE : LONG
{
	IO_NONE_SENDING = 0
	, IO_SENDING
};

enum class IO_POST_ERROR : INT8
{
	SUCCESS = 0
	, IS_DELETED_SESSION
	, FAILED_RECV_POST
	, FAILED_SEND_POST
	, INVALID_OPERATION_TYPE
	, INVALID_SEND_BUFFER_SIZE
};

enum class PACKET_ID : unsigned int
{
	INVALID_PACKET = 0
	, ECHO_STRING_PACEKT
	, PING
	, PONG
	, GAME2DB_BATCHED_DB_JOB
	, DB2GAME_BATCHED_DB_JOB
};

enum class ERROR_CODE : unsigned short
{
	SUCCESS = 0
	, BATCHED_DB_JOB_SIZE_OVERFLOWED
};

enum class LOG_LEVEL : unsigned char
{
	Verbosse = 1,
	Debug,
	Info,
	Warning,
	Error,
	Fatal,
};