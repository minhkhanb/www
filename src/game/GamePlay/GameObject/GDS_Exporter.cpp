#include "pch.h"
#include "../../GameCore/CGame.h"
//The code generate will auto appends.
//@[ginha.do]
//Note:generate code auto
const char* GDS_Exporter::daily_text[16] = 
{
		"Do you fear me?",		"Kill 10 enemies in a single run",
		"More Than a Cowboy",		"Use a power-up",
		"Collector",		"Collect 20 coins",
		"Bomb Miner",		"Activate 10 bombs",
		"I'm invincible!",		"Kill 100 enemies in a single run",
		"Bounty Hunter",		"Collect 20 coins in a single run",
		"I'm Rich",		"Collect 100 coins",
		"Bomb Master",		"Activate 30 bombs",

};
//@[ginha.do]
//Note:generate code auto
const int GDS_Exporter::daily_number[16] = 
{
		10,		20,
		1,		20,
		20,		50,
		10,		50,
		100,		80,
		20,		80,
		100,		80,
		30,		80,

};
//@[ginha.do]
//Note:generate code auto
const char* GDS_Exporter::shop_charater_name[8] = 
{
		"Shane",		"Civilian.",
		"Roy Rogers",		"(Double Coin)",
		"Johnny Ringo",		"(One extra live!)",
		"Clint Westwood",		"(Double Score)",

};
//@[ginha.do]
//Note:generate code auto
const float GDS_Exporter::shop_charater[12] = 
{
		3.0,		0.0,		0.0,
		57.0,		3500.0,		700.0,
		58.0,		8000.0,		1800.0,
		56.0,		15000.0,		4000.0,

};
//@[ginha.do]
//Note:generate code auto
const float GDS_Exporter::ai_boss_des[18] = 
{
		80.0,		500.0,		20.0,		70.0,		6.0,		3.0,
		50.0,		450.0,		15.0,		50.0,		8.0,		2.0,
		20.0,		350.0,		10.0,		30.0,		10.0,		1.0,

};
//@[ginha.do]
//Note:generate code auto
const float GDS_Exporter::check_point_des[72] = 
{
		1.0,		0.0,		1.0,		0.0,		300.0,		150.0,		10.0,		10.0,		1.0,
		2.0,		300.0,		2.0,		0.0,		300.0,		8.0,		15.0,		15.0,		1.0,
		3.0,		700.0,		1.0,		0.0,		1200.0,		300.0,		20.0,		20.0,		1.0,
		4.0,		1200.0,		2.0,		0.0,		1200.0,		20.0,		25.0,		25.0,		1.0,
		5.0,		1800.0,		3.0,		0.0,		2500.0,		500.0,		30.0,		30.0,		1.0,
		6.0,		2500.0,		2.0,		0.0,		2500.0,		30.0,		30.0,		30.0,		1.0,
		7.0,		3200.0,		3.0,		0.0,		4000.0,		1000.0,		30.0,		30.0,		1.0,
		7.0,		4000.0,		2.0,		0.0,		4000.0,		1500.0,		30.0,		30.0,		1.0,

};
//@[ginha.do]
//Note:generate code auto
const char* GDS_Exporter::boss3_type_des[8] = 
{
		"0,1",		"0",
		"0,1,2",		"4",
		"1,2,3",		"4,5",
		"2,3,4,1",		"4,5",

};
//@[ginha.do]
//Note:generate code auto
const float GDS_Exporter::boss3_des[16] = 
{
		8.0,		1.0,		100.0,		100.0,
		8.0,		1.0,		80.0,		60.0,
		9.0,		0.9,		50.0,		40.0,
		10.0,		0.8,		30.0,		20.0,

};
//@[ginha.do]
//Note:generate code auto
const char* GDS_Exporter::string_des[87] = 
{
		"0",		"Pwu Name",		"Rapid fire",
		"1",		"Description",		"Shoot quicker than they can see",
		"2",		"Pwu Name",		"Dead eye",
		"3",		"Description",		"Deal more gun damage",
		"4",		"Pwu Name",		"Jolly o' boots",
		"5",		"Description",		"Run faster",
		"6",		"Pwu Name",		"Coin Magnet",
		"7",		"Description",		"Let those coin love ya even more",
		"8",		"Pwu Name",		"Machete",
		"9",		"Description",		"Increase the duration of being a badass",
		"10",		"Pwu Name",		"The calvary",
		"11",		"Description",		"More time for horse ridin'",
		"12",		"Pwu Name",		"Team Up!",
		"13",		"Description",		"Let your buds help you some longer",
		"14",		"Pwu Name",		"Tough as nails",
		"15",		"Description",		"Increase your health",
		"16",		"Pwu Unlock",		"Reach <Y>Town %d<C> to unlock",
		"17",		"Pwu Unlock InGame",		"A new Power-up is unlocked!",
		"18",		"Tutorial S1",		"Hold and drag your finger to move our cowboy around!",
		"19",		"Tutorial S2",		"Tap the shoot button for our cowboy to shoot!",
		"20",		"Tutorial S3",		"Don't ya miss that power-up. It will make ya badass!",
		"21",		"Tutorial S4",		"Stop them from stealin' all the gold!",
		"22",		"Tutorial S5",		"You've finally killed the most dangerous Bandit. Let's continue your journey in the <Y>Endless Mode<C>!",
		"23",		"Notification",		"The citizen of the great Americas need your help! Please come back!",
		"24",		"Notification",		"The golden train is in danger! They need your help! ",
		"25",		"Notification",		"Hernan Cortes, the Mexican mayor needs to see you. Let's set up an appoinment with him.",
		"26",		"Reset Game",		"Reset Game?",
		"27",		"Reset Game",		"Do you want to reset the game and purchases and get back all the coins you've earned?",
		"28",		"Reset Game",		"Please check your network connection and try again!",

};
//@[ginha.do]
//Note:generate code auto
const char* GDS_Exporter::achievement_name[42] = 
{
		"Kill 20 enemies in a single run",		"WWW_1",
		"Kill 1000 enemies",		"WWW_2",
		"Collect 10 coins in a single run",		"WWW_3",
		"Collect 50 coins in a single run",		"WWW_4",
		"Collect 1000 coins",		"WWW_5",
		"Collect 10000 coins",		"WWW_6",
		"Use 2 power-ups in a single run",		"WWW_7",
		"Kill 10 enemies that jump out from barrels",		"WWW_8",
		"Kill 300 enemies that jump out from barrels",		"WWW_9",
		"Let rockets open 10 barrels in a single run",		"WWW_10",
		"Let rockets open 200 barrels",		"WWW_11",
		"Let bombs open 10 barrels",		"WWW_12",
		"Let bombs open 200 barrels",		"WWW_13",
		"Activate 5 bombs in a single run",		"WWW_14",
		"Activate 15 bombs in a single run",		"WWW_15",
		"Activate 50 bombs",		"WWW_16",
		"Activate 500 bombs",		"WWW_17",
		"Kill 500 enemies using rockets",		"WWW_18",
		"Kill 500 enemies using bombs",		"WWW_19",
		"Upgrade any power-up",		"WWW_20",
		"Max the upgrades of all power-ups",		"WWW_21",

};
//@[ginha.do]
//Note:generate code auto
const float GDS_Exporter::achievement_des[42] = 
{
		20.0,		10.0,
		1000.0,		20.0,
		10.0,		10.0,
		50.0,		20.0,
		1000.0,		30.0,
		10000.0,		40.0,
		2.0,		10.0,
		10.0,		20.0,
		300.0,		40.0,
		10.0,		20.0,
		200.0,		30.0,
		10.0,		20.0,
		200.0,		30.0,
		5.0,		10.0,
		15.0,		20.0,
		50.0,		30.0,
		500.0,		40.0,
		500.0,		30.0,
		500.0,		30.0,
		1.0,		10.0,
		1.0,		30.0,

};
//@[ginha.do]
//Note:generate code auto
const char* GDS_Exporter::iap_name_des[20] = 
{
		"Pile of coins",		"www_c_1",		"0.99",		"www_c_1",
		"Bag of coins",		"www_c_2",		"2.99",		"www_c_2",
		"Sack of coins",		"www_c_3",		"4.99",		"www_c_3",
		"Box of coins",		"www_c_4",		"9.99",		"www_c_4",
		"Remove Ads",		"www_adsremove",		"2.99",		"www_adsremove",

};
//@[ginha.do]
//Note:generate code auto
const float GDS_Exporter::iap_value_des[5] = 
{
		500.0,
		1550.0,
		2600.0,
		5250.0,
		0.0,

};
//@[ginha.do]
//Note:generate code auto
const float GDS_Exporter::live_des[8] = 
{
		2.0,		0.0,
		3.0,		2000.0,
		4.0,		6000.0,
		5.0,		10000.0,

};
//@[ginha.do]
//Note:generate code auto
const float GDS_Exporter::cavalry_des[28] = 
{
		7000.0,		0.0,		1700.0,		70.0,		3.5,		1.0,		4.5,
		8000.0,		500.0,		1700.0,		80.0,		3.5,		1.0,		4.8,
		10000.0,		1250.0,		1700.0,		90.0,		3.5,		1.0,		5.2,
		13000.0,		2000.0,		1700.0,		100.0,		3.5,		1.0,		5.5,

};
//@[ginha.do]
//Note:generate code auto
const float GDS_Exporter::mule_des[28] = 
{
		6000.0,		0.0,		800.0,		60.0,		2.6,		1.0,		4.0,
		8000.0,		350.0,		800.0,		70.0,		2.6,		1.0,		4.3,
		10000.0,		800.0,		800.0,		80.0,		2.6,		1.0,		4.8,
		12000.0,		1500.0,		800.0,		100.0,		2.6,		1.0,		5.2,

};
//@[ginha.do]
//Note:generate code auto
const char* GDS_Exporter::machete_type_des[4] = 
{
		"1",
		"2,3",
		"2,3",
		"2,3",

};
//@[ginha.do]
//Note:generate code auto
const float GDS_Exporter::machete_des[24] = 
{
		5000.0,		0.0,		0.0,		6.0,		50.0,		1.2,
		6000.0,		200.0,		0.0,		12.0,		50.0,		1.2,
		7000.0,		500.0,		0.0,		15.0,		55.0,		1.2,
		9000.0,		750.0,		0.0,		20.0,		60.0,		1.2,

};
//@[ginha.do]
//Note:generate code auto
const float GDS_Exporter::magnet_des[8] = 
{
		10.0,		0.0,
		50.0,		200.0,
		80.0,		500.0,
		120.0,		1000.0,

};
//@[ginha.do]
//Note:generate code auto
const float GDS_Exporter::charater_des[12] = 
{
		4.0,		0.0,		2.0,
		4.3,		100.0,		2.0,
		4.6,		300.0,		2.0,
		5.0,		600.0,		2.0,

};
//@[ginha.do]
//Note:generate code auto
const float GDS_Exporter::gun_des[36] = 
{
		10.0,		1.0,		1.0,		450.0,		0.0,		0.4,
		11.0,		0.8,		2.0,		500.0,		50.0,		0.4,
		11.0,		0.8,		3.0,		650.0,		100.0,		0.3,
		12.0,		0.7,		4.0,		750.0,		300.0,		0.3,
		12.0,		0.6,		5.0,		800.0,		600.0,		0.2,
		13.0,		0.5,		6.0,		900.0,		1200.0,		0.2,

};
//@[ginha.do]
//Note:generate code auto
const float GDS_Exporter::enemy_des[35] = 
{
		1.0,		1.0,		1.1,		1.0,		1.0,		0.0,		0.0,
		2.0,		1.0,		1.1,		2.0,		2.0,		0.0,		0.0,
		3.0,		1.0,		1.3,		3.0,		3.0,		0.0,		0.0,
		4.0,		2.0,		1.3,		4.0,		4.0,		120.0,		5.0,
		5.0,		2.0,		1.5,		12.0,		5.0,		150.0,		5.5,

};
//@[ginha.do]
//Note:generate code auto
const char* GDS_Exporter::lvl_obs[42] = 
{
		"0,1,2,3,4",		"1",		"0",
		"0,1,2,3,4",		"1",		"0,1",
		"39,40,41,42,43,44,45",		"1,2",		"0,1,2,3",
		"39,40,41,42,43,44,45",		"1,2,3",		"0,1,2,3,4",
		"5,6,7,8,9,10,11,12,13,0,1,2,3,4",		"2,3",		"0,1,2,3,4,6",
		"5,6,7,8,9,10,11,12,13,14,15,1,2,16,17,18,19,20",		"2,3,4",		"3,4,5,6,8,10",
		"39,40,41,42,43,44,45",		"2,3,4",		"4,5,6,7,9,2,3,1",
		"39,40,41,42,43,44,45",		"2,3,4",		"4,5,6,7,9,10,11,12,1,2,3",
		"20,21,22,23,24,25,26,27,28,29,30,32,33,34,35,36,37,38",		"2,3,4,5",		"4,5,6,7,9,10,11,12,1,2,3",
		"30,32,33,34,35,36,37,38,12,13,14,15,16,17,18",		"3,4,5",		"4,5,6,7,9,10,11,12,1,2,3",
		"39,40,41,42,43,44,45",		"3,4,5",		"4,5,6,7,9,2,3,1",
		"39,40,41,42,43,44,45",		"3,4,5",		"4,5,6,7,9,10,11,12,1,2,3",
		"20,21,22,23,24,25,26,27,28,29,30,32,33,34,35,36,37,38",		"3,4,5",		"4,5,6,7,9,10,11,12,1,2,3",
		"30,32,33,34,35,36,37,38,19,20,21,22,23,25,26",		"3,4,5",		"4,5,6,7,9,10,11,12,1,2,3",

};
//@[ginha.do]
//Note:generate code auto
const float GDS_Exporter::lvl_des[210] = 
{
		100.0,		2.0,		4.0,		30.0,		35.0,		0.0,		0.0,		0.0,		1.0,		3.0,		70.0,		20.0,		10.0,		0.0,		0.0,
		300.0,		3.0,		4.0,		35.0,		40.0,		0.0,		0.0,		1.0,		2.0,		2.0,		70.0,		20.0,		10.0,		0.0,		0.0,
		500.0,		2.0,		3.0,		40.0,		45.0,		0.0,		0.0,		2.0,		3.0,		2.0,		50.0,		40.0,		10.0,		0.0,		0.0,
		700.0,		2.0,		5.0,		45.0,		50.0,		0.0,		0.0,		2.0,		5.0,		2.0,		20.0,		50.0,		30.0,		0.0,		0.0,
		1000.0,		3.0,		7.0,		50.0,		60.0,		0.0,		0.0,		3.0,		5.0,		2.0,		0.0,		65.0,		35.0,		0.0,		0.0,
		1200.0,		4.0,		7.0,		55.0,		65.0,		0.0,		0.0,		4.0,		6.0,		1.0,		0.0,		20.0,		60.0,		20.0,		0.0,
		1500.0,		3.0,		5.0,		60.0,		70.0,		0.0,		0.0,		2.0,		5.0,		1.0,		0.0,		15.0,		60.0,		25.0,		0.0,
		1800.0,		4.0,		6.0,		65.0,		75.0,		0.0,		0.0,		3.0,		5.0,		1.0,		0.0,		10.0,		50.0,		40.0,		0.0,
		2200.0,		7.0,		10.0,		70.0,		80.0,		0.0,		0.0,		4.0,		7.0,		1.0,		0.0,		0.0,		50.0,		40.0,		10.0,
		2500.0,		4.0,		7.0,		75.0,		85.0,		0.0,		0.0,		3.0,		6.0,		1.0,		0.0,		0.0,		40.0,		40.0,		20.0,
		2800.0,		4.0,		7.0,		80.0,		90.0,		0.0,		0.0,		4.0,		6.0,		1.0,		0.0,		0.0,		35.0,		40.0,		25.0,
		3200.0,		5.0,		8.0,		85.0,		100.0,		0.0,		0.0,		4.0,		8.0,		1.0,		0.0,		0.0,		20.0,		50.0,		30.0,
		3500.0,		5.0,		8.0,		90.0,		105.0,		0.0,		1.0,		4.0,		6.0,		1.0,		0.0,		0.0,		25.0,		40.0,		35.0,
		4000.0,		6.0,		8.0,		95.0,		110.0,		0.0,		1.0,		6.0,		9.0,		1.0,		0.0,		0.0,		30.0,		30.0,		40.0,

};
