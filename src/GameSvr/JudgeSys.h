#ifndef _JUDGE_SYSTEM_H_
#define _JUDGE_SYSTEM_H_

#include "stdafx.h"

#include "MemPool.h"
#include "JudgeDef.h"
#include "JudgeConfig.h"
#include "JudgeDB.h"
#include "JudgeLog.h"
#include "JudgeCompiler.h"
#include "JudgeKeeper.h"

#include "ThreadCompiler.h"
#include "ThreadJudger.h"

class JudgeSys
{
public:
	static bool initJudgeSys();
	static void CloseJudgeSys();

public:
	static bool	initMemPool();
	static CMemPool<SubmitionInfo>*	getCompileMemPool();
	static CMemPool<MatchInfo>*		getJudgeMemPool();

public:
	static bool initJudgePool();
	static void destoryJudgePool();

public:
	static void getPlayerFilePrefix(std::string& sFilePrefix, const SubmitionInfo& siBody);
};

#endif // _JUDGE_SYSTEM_H_