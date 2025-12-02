#include "GlobalProgramUtilities.h"

const int projectRes[] = { 1920,1080 };

int getProjectRes(int n)
{
	if (n == 0)
		return projectRes[0];
	return projectRes[1];
}
