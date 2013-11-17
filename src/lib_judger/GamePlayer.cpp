#include "stdafx.h"
#include "GamePlayer.h"
#include "SandBox.h"

bool GamePlayer::initPlayer()
{
	bool b_ret = true;
	b_ret = b_ret && m_process.initPipe();
	b_ret = b_ret && m_process.setInputPipe();
	b_ret = b_ret && m_process.setOutputPipe();
	return b_ret;
}

bool GamePlayer::createPlayer(const char* sProcCmd, const char* sProcWorkDir/*= NULL*/)
{
	bool b_ret = true;
	b_ret = b_ret && m_process.getProcess().create(sProcCmd, sProcWorkDir);
	b_ret = b_ret && (-1 != m_process.getProcess().run(0));
	return b_ret;
}

int GamePlayer::sendMsg(const char* sMsg)
{
	std::string _msg;
	_msg = sMsg;
	int len = _msg.length();
	if (len <= 0 || sMsg[len-1] != '\n')
		_msg.append("\n");

	int ret_len = m_process.send(_msg.c_str(), _msg.length());
	if (ret_len != _msg.length())
	{
		int n_ret = getStatus();
		if (n_ret == S_PLAYER_RUN_OK)
			return S_PLAYER_RUNTIME_ERROR;
		else
			return n_ret;
	}
	else
		return S_PLAYER_RUN_OK;
}

int GamePlayer::recvMsg( char* sMsgBuff, int nBuffSize )
{
	if(m_process.recv(sMsgBuff, nBuffSize) < 0)
	{
		int n_ret = getStatus();
		if (n_ret == S_PLAYER_RUN_OK)
			return S_PLAYER_RUNTIME_ERROR;
		else
			return n_ret;
	}
	else
	{
		int n_ret = getStatus();
		if (n_ret != S_PLAYER_RUN_OK)
			return n_ret;
	}
	return S_PLAYER_RUN_OK;
}

int GamePlayer::getStatus()
{
	int n_ret = m_process.getProcess().getExitCode();
	switch (n_ret)
	{
	case CSandBox::PROC_STATUS_TIME_LIMITED:
		return S_TIME_LIMIT_EXCEEDED;
	case CSandBox::PROC_STATUS_MEMORY_LIMITED:
		return S_MEMORY_LIMIT_EXCEEDED;
	case CSandBox::PROC_STATUS_RUNTIME_ERROR:
	case CSandBox::PROC_STATUS_ERROR_STATUS:
		return S_PLAYER_RUNTIME_ERROR;
	case CSandBox::PROC_STATUS_PROC_EXIT:
		return S_PLAYER_SELF_CLOSED;
	default:
		break;
	}
	return S_PLAYER_RUN_OK;
}

void GamePlayer::closePlayer()
{
	m_process.getProcess().terminate();
}
