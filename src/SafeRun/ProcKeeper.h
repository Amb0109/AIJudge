#ifndef _PROCESS_KEEPER_H_
#define _PROCESS_KEEPER_H_

#include "stdafx.h"
#include "ProcBase.h"
#include "SandBox.h"

class ProcKeeper
{
public:
	ProcKeeper();
	~ProcKeeper();

public:
	bool	init(const char* sConfig);
	void	setMonitorInterval(int tiInterval);
	bool	createProcess();
	int		runAndMonitor();
	void	destory();

private:
	CSandBox		m_sandbox;
	CProcBase		m_process;

	int				m_monitor_interval;
	int				m_start_clock;

	std::string		m_proc_cmd;
	// 由于存在中断，所以限制CPU时间会比较准
	// 超出时返回超时PROC_STATUS_TIME_LIMITED
	int				m_time_limit;
	// 由于中断不占用CPU时间，则会由于中断而造成进程无法结束
	// 添加持续时间限制，非CPU时间，为系统时间差，m_monitor_interval影响此设置的精度 
	// 与 m_time_limit 相同，超出时返回超时PROC_STATUS_TIME_LIMITED
	int				m_duration_limit;
	// 内存限制，超出时返回PROC_STAUTS_MEMORY_LIMITED
	int				m_memory_limit;
};

#endif // _PROCESS_KEEPER_H_
