#include "stdafx.h"
#include "Othello.h"
#include "JsonObject.h"
#include "GameJudger.h"
#include "GamePlayer.h"

#define getCType(_turn) (_turn?OthelloLogic::C_WHITE:OthelloLogic::C_BLACK)

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

static GameJudger	game_judger;
static GamePlayer	players[2];
static OthelloLogic	othello_game;

int		getStatusCode(int nJudgerCode, OthelloLogic::CHESS_TYPE cFlag);

int		GameRun();
void	SaveRes(int nRet);

int main(int argc, char* argv[])
{
	if (argc < 2) return -1;

	puts(argv[1]);

	bool b_ret = true;
	b_ret = b_ret && game_judger.initConfig(argv[1]);
	b_ret = b_ret && game_judger.initPlayer(players[0], 0);
	b_ret = b_ret && game_judger.initPlayer(players[1], 1);
	if (!b_ret) return -1;

	othello_game.initGame();

	int n_ret = 0;
	n_ret = GameRun();
	SaveRes(n_ret);

	players[0].closePlayer();
	players[1].closePlayer();

	return 0;
}

char buff[1024];
int GameRun()
{
	players[0].sendMsg("first");
	players[1].sendMsg("second");

	int x, y, _turn = 0;
	int _ret;
	while(true)
	{
		_ret = players[_turn].recvMsg(buff, 1024);
		_ret = getStatusCode(_ret, getCType(_turn));
		if (_ret != PLAYER_RUN_OK)
			return _ret;

		if( 2 != sscanf(buff, "%d %d", &x, &y) )
			return _turn?WHITE_INVALID_OUTPUT:BLACK_INVALID_OUTPUT;

		if(!othello_game.putChess(x, y, getCType(_turn)))
			return _turn?WHITE_INVALID_OUTPUT:BLACK_INVALID_OUTPUT;

		puts(buff);
		othello_game.asciiBoard();

		if (othello_game.isOver())
			break;

		_turn ^= 1;
		_ret = players[_turn].sendMsg(buff);
		_ret = getStatusCode(_ret, getCType(_turn));
		if (_ret != PLAYER_RUN_OK)
			return _ret;
	}
	
	return PLAYER_RUN_OK;
}

void SaveRes(int nRet)
{
	CJsonValue	game_result;

	game_result["judge_result"] = (int)nRet;

	game_result["black"]	= (int)othello_game.getBlackCnt();
	game_result["white"]	= (int)othello_game.getWhiteCnt();

	OthelloLogic::REPALY_VEC& repaly_vec = othello_game.getReplay();
	game_result["game_steps"] = (int)repaly_vec.size();
	for (unsigned i=0; i<repaly_vec.size(); i++)
		game_result["chess_manual"].append(repaly_vec[i]);

	game_judger.saveResult(game_result);
}

int getStatusCode(int nJudgerCode, OthelloLogic::CHESS_TYPE cFlag)
{
	switch (nJudgerCode)
	{
	case GamePlayer::S_PLAYER_RUN_OK:
		return PLAYER_RUN_OK;
	case GamePlayer::S_TIME_LIMIT_EXCEEDED:
		return (cFlag == OthelloLogic::C_BLACK) ?
							BLACK_TIME_LIMITE_EXCEEDED : WHITE_TIME_LIMITE_EXCEEDED;
	case GamePlayer::S_MEMORY_LIMIT_EXCEEDED:
		return (cFlag == OthelloLogic::C_BLACK) ?
							BLACK_MEMORY_LIMITE_EXCEEDED : WHITE_MEMORY_LIMITE_EXCEEDED;
	case GamePlayer::S_PLAYER_RUNTIME_ERROR:
	case GamePlayer::S_PLAYER_SELF_CLOSED:
		return (cFlag == OthelloLogic::C_BLACK) ?
							BLACK_RUNTIME_ERROR : WHITE_RUNTIME_ERROR;
	default:
		return JUDGE_INTERNAL_ERROR;
	}
}
