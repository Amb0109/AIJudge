#include "stdafx.h"
#include "JudgeCompiler.h"

#include "ProcBase.h"

#include "JudgeSys.h"


JudgeCompiler::JudgeCompiler()
{
	m_judge_config = JudgeConfig::getConfig();
	
	m_work_dir.clear();
	m_err_file.clear();

	clearDataCache();
}


bool JudgeCompiler::doCompile(SubmitionInfo& siBody)
{
	bool b_ret = true;

	b_ret = b_ret && initDataCache(siBody);
	b_ret = b_ret && saveCodeFile();
	b_ret = b_ret && doCompileCode();
	
	ADD_LOG("%s :%d -- %s", __FUNCTION__, m_data_cache.run_id, b_ret?"OK":"FAULT");

	return b_ret;
}

bool JudgeCompiler::fillFileName()
{
	switch (m_data_cache.language)
	{
	case L_GCC:
	case L_C:
		m_data_cache.src_file = m_data_cache.file_prefix + ".c";
		m_data_cache.bin_file = m_data_cache.file_prefix + ".exe";
		break;
	case L_GPP:
	case L_CPP:
		m_data_cache.src_file = m_data_cache.file_prefix + ".cpp";
		m_data_cache.bin_file = m_data_cache.file_prefix + ".exe";
		break;
	default:
		ADD_LOG("%s:%d Unknown Language!", __FUNCTION__, __LINE__);
		return false;
	}

	return true;
}

bool JudgeCompiler::getCompileCommand()
{
	switch (m_data_cache.language)
	{
	case L_GPP:
		BaseFunc::strFormat(m_data_cache.compile_command,
			JUDGE_CONFIG::CMDLINE_GUNC_COMPILE_FMT,
			m_judge_config->g_gpp_compiler.c_str(),
			m_data_cache.src_file.c_str(),
			m_data_cache.bin_file.c_str());
		break;
	case L_GCC:
		BaseFunc::strFormat(m_data_cache.compile_command,
			JUDGE_CONFIG::CMDLINE_GUNC_COMPILE_FMT,
			m_judge_config->g_gcc_compiler.c_str(),
			m_data_cache.src_file.c_str(),
			m_data_cache.bin_file.c_str());
		break;
	case L_CPP:
	case L_C:
		BaseFunc::strFormat(m_data_cache.compile_command,
			JUDGE_CONFIG::CMDLINE_MSVC_COMPILE_FMT,
			m_judge_config->g_msvc_compiler.c_str(),
			m_data_cache.src_file.c_str(),
			m_data_cache.bin_file.c_str());
		break;
	default:
		ADD_LOG("%s:%d Unknown Language!", __FUNCTION__, __LINE__);
		return false;
	}

	return true;
}

bool JudgeCompiler::doCompileCode()
{
	CProcBase compile_subproc;
	int compile_timeout = m_judge_config->g_compiler_timeout;
	compile_subproc.setErrorFile(m_err_file.c_str());
	compile_subproc.create(m_data_cache.compile_command.c_str(), m_work_dir.c_str());
	int n_ret = compile_subproc.run(compile_timeout);
	compile_subproc.terminate();

	switch ( n_ret )
	{
	case 1:
		{
			int exit_code = compile_subproc.getExitCode();
			if (exit_code)
				m_data_cache.compile_result = CR_ERR_CE;
			else
				m_data_cache.compile_result = CR_SUCCESS;
		}
		break;
	case 0:
		m_data_cache.compile_result = CR_ERR_TIMEOUT;
		break;
	case -1:
		m_data_cache.compile_result = CR_ERR_UNKNOWN;
		break;
	}

	bool b_ret = true;
	b_ret = b_ret && getCompileError();
	b_ret = b_ret && uploadCompileResult();
	b_ret = b_ret && storePlayerExecFile();

	ADD_LOG("%s :%d -- %s", __FUNCTION__, m_data_cache.run_id, b_ret?"OK":"FAULT");
	return b_ret;
}

void JudgeCompiler::setWorkDir( const char* sWorkDir )
{
	char work_dir[260];
	BaseFunc::getFullPath(work_dir, sWorkDir);
	m_work_dir = work_dir;

	m_err_file = m_work_dir + JUDGE_CONFIG::_COMPILE_RESULT_TMP_FILE;
}

bool JudgeCompiler::initDataCache( SubmitionInfo& siBody )
{
	clearDataCache();

	m_data_cache.run_id		= siBody.RunID;
	m_data_cache.game_id	= siBody.GameID;
	m_data_cache.user_id	= siBody.UserID;
	m_data_cache.language	= siBody.LanguageID;

	JudgeSys::getPlayerFilePrefix(m_data_cache.file_prefix, siBody);
	
	bool b_ret = true;
	b_ret = b_ret && fillFileName();
	b_ret = b_ret && getCompileCommand();
	ADD_LOG("%s :%d -- %s", __FUNCTION__, m_data_cache.run_id, b_ret?"OK":"FAULT");
	return b_ret;
}

void JudgeCompiler::clearDataCache()
{
	m_data_cache.run_id		= 0;
	m_data_cache.game_id	= 0;
	m_data_cache.user_id	= 0;
	m_data_cache.language	= L_NULL;
	
	m_data_cache.file_prefix.clear();
	m_data_cache.src_file.clear();
	m_data_cache.bin_file.clear();
	m_data_cache.compile_command.clear();

	m_data_cache.compile_result = CR_NULL;
	m_data_cache.compile_error.clear();
}

bool JudgeCompiler::saveCodeFile()
{
	std::string src_full_path = m_work_dir + m_data_cache.src_file;
	bool b_ret = true;
	b_ret = b_ret && JudgeDB::saveSourceFile(src_full_path.c_str(), m_data_cache.run_id);
	ADD_LOG("%s :%d -- %s", __FUNCTION__, m_data_cache.run_id, b_ret?"OK":"FAULT");
	return b_ret;
}

bool JudgeCompiler::getCompileError()
{
	switch (m_data_cache.compile_result)
	{
	case CR_SUCCESS:
		break;
	case CR_ERR_CE:
		{
			std::string res_buff;
			if( -1 == BaseFunc::getFileContent(res_buff, m_err_file.c_str()) )
			{
				ADD_LOG("%s :%d getFileContent():%s -- FAULT", __FUNCTION__, m_data_cache.run_id, m_err_file.c_str());
				return false;
			}

			std::string res_buff_2;
			BaseFunc::ConvertGBKToUtf8(res_buff_2, res_buff);
			res_buff = res_buff_2;

			if (0 != res_buff.size())
			{
				if(!JudgeDB::getEscapeString(m_data_cache.compile_error, res_buff))
				{
					ADD_LOG("%s :%d getEscapeString() -- FAULT", __FUNCTION__, m_data_cache.run_id);
					return false;
				}
			}
		}
		break;
	case CR_ERR_TIMEOUT:
		m_data_cache.compile_error = "Compile Time Out!";
		break;
	case CR_ERR_UNKNOWN:
		m_data_cache.compile_error = "Unknown Error!";
		break;
	default:
		return false;
	}

	return true;
}

bool JudgeCompiler::uploadCompileResult()
{
	bool b_ret = JudgeDB::uploadCompileResult(
		m_data_cache.run_id,
		m_data_cache.user_id,
		m_data_cache.game_id,
		m_data_cache.compile_result == CR_SUCCESS,
		m_data_cache.compile_error.c_str());

	if (!b_ret)
	{
		ADD_LOG("%s:%d Store Exec File Error!", __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}

bool JudgeCompiler::storePlayerExecFile()
{
	if (CR_SUCCESS == m_data_cache.compile_result)
	{
		bool b_ret = true;
		std::string bin_full_path;
		std::string bin_store_path;
		char bin_md5_check[35] = {0};

		bin_full_path = m_work_dir + m_data_cache.bin_file;
		b_ret = b_ret && BaseFunc::MD5File(bin_full_path.c_str(), bin_md5_check);

		bin_store_path = m_judge_config->g_player_exec_dir + m_data_cache.bin_file;
		b_ret = b_ret && BaseFunc::copyFile(bin_store_path.c_str(), bin_full_path.c_str());

		b_ret = b_ret && JudgeDB::storePlayerExecFile(m_data_cache.run_id,
													m_data_cache.bin_file.c_str(),
													bin_md5_check);

		if(!b_ret)
		{
			ADD_LOG("%s:%d Store Exec File Error!", __FUNCTION__, __LINE__);
			return false;
		}
		return true;
	}
	else
		return true;
}

