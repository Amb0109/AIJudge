#include "stdafx.h"
#include "Othello.h"
#include "BaseFunc.h"

const int OthelloLogic::DIR_X[8] = {1, 1, 1, 0, 0, -1, -1, -1};
const int OthelloLogic::DIR_Y[8] = {0, -1, 1, -1, 1, 0, -1, 1};

OthelloLogic::OthelloLogic()
{
	memset(_board, C_EMPTY, sizeof(_board));
	n_black_chess = 0;
	n_white_chess = 0;
	n_empty = 0;
}

OthelloLogic::~OthelloLogic()
{
}

void OthelloLogic::initGame()
{
	for (int ix = 0 ; ix < N_BOARD; ++ix)
	{
		for (int iy = 0; iy < N_BOARD; ++iy)
		{
			_board [ix][iy] = C_EMPTY;
		}
	}
	_board [3][3] = _board [4][4] = C_WHITE;
	_board [3][4] = _board [4][3] = C_BLACK;

	n_black_chess = 2;
	n_white_chess = 2;
	n_empty = N_BOARD * N_BOARD - n_white_chess - n_black_chess;

	m_replay.clear();
}

bool OthelloLogic::inBoard(int x, int y)
{
	if ( x<0 || x >= N_BOARD ) return false;
	if ( y<0 || y >= N_BOARD ) return false;
	return true;
}

OthelloLogic::CHESS_TYPE OthelloLogic::antiC(CHESS_TYPE c_flg)
{
	return (3-c_flg);
}

bool OthelloLogic::putChess(int x, int y, CHESS_TYPE c_flg)
{
	if ( c_flg != C_BLACK && c_flg != C_WHITE ) return false;
	if ( (x == -1 || y == -1) && !isValid(c_flg) ) goto save_move;
	if ( !inBoard(x, y) ) return false;
	if ( _board[x][y] != C_EMPTY ) return false;

	bool bFlip = false;
	for (int dir = 0; dir < 8; ++dir)
	{
		int tx = x+DIR_X[dir];
		int ty = y+DIR_Y[dir];
		if ( !inBoard(tx, ty) ) continue;
		if ( _board[tx][ty] != antiC(c_flg) ) continue;

		while ( inBoard(tx, ty) && _board[tx][ty] == antiC(c_flg) )
		{
			tx += DIR_X[dir];
			ty += DIR_Y[dir];
		}

		if ( inBoard(tx, ty) && _board[tx][ty] == c_flg)
		{
			bFlip = true;
			tx -= DIR_X[dir];
			ty -= DIR_Y[dir];
			while (inBoard(tx, ty) && _board[tx][ty] == antiC(c_flg) )
			{
				if( c_flg == C_BLACK )
				{
					++n_black_chess;
					--n_white_chess;
				}
				else
				{
					++n_white_chess;
					--n_black_chess;
				}
				_board[tx][ty] = c_flg;
				tx -= DIR_X[dir];
				ty -= DIR_Y[dir];
			}
		}
	}

	if (!bFlip) return false;
	_board[x][y] = c_flg;

save_move:
	// 记录棋谱
	saveStepMove(x, y, c_flg);
	return true;
}

void OthelloLogic::saveStepMove(int x, int y, CHESS_TYPE c_flg)
{
	// 记录棋谱

	std::string this_step;
	if (x == -1 || y == -1)
	{
		this_step = "XFF";
	}
	else
	{
		--n_empty;
		c_flg == C_BLACK ? (++n_black_chess) : (++n_white_chess) ;

		BaseFunc::strFormat(this_step, "%s%X%X",
			c_flg == C_BLACK?"B":"W", x, y);

	}
	for (int i=0; i<N_BOARD; ++i)
	{
		for (int j=0; j<N_BOARD; ++j)
		{
			switch (_board[i][j])
			{
			case C_EMPTY:
				this_step.append(".");
				break;
			case C_BLACK:
				this_step.append("B");
				break;
			case C_WHITE:
				this_step.append("W");
				break;
			}
		}
	}
	
	markValids(antiC(c_flg));
	for (int i=0; i<m_next_list.size(); ++i)
	{
		if (this_step[m_next_list[i]+3] == '.')
			this_step[m_next_list[i]+3] = '*';
	}

	m_replay.push_back(this_step);
}

bool OthelloLogic::isValid(CHESS_TYPE c_flg)
{
	for (int ix = 0; ix < N_BOARD; ++ix)
	{
		for (int iy = 0; iy < N_BOARD; ++iy)
		{
			if ( _board[ix][iy] != C_EMPTY ) continue;

			for (int dir = 0; dir < 8; ++dir)
			{
				int tx = ix+DIR_X[dir];
				int ty = iy+DIR_Y[dir];
				if ( !inBoard(tx, ty) ) continue;
				if ( _board[tx][ty] != antiC(c_flg) ) continue;
				while ( inBoard(tx, ty) && _board[tx][ty] == antiC(c_flg) )
				{
					tx += DIR_X[dir];
					ty += DIR_Y[dir];
				}
				if ( inBoard(tx, ty) && _board[tx][ty] == c_flg)
				{
					return true;
				}
			}
		}
	}
	return false;
}

int OthelloLogic::markValids(CHESS_TYPE c_flg)
{
	m_next_list.clear();
	for (int ix = 0; ix < N_BOARD; ++ix)
	{
		for (int iy = 0; iy < N_BOARD; ++iy)
		{
			if ( _board[ix][iy] != C_EMPTY ) continue;

			for (int dir = 0; dir < 8; ++dir)
			{
				int tx = ix+DIR_X[dir];
				int ty = iy+DIR_Y[dir];
				if ( !inBoard(tx, ty) ) continue;
				if ( _board[tx][ty] != antiC(c_flg) ) continue;
				while ( inBoard(tx, ty) && _board[tx][ty] == antiC(c_flg) )
				{
					tx += DIR_X[dir];
					ty += DIR_Y[dir];
				}
				if ( inBoard(tx, ty) && _board[tx][ty] == c_flg)
				{
					m_next_list.push_back(ix * N_BOARD + iy);
					break;
				}
			}
		}
	}
	return m_next_list.size();
}

bool OthelloLogic::isOver()
{
	//n_black_chess = 0;
	//n_white_chess = 0;
	//n_empty = 0;

	//for (int x = 0; x < N_BOARD; ++x)
	//{
	//	for (int y = 0; y < N_BOARD; ++y)
	//	{
	//		if ( _board[x][y] == C_BLACK ) n_black_chess ++;
	//		if ( _board[x][y] == C_WHITE ) n_white_chess ++;
	//		if ( _board[x][y] == C_EMPTY ) n_empty ++;
	//	}
	//}

	if ( n_black_chess == 0 ) return true;
	if ( n_white_chess == 0 ) return true;
	if ( n_empty == 0 ) return true;

	if ( isValid(C_BLACK) ) return false;
	if ( isValid(C_WHITE) ) return false;

	return true;
}

void OthelloLogic::asciiBoard()
{
	for (int x = 0; x < N_BOARD; ++x)
	{
		for (int y = 0; y < N_BOARD; ++y)
		{
			switch (_board[x][y])
			{
			case C_EMPTY:
				putchar('.');
				break;
			case C_BLACK:
				putchar('B');
				break;
			case C_WHITE:
				putchar('W');
				break;
			}
		}
		puts("");
	}
	puts("");
}

OthelloLogic::REPALY_VEC& OthelloLogic::getReplay()
{
	return m_replay;
}

int OthelloLogic::getBlackCnt()
{
	return n_black_chess;
}

int OthelloLogic::getWhiteCnt()
{
	return n_white_chess;
}
