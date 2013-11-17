#include "stdafx.h"
#include "JudgeKeeper.h"

#include "JsonObject.h"
#include "ProcBase.h"


#include "JudgeSys.h"


JudgeKeeper::JudgeKeeper()
{
	m_judge_config = JudgeConfig::getConfig();

	m_work_dir.clear();
	m_jres_file.clear();
	m_judger_program.clear();

	m_safe_run = m_judge_config->g_safe_run_path;
	char safe_run_fp[MAX_PATH];
	BaseFunc::getFullPath(safe_run_fp, m_safe_run.c_str());
	m_safe_run = safe_run_fp;

	CJsonValue json_judge_err;
	json_judge_err["judge_result"] = -1;
	CJsonOperator::writeToString(m_judge_error, json_judge_err);

	clearDataCache();
}

JudgeKeeper::~JudgeKeeper()
{

}

void JudgeKeeper::setWorkDir(const char* sWorkDir)
{
	char _work_dir[MAX_PATH];
	BaseFunc::getFullPath(_work_dir, sWorkDir);
	m_work_dir = _work_dir;

	m_judger_program = m_work_dir + JUDGE_CONFIG::_JDUGER_FILE_NAME;
	m_jres_file = m_work_dir + JUDGE_CONFIG::_JUDGE_RESULT_TMP_FILE;
}

bool JudgeKeeper::doJudge(MatchInfo& miBody)
{
	bool b_ret = true;
	b_ret = b_ret && initDataCache(miBody);
	b_ret = b_ret && prepareJudgeFiles();
	b_ret = b_ret && runGameJudger();
	uploadMatchResult(b_ret);

	ADD_LOG("%s :%d -- %s", __FUNCTION__,
		m_data_cache.match_id, b_ret?"OK":"FAULT");
	return b_ret;
}

bool JudgeKeeper::initDataCache( MatchInfo& miBody )
{
	bool b_ret = true;
	clearDataCache();

	m_data_cache.match_id	= miBody.MatchID;
	
	m_data_cache.player_cnt = miBody.PalyerCnt;
	for (int i=0; i<m_data_cache.player_cnt; ++i)
	{
		PlayerInfo& player = m_data_cache.player_info[i];

		player.player_id = miBody.Players[i];
		b_ret = b_ret && JudgeDB::getPlayerExecInfo(
										player.player_id,
										player.file_path,
										player.file_md5);
	}
	
	m_data_cache.game_id	= miBody.GameID;
	b_ret = b_ret && JudgeDB::getJudgerExecInfo(
								m_data_cache.game_id,
								m_data_cache.judger_path,
								m_data_cache.judger_md5,
								m_data_cache.judger_config);
	m_data_cache.judger_path = m_judge_config->g_judger_exec_dir
								+ m_data_cache.judger_path;
	b_ret = b_ret && getJudgeParam();

	ADD_LOG("%s :%d -- %s", __FUNCTION__,
		m_data_cache.match_id, b_ret?"OK":"FAULT");
	return b_ret;
}

bool JudgeKeeper::prepareJudgeFiles()
{
	bool b_ret = true;
	
	for (int i=0; i < m_data_cache.player_cnt; ++i)
	{
		PlayerInfo& player_info = m_data_cache.player_info[i];
		
		std::string player_path_from	= m_judge_config->g_player_exec_dir + player_info.file_path;
		std::string player_path_to		= m_work_dir +  player_info.file_path;

		b_ret = b_ret && BaseFunc::copyFile(player_path_to.c_str(), player_path_from.c_str());

		ADD_LOG("%s :%d -- %s %s:%d", __FUNCTION__,
			m_data_cache.match_id, b_ret?"OK":"FAULT", player_info.file_path.c_str(), ::GetLastError());
	}

	b_ret = b_ret && BaseFunc::copyFile(
								m_judger_program.c_str(),
								m_data_cache.judger_path.c_str());
    
	ADD_LOG("%s :%d -- %s %s", __FUNCTION__,
		m_data_cache.match_id, b_ret?"OK":"FAULT", m_data_cache.judger_path.c_str());

	return b_ret;
}

bool JudgeKeeper::runGameJudger()
{
	CProcBase judger_proc;

	std::string judger_cmd;
	BaseFunc::strFormat(judger_cmd, "%s %s",
		m_judger_program.c_str(),
		m_data_cache.judger_safe_param.c_str());

	judger_proc.create(judger_cmd.c_str(), m_work_dir.c_str());
	judger_proc.run(-1);
	judger_proc.terminate();
	return true;
}


bool JudgeKeeper::getJudgeParam()
{
	CJsonValue param_ret;
	param_ret["player_cnt"] = m_data_cache.player_cnt;

	CJsonValue judge_config;
	CJsonOperator::readFromString(judge_config, m_data_cache.judger_config);

	for(int i=0; i<m_data_cache.player_cnt; ++i)
	{
		CJsonValue player_info;
		player_info["time_limit"]		= judge_config["time_limit"].asInt();
		player_info["duration_limit"]	= judge_config["duration_limit"].asInt();
		player_info["memory_limit"]		= judge_config["memory_limit"].asInt();
		player_info["proc_exec"]		= m_data_cache.player_info[i].file_path;
		param_ret["player"].append(player_info);
	}

	char s_safe_run[MAX_PATH];
	char s_work_dir[MAX_PATH];
	BaseFunc::getSafePath(s_safe_run, m_safe_run.c_str());
	BaseFunc::getSafePath(s_work_dir, m_work_dir.c_str());

	param_ret["safe_run"] = s_safe_run;
	param_ret["work_dir"] = s_work_dir;

	bool b_ret = true;
	b_ret = b_ret && CJsonOperator::writeToString(
			m_data_cache.judger_param, param_ret);
	ADD_LOG("JudgeParam: %s", m_data_cache.judger_param.c_str());

	if (b_ret)
	{
		BaseFunc::addBackSlash(m_data_cache.judger_safe_param,
							m_data_cache.judger_param);
	}
	
	return b_ret;
}

bool JudgeKeeper::uploadMatchResult(bool& bJudgeRes)
{
	std::string res_buff;
	if (bJudgeRes)
	{
		if( -1 == BaseFunc::getFileContent(res_buff, m_jres_file.c_str()) )
		{
			ADD_LOG("%s :%d getFileContent():%s -- FAULT", __FUNCTION__, m_data_cache.match_id, m_jres_file.c_str());
			bJudgeRes = false;
		}
	}
	if (!bJudgeRes)
		res_buff = m_judge_error;

	bool b_ret = true;
	b_ret = b_ret && JudgeDB::getEscapeString(m_data_cache.judge_res, res_buff);
	b_ret = b_ret && JudgeDB::uploadMatchResult(m_data_cache.match_id, m_data_cache.judge_res.c_str());
	if (!b_ret)
		ADD_LOG("%s :%d -- FAULT", __FUNCTION__, m_data_cache.match_id);
	return b_ret;
}

void JudgeKeeper::clearDataCache()
{
	m_data_cache.match_id = 0;
	m_data_cache.judge_res.clear();

	m_data_cache.game_id = 0;
	m_data_cache.judger_path.clear();
	m_data_cache.judger_md5.clear();
	m_data_cache.judger_param.clear();
	m_data_cache.judger_safe_param.clear();
	m_data_cache.judger_cmd.clear();

	m_data_cache.player_cnt = 0;
	for (int i=0; i<JUDGE_CONFIG::_MAX_PLAYER_CNT; ++i)
	{
		m_data_cache.player_info[i].player_id = 0;
		m_data_cache.player_info[i].file_path.clear();
		m_data_cache.player_info[i].file_md5.clear();
	}
}
