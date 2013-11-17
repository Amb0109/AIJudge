#include "stdafx.h"
#include "ProcKeeper.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 2)
		return -1;

	ProcKeeper proc_keeper;
	if (!proc_keeper.init(argv[1]))
		return -1;

	if (!proc_keeper.createProcess())
		return -1;
	
	return proc_keeper.runAndMonitor();
}

