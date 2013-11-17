#include "stdafx.h"
#include "ThreadCompiler.h"
#include "JudgeSys.h"
#include "JudgeDB.h"
#include "MemPool.h"


CCompilerThread::CCompilerThread()
{
	m_judge_config = JudgeConfig::getConfig();
	m_mem_pool = JudgeSys::getCompileMemPool();
}

CCompilerThread::~CCompilerThread()
{
}

void CCompilerThread::onInit()
{
	setThreadPriority(TPL_LOWEST);
	CThreadBase::onInit();
}

void CCompilerThread::onDestory()
{
	CThreadBase::onDestory();
}

bool CCompilerThread::setWorkDir(const char* sWorkDir)
{
	m_work_dir.setFolder(sWorkDir);
	bool b_ret = true;
	b_ret = b_ret && m_work_dir.create();
	b_ret = b_ret && m_work_dir.lock();
	if(b_ret) m_judge_compiler.setWorkDir(sWorkDir);
	return b_ret;
}

bool CCompilerThread::procTask(SubmitionInfo* theSubmition)
{
	m_work_dir.clear();
	m_judge_compiler.doCompile(*theSubmition);
	popTask();
	m_mem_pool->delRef(theSubmition);
	return true;
}

void CCompilerThread::addTask( SubmitionInfo* newSubmition )
{
	m_mem_pool->addRef(newSubmition);
	CTaskThread<SubmitionInfo*>::addTask(newSubmition);
}







///////////////////////////////////////////
CompilerPool::CompilerPool()
{
	m_judge_config = JudgeConfig::getConfig();
	m_mem_pool = JudgeSys::getCompileMemPool();
	//m_is_busy = false;
	m_compiler_cnt = m_judge_config->g_compiler_thread_cnt;
	while (!m_task_que.empty()) m_task_que.pop();
}

void CompilerPool::onInit()
{
	setThreadPriority(TPL_LOWEST);
	
	bool b_ret = true;
	m_compiler_vec.clear();
	char cache_dir[MAX_PATH];
	for (int i=0; i<m_compiler_cnt; ++i)
	{
		sprintf(cache_dir, "%s_compile_%d/", m_judge_config->g_judge_cache_dir.c_str(), i);
		b_ret = b_ret && newCompilerThread(cache_dir);
		ADD_SERVER_LOG("Create Compiler %d -- %s", i, b_ret?"OK":"FAULT");
	}
	CThreadBase::onInit();
}

bool CompilerPool::onProcess()
{
	SubmitionInfo* compile_task = NULL;
	{
		CAutoLock _LOCK(&m_judge_lock);
		if (!m_task_que.empty())
		{
			compile_task = m_task_que.front();
			m_task_que.pop();
		}
		else
		{
			SubmitionInfo* tmp_submition = m_mem_pool->allocBlock();
			if( JudgeDB::fetchNewSubmit(tmp_submition) )
				addTask(tmp_submition);
			else
				BaseFunc::sleep(30);
			m_mem_pool->freeBlock(tmp_submition);
		}
	}
	if (compile_task == NULL)
	{
		BaseFunc::sleep(10);
		return true;
	}

beg_for_proc:
	bool compiled = false;
	for_std_unit(COMPILER_THREAD_VEC, m_compiler_vec, it_pcompiler)
	{
		CCompilerThread*& pcompiler = *it_pcompiler;
		if(pcompiler->isEmpty())
		{
			pcompiler->addTask(compile_task);
			m_mem_pool->delRef(compile_task);
			compiled = true;
			break;
		}
	}

	BaseFunc::sleep(10);

	if (!compiled)
		goto beg_for_proc;

	return true;
}

void CompilerPool::onDestory()
{
	for_std_unit(COMPILER_THREAD_VEC, m_compiler_vec, it_pcompiler)
	{
		CCompilerThread*& pcompiler = *it_pcompiler;
		pcompiler->Close();
	}
	m_compiler_vec.clear();
	CThreadBase::onDestory();
}

CompilerPool* CompilerPool::getInstance()
{
	static CompilerPool gCompilerPool;
	return &gCompilerPool;
}

void CompilerPool::addTask(SubmitionInfo* pSumbitionInfo)
{
	CAutoLock _LOCK(&m_judge_lock);
	m_mem_pool->addRef(pSumbitionInfo);
	m_task_que.push(pSumbitionInfo);
}

bool CompilerPool::newCompilerThread(const char* sCacheDir)
{
	bool b_ret = true;

	CCompilerThread* p_compiler = NULL;
	p_compiler = new CCompilerThread();
	b_ret = b_ret && (p_compiler != NULL);
	if (!b_ret)
	{
		ADD_SERVER_LOG("%s: new CCompilerThread() -- Fault", __FUNCTION__);
		return false;
	}

	b_ret = b_ret && p_compiler->setWorkDir(sCacheDir);
	if (!b_ret)
	{
		ADD_SERVER_LOG("%s: p_compiler->setWorkDir(sCacheDir) -- Fault", __FUNCTION__);
		b_ret = false;
	}

	b_ret = b_ret && p_compiler->Create();

	if(b_ret) m_compiler_vec.push_back(p_compiler);
	else delete p_compiler;

	return b_ret;
}

