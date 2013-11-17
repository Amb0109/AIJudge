#include "stdafx.h"
#include "ForTest.h"
//
//static char sTestStr[][64] = {
//	"",
//	"a",
//	"abc",
//	"message digest",
//	"abcdefghijklmnopqrstuvwxyz",
//	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
//
//};
//
//static CLogFile Log("test.log");
//static CConfigFile Config("test.ini");
//static CMemPool<TestTask> TaskMemPool(16);
//
//void Test_MD5_LOG()
//{
//	char sMD5Digest[64];
//
//	for (int i=0; i<6; i++)
//	{
//		BaseFunc::MD5String(sTestStr[i], sMD5Digest);
//		//Log.Write("[MD5] %s : %s", sTestStr[i], sMD5Digest);
//	}
//}
//
//void Test_INI()
//{
//	//Log.Write("[TEST] test %d", 0);
//	Config.setStrValue("TEST", "test_key", "test_val");
//}
//
//void Test_Pipe()
//{
//	CPipeProc sp;
//	sp.Create("python test.py");
//	sp.Run();
//	char msg[] = "123!!!@#$%\n"; 
//	sp.Send(msg, strlen(msg));
//	char str[1000];
//	int sz = sp.Recv(str, sizeof(str));
//	if (sz != ERRSIZE)
//	{
//		str[sz] = 0;
//		printf("%d %s\n", sz, str);
//	}
//}
//
//void Test_Thd_Mem()
//{
//	TestThread thd;
//	thd.Create();
//
//	TestTask* p_task = TaskMemPool.RequestBlock();
//	p_task->a = 100;
//	p_task->b = 123;
//
//	thd.addTask(p_task);
//
//	p_task = TaskMemPool.RequestBlock();
//	p_task->a = 12345;
//	p_task->b = 654321;
//
//	thd.addTask(p_task);
//
//	Sleep(10000);
//
//	thd.Close(10000);
//}
//
//void Test_DB()
//{
//	bool b_ret = true;
//	CDBObject db;
//	b_ret = b_ret && db.InitLibrary();
//
//	DATABASE_KEY db_key = {0};
//	db_key.sServer = "localhost";
//	db_key.sDBName = "botcraft";
//	db_key.sUser = "root";
//	db_key.sPassword = "admin";
//	db_key.nPort = 3306;
//
//	b_ret = b_ret && db.ConnectDB(&db_key);
//
//	//char* query = "INSERT INTO test_table (test_val) VALUES (\'HELLO\')";
//	//b_ret = db.Query(query);
//
//	//int i_ret;
//	//char* query2 = "SELECT * FROM test_table";
//	//CDBObject::PQUERY_RESULT p_query_res;
//	//i_ret = db.QueryRes(query2, p_query_res);
//
//	//for (int i=0; i<i_ret; ++i) {
//	//	char** row = NULL;
//	//	row = db.Fetch(p_query_res);
//	//	printf("%s: %s\n", row[0], row[1]);
//	//}
//
//	//db.CloseQuery(p_query_res);
//
//	db.CloseDB();
//}
//
//void TestJson()
//{
//	CJsonValue json;
//	CJsonOperator::readFromFile(json, "test.json");
//	
//	std::string str1 = json["encoding"].asString();
//	int int1 = json["indent"]["length"].asInt();
//	int int2 = json["indent"]["use_space"].asInt();
//
//
//	CJsonOperator::writeToFile("test_2.json", json);
//
//	CJsonOperator::writeToString(str1, json);
//}
//
//void ADD_MATCH(int p1, int p2)
//{
//	bool b_ret = true;
//	CDBObject db;
//	b_ret = b_ret && db.InitLibrary();
//
//	DATABASE_KEY db_key = {0};
//	db_key.sServer = "localhost";
//	db_key.sDBName = "game_svr";
//	db_key.sUser = "root";
//	db_key.sPassword = "admin";
//	db_key.nPort = 3306;
//
//	b_ret = b_ret && db.ConnectDB(&db_key);
//
//	const char* queryf = "INSERT INTO gs_match (game_id, match_body) VALUES (1000, \'{\"p1\":%d,\"p2\":%d}\')";
//	char query[1024];
//	sprintf(query, queryf, p1, p2);
//	puts(query);
//	b_ret = db.Query(query);
//	db.CloseDB();
//}
