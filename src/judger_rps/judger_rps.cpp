#include "stdafx.h"
#include "JsonObject.h"
#include "GameJudger.h"
#include "GamePlayer.h"


enum RES_CODE
{
	JUDGE_INTERNAL_ERROR = -1,

	PLAYER_RUN_OK = 0,

	BLACK_INVALID_OUTPUT,
	BLACK_TIME_LIMITE_EXCEEDED,
	BLACK_MEMORY_LIMITE_EXCEEDED,
	BLACK_RUNTIME_ERROR,

	WHITE_INVALID_OUTPUT,
	WHITE_TIME_LIMITE_EXCEEDED,
	WHITE_MEMORY_LIMITE_EXCEEDED,
	WHITE_RUNTIME_ERROR
};

#define getRes(x,y) (((x)-(y)+3)%3)

static GameJudger	game_judger;
static GamePlayer	players[2];

static std::vector<std::string> game_res;
int black_score;
int white_score;

int		GameRun();
void	SaveRes(int nRet);

int		getStatusCode(int nJudgerCode, int nPlayerId);

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 2) return -1;

	puts(argv[1]);

	bool b_ret = true;
	b_ret = b_ret && game_judger.initConfig(argv[1]);
	b_ret = b_ret && game_judger.initPlayer(players[0], 0);
	b_ret = b_ret && game_judger.initPlayer(players[1], 1);
	if (!b_ret) return -1;

	black_score = 0;
	white_score = 0;
	game_res.clear();

	int n_ret = 0;
	n_ret = GameRun();
	SaveRes(n_ret);

	players[0].closePlayer();
	players[1].closePlayer();

	return 0;
}

const char* RPS[] = {
	"Rock",
	"Paper",
	"Scissors"
};

char buff[1024];
int GameRun()
{
	int _ret = 0;
	for (int i=0; i<7; i++)
	{
		_ret = players[0].sendMsg("Start");
		_ret = getStatusCode(_ret, 0);
		if (_ret != PLAYER_RUN_OK)
			return _ret;

		_ret = players[0].recvMsg(buff, 1024);
		_ret = getStatusCode(_ret, 0);
		if (_ret != PLAYER_RUN_OK)
			return _ret;

		int black_player = -1;
		for (int i=0; i<3; ++i)
		{
			if(strncmp(RPS[i], buff, sizeof(RPS[i])) == 0)
			{
				black_player = i;
				break;
			}
		}
		if (black_player == -1)
			return BLACK_INVALID_OUTPUT;

		_ret = players[1].sendMsg("Start");
		_ret = getStatusCode(_ret, 1);
		if (_ret != PLAYER_RUN_OK)
			return _ret;

		_ret = players[1].recvMsg(buff, 1024);
		_ret = getStatusCode(_ret, 1);
		if (_ret != PLAYER_RUN_OK)
			return _ret;

		int white_player = -1;
		for (int i=0; i<3; ++i)
		{
			if(strncmp(RPS[i], buff, sizeof(RPS[i])) == 0)
			{
				white_player = i;
				break;
			}
		}
		if (white_player == -1)
			return WHITE_INVALID_OUTPUT;
		
		switch (getRes(white_player,black_player))
		{
		case 1:
			black_score += 3;
			break;
		case 2:
			white_score += 3;
			break;
		case 0:
			black_score += 1;
			white_score += 1;
			break;
		}
		std::string step;
		step.insert(step.begin(), RPS[black_player][0]);
		step.insert(step.begin(), RPS[white_player][0]);
		game_res.push_back(step);
	}
	return PLAYER_RUN_OK;
}

void SaveRes(int nRet)
{
	CJsonValue	game_result;

	game_result["judge_result"] = (int)nRet;

	game_result["black"]	= black_score;
	game_result["white"]	= white_score;

	game_result["game_steps"] = (int)game_res.size();
	for (unsigned i=0; i<game_res.size(); i++)
		game_result["chess_manual"].append(game_res[i]);

	game_judger.saveResult(game_result);
}

int getStatusCode(int nJudgerCode, int nPlayerId)
{
	switch (nJudgerCode)
	{
	case GamePlayer::S_PLAYER_RUN_OK:
		return PLAYER_RUN_OK;
	case GamePlayer::S_TIME_LIMIT_EXCEEDED:
		return (!nPlayerId) ? BLACK_TIME_LIMITE_EXCEEDED : WHITE_TIME_LIMITE_EXCEEDED;
	case GamePlayer::S_MEMORY_LIMIT_EXCEEDED:
		return (!nPlayerId) ? BLACK_MEMORY_LIMITE_EXCEEDED : WHITE_MEMORY_LIMITE_EXCEEDED;
	case GamePlayer::S_PLAYER_RUNTIME_ERROR:
	case GamePlayer::S_PLAYER_SELF_CLOSED:
		return (!nPlayerId) ? BLACK_RUNTIME_ERROR : WHITE_RUNTIME_ERROR;
	default:
		return JUDGE_INTERNAL_ERROR;
	}
}
