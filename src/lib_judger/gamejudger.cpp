#include "stdafx.h"
#include "BaseFunc.h"
#include "GameJudger.h"

bool GameJudger::initConfig(const char* sGameConfig)
{
	CJsonValue json_config;
	m_game_config = sGameConfig;
	bool b_ret = CJsonOperator::readFromString(json_config, m_game_config);
	if (!b_ret) return false;

	m_safe_run = json_config["safe_run"].asString();
	m_work_dir = json_config["work_dir"].asString();
	
	if (m_safe_run.empty()) return false;
	if (m_work_dir.empty()) return false;
	return true;
}

bool GameJudger::initPlayer(GamePlayer& pGamePlayer, unsigned nPlayerID)
{
	CJsonValue json_config;
	bool b_ret = CJsonOperator::readFromString(json_config, m_game_config);
	if (!b_ret) return false;

	std::string safe_run_param;
	std::string safe_run_command;

	CJsonOperator::writeToString(safe_run_param, json_config["player"][nPlayerID]);
	BaseFunc::addBackSlash(safe_run_command, safe_run_param);
	safe_run_command = m_safe_run + " " + safe_run_command;

	b_ret = b_ret && pGamePlayer.initPlayer();
	b_ret = b_ret && pGamePlayer.createPlayer(safe_run_command.c_str(), m_work_dir.c_str());
	return b_ret;
}

bool GameJudger::saveResult(CJsonValue& jGameResult)
{
	return CJsonOperator::writeToFile("_JUDGE_RES.TMP", jGameResult, true);
}
