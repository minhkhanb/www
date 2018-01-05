#include "pch.h"
#include "../../GameCore/CGame.h"

int Design_Util::getLevlFromDes(float distance)
{
	int res = 0;
	for (int i = 0; i < TOTAL_LVL_DES_ROW; i++)
	{
		if (distance >= GET_DES().getLevelDes(i, LVL_DES_DISTANCE))
		{
			res = i+1;
		}
		else
		{
			break;
		}
	}
	return res;
}

int Design_Util::getLevlCheckPointDes(float distance)
{
	int res = 0;
	for (int i = 0; i < TOTAL_CHECK_POINT_DES_ROW; i++)
	{
		float next = GET_DES().getCheckPointDes(i, CHECK_POINT_DES_DISTANCE);
		if (distance >= next)
		{
			res = i;
		}
		else
		{
			break;
		}
	}
	return res;
}

int Design_Util::getLevlBoseDes(float distance, int param)
{
	int res = 0;
	
	return res;
}

int Design_Util::ConverNumber(const char* string)
{
	char str[5];
	sprintf(str, "%s", string);
	int len = strlen(str);
	int res = 0;
	for (int i = 0; i < len; i++)
	{
		res += (str[i] - 48) * CMath::POW(10, len - i - 1);
	}
	return res;
}

//Note:generate code auto
