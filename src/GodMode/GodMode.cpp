#include "stdafx.h"
#include "ForTest.h"
#include "SandBox.h"
#include "ProcBase.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//CProcBase proc;
	//proc.Create("python");

	//CSandBox sandbox;
	//sandbox.init();
	//sandbox.setLimit(1000, 20*1024*1024);
	//sandbox.assignProcess(proc);

	//proc.Run(0);
	//
	//while (true)
	//{
	//	int ret = sandbox.getStatus(10);
	//	printf("%d\n", ret);
	//	printf("%d %d ec: %u\n",
	//		proc.getTime(),
	//		proc.getMemory(),
	//		proc.GetExitCode());
	//}

	std::string hehe, hehe2;
	BaseFunc::getFileContent(hehe, "gbk.txt");
	BaseFunc::ConvertGBKToUtf8(hehe2, hehe.c_str());
	FILE* pfile = fopen("utf8.txt", "wb");
	fwrite(hehe2.c_str(), 1, hehe2.size(), pfile);
	fclose(pfile);


	//int cmd;

	//while(~scanf("%d", &cmd))
	//{
	//	switch (cmd)
	//	{
	//	case 1:
	//		Test_MD5_LOG();
	//		break;
	//	case 2:
	//		Test_INI();
	//		break;
	//	case 3:
	//		Test_Pipe();
	//		break;
	//	case 4:
	//		Test_Thd_Mem();
	//		break;
	//	case 5:
	//		Test_DB();
	//		break;
	//	case 6:
	//		TestJson();
	//		break;
	//	case 11:
	//		int p1, p2;
	//		printf(":");
	//		scanf("%d%d", &p1, &p2);
	//		ADD_MATCH(p1, p2);
	//		break;
	//	default:
	//		Test_MD5_LOG();
	//		Test_INI();
	//		Test_Pipe();
	//		Test_Thd_Mem();
	//		Test_DB();
	//		break;
	//	}
	//}

	return 0;
}
