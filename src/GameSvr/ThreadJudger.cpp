#include "stdafx.h"
#include "JudgeDB.h"
#include "ThreadJudger.h"
#include "MemPool.h"

CJudgerThread::CJudgerThread()
{
	m_judge_config = JudgeConfig::getConfig();
	m_mem_pool = JudgeSys::getJudgeMemPool();
}

void CJudgerThread::onInit()
{
	setThreadPriority(TPL_LOWEST);
	CThreadBase::onInit();
}

bool CJudgerThread::procTask(MatchInfo* theMatch)
{
	m_work_dir.clear();
	m_judge_keeper.doJudge(*theMatch);
	popTask();
	m_mem_pool->delRef(theMatch);
	return true;
}

void CJudgerThread::onDestory()
{
	CThreadBase::onDestory();
}

bool CJudgerThread::setWorkDir(const char* sWorkDir)
{
	m_work_dir.setFolder(sWorkDir);
	bool b_ret = true;
	b_ret = b_ret && m_work_dir.create();
	b_ret = b_ret && m_work_dir.lock();
	if(b_ret) m_judge_keeper.setWorkDir(sWorkDir);
	return b_ret;
}

void CJudgerThread::addTask( MatchInfo* newMatch )
{
	m_mem_pool->addRef(newMatch);
	CTaskThread<MatchInfo*>::addTask(newMatch);
}





CJudgerPool::CJudgerPool()
{
	m_judge_config = JudgeConfig::getConfig();
	m_mem_pool = JudgeSys::getJudgeMemPool();
	m_judger_cnt = m_judge_config->g_judger_thread_cnt;
	while (!m_task_que.empty()) m_task_que.pop();
}

void CJudgerPool::onInit()
{
	setThreadPriority(TPL_LOWEST);

	bool b_ret = true;
	m_judger_vec.clear();
	char cache_dir[MAX_PATH];
	for (int i=0; i<m_judger_cnt; ++i)
	{
		sprintf(cache_dir, "%s_judge_%d/", m_judge_config->g_judge_cache_dir.c_str(), i);
		b_ret = b_ret && newJudgerThread(cache_dir);
		ADD_SERVER_LOG("Create Judger %d -- %s", i, b_ret?"OK":"FAULT");
	}
	CThreadBase::onInit();
}

bool CJudgerPool::onProcess()
{
	MatchInfo* judge_task = NULL;
	{
		CAutoLock _LOCK(&m_judge_lock);
		if (!m_task_que.empty())
		{
			judge_task = m_task_que.front();
			m_task_que.pop();
		}
		else
		{
			MatchInfo* tmp_judge = m_mem_pool->allocBlock();
			if( JudgeDB::fetchNewMatch(tmp_judge) )
				addTask(tmp_judge);
			else
				BaseFunc::sleep(30);
			JudgeSys::getJudgeMemPool()->freeBlock(tmp_judge);
		}
	}
	if (judge_task == NULL)
	{
		BaseFunc::sleep(10);
		return true;
	}

beg_for_proc:
	bool judged = false;
	for_std_unit(JUDGE_THREAD_VEC, m_judger_vec, it_pjudger)
	{
		CJudgerThread*& pjudger = *it_pjudger;
		if(pjudger->isEmpty())
		{
			pjudger->addTask(judge_task);
			m_mem_pool->delRef(judge_task);
			judged = true;
			break;
		}
	}

	BaseFunc::sleep(10);

	if (!judged)
	{
		goto beg_for_proc;
	}

	return true;
}

void CJudgerPool::onDestory()
{
	for_std_unit(JUDGE_THREAD_VEC, m_judger_vec, it_pjudger)
	{
		CJudgerThread*& pjudger = *it_pjudger;
		pjudger->Close();
	}
	m_judger_vec.clear();
	CThreadBase::onDestory();
}

CJudgerPool* CJudgerPool::getInstance()
{
	static CJudgerPool gJudgePool;
	return &gJudgePool;
}

void CJudgerPool::addTask( MatchInfo* pMatchInfo )
{
	CAutoLock _LOCK(&m_judge_lock);
	m_mem_pool->addRef(pMatchInfo);
	m_task_que.push(pMatchInfo);
}

bool CJudgerPool::newJudgerThread(const char* sCacheDir)
{
	bool b_ret = true;

	CJudgerThread* p_judger = NULL;
	p_judger = new CJudgerThread();
	b_ret = b_ret && (p_judger != NULL);
	if (!b_ret)
	{
		ADD_SERVER_LOG("%s: new CJudgerThread() -- Fault", __FUNCTION__);
		return false;
	}

	b_ret = b_ret && p_judger->setWorkDir(sCacheDir);
	if (!b_ret)
	{
		ADD_SERVER_LOG("%s: p_judger->setWorkDir(sCacheDir) -- Fault", __FUNCTION__);
		b_ret = false;
	}

	b_ret = b_ret && p_judger->Create();

	if(b_ret) m_judger_vec.push_back(p_judger);
	else delete p_judger;

	return b_ret;
}

