#ifndef _JUDGE_DEFINE_H_
#define _JUDGE_DEFINE_H_

#include "stdafx.h"

namespace JUDGE_CONFIG
{
// ...
static const char*	_SERVICE_NAME				= "GameJudgeSvr";
static const char*	_SERVICE_DISPLAY_NAME		= "HDOJ Game Judge Server";
static const char*	_JUDGE_CONFIG_PATH			= "config.ini";
static const char*	_JUDGE_LOG_PATH				= "judge.log";
static const char*	_SERVER_LOG_PATH			= "server.log";

static const int	_MAX_PLAYER_CNT				= 6;

static const char*	_JDUGER_FILE_NAME			= "_JUDGER.EXE";
static const char*	_COMPILE_RESULT_TMP_FILE	= "_COMPILE_RES.TMP";
static const char*	_JUDGE_RESULT_TMP_FILE		= "_JUDGE_RES.TMP";

// compiler command line
static const char*	CMDLINE_GUNC_COMPILE_FMT	= "%s \"%s\" -o \"%s\" -ansi -O2 -w -lm -DONLINE_JUDGE --static";
static const char*	CMDLINE_MSVC_COMPILE_FMT	= "%s /O1 /ML /EHsc /nologo /w /D \"ONLINE_JUDGE\" /INCREMENTAL:NO \"%s\" /Fe\"%s\" /Fo\"/_tmp/\\\" /link /SUBSYSTEM:CONSOLE kernel32.lib";
static const char*	CMDLINE_PAS_COMPILE_FMT		= "%s -CC -W -H -Q -$M- -$I+ -$O+ -DONLINE_JUDGE -E\"%s\" \"%s\"";
static const char*	CMDLINE_JAVA_COMPILE_FMT	= "%s -g:none -Xlint:none -O -d \"%s\" \"%s\"";

// sql querys
static const char*	SQL_FETCH_NEW_SUBMITION		= "CALL fetch_new_submition()";
static const char*	SQL_FETCH_NEW_MATCH			= "CALL fetch_new_match()";
static const char*	SQL_UPLOAD_COMPILE_RESULT	= "CALL upload_compile_result(%d,%d,%d,%d,\'%s\')";
static const char*	SQL_UPLOAD_JUDGE_RESULT		= "CALL upload_match_result(%d,\'%s\')";
static const char*	SQL_STORE_PLAYER_EXEC_FILE	= "CALL store_player_exec_file(%d,\'%s\',\'%s\')";
static const char*	SQL_FETCH_SUBMITION_CODE	= "SELECT * FROM web_source WHERE runId = %d";
static const char*	SQL_GET_PLAYER_EXEC_INFO	= "SELECT * FROM file_player_exec WHERE runId = %d";
static const char*	SQL_GET_JUDGER_EXEC_INFO	= "SELECT * FROM file_judger_exec WHERE gameId = %d";
} // namespace JUDGE_CONFIG


namespace DEFAULT_CONFIG
{
// System
static const int	DEFAULT_COMPILER_THREAD_CNT		= 3;			// 编译线程数
static const int	DEFAULT_JUDGER_THREAD_CNT		= 3;			// 评测线程数
static const int	DEFAULT_COMPILER_TIMEOUT		= 20000;		// 编译超时时间
static const int	DEFAULT_MEMPOOL_PAGE_SIZE		= 16;			// 内存池页面大小

static const char*	DEFAULT_JUDGE_CACHE_DIRECTORY	= "_cache/";	// 评测缓存区
static const char*	DEFAULT_JUDGE_LOG_DIRECTORY		= "_logs/";		// 评测log存放目录
static const char*	DEFAULT_PLAYER_EXEC_DIRECTORY	= "_bin/";		// AI程序仓库
static const char*	DEFAULT_JUDGER_EXEC_DIRECTORY	= "_judger/";	// 评测程序存放目录
static const char*	DEFAULT_SAFE_RUN_PATH			= "SafeRun.exe"; // SafeRun位置

// Compiler 各编译器指令
static const char*	DEFAULT_GPP_COMPILER_APP	= "g++";
static const char*	DEFAULT_GCC_COMPILER_APP	= "gcc";
static const char*	DEFAULT_MSVC_COMPILER_APP	= "c++";

// Database 数据库信息
static const char*	DEFAULT_DATABASE_SERVER		= "localhost";
static const char*	DEFAULT_DATABASE_DBNAME		= "ai_oj";
static const char*	DEFAULT_DATABASE_USER		= "root";
static const char*	DEFAULT_DATABASE_PASS		= "admin";
static const int	DEFAULT_DATABASE_PORT		= 3306;

} // namespace DEFAULT_CONFIG


typedef enum _CODE_LANGUAGE_ENUM
{
	L_NULL = -1,

	L_GPP,
	L_GCC,
	L_CPP,
	L_C,
	L_JAVA,
	L_PASCAL
} CodeLanguage;

typedef struct _SUBMITION_INFO_STRUCT
{
	int			RunID;
	int			GameID;
	int			UserID;
	CodeLanguage	LanguageID;
} SubmitionInfo;

typedef struct _MATCH_INFO_STRUCT
{
	int			MatchID;
	int			GameID;

	int			PalyerCnt;
	int			Players[JUDGE_CONFIG::_MAX_PLAYER_CNT];
} MatchInfo;

#endif // _JUDGE_DEFINE_H_