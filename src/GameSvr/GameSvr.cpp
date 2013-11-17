#include "stdafx.h"

#ifdef _MAKE_SERVICE
#include "ServiceBase.h"
#include "ServiceCtrl.h"
#endif

#include "JudgeSys.h"

bool doInit();
void doDestory();

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _MAKE_SERVICE

	if (argc >= 2)
	{
		bool b_ret;
		ServiceCtrl svr_ctrl;
		svr_ctrl.OpenManager();
		if (strcmp(argv[1], "install") == 0)
		{
			char module_file[MAX_PATH];
			BaseFunc::getModuleFilePath(module_file);
			b_ret = svr_ctrl.installService(
				JUDGE_CONFIG::_SERVICE_NAME,
				JUDGE_CONFIG::_SERVICE_DISPLAY_NAME,
				module_file);
		}
		else if (strcmp(argv[1], "remove") == 0)
		{
			b_ret = svr_ctrl.removeService(JUDGE_CONFIG::_SERVICE_NAME);
		}
		else if (strcmp(argv[1], "start") == 0)
		{
			b_ret = svr_ctrl.startService(JUDGE_CONFIG::_SERVICE_NAME);
		}
		else if (strcmp(argv[1], "stop") == 0)
		{
			b_ret = svr_ctrl.stopService(JUDGE_CONFIG::_SERVICE_NAME);
		}
		printf("RESULT: %s:%d\n", b_ret?"OK":"FAULT", BaseFunc::getSysError());
		return 0;
	}

	ServiceBase* p_service = ServiceBase::getInstance();
	p_service->setServerInitFunc(doInit);
	p_service->setServerStopFunc(doDestory);
	p_service->initService(JUDGE_CONFIG::_SERVICE_NAME);
#else
	if( !doInit() )
		return false;

	getchar();

	doDestory();
#endif
	return 0;
}

bool doInit()
{
	bool b_ret = true;

#ifdef _MAKE_SERVICE
	// WIN服务会把目录设置到 windows/system32 下
	// 这里把目录改为Module文件所在目录
	char file_dir[MAX_PATH];
	BaseFunc::getCurrentDirectory(file_dir);
	BaseFunc::setCurrentDirectory(file_dir);
#endif

	b_ret = b_ret && JudgeDB::initDataBase();
	b_ret = b_ret && JudgeSys::initJudgeSys();
	return b_ret;
}

void doDestory()
{
	ADD_SERVER_LOG("%s\n", __FUNCTION__);
	JudgeSys::CloseJudgeSys();
	JudgeDB::closeDataBase();
}
