#ifndef _JUDGE_KEEPER_H_
#define _JUDGE_KEEPER_H_

#include "stdafx.h"
#include "JudgeDef.h"

class JudgeConfig;

class JudgeKeeper
{
public:
	JudgeKeeper();
	~JudgeKeeper();

public:
	bool doJudge(MatchInfo& miBody);
	void setWorkDir(const char* sWorkDir);

protected:
	bool initDataCache(MatchInfo& miBody);
	void clearDataCache();
	bool getJudgeParam();
	bool prepareJudgeFiles();
	bool runGameJudger();
	bool uploadMatchResult(bool& bJudgeRes);

private:
	JudgeConfig*	m_judge_config;

	std::string		m_safe_run;
	std::string		m_work_dir;
	std::string		m_jres_file;
	std::string		m_judger_program;
	std::string		m_judge_error;

private:
	typedef struct _PLAYER_INFO_STRUCT
	{
		int				player_id;
		std::string		file_path;
		std::string		file_md5;
	} PlayerInfo;

	struct _JUDGER_DATA_CACHE
	{
		int				match_id;
		std::string		judge_res;

		// judger
		int				game_id;
		std::string		judger_path;
		std::string		judger_md5;
		std::string		judger_param;
		std::string		judger_safe_param;
		std::string		judger_cmd;
		std::string		judger_config;

		// players
		int				player_cnt;
		PlayerInfo		player_info [JUDGE_CONFIG::_MAX_PLAYER_CNT];
	} m_data_cache;
};

#endif // _JUDGE_KEEPER_H_