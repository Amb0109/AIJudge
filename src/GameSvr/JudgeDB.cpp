#include "stdafx.h"
#include "JudgeDB.h"
#include "DBObject.h"
#include "JudgeSys.h"

CDBObject* JudgeDB::getDBObj()
{
	static CDBObject gJudgeDB;
	return &gJudgeDB;
}

CSafeLock* JudgeDB::getLock()
{
	static CSafeLock gDBLock;
	return &gDBLock;
}

void JudgeDB::closeDataBase()
{
	getDBObj()->CloseDB();
}

bool JudgeDB::initDataBase()
{
	bool b_ret = true;
	CDBObject* p_db_obj = getDBObj();
	if(!p_db_obj->InitLibrary())
	{
		ADD_SERVER_LOG("%s: p_db_obj->InitLibrary() -- Fault", __FUNCTION__);
		assert(false);
		b_ret = false;
	}

	if (b_ret)
	{
		JudgeConfig* p_judge_config = JudgeConfig::getConfig();
		DatabaseKey db_key;
		db_key.sServer		= p_judge_config->g_database_server;
		db_key.sDBName		= p_judge_config->g_database_dbname;
		db_key.sUser		= p_judge_config->g_database_user;
		db_key.sPassword	= p_judge_config->g_database_pass;
		db_key.nPort		= p_judge_config->g_database_port;

		if( !p_db_obj->ConnectDB(&db_key) )
		{
			ADD_SERVER_LOG("%s: p_db_obj->ConnectDB(&db_key) -- Fault", __FUNCTION__);
			assert(false);
			b_ret = false;
		}
	}

	ADD_SERVER_LOG("%s -- %s", __FUNCTION__, b_ret?"OK":"FAULT");
	
	return b_ret;
}


bool JudgeDB::fetchNewSubmit(SubmitionInfo* siBody)
{
	CAutoLock _LOCK(getLock());

	const char* sql_query = JUDGE_CONFIG::SQL_FETCH_NEW_SUBMITION;

	CDBObject* p_judge_db = getDBObj();
	if( !p_judge_db->Query(sql_query) )
	{
		ADD_LOG("DBError %s, %d: %s", __FUNCTION__, __LINE__, p_judge_db->Error());
		assert(false);
		return false;
	}

	CDBObject::PQUERY_RESULT sql_res = NULL;
	sql_res = p_judge_db->StoreResult();
	if (sql_res == NULL)
		return false;

	bool b_ret = false;
	char** res_body = NULL;
	res_body = p_judge_db->FetchRow(sql_res);
	if( res_body == NULL)
		goto end;

	b_ret = true;
	memset(siBody, 0, sizeof(SubmitionInfo));
	siBody->RunID		= BaseFunc::strToInteger(res_body[0]);
	siBody->GameID		= BaseFunc::strToInteger(res_body[1]);
	siBody->UserID		= BaseFunc::strToInteger(res_body[2]);
	siBody->LanguageID	= (CodeLanguage)BaseFunc::strToInteger(res_body[3]);

end:
	p_judge_db->FreeResult(sql_res);
	p_judge_db->ClearResult();

	return b_ret;
}


//bool JudgeDB::fetchSubmit(int RunID, SubmitionInfo* siBody)
//{
//	bool b_ret = false;
//
//	char sql_query[256];
//	sprintf(sql_query, DEF_VAR::SQL_FETCH_SUBMITION_BY_RUNID, RunID);
//
//	CDBObject* p_judge_db = getDBObj();
//	if( !p_judge_db->Query(sql_query) )
//	{
//		ADD_LOG("DBError %s, %d: %s", __FUNCTION__, __LINE__, p_judge_db->Error());
//		return false;
//	}
//
//	CDBObject::PQUERY_RESULT sql_res = NULL;
//	sql_res = p_judge_db->StoreResult();
//	if (sql_res == NULL)
//		return false;
//
//	b_ret = false;
//	char** res_body = NULL;
//	res_body = p_judge_db->FetchRow(sql_res);
//	if( res_body == NULL) 
//		goto end;
//
//	sscanf(res_body[0], "%d", &(siBody->RunID));
//	sscanf(res_body[1], "%d", &(siBody->LanguageID));
//	siBody->SrcPath[0] = '\0';
//	siBody->BinPath[0] = '\0';
//	b_ret = true;
//
//end:
//	p_judge_db->FreeResult(sql_res);
//	p_judge_db->ClearResult();
//
//	return b_ret;
//}

bool JudgeDB::fetchNewMatch(MatchInfo* miBody)
{
	CAutoLock _LOCK(getLock());

	const char* sql_query = JUDGE_CONFIG::SQL_FETCH_NEW_MATCH;

	CDBObject* p_judge_db = getDBObj();
	if( !p_judge_db->Query(sql_query) )
	{
		ADD_LOG("DBError %s, %d: %s", __FUNCTION__, __LINE__, p_judge_db->Error());
		assert(false);
		return false;
	}

	CDBObject::PQUERY_RESULT sql_res = NULL;
	sql_res = p_judge_db->StoreResult();
	if (sql_res == NULL)
		return false;

	bool b_ret = false;
	char** res_body = NULL;
	res_body = p_judge_db->FetchRow(sql_res);
	if( res_body == NULL)
		goto end;

	b_ret = true;
	memset(miBody, 0, sizeof(MatchInfo));
	miBody->MatchID		= BaseFunc::strToInteger(res_body[0]);
	miBody->GameID		= BaseFunc::strToInteger(res_body[1]);
	miBody->PalyerCnt	= BaseFunc::strToInteger(res_body[2]);
	
	for (int i=0; i<miBody->PalyerCnt; ++i)
		miBody->Players[i]	= BaseFunc::strToInteger(res_body[i+3]);

end:
	p_judge_db->FreeResult(sql_res);
	p_judge_db->ClearResult();

	return b_ret;
}


bool JudgeDB::uploadMatchResult(int nMatchID,
								const char* sMatchResult)
{
	CAutoLock _LOCK(getLock());

	std::string sql_query;
	BaseFunc::strFormat(sql_query,
		JUDGE_CONFIG::SQL_UPLOAD_JUDGE_RESULT,
		nMatchID,
		sMatchResult);

	CDBObject* p_judge_db = getDBObj();
	if( !p_judge_db->Query(sql_query.c_str()) )
	{
		ADD_LOG("DBError %s, %d: %s", __FUNCTION__, __LINE__, p_judge_db->Error());
		assert(false);
		return false;
	}

	return true;
}


bool JudgeDB::saveSourceFile(const char* sCodePath, int nRunID)
{
	CAutoLock _LOCK(getLock());

	std::string sql_query;
	BaseFunc::strFormat(sql_query,
		JUDGE_CONFIG::SQL_FETCH_SUBMITION_CODE,
		nRunID
		);

	CDBObject* p_judge_db = getDBObj();
	if( !p_judge_db->Query(sql_query.c_str()) )
	{
		ADD_LOG("DBError %s, %d: %s", __FUNCTION__, __LINE__, p_judge_db->Error());
		assert(false);
		return false;
	}

	CDBObject::PQUERY_RESULT sql_res = NULL;
	sql_res = p_judge_db->StoreResult();
	if (sql_res == NULL)
		return false;

	bool b_ret = false;
	char** res_body = NULL;
	res_body = p_judge_db->FetchRow(sql_res);
	if( res_body == NULL)
		goto end;

	b_ret = true;
	FILE* p_file = fopen(sCodePath, "wb");
	if (p_file == NULL)
	{
		ADD_LOG("%s:%d Save File Fault!", __FUNCTION__, __LINE__);
		b_ret = false;
		goto end;
	}

	if( -1 == fputs(res_body[1], p_file))
		b_ret = false;

	fclose(p_file);

end:
	p_judge_db->FreeResult(sql_res);
	p_judge_db->ClearResult();

	return b_ret;
}

bool JudgeDB::uploadCompileResult(
				int nRunID,
				int nUserID,
				int nGameID,
				bool bCompileRes,
				const char* sCompileError)
{
	CAutoLock _LOCK(getLock());

	std::string sql_query;
	BaseFunc::strFormat(sql_query,
		JUDGE_CONFIG::SQL_UPLOAD_COMPILE_RESULT,
		nRunID,
		nUserID,
		nGameID,
		bCompileRes,
		sCompileError);

	CDBObject* p_judge_db = getDBObj();
	if( !p_judge_db->Query(sql_query.c_str()) )
	{
		ADD_LOG("DBError %s, %d: %s", __FUNCTION__, __LINE__, p_judge_db->Error());
		assert(false);
		return false;
	}

	return true;
}

bool JudgeDB::storePlayerExecFile(int nRunID,
								const char* sFilePath,
								const char* sFileMd5 )
{
	CAutoLock _LOCK(getLock());

	std::string sql_query;
	BaseFunc::strFormat(sql_query,
		JUDGE_CONFIG::SQL_STORE_PLAYER_EXEC_FILE,
		nRunID,
		sFilePath,
		sFileMd5);

	CDBObject* p_judge_db = getDBObj();
	if( !p_judge_db->Query(sql_query.c_str()) )
	{
		ADD_LOG("DBError %s, %d: %s", __FUNCTION__, __LINE__, p_judge_db->Error());
		assert(false);
		return false;
	}

	return true;
}

bool JudgeDB::getJudgerExecInfo(int nGameID, std::string& sPath, std::string& sMd5, std::string& sConfig)
{
	CAutoLock _LOCK(getLock());

	std::string sql_query;
	BaseFunc::strFormat(sql_query,
		JUDGE_CONFIG::SQL_GET_JUDGER_EXEC_INFO,
		nGameID
		);

	CDBObject* p_judge_db = getDBObj();
	if( !p_judge_db->Query(sql_query.c_str()) )
	{
		ADD_LOG("DBError %s, %d: %s", __FUNCTION__, __LINE__, p_judge_db->Error());
		assert(false);
		return false;
	}

	CDBObject::PQUERY_RESULT sql_res = NULL;
	sql_res = p_judge_db->StoreResult();
	if (sql_res == NULL)
		return false;

	bool b_ret = false;
	char** res_body = NULL;
	res_body = p_judge_db->FetchRow(sql_res);
	if( res_body == NULL)
		goto end;

	b_ret = true;
	sPath	= res_body[1];
	sMd5	= res_body[2];
	sConfig	= res_body[3];

end:
	p_judge_db->FreeResult(sql_res);
	p_judge_db->ClearResult();

	return b_ret;
}

bool JudgeDB::getPlayerExecInfo(int nRunID, std::string& sPath, std::string& sMd5)
{
	CAutoLock _LOCK(getLock());

	std::string sql_query;
	BaseFunc::strFormat(sql_query,
		JUDGE_CONFIG::SQL_GET_PLAYER_EXEC_INFO,
		nRunID
		);

	CDBObject* p_judge_db = getDBObj();
	if( !p_judge_db->Query(sql_query.c_str()) )
	{
		ADD_LOG("DBError %s, %d: %s", __FUNCTION__, __LINE__, p_judge_db->Error());
		assert(false);
		return false;
	}

	CDBObject::PQUERY_RESULT sql_res = NULL;
	sql_res = p_judge_db->StoreResult();
	if (sql_res == NULL)
		return false;

	bool b_ret = false;
	char** res_body = NULL;
	res_body = p_judge_db->FetchRow(sql_res);
	if( res_body == NULL)
		goto end;

	b_ret = true;
	sPath	= res_body[1];
	sMd5	= res_body[2];

end:
	p_judge_db->FreeResult(sql_res);
	p_judge_db->ClearResult();

	return b_ret;
}

bool JudgeDB::getEscapeString( std::string& sDstString, const std::string& sSrcString )
{
	CAutoLock _LOCK(getLock());
	
	CDBObject* p_judge_db = getDBObj();
	sDstString.clear();
	sDstString.resize(sSrcString.length() * 2 + 1);
	int _len = p_judge_db->getEscapeString(
		const_cast<char*>(sDstString.c_str()),
		sSrcString.c_str(), sSrcString.length());
	sDstString.resize(_len);
	return true;
}

