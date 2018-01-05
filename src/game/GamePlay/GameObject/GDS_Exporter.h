#if !_GDS_EXPORTER_H
#define _GDS_EXPORTER_H
class GDS_Exporter
{
public:
//The code generate will auto appends.
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_DAILY_TEXT	(2)
#define TOTAL_DAILY_TEXT_ROW	(8)

#define DAILY_TEXT_NAME	(0)
#define DAILY_TEXT_DECRIPTION	(1)
//end define
static const char* daily_text[16];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_DAILY_NUMBER	(2)
#define TOTAL_DAILY_NUMBER_ROW	(8)

#define DAILY_NUMBER_VALUE	(0)
#define DAILY_NUMBER_REWARD	(1)
//end define
static const int daily_number[16];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_SHOP_CHARATER_NAME	(2)
#define TOTAL_SHOP_CHARATER_NAME_ROW	(4)

#define SHOP_CHARATER_NAME_NAME	(0)
#define SHOP_CHARATER_NAME_DESCRIPTION	(1)
//end define
static const char* shop_charater_name[8];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_SHOP_CHARATER	(3)
#define TOTAL_SHOP_CHARATER_ROW	(4)

#define SHOP_CHARATER_SPRITE_ID	(0)
#define SHOP_CHARATER_PRICE	(1)
#define SHOP_CHARATER_DISTANCE_UNLOCK	(2)
//end define
static const float shop_charater[12];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_AI_BOSS_DES	(6)
#define TOTAL_AI_BOSS_DES_ROW	(3)

#define AI_BOSS_DES_PERCENT	(0)
#define AI_BOSS_DES_DISTANCE	(1)
#define AI_BOSS_DES_PERCENT_ROCKET	(2)
#define AI_BOSS_DES_PERCENT_BOME	(3)
#define AI_BOSS_DES_NUM_BOME	(4)
#define AI_BOSS_DES_TIME_DEPLAY_BOME	(5)
//end define
static const float ai_boss_des[18];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_CHECK_POINT_DES	(9)
#define TOTAL_CHECK_POINT_DES_ROW	(8)

#define CHECK_POINT_DES_LOCATION	(0)
#define CHECK_POINT_DES_DISTANCE	(1)
#define CHECK_POINT_DES_BOSS_TYPE	(2)
#define CHECK_POINT_DES_BOSS_STATE	(3)
#define CHECK_POINT_DES_BOSS_SPAWN	(4)
#define CHECK_POINT_DES_BOSS_HP	(5)
#define CHECK_POINT_DES_MIN_COIN_REWARD	(6)
#define CHECK_POINT_DES_MAX_COIN_REWARD	(7)
#define CHECK_POINT_DES_NUM_OF_APPLE	(8)
//end define
static const float check_point_des[72];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_BOSS3_TYPE_DES	(2)
#define TOTAL_BOSS3_TYPE_DES_ROW	(4)

#define BOSS3_TYPE_DES_TYPE_BULLET	(0)
#define BOSS3_TYPE_DES_TYPE_ENEMY	(1)
//end define
static const char* boss3_type_des[8];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_BOSS3_DES	(4)
#define TOTAL_BOSS3_DES_ROW	(4)

#define BOSS3_DES_BULLET_SPEED	(0)
#define BOSS3_DES_TIME_GUN	(1)
#define BOSS3_DES_HP_WARNING	(2)
#define BOSS3_DES_HP_WARNING_1	(3)
//end define
static const float boss3_des[16];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_STRING_DES	(3)
#define TOTAL_STRING_DES_ROW	(29)

#define STRING_DES_ID	(0)
#define STRING_DES_TYPE	(1)
#define STRING_DES_ENG	(2)
//end define
static const char* string_des[87];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_ACHIEVEMENT_NAME	(2)
#define TOTAL_ACHIEVEMENT_NAME_ROW	(21)

#define ACHIEVEMENT_NAME_DESCRIPTION	(0)
#define ACHIEVEMENT_NAME_ACHIEVEMENT_ID	(1)
//end define
static const char* achievement_name[42];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_ACHIEVEMENT_DES	(2)
#define TOTAL_ACHIEVEMENT_DES_ROW	(21)

#define ACHIEVEMENT_DES_VALUE	(0)
#define ACHIEVEMENT_DES_POINT_REWARD	(1)
//end define
static const float achievement_des[42];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_IAP_NAME_DES	(4)
#define TOTAL_IAP_NAME_DES_ROW	(5)

#define IAP_NAME_DES_NAME	(0)
#define IAP_NAME_DES_APPLE_ID	(1)
#define IAP_NAME_DES_PRICE	(2)
#define IAP_NAME_DES_GOOGLE_ID	(3)
//end define
static const char* iap_name_des[20];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_IAP_VALUE_DES	(1)
#define TOTAL_IAP_VALUE_DES_ROW	(5)

#define IAP_VALUE_DES_VALUE	(0)
//end define
static const float iap_value_des[5];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_LIVE_DES	(2)
#define TOTAL_LIVE_DES_ROW	(4)

#define LIVE_DES_TIME	(0)
#define LIVE_DES_UPGRADE_COST	(1)
//end define
static const float live_des[8];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_CAVALRY_DES	(7)
#define TOTAL_CAVALRY_DES_ROW	(4)

#define CAVALRY_DES_TIME	(0)
#define CAVALRY_DES_UPGRADE_COST	(1)
#define CAVALRY_DES_DISTANCE_UNLOCK	(2)
#define CAVALRY_DES_SPAWN_RATIO	(3)
#define CAVALRY_DES_SPEED_SCROLL_MAP	(4)
#define CAVALRY_DES_MINUS_ENEMY_SPEED	(5)
#define CAVALRY_DES_MOVEMENT_SPEED	(6)
//end define
static const float cavalry_des[28];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_MULE_DES	(7)
#define TOTAL_MULE_DES_ROW	(4)

#define MULE_DES_TIME	(0)
#define MULE_DES_UPGRADE_COST	(1)
#define MULE_DES_DISTANCE_UNLOCK	(2)
#define MULE_DES_SPAWN_RATIO	(3)
#define MULE_DES_SPEED_SCROLL_MAP	(4)
#define MULE_DES_MINUS_ENEMY_SPEED	(5)
#define MULE_DES_MOVEMENT_SPEED	(6)
//end define
static const float mule_des[28];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_MACHETE_TYPE_DES	(1)
#define TOTAL_MACHETE_TYPE_DES_ROW	(4)

#define MACHETE_TYPE_DES_ENEMY_TYPE	(0)
//end define
static const char* machete_type_des[4];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_MACHETE_DES	(6)
#define TOTAL_MACHETE_DES_ROW	(4)

#define MACHETE_DES_TIME	(0)
#define MACHETE_DES_UPGRADE_COST	(1)
#define MACHETE_DES_DISTANCE_UNLOCK	(2)
#define MACHETE_DES_SPAM_ENEMY	(3)
#define MACHETE_DES_SPAWN_RATIO	(4)
#define MACHETE_DES_MINUS_ENEMY_SPEED	(5)
//end define
static const float machete_des[24];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_MAGNET_DES	(2)
#define TOTAL_MAGNET_DES_ROW	(4)

#define MAGNET_DES_DISTANCE_EFFECT	(0)
#define MAGNET_DES_UPGRADE_COST	(1)
//end define
static const float magnet_des[8];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_CHARATER_DES	(3)
#define TOTAL_CHARATER_DES_ROW	(4)

#define CHARATER_DES_MOVE_SPEED	(0)
#define CHARATER_DES_UPGRADE_COST	(1)
#define CHARATER_DES_SPEED_SCROLL_MAP	(2)
//end define
static const float charater_des[12];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_GUN_DES	(6)
#define TOTAL_GUN_DES_ROW	(6)

#define GUN_DES_SPEED	(0)
#define GUN_DES_TIME_GUN	(1)
#define GUN_DES_DAME	(2)
#define GUN_DES_SHOOT_DISTANCE	(3)
#define GUN_DES_UPGRADE_COST	(4)
#define GUN_DES_TIME_BULLET_DELAY	(5)
//end define
static const float gun_des[36];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_ENEMY_DES	(7)
#define TOTAL_ENEMY_DES_ROW	(5)

#define ENEMY_DES_TYPE	(0)
#define ENEMY_DES_COIN_VALUE	(1)
#define ENEMY_DES_MOVEMENT_SPEED	(2)
#define ENEMY_DES_HP	(3)
#define ENEMY_DES_DAME	(4)
#define ENEMY_DES_TIME_GUN	(5)
#define ENEMY_DES_BULLET_SPEED	(6)
//end define
static const float enemy_des[35];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_LVL_OBS	(3)
#define TOTAL_LVL_OBS_ROW	(14)

#define LVL_OBS_OBSTACLE_TYPE	(0)
#define LVL_OBS_ENEMY_TYPE	(1)
#define LVL_OBS_ROCKET_TYPE	(2)
//end define
static const char* lvl_obs[42];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_LVL_DES	(15)
#define TOTAL_LVL_DES_ROW	(14)

#define LVL_DES_DISTANCE	(0)
#define LVL_DES_MIN_OBS	(1)
#define LVL_DES_MAX_OBS	(2)
#define LVL_DES_MIN_ENEMY	(3)
#define LVL_DES_MAX_ENEMY	(4)
#define LVL_DES_MIN_COIN	(5)
#define LVL_DES_MAX_COIN	(6)
#define LVL_DES_MIN_ROCKET	(7)
#define LVL_DES_MAX_ROCKET	(8)
#define LVL_DES_TIME_MIN_RELEASE_ROCKET	(9)
#define LVL_DES_ENEMY_1_RATIO	(10)
#define LVL_DES_ENEMY_2_RATIO	(11)
#define LVL_DES_ENEMY_3_RATIO	(12)
#define LVL_DES_ENEMY_4_RATIO	(13)
#define LVL_DES_ENEMY_5_RATIO	(14)
//end define
static const float lvl_des[210];
};


#endif
