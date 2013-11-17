#ifndef _GAME_PLAYER_API_H_
#define _GAME_PLAYER_API_H_

#include "stdafx.h"
#include "ProcBase.h"

class GameJudger;
class GamePlayer
{
	friend GameJudger;
public:
	enum
	{
		S_PLAYER_RUN_OK = 0,

		S_TIME_LIMIT_EXCEEDED,
		S_MEMORY_LIMIT_EXCEEDED,
		S_PLAYER_SELF_CLOSED,
		S_PLAYER_RUNTIME_ERROR
	};

public:
	GamePlayer() {}
	~GamePlayer() {}

public:
	virtual int		sendMsg(const char* sMsg);
	virtual int		recvMsg(char* sMsgBuff, int nBuffSize);
	virtual void	closePlayer();

protected:
	virtual bool	initPlayer();
	virtual bool	createPlayer(const char* sProcCmd, const char* sProcWorkDir = NULL);

	virtual int		getStatus();

private:
	CPipeProc		m_process;
};

#endif // _GAME_PLAYER_API_H_