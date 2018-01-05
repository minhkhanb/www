#ifndef _DESIGN_UTIL_H
#define _DESIGN_UTIL_H

class  Design_Util
{
public:
	Design_Util()	{};

	
	static float getLevelDes(int index, int param)
	{
		return GDS_Exporter::lvl_des[index*TOTAL_LVL_DES + param];
	};

	
	static const char* getLevelObs(int index, int param)
	{
		return GDS_Exporter::lvl_obs[index*TOTAL_LVL_OBS+param];
	};
	
	static float getShopCharaterDes(int index, int param)
	{
		return GDS_Exporter::shop_charater[index*TOTAL_SHOP_CHARATER + param];
	};


	static const char* getShopCharaterName(int index, int param)
	{
		return GDS_Exporter::shop_charater_name[index*TOTAL_SHOP_CHARATER_NAME + param];
	};

	static const char* getDailyQuest_text(int index, int param)
	{
		return GDS_Exporter::daily_text[index*TOTAL_DAILY_TEXT + param];
	};

	static const int getDailyQuest_number(int index, int param)
	{
		return GDS_Exporter::daily_number[index*TOTAL_DAILY_NUMBER + param];
	};

	static float getGunDes(int index, int param)
	{
		return GDS_Exporter::gun_des[index*TOTAL_GUN_DES + param];
	};

	
	static float getEnemyDes(int index, int param)
	{
		return GDS_Exporter::enemy_des[index*TOTAL_ENEMY_DES+param];
	};

	static float getCharaterDes(int index, int param)
	{
		return GDS_Exporter::charater_des[index*TOTAL_CHARATER_DES + param];
	};

	static float getMagnetDes(int index, int param)
	{
		return GDS_Exporter::magnet_des[index*TOTAL_MAGNET_DES + param];
	};

	static float getMacheteDes(int index, int param)
	{
		return GDS_Exporter::machete_des[index*TOTAL_MACHETE_DES + param];
	};

	static const char* getMacheteTypeDes(int index)
	{
		return GDS_Exporter::machete_type_des[index];
	};

	static float getMuleDes(int index, int param)
	{
		return GDS_Exporter::mule_des[index*TOTAL_MULE_DES + param];
	};

	static float getCavalryDes(int index, int param)
	{
		return GDS_Exporter::cavalry_des[index*TOTAL_CAVALRY_DES + param];
	};

	static float getAliveDes(int index, int param)
	{
		return GDS_Exporter::live_des[index*TOTAL_LIVE_DES + param];
	};

	static float getIAPValuDes(int index, int param)
	{
		return GDS_Exporter::iap_value_des[index*TOTAL_IAP_VALUE_DES + param];
	};

	static const char* getIAPNameDes(int index, int param)
	{
		return GDS_Exporter::iap_name_des[index*TOTAL_IAP_NAME_DES + param];
	};

	static const char* getString(int index)
	{
		return GDS_Exporter::string_des[index*TOTAL_STRING_DES + STRING_DES_ENG];
	};

	

	static float getCheckPointDes(int index, int param)
	{
		return GDS_Exporter::check_point_des[index*TOTAL_CHECK_POINT_DES + param];
	};
	static float getAIBossDes(int index, int param)
	{
		return GDS_Exporter::ai_boss_des[index*TOTAL_AI_BOSS_DES + param];

	};

	static float getBoss3Des(int index, int param)
	{
		return GDS_Exporter::boss3_des[index*TOTAL_BOSS3_DES + param];

	};

	static const char* getBoss3TypeDes(int index, int param)
	{
		return GDS_Exporter::boss3_type_des[index*TOTAL_BOSS3_TYPE_DES + param];
	};

	static float getAchievementDes(int index, int param)
	{
		return GDS_Exporter::achievement_des[index*TOTAL_ACHIEVEMENT_DES + param];

	};
	
	static const char* getAchievementName(int index, int param)
	{
		return GDS_Exporter::achievement_name[index*TOTAL_ACHIEVEMENT_NAME + param];
	};
	
	
	//static const int _charterSpriteID[4];

	int getLevlCheckPointDes(float distance);
	int getLevlBoseDes(float distance, int param);	
	int getLevlFromDes(float distance);
	int ConverNumber(const char* string);
	
};

#endif