#include "stdafx.h"
#include "JudgeSys.h"
#include "ProcBase.h"
#include "MemPool.h"

bool JudgeSys::initMemPool()
{
	bool b_ret = true;
	b_ret = b_ret && getCompileMemPool()->newPage();
	b_ret = b_ret && getJudgeMemPool()->newPage();
	ADD_SERVER_LOG("%s -- %s", __FUNCTION__, b_ret?"OK":"FAULT");
	return b_ret;
}

CMemPool<SubmitionInfo>* JudgeSys::getCompileMemPool()
{
	JudgeConfig* p_judge_config = JudgeConfig::getConfig();
	static CMemPool<SubmitionInfo> gCompileMemPool(p_judge_config->g_mempool_page_size);
	return &gCompileMemPool;
}

CMemPool<MatchInfo>* JudgeSys::getJudgeMemPool()
{
	JudgeConfig* p_judge_config = JudgeConfig::getConfig();
	static CMemPool<MatchInfo> gJudgeMemPool(p_judge_config->g_mempool_page_size);
	return &gJudgeMemPool;
}

void JudgeSys::CloseJudgeSys()
{
	destoryJudgePool();

	getCompileMemPool()->clearMemPool();
	getJudgeMemPool()->clearMemPool();
}

bool JudgeSys::initJudgeSys()
{
	bool b_ret = true;
	b_ret = b_ret && initMemPool();
	b_ret = b_ret && initJudgePool();
	ADD_SERVER_LOG("%s -- %s", __FUNCTION__, b_ret?"OK":"FAULT");
	return true;
}

void JudgeSys::getPlayerFilePrefix(std::string& sFilePrefix, const SubmitionInfo& siBody)
{
	sFilePrefix.clear();
	std::string file_ext = "";
	switch (siBody.LanguageID)
	{
	case L_C:
		file_ext = "C";
		break;
	case L_CPP:
		file_ext = "CPP";
		break;
	case L_GCC:
		file_ext = "G";
		break;
	case L_GPP:
		file_ext = "GPP";
		break;
	default:
		break;
	}

	BaseFunc::strFormat(sFilePrefix, "%X_%X_%d_%s",
		siBody.RunID,
		siBody.UserID,
		siBody.GameID,
		file_ext.c_str());
}

bool JudgeSys::initJudgePool()
{
	bool b_ret = true;
	b_ret = b_ret && CompilerPool::getInstance()->Create();
	b_ret = b_ret && CJudgerPool::getInstance()->Create();
	return b_ret;
}

void JudgeSys::destoryJudgePool()
{
	CompilerPool::getInstance()->Close(2000);
	CJudgerPool::getInstance()->Close(2000);
}
