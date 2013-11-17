#ifndef _JUDGE_LOG_HPP_
#define _JUDGE_LOG_HPP_

#include "stdafx.h"

#include "LogFile.h"

#define ADD_LOG			GetJudgeLog()->fWrite
#define ADD_SERVER_LOG	GetServerLog()->fWrite

CLogFile* GetServerLog();
CDailyLog* GetJudgeLog();

#endif // _JUDGE_LOG_HPP_