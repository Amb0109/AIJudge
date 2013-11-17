#include "stdafx.h"
#include "JudgeConfig.h"

JudgeConfig* JudgeConfig::getConfig()
{
	static JudgeConfig gJudgeConfig(JUDGE_CONFIG::_JUDGE_CONFIG_PATH);
	return &gJudgeConfig;
}

JudgeConfig::JudgeConfig(const char* sFilePath)
: m_config_file(sFilePath)
{
	initConfig();
}

bool JudgeConfig::initConfig()
{
	g_compiler_thread_cnt	= _safeReadConfig("System", "CompilerThreadCount", DEFAULT_CONFIG::DEFAULT_COMPILER_THREAD_CNT);
	g_judger_thread_cnt		= _safeReadConfig("System", "JudgerThreadCount", DEFAULT_CONFIG::DEFAULT_JUDGER_THREAD_CNT);
	g_compiler_timeout		= _safeReadConfig("System", "CompilerTimeOut", DEFAULT_CONFIG::DEFAULT_COMPILER_TIMEOUT);
	g_mempool_page_size		= _safeReadConfig("System", "MemoryPoolPageSize", DEFAULT_CONFIG::DEFAULT_MEMPOOL_PAGE_SIZE);

	g_judge_cache_dir		= _safeReadConfig("System", "JudgeCacheDirectory", DEFAULT_CONFIG::DEFAULT_JUDGE_CACHE_DIRECTORY);
	g_judge_log_dir			= _safeReadConfig("System", "JudgeLogDirectory", DEFAULT_CONFIG::DEFAULT_JUDGE_LOG_DIRECTORY);
	g_judger_exec_dir		= _safeReadConfig("System", "JudgerExecDirectory", DEFAULT_CONFIG::DEFAULT_JUDGER_EXEC_DIRECTORY);
	g_player_exec_dir		= _safeReadConfig("System", "PlayerExecDirectory", DEFAULT_CONFIG::DEFAULT_PLAYER_EXEC_DIRECTORY);
	g_safe_run_path			= _safeReadConfig("System", "SafeRunPath", DEFAULT_CONFIG::DEFAULT_SAFE_RUN_PATH);

	g_gpp_compiler			= _safeReadConfig("Compiler", "GUN_CPP", DEFAULT_CONFIG::DEFAULT_GPP_COMPILER_APP);
	g_gcc_compiler			= _safeReadConfig("Compiler", "GUN_C", DEFAULT_CONFIG::DEFAULT_GCC_COMPILER_APP);
	g_msvc_compiler			= _safeReadConfig("Compiler", "MSVC", DEFAULT_CONFIG::DEFAULT_MSVC_COMPILER_APP);

	g_database_server		= _safeReadConfig("DATABASE", "Server", DEFAULT_CONFIG::DEFAULT_DATABASE_SERVER);
	g_database_port			= _safeReadConfig("DATABASE", "Port", DEFAULT_CONFIG::DEFAULT_DATABASE_PORT);
	g_database_dbname		= _safeReadConfig("DATABASE", "DBName", DEFAULT_CONFIG::DEFAULT_DATABASE_DBNAME);
	g_database_user			= _safeReadConfig("DATABASE", "User", DEFAULT_CONFIG::DEFAULT_DATABASE_USER);
	g_database_pass			= _safeReadConfig("DATABASE", "Pass", DEFAULT_CONFIG::DEFAULT_DATABASE_PASS);

	return true;
}

int JudgeConfig::_safeReadConfig(const char* sSection, const char* sKey, int DefaultVal /*= 0 */)
{
	int ret_config_val;
	if ( !m_config_file.getIntValue(sSection, sKey, ret_config_val) )
	{
		m_config_file.setIntValue(sSection, sKey, DefaultVal);
		return DefaultVal;
	}
	else
		return ret_config_val;
}

std::string JudgeConfig::_safeReadConfig( const char* sSection, const char* sKey, const char* DefaultVal /*= ""*/ )
{
	char ret_config_val[1024];
	if ( !m_config_file.getStrValue(sSection, sKey, ret_config_val, 1024) )
	{
		m_config_file.setStrValue(sSection, sKey, DefaultVal);
		return std::string(DefaultVal);
	}
	else
		return std::string(ret_config_val);
}
