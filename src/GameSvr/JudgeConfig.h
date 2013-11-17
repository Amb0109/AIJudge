#ifndef _GLOBAL_VARIABLE_H_
#define _GLOBAL_VARIABLE_H_

#include "stdafx.h"
#include "ConfigFile.h"
#include "JudgeDef.h"

class JudgeConfig
{
public:
	explicit JudgeConfig(const char* sFilePath);
	~JudgeConfig(){}

	static JudgeConfig* getConfig();

public:
	bool			initConfig();

protected:
	int				_safeReadConfig(const char* sSection, const char* sKey, int DefaultVal = 0 );
	std::string		_safeReadConfig(const char* sSection, const char* sKey, const char* DefaultVal = "");

public:
	///////////////////////////////////////////
	int				g_compiler_thread_cnt;
	int				g_judger_thread_cnt;
	int				g_compiler_timeout;
	int				g_mempool_page_size;

	std::string		g_judge_cache_dir;
	std::string		g_judge_log_dir;
	std::string		g_judger_exec_dir;
	std::string		g_player_exec_dir;
	std::string		g_safe_run_path;

	std::string		g_gpp_compiler;
	std::string		g_gcc_compiler;
	std::string		g_msvc_compiler;

	std::string		g_database_server;
	int				g_database_port;
	std::string		g_database_dbname;
	std::string		g_database_user;
	std::string		g_database_pass;

	//////////////////////////////////////////

private:
	CConfigFile		m_config_file;
};

#endif // _GLOBAL_VARIABLE_H_