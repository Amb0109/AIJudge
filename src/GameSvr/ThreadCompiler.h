#ifndef _COMPILER_THREAD_H_
#define _COMPILER_THREAD_H_

#include "stdafx.h"
#include "JudgeSys.h"
#include "ThreadBase.h"
#include "LockedFolder.h"


class CCompilerThread : public CTaskThread<SubmitionInfo*>
{
public:
	CCompilerThread();
	~CCompilerThread();

public:
	bool			setWorkDir(const char* sWorkDir);
	virtual void	addTask(SubmitionInfo* newSubmition);

protected:
	virtual void	onInit();
	virtual bool	procTask(SubmitionInfo* theSubmition);
	virtual void	onDestory();

private:
	JudgeConfig*				m_judge_config;
	JudgeCompiler				m_judge_compiler;

	CMemPool<SubmitionInfo>*	m_mem_pool;
	CLockedFolder				m_work_dir;
};

class CompilerPool : public CThreadBase
{
public:
	typedef std::vector<CCompilerThread*>	COMPILER_THREAD_VEC;
	typedef std::queue<SubmitionInfo*>		SUBMITION_TASK_QUE;

public:
	CompilerPool();

public:
	static CompilerPool* getInstance();

	void addTask(SubmitionInfo* pSubmitionInfo);

protected:
	virtual void onInit();
	virtual bool onProcess();
	virtual void onDestory();

	virtual bool newCompilerThread(const char* sCacheDir);

private:
	JudgeConfig*				m_judge_config;

	COMPILER_THREAD_VEC			m_compiler_vec;
	int							m_compiler_cnt;

	CMemPool<SubmitionInfo>*	m_mem_pool;

	SUBMITION_TASK_QUE			m_task_que;

	CSafeLock					m_judge_lock;
};

#endif // _COMPILER_THREAD_H_