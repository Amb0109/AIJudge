#ifndef _GAME_JUDGER_API_H_
#define _GAME_JUDGER_API_H_

#include "stdafx.h"
#include "GamePlayer.h"
#include "JsonObject.h"

class GameJudger
{
public:
	GameJudger() {}
	~GameJudger() {}

public:
	virtual bool initConfig(const char* sGameConfig);
	virtual bool initPlayer(GamePlayer& pGamePlayer, unsigned nPlayerID);
	virtual bool saveResult(CJsonValue& jGameResult);

protected:
	std::string		m_game_config;

	std::string		m_safe_run;
	std::string		m_work_dir;
};

#endif // _GAME_JUDGER_API_H_
