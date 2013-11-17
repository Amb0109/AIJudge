#ifndef _GAMELOGIC_OTHELLO_H_
#define _GAMELOGIC_OTHELLO_H_

#include "stdafx.h"

class OthelloLogic
{
public:
	typedef int								CHESS_TYPE;
	typedef std::vector<std::string>		REPALY_VEC;
	typedef std::vector<int>				CHESS_POS_VEC;

	static const int N_BOARD = 8;

	static const CHESS_TYPE C_EMPTY = 0;
	static const CHESS_TYPE C_BLACK = 1;
	static const CHESS_TYPE C_WHITE = 2;

	static const int DIR_X[8];
	static const int DIR_Y[8];

public:
	OthelloLogic ();
	~OthelloLogic ();

public:
	void		initGame();
	bool		putChess(int x, int y, CHESS_TYPE c_flg);

	CHESS_TYPE	antiC(CHESS_TYPE c_flg);
	bool		inBoard(int x, int y);
	bool		isValid(CHESS_TYPE c_flg);
	int			markValids(CHESS_TYPE c_flg);
	bool		isOver();
	void		saveStepMove(int x, int y, CHESS_TYPE c_flg);
	REPALY_VEC&	getReplay();

	void		asciiBoard();

	int			getBlackCnt();
	int			getWhiteCnt();

private:
	CHESS_TYPE		_board[N_BOARD][N_BOARD];
	REPALY_VEC		m_replay;
	CHESS_POS_VEC	m_next_list;

	int				n_black_chess;
	int				n_white_chess;
	int				n_empty;
};

#endif // _GAMELOGIC_OTHELLO_H_
