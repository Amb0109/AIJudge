#ifndef _JUDGE_COMPILER_H_
#define _JUDGE_COMPILER_H_

#include "stdafx.h"
#include "JudgeDef.h"

class JudgeConfig;

class JudgeCompiler
{
public:
	JudgeCompiler();
	~JudgeCompiler() {}

public:
	void setWorkDir(const char* sWorkDir);
	bool doCompile(SubmitionInfo& siBody);

protected:
	bool readCompileError(const char* sErrFile, char* sBuff, int nBuffSize);
	bool initDataCache(SubmitionInfo& siBody);
	void clearDataCache();
	bool fillFileName();
	bool getCompileCommand();
	
	bool saveCodeFile();

	bool doCompileCode();
	bool getCompileError();
	bool uploadCompileResult();
	bool storePlayerExecFile();

private:
	JudgeConfig*	m_judge_config;

	std::string		m_work_dir;
	std::string		m_err_file;

	typedef enum _COMPILER_RESULT_ENUM {
		CR_NULL		= 0,
		CR_SUCCESS,
		CR_ERR_CE,
		CR_ERR_TIMEOUT,
		CR_ERR_UNKNOWN
	} COMPILE_RESULT;

	struct _COMPILER_DATA_CACHE
	{
		int				run_id;
		int				game_id;
		int				user_id;
		CodeLanguage		language;

		std::string		file_prefix;
		std::string		src_file;
		std::string		bin_file;
		std::string		compile_command;
	
		COMPILE_RESULT	compile_result;
		std::string		compile_error;
	} m_data_cache;

};

#endif // _JUDGE_COMPILER_H_