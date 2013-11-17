#include "stdafx.h"
#include "JudgeLog.h"

#include "LogFile.h"
#include "JudgeConfig.h"



CLogFile* GetServerLog()
{
	static CLogFile gServerLog(JUDGE_CONFIG::_SERVER_LOG_PATH);
	return &gServerLog;
}

CDailyLog* GetJudgeLog()
{
	JudgeConfig* p_judge_config = JudgeConfig::getConfig();
	static CDailyLog gJudgeLog(p_judge_config->g_judge_log_dir.c_str(), "Log");
	return &gJudgeLog;
}