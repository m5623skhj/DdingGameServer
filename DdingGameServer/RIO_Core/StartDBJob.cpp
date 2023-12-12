#include "PreCompile.h"
#include "StartDBJob.h"

DBJob_StartDBJob::DBJob_StartDBJob(RIOSession& inOwner, DBJobStart& packet, DBJobStart& rollbackPacket)
	: DBJob(inOwner, packet, rollbackPacket)
{

}

void DBJob_StartDBJob::OnCommit()
{

}

void DBJob_StartDBJob::OnRollback()
{

}
