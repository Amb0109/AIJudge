#ifndef _JUDGE_DATABASE_H_
#define _JUDGE_DATABASE_H_
#include "stdafx.h"
#include "JudgeDef.h"
#include "DBObject.h"

class JudgeDB
{
public:
	static bool initDataBase();
	static void closeDataBase();

	static bool	fetchNewSubmit(SubmitionInfo* siBody);
//	static bool fetchSubmit(int RunID, SubmitionInfo* siBody);

	static bool uploadCompileResult(int nRunID,
									int nUserID,
									int nGameID,
									bool bCompileRes,
									const char* sCompileError);

	static bool storePlayerExecFile(int nRunID,
									const char* sFilePath,
									const char* sFileMd5);


	static bool	fetchNewMatch(MatchInfo* miBody);
//	static bool	fetchMatch(int RunID, MatchInfo* miBody);
	static bool uploadMatchResult(	int nMatchID,
									const char* sMatchResult);

	static bool	getJudgerExecInfo(int nGameID, std::string& sPath, std::string& sMd5, std::string& sConfig);
	static bool	getPlayerExecInfo(int nRunID, std::string& sPath, std::string& sMd5);

	static bool saveSourceFile(const char* sCodePath, int RunID);

	static bool getEscapeString(std::string& sDstString, const std::string& sSrcString);

protected:
	static CDBObject* getDBObj();
	static CSafeLock* getLock();
};

#endif // _JUDGE_DATABASE_H_