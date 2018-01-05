#include "pch.h"
#include "../GameCore/CGame.h"
#include "../GameCore/Os_Functions.h"


extern "C" int	Android_GetIAPDesNum()
{
	return TOTAL_IAP_NAME_DES_ROW;
}

extern "C" const char* Android_GetIAPDesList(int index)
{
	return GDS_Exporter::iap_name_des[TOTAL_IAP_NAME_DES*index + IAP_NAME_DES_GOOGLE_ID];
}

extern "C" void Android_OnBuyInAppItemComplete(int result_code, const char* item_id)
{
	PDEBUG("Android_OnBuyInAppItemComplete result_code = %d\n", result_code);
	if (result_code == 0)
	{
		PURCHASE_FUNC(item_id, enumIAPResponseStatus::IAP_RESPONSE_STATUS_SUCCESS);
	}
	else if (result_code == 2)
	{
		PURCHASE_FUNC(item_id, enumIAPResponseStatus::IAP_RESPONSE_STATUS_SUCCESS_RESTORE);
	}
	else
	{
		PURCHASE_FUNC(item_id, enumIAPResponseStatus::IAP_RESPONSE_STATUS_FAIL);
	}
}

extern "C" void Android_OnUpdateInAppItem(int index, const char* price, bool isOwned)
{
	if (isOwned)
	{
		PURCHASE_FUNC(Android_GetIAPDesList(index), enumIAPResponseStatus::IAP_RESPONSE_STATUS_SUCCESS);
		PDEBUG("Owned item: index = %d, id = %s, price = %s", index, Android_GetIAPDesList(index), price);
	}
	else
	{
		PDEBUG("Not yet owned item: index = %d, id = %s, price = %s", index, Android_GetIAPDesList(index), price);
	}
}