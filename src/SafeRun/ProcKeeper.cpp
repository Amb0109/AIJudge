#include "stdafx.h"
#include "ProcKeeper.h"

#include "JsonObject.h"

ProcKeeper::ProcKeeper()
{
	m_monitor_interval	= 100;
	m_start_clock		= 0;

	m_proc_cmd.clear();
	m_time_limit		= 0;
	m_memory_limit		= 0;
	m_duration_limit	= 0;
}

ProcKeeper::~ProcKeeper()
{
	destory();
}

bool ProcKeeper::init(const char* sConfig)
{
	CJsonValue config;
	bool b_ret = CJsonOperator::readFromString(config, sConfig);
	if (!b_ret) return false;

	m_proc_cmd			= config["proc_exec"].asString();
	m_time_limit		= config["time_limit"].asInt();
	m_duration_limit	= config["duration_limit"].asInt();
	m_memory_limit		= config["memory_limit"].asInt();

	return true;
}

void ProcKeeper::setMonitorInterval(int tiInterval)
{
	m_monitor_interval = tiInterval;
}

bool ProcKeeper::createProcess()
{
	bool b_ret = true;
	
	DeviceHandleT input_handle	= ::GetStdHandle(STD_INPUT_HANDLE);
	DeviceHandleT output_handle	= ::GetStdHandle(STD_OUTPUT_HANDLE);
	DeviceHandleT error_handle	= ::GetStdHandle(STD_ERROR_HANDLE);

	// SafeRun作为一个过渡层，把自己的标准输入输出句柄传递给子进程
	b_ret = b_ret && m_process.setInputDevice(input_handle);
	b_ret = b_ret && m_process.setOutputDevice(output_handle);
	b_ret = b_ret && m_process.setErrorDevice(error_handle);
	b_ret = b_ret && m_process.create(m_proc_cmd.c_str());

	// 关掉自己的输入输出句柄
	::CloseHandle(input_handle);
	::CloseHandle(output_handle);
	::CloseHandle(error_handle);

	// 设置沙箱限制 & 绑定
	b_ret = b_ret && m_sandbox.init();
	b_ret = b_ret && m_sandbox.setLimit(m_time_limit, m_memory_limit);
	b_ret = b_ret && m_sandbox.assignProcess(m_process);
	
	return b_ret;
}

int ProcKeeper::runAndMonitor()
{
	int n_ret = 0;
	m_process.run(0);
	m_start_clock = clock();
	while (true)
	{
		n_ret = m_sandbox.getStatus(m_monitor_interval);
		if (n_ret != CSandBox::PROC_STATUS_PROC_RUNNING)
			break;
		if (clock() - m_start_clock >= m_duration_limit)
			return CSandBox::PROC_STATUS_TIME_LIMITED;
	}
	return n_ret;
}

void ProcKeeper::destory()
{
	m_process.terminate();
}



