#ifndef _TEST_ALL_H_
#define _TEST_ALL_H_

#include "stdafx.h"
#include "LogFile.h"
#include "ConfigFile.h"
#include "BaseFunc.h"
#include "DBObject.h"
#include "ProcBase.h"
#include "SandBox.h"
#include "ThreadBase.h"
#include "MemPool.h"
#include "JsonObject.h"
//
//
//
//typedef struct _TestTask
//{
//	int a, b;
//	_TestTask() : a(0), b(0) {}
//} TestTask;
//
//class TestThread : public CTaskThread<TestTask*>
//{
//protected:
//	virtual bool procTask(TestTask* pTask) {
//		printf ("%d + %d = %d\n",
//			pTask->a, pTask->b, pTask->a+pTask->b);
//		return true;
//	}
//};
//
//
//void Test_MD5_LOG();
//void Test_INI();
//void Test_Pipe();
//void Test_DB();
//void Test_Thd_Mem();
//void TestJson();
//
//
//void ADD_MATCH(int p1, int p2);
#endif // _TEST_ALL_H_
