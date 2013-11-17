#ifndef _JUDGE_THREAD_H_
#define _JUDGE_THREAD_H_

#include "stdafx.h"
#include "BaseFunc.h"
#include "JudgeConfig.h"
#include "ThreadBase.h"
#include "JudgeDef.h"
#include "JudgeSys.h"
#include "LockedFolder.h"


class CJudgerThread : public CTaskThread<MatchInfo*>
{
public:
	CJudgerThread();

public:
	bool			setWorkDir(const char* sWorkDir);

	virtual void	addTask(MatchInfo* newMatch);

protected:
	virtual void onInit();
	virtual bool procTask(MatchInfo* theMatch);
	virtual void onDestory();

private:
	JudgeConfig*	m_judge_config;

	CLockedFolder	m_work_dir;

	CMemPool<MatchInfo>*	m_mem_pool;
	JudgeKeeper		m_judge_keeper;
};


class CJudgerPool : public CThreadBase
{
public:
	typedef std::vector<CJudgerThread*>	JUDGE_THREAD_VEC;
	typedef std::queue<MatchInfo*>		MATCH_TASK_QUE;

public:
	CJudgerPool();
	static CJudgerPool* getInstance();

public:
	void addTask(MatchInfo* pMatchInfo);

protected:
	virtual void onInit();
	virtual bool onProcess();
	virtual void onDestory();

	virtual bool newJudgerThread(const char* sCacheDir);

private:
	JudgeConfig*		m_judge_config;

	JUDGE_THREAD_VEC	m_judger_vec;
	int					m_judger_cnt;

	CMemPool<MatchInfo>* m_mem_pool;

	MATCH_TASK_QUE		m_task_que;

	CSafeLock			m_judge_lock;
};


#endif // _JUDGE_THREAD_H_