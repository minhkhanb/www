#include "pch.h"
#include "../../GameCore/CGame.h"
#include "CUser.h"
#include "../../GameCore/Os_Functions.h"
#include "../Game_ExternFunctions.h"
extern "C" int getGlobalRank();

void CUser::Init()
{
	_play_game_num = 1;
	_hadRemovedAds = false;
	play_current_date = getCurrentDate();
}

void CUser::DataDefault()
{
	for (int i = 0; i < MAX_SAVE_BUFFER_CACHE; i++)
	{
		_buffer[i] = 0;
	}
	_versionFormat = SAVE_GAME_VERSION_0;
	_userId = 0;
	_highScore = 0;
	_timePlay = 0;
	_coin = 0;
	_total_coin = 0;
	isFirstPlay = true;
	sprintf(_userName, "%s", "Your name");

	_removeAdd = false;
	CHARATER().SetLVLAlive(0);
	CHARATER().SetLVLRevolver(0);
	CHARATER().SetLVLFastTrigger(0);
	CHARATER().SetLVLSpeed(0);
	CHARATER().SetLVLMagnet(0);
	CHARATER().SetLVLMachete(0);
	CHARATER().SetLVLMule(0);
	CHARATER().SetLVLCavalry(0);

	_isUnlockMachete = false;
	_isUnlockMule = false;
	_isUnlockCavalry = false;

	_lvlLocation = 0;
	_currentLocation = 0;
	_stateBoss = BOSS_STATE_NONE;

	TUTORIAL()._step = 0;
	SynForNewVersion(_versionFormat);
	_completeAdventureMode = false;

	//update version 1
	for (int i = 0; i < MAX_ACHIEVEMENT_VER_0; i++)
	{
		ACHIEVEMENT()._percent[i] = 0;
		ACHIEVEMENT()._isComplete[i] = ACHIEVEMENT_NONE;
	}
	ACHIEVEMENT()._totalEnemyDie = 0;
	ACHIEVEMENT()._totalCoinColect = 0;
	ACHIEVEMENT()._totalEnemyJumpBarrelDie = 0;
	ACHIEVEMENT()._totalRocketOpenBarrel = 0;
	ACHIEVEMENT()._totalBombOpenBarrel = 0;
	ACHIEVEMENT()._totalBombActive = 0;
	ACHIEVEMENT()._totalEnemyDieByRocket = 0;
	ACHIEVEMENT()._totalEnemyDieByBomb = 0;

	_currentUseCharater = 0;
	for (int i = 0; i < MAX_CHARATER; i++)
	{
		_unlockCharater[i] = false;
		_useCharacter[i] = false;
	}
	_unlockCharater[_currentUseCharater] = true;
	_useCharacter[_currentUseCharater] = true;
	time_t seconds;
	seconds = time(NULL);
	int daysSince1970 = seconds / (60 * 60 * 24);
	_dailyQuest_curDate = daysSince1970;

	ACHIEVEMENT()._numDailyBombActive = 0;
	ACHIEVEMENT()._totalDailyCoinCollect = 0;
	ACHIEVEMENT()._numDailyRocketByBarrel = 0;
	ACHIEVEMENT()._numDailyEnemyJumpBarrel = 0;
	ACHIEVEMENT()._numDailyUsePowerUp = 0;
	ACHIEVEMENT().ResetInfoQuest();
	AddCoin(50);
	_hasRate = false;
	_num_play_in_asession = 0;
	_rate_cancel_count = 0;
	//Update version 3
	/*for (int i = 0; i < MAX_QUEST; i++) {
		ACHIEVEMENT()._percentQuest[i] = 0;
		ACHIEVEMENT()._isQuestComplete[i] = QUEST_NONE;
	}*/
	sprintf(_game_promo_icon_id, "%s", "gameID");
	sprintf(_game_promo_link_store, "%s", "linkStore");
	sprintf(_game_promo_deviceID, "%s", "deviceID");
	_game_promo_revision = 0;
	_game_promo_has_show = true;
	_game_promo_delay_show = time(NULL);
	_game_promo_order = 0;
	_has_touch_promo = false;
}

void CUser::SynForNewVersion(INT_TIME fromVersion)
{
	for (INT_TIME i = fromVersion; i <= CURRENT_SAVE_VERSION; i++)
	{
		NewDataDefault(i);
	}

	_versionFormat = CURRENT_SAVE_VERSION;
}

void CUser::NewDataDefault(INT_TIME version)
{
	switch (version)
	{
	case SAVE_GAME_VERSION_0: //version 0  1.0.0
		break;

	case SAVE_GAME_VERSION_1: //version 0  1.1.1
		for (int i = 0; i < MAX_ACHIEVEMENT_VER_0; i++)
		{
			ACHIEVEMENT()._percent[i] = 0;
			ACHIEVEMENT()._isComplete[i] = ACHIEVEMENT_NONE;
		}
		ACHIEVEMENT()._totalEnemyDie = 0;
		ACHIEVEMENT()._totalCoinColect = 0;
		ACHIEVEMENT()._totalEnemyJumpBarrelDie = 0;
		ACHIEVEMENT()._totalRocketOpenBarrel = 0;
		ACHIEVEMENT()._totalBombOpenBarrel = 0;
		ACHIEVEMENT()._totalBombActive = 0;
		ACHIEVEMENT()._totalEnemyDieByRocket = 0;
		ACHIEVEMENT()._totalEnemyDieByBomb = 0;

		_currentUseCharater = 0;
		for (int i = 0; i < MAX_CHARATER; i++)
		{
			_unlockCharater[i] = false;
			_useCharacter[i] = false;
		}
		_unlockCharater[_currentUseCharater] = true;
		_useCharacter[_currentUseCharater] = true;
		break;

	case SAVE_GAME_VERSION_3:
		ACHIEVEMENT()._numEnemyDie = 0;
		ACHIEVEMENT()._numPowerUse = 0;
		ACHIEVEMENT()._numEnemyJumpBarrelDie = 0;
		ACHIEVEMENT()._numCoinColect = 0;
		//Update version 3
		/*for (int i = 0; i < MAX_QUEST; i++) {
			ACHIEVEMENT()._percentQuest[i] = 0;
			ACHIEVEMENT()._isQuestComplete[i] = QUEST_NONE;
		}*/
		break;
	case SAVE_GAME_VERSION_4:
	{

		time_t seconds;
		seconds = time(NULL);
		int daysSince1970 = seconds / (60 * 60 * 24);
		_dailyQuest_curDate = daysSince1970;
		ACHIEVEMENT()._numDailyBombActive = 0;
		ACHIEVEMENT()._totalDailyCoinCollect = 0;
		ACHIEVEMENT()._numDailyRocketByBarrel = 0;
		ACHIEVEMENT()._numDailyEnemyJumpBarrel = 0;
		ACHIEVEMENT()._numDailyUsePowerUp = 0;
		ACHIEVEMENT().ResetInfoQuest();
	}
	break;
	case SAVE_GAME_VERSION_5:
	{
		ACHIEVEMENT()._dailyQuestCurrent = 0;
		_hasRate = false;
		time_t seconds;
		seconds = time(NULL);
		int daysSince1970 = seconds / (60 * 60 * 24);
		_dailyQuest_curDate = daysSince1970;
	}
	break;
	case SAVE_GAME_VERSION_6:
	{
		time_t seconds;
		seconds = time(NULL);
		int daysSince1970 = seconds / (60 * 60 * 24);
		_dailyQuest_curDate = daysSince1970;
		_num_play_in_asession = 0;
		isFirstPlay = true;
	}
	break;
	case SAVE_GAME_VERSION_7:
		_rate_cancel_count = 0;
		break;

	case SAVE_GAME_VERSION_8:
	{
		_game_promo_revision = 0;
		_game_promo_has_show = true;
		_game_promo_delay_show = time(NULL);
		_game_promo_order = 0;
		_has_touch_promo = false;
	}
	break;
	}


}

void CUser::DataSave()
{
	DataToBuffer();

#if defined(MAC_OS) || defined(_WINDOWS_PHONE8) || defined (ANDROID_OS)
	OS_SaveAppData(SAVE_FILE_NAME, _buffer, MAX_SAVE_BUFFER_CACHE, false);
#else

	FILE * stream = NULL;

	stream = fopen(SAVE_FILE_NAME, "wb");

	if (stream != NULL)
	{
		fwrite((void *)_buffer, sizeof(BYTE), MAX_SAVE_BUFFER_CACHE, stream);
	}

	if (stream != NULL)
	{
		fclose(stream);
	}
#endif
}

void CUser::DataLoad()
{
	int saved = 0;
	Buffer* buff = OS_LoadAppData(SAVE_FILE_NAME);
	if (buff != NULL)
	{
		saved = buff->Length();
		memcpy(_buffer, buff->Data(), saved);
		delete buff;
	}
	/*DataDefault();
	DataSave();*/
	if (saved == 0)
	{
		DataDefault();
		DataSave();
	}
	else
	{
		DataFromBuffer();
	}
}

void CUser::DataToBuffer()
{
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	printf("\n ========= BEGIN SAVE version %d ==========", _versionFormat);

	/*GAME()->SetInt64At(_buffer, SAVE_FORMAT_CURRENT_VERSION, _versionFormat);
	int t = 0;*/

	//for version  0
	GAME()->SetInt64At(_buffer, SAVE_FORMAT_CURRENT_VERSION, _versionFormat);
	GAME()->SetInt64At(_buffer, SAVE_FORMAT_USER_ID, _userId);
	GAME()->SetInt64At(_buffer, SAVE_FORMAT_OWNNER, _ower_save);
	GAME()->SetInt64At(_buffer, SAVE_FORMAT_TIMEPLAY, _timePlay);
	GAME()->SetInt32At(_buffer, SAVE_FORMAT_HIGHT_SCORE, _highScore);
	GAME()->SetInt32At(_buffer, SAVE_FORMAT_COIN, _coin);

	for (int i = 0; i < 128; i++)
	{
		GAME()->SetCharAt(_buffer, SAVE_FORMAT_USER_NAME + i, _userName[i]);
	}

	GAME()->SetByteAt(_buffer, SAVE_FORMAT_LVL_ALIVE, CHARATER().GetLVLAlive());
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_LVL_REVOLVER, CHARATER().GetLVLRevolver());
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_LVL_FAST_TRGGER, CHARATER().GetLVLFastTrigger());
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_LVL_SPEED, CHARATER().GetLVLSpeed());
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_LVL_MAGNET, CHARATER().GetLVLMagnet());
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_LVL_MACHETE, CHARATER().GetLVLMachete());
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_LVL_MULE, CHARATER().GetLVLMule());
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_LVL_CAVALRY, CHARATER().GetLVLCavalry());

	GAME()->SetByteAt(_buffer, SAVE_FORMAT_UNLOCK_MACHETE, _isUnlockMachete);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_UNLOCK_MULE, _isUnlockMule);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_UNLOCK_CAVALRY, _isUnlockCavalry);

	GAME()->SetByteAt(_buffer, SAVE_FORMAT_LVL_LOCATION, _lvlLocation);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_CURRENT_LOCATION, _currentLocation);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_STATE_BOSS, _stateBoss);


	GAME()->SetByteAt(_buffer, SAVE_FORMAT_ADD_REMOVE, _removeAdd);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_TUTORIAL_STEP, TUTORIAL()._step);

	GAME()->SetInt32At(_buffer, SAVE_FORMAT_TOTAL_COIN, _total_coin);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_ADVENTURE_MODE, _completeAdventureMode);

	//update version 1
	for (int i = 0; i < MAX_ACHIEVEMENT_VER_0; i++)
	{
		GAME()->SetInt16At(_buffer, SAVE_FORMAT_ACHIEVEMENT_PERCENT + i * 2, ACHIEVEMENT()._percent[i]);
		GAME()->SetByteAt(_buffer, SAVE_FORMAT_ACHIEVEMENT_COMPLETE + i, ACHIEVEMENT()._isComplete[i]);
	}

	GAME()->SetInt16At(_buffer, SAVE_FORMAT_TOTAL_ENEMY_DIE, ACHIEVEMENT()._totalEnemyDie);
	GAME()->SetInt32At(_buffer, SAVE_FORMAT_TOTAL_COIN_COLECT, ACHIEVEMENT()._totalCoinColect);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_TOTAL_ENEMY_JUMP_BARREL, ACHIEVEMENT()._totalEnemyJumpBarrelDie);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_TOTAL_ROCKET_OPEN_BARREL, ACHIEVEMENT()._totalRocketOpenBarrel);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_TOTAL_BOMB_OPEN_BARREL, ACHIEVEMENT()._totalBombOpenBarrel);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_TOTAL_BOMB_ACTIVE, ACHIEVEMENT()._totalBombActive);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_TOTAL_ENEMY_DIE_BY_ROCKET, ACHIEVEMENT()._totalEnemyDieByRocket);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_TOTAL_ENEMY_DIE_BY_BOMB, ACHIEVEMENT()._totalEnemyDieByBomb);

	GAME()->SetByteAt(_buffer, SAVE_FORMAT_CURRENT_USE_CHARATER, _currentUseCharater);
	for (int i = 0; i < MAX_CHARATER; i++)
	{
		GAME()->SetByteAt(_buffer, SAVE_FORMAT_UNLOCK_CHARATER + 1 * i, _unlockCharater[i]);
		GAME()->SetByteAt(_buffer, SAVE_FORMAT_USE_CHARATER + 1 * i, _useCharacter[i]);
	}

	GAME()->SetInt32At(_buffer, SAVE_FORMAT_USER_PLAY_NUM, _play_game_num);

	GAME()->SetByteAt(_buffer, SAVE_FORMAT_SAVE_REMOVE_ADS, _hadRemovedAds);

	GAME()->SetInt32At(_buffer, SAVE_FORMAT_SAVE_NUM_ENEMYDIE_SINGLE_RUN, ACHIEVEMENT()._numEnemyDie);
	GAME()->SetInt32At(_buffer, SAVE_FORMAT_SAVE_POWER_USE, ACHIEVEMENT()._numPowerUse);
	GAME()->SetInt32At(_buffer, SAVE_FORMAT_SAVE_NUM_ENEMYDIE_JUMP_BARREL_TOTAL, ACHIEVEMENT()._numEnemyJumpBarrelDie);
	GAME()->SetInt32At(_buffer, SAVE_FORMAT_SAVE_OPEN_ROCKET_BARREL_TOTAL, ACHIEVEMENT()._totalRocketOpenBarrel);
	GAME()->SetInt32At(_buffer, SAVE_FORMAT_SAVE_COIN_COLLECT_SINGLE_RUN, ACHIEVEMENT()._numCoinColect);
	GAME()->SetInt32At(_buffer, SAVE_FORMAT_SAVE_COIN_COLLECT_TOTAL, ACHIEVEMENT()._totalCoinColect);
	GAME()->SetInt32At(_buffer, SAVE_FORMAT_SAVE_BOMB_ACTIVE_TOTAL, ACHIEVEMENT()._totalBombActive);

	if (game->playing_current_date_local > play_current_date)
	{
		play_current_date = game->playing_current_date_local;
		GAME()->SetInt32At(_buffer, SAVE_FORMAT_USER_CURRENT_DATE, play_current_date);
	}


	//update version 3
	for (int i = 0; i < MAX_QUEST; i++)
	{
		GAME()->SetInt16At(_buffer, SAVE_FORMAT_QUEST_PERCENT + i * 2, ACHIEVEMENT()._currentQuest[i]);//fix bug save percen
		GAME()->SetByteAt(_buffer, SAVE_FORMAT_QUEST_COMPLETE + i, ACHIEVEMENT()._isQuestComplete[i]);
		GAME()->SetInt16At(_buffer, SAVE_FORMAT_QUEST_ID + i * 2, game->_quest_info[i]._id);
	}

	GAME()->SetInt32At(_buffer, SAVE_FORMAT_DAILYQUEST_CUR_DATE, _dailyQuest_curDate);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_NUM_DAILY_BOM_ACTIVE, ACHIEVEMENT()._numDailyBombActive);
	GAME()->SetInt32At(_buffer, SAVE_FORMAT_TOTAL_DAILY_COIN_COLL, ACHIEVEMENT()._totalDailyCoinCollect);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_NUM_DAILY_ENEMY_BY_BARREL, ACHIEVEMENT()._numDailyEnemyJumpBarrel);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_NUM_DAILY_ROCKET_BY_BARREL, ACHIEVEMENT()._numDailyRocketByBarrel);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_NUM_DAILY_USE_PWUP, ACHIEVEMENT()._numDailyUsePowerUp);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_DAILY_QUEST_CURRENT, ACHIEVEMENT()._dailyQuestCurrent);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_HAS_RATE, _hasRate);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_IS_FIRST_PLAY, isFirstPlay);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_USER_NUM_PLAY_IN_ASESSION, _num_play_in_asession);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_USER_RATE_CANCEL_COUNT, _rate_cancel_count);

	//ver8
	for (int i = 0; i<128; i++)
	{
		GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_DEVICE_ID, _game_promo_deviceID[i]);
		GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_ICON_ID, _game_promo_icon_id[i]);
		GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_LINK_STORE, _game_promo_link_store[i]);
	}
	GAME()->SetInt64At(_buffer, SAVE_FORMAT_USER_GAME_PROMO_DELAY_SHOW, _game_promo_delay_show);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_HAS_SHOW, _game_promo_has_show);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_USER_GAME_PROMO_ORDER, _game_promo_order);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_USER_GAME_PROMO_REVISION, _game_promo_revision);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_HAS_TOUCH_PROMO, _has_touch_promo);

	printf("\n ========= END SAVE ========== \n");

}

void CUser::DataFromBuffer()
{
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	/*_versionFormat = GAME()->GetInt64At(_buffer, SAVE_FORMAT_CURRENT_VERSION);
	int t = 0;*/
	_versionFormat = GAME()->GetInt64At(_buffer, SAVE_FORMAT_CURRENT_VERSION);
	printf("\n ========= LOAD SAVE version %d ==========", _versionFormat);

	//for version  0
	_userId = GAME()->GetInt64At(_buffer, SAVE_FORMAT_USER_ID);
	_ower_save = GAME()->GetInt64At(_buffer, SAVE_FORMAT_OWNNER);
	_highScore = GAME()->GetInt32At(_buffer, SAVE_FORMAT_HIGHT_SCORE);
	_timePlay = GAME()->GetInt32At(_buffer, SAVE_FORMAT_TIMEPLAY);
	_coin = GAME()->GetInt32At(_buffer, SAVE_FORMAT_COIN);

	for (int i = 0; i < 128; i++)
	{
		_userName[i] = GAME()->GetCharAt(_buffer, SAVE_FORMAT_USER_NAME + i);
	}

	CHARATER().SetLVLAlive(GAME()->GetByteAt(_buffer, SAVE_FORMAT_LVL_ALIVE));
	CHARATER().SetLVLRevolver(GAME()->GetByteAt(_buffer, SAVE_FORMAT_LVL_REVOLVER));
	CHARATER().SetLVLFastTrigger(GAME()->GetByteAt(_buffer, SAVE_FORMAT_LVL_FAST_TRGGER));
	CHARATER().SetLVLSpeed(GAME()->GetByteAt(_buffer, SAVE_FORMAT_LVL_SPEED));
	CHARATER().SetLVLMagnet(GAME()->GetByteAt(_buffer, SAVE_FORMAT_LVL_MAGNET));
	CHARATER().SetLVLMachete(GAME()->GetByteAt(_buffer, SAVE_FORMAT_LVL_MACHETE));
	CHARATER().SetLVLMule(GAME()->GetByteAt(_buffer, SAVE_FORMAT_LVL_MULE));
	CHARATER().SetLVLCavalry(GAME()->GetByteAt(_buffer, SAVE_FORMAT_LVL_CAVALRY));

	_isUnlockMachete = GAME()->GetByteAt(_buffer, SAVE_FORMAT_UNLOCK_MACHETE);
	_isUnlockMule = GAME()->GetByteAt(_buffer, SAVE_FORMAT_UNLOCK_MULE);
	_isUnlockCavalry = GAME()->GetByteAt(_buffer, SAVE_FORMAT_UNLOCK_CAVALRY);

	_lvlLocation = GAME()->GetByteAt(_buffer, SAVE_FORMAT_LVL_LOCATION);
	_currentLocation = GAME()->GetByteAt(_buffer, SAVE_FORMAT_CURRENT_LOCATION);
	_stateBoss = GAME()->GetByteAt(_buffer, SAVE_FORMAT_STATE_BOSS);


	_removeAdd = GAME()->GetByteAt(_buffer, SAVE_FORMAT_ADD_REMOVE);
	TUTORIAL()._step = GAME()->GetByteAt(_buffer, SAVE_FORMAT_TUTORIAL_STEP);

	_total_coin = GAME()->GetInt32At(_buffer, SAVE_FORMAT_TOTAL_COIN);
	_completeAdventureMode = GAME()->GetByteAt(_buffer, SAVE_FORMAT_ADVENTURE_MODE);


	//update version 1
	for (int i = 0; i < MAX_ACHIEVEMENT_VER_0; i++)
	{
		ACHIEVEMENT()._percent[i] = GAME()->GetInt16At(_buffer, SAVE_FORMAT_ACHIEVEMENT_PERCENT + i * 2);
		ACHIEVEMENT()._isComplete[i] = GAME()->GetByteAt(_buffer, SAVE_FORMAT_ACHIEVEMENT_COMPLETE + i);
	}

	ACHIEVEMENT()._totalEnemyDie = GAME()->GetInt16At(_buffer, SAVE_FORMAT_TOTAL_ENEMY_DIE);
	ACHIEVEMENT()._totalCoinColect = GAME()->GetInt32At(_buffer, SAVE_FORMAT_TOTAL_COIN_COLECT);
	ACHIEVEMENT()._totalEnemyJumpBarrelDie = GAME()->GetInt16At(_buffer, SAVE_FORMAT_TOTAL_ENEMY_JUMP_BARREL);
	ACHIEVEMENT()._totalRocketOpenBarrel = GAME()->GetInt16At(_buffer, SAVE_FORMAT_TOTAL_ROCKET_OPEN_BARREL);
	ACHIEVEMENT()._totalBombOpenBarrel = GAME()->GetInt16At(_buffer, SAVE_FORMAT_TOTAL_BOMB_OPEN_BARREL);
	ACHIEVEMENT()._totalBombActive = GAME()->GetInt16At(_buffer, SAVE_FORMAT_TOTAL_BOMB_ACTIVE);
	ACHIEVEMENT()._totalEnemyDieByRocket = GAME()->GetInt16At(_buffer, SAVE_FORMAT_TOTAL_ENEMY_DIE_BY_ROCKET);
	ACHIEVEMENT()._totalEnemyDieByBomb = GAME()->GetInt16At(_buffer, SAVE_FORMAT_TOTAL_ENEMY_DIE_BY_BOMB);

	_currentUseCharater = GAME()->GetByteAt(_buffer, SAVE_FORMAT_CURRENT_USE_CHARATER);
	for (int i = 0; i < MAX_CHARATER; i++)
	{
		_unlockCharater[i] = GAME()->GetByteAt(_buffer, SAVE_FORMAT_UNLOCK_CHARATER + 1 * i);
		_useCharacter[i] = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USE_CHARATER + 1 * i);
	}

	_play_game_num = GAME()->GetInt32At(_buffer, SAVE_FORMAT_USER_PLAY_NUM);

	_hadRemovedAds = GAME()->GetByteAt(_buffer, SAVE_FORMAT_SAVE_REMOVE_ADS);

	ACHIEVEMENT()._numEnemyDie = GAME()->GetInt32At(_buffer, SAVE_FORMAT_SAVE_NUM_ENEMYDIE_SINGLE_RUN);
	ACHIEVEMENT()._numPowerUse = GAME()->GetInt32At(_buffer, SAVE_FORMAT_SAVE_POWER_USE);
	ACHIEVEMENT()._numEnemyJumpBarrelDie = GAME()->GetInt32At(_buffer, SAVE_FORMAT_SAVE_NUM_ENEMYDIE_JUMP_BARREL_TOTAL);
	ACHIEVEMENT()._totalRocketOpenBarrel = GAME()->GetInt32At(_buffer, SAVE_FORMAT_SAVE_OPEN_ROCKET_BARREL_TOTAL);
	ACHIEVEMENT()._numCoinColect = GAME()->GetInt32At(_buffer, SAVE_FORMAT_SAVE_COIN_COLLECT_SINGLE_RUN);
	ACHIEVEMENT()._totalCoinColect = GAME()->GetInt32At(_buffer, SAVE_FORMAT_SAVE_COIN_COLLECT_TOTAL);
	ACHIEVEMENT()._totalBombActive = GAME()->GetInt32At(_buffer, SAVE_FORMAT_SAVE_BOMB_ACTIVE_TOTAL);

	//update version 3
	for (int i = 0; i < MAX_QUEST; i++)
	{
		ACHIEVEMENT()._currentQuest[i] = GAME()->GetInt16At(_buffer, SAVE_FORMAT_QUEST_PERCENT + i * 2);//fix bug save percen
		ACHIEVEMENT()._isQuestComplete[i] = GAME()->GetByteAt(_buffer, SAVE_FORMAT_QUEST_COMPLETE + i);
		game->_quest_info[i]._id = GAME()->GetInt16At(_buffer, SAVE_FORMAT_QUEST_ID + i * 2);
	}


	play_current_date = GAME()->GetInt32At(_buffer, SAVE_FORMAT_USER_CURRENT_DATE);
	//Reset quest


	_dailyQuest_curDate = GAME()->GetInt32At(_buffer, SAVE_FORMAT_DAILYQUEST_CUR_DATE);
	ACHIEVEMENT()._numDailyBombActive = GAME()->GetInt16At(_buffer, SAVE_FORMAT_NUM_DAILY_BOM_ACTIVE);
	ACHIEVEMENT()._totalDailyCoinCollect = GAME()->GetInt32At(_buffer, SAVE_FORMAT_TOTAL_DAILY_COIN_COLL);
	ACHIEVEMENT()._numDailyEnemyJumpBarrel = GAME()->GetInt16At(_buffer, SAVE_FORMAT_NUM_DAILY_ENEMY_BY_BARREL);
	ACHIEVEMENT()._numDailyRocketByBarrel = GAME()->GetInt16At(_buffer, SAVE_FORMAT_NUM_DAILY_ROCKET_BY_BARREL);
	ACHIEVEMENT()._numDailyUsePowerUp = GAME()->GetInt16At(_buffer, SAVE_FORMAT_NUM_DAILY_USE_PWUP);
	ACHIEVEMENT()._dailyQuestCurrent = GAME()->GetInt16At(_buffer, SAVE_FORMAT_DAILY_QUEST_CURRENT);

	_hasRate = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_HAS_RATE);
	isFirstPlay = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_IS_FIRST_PLAY);
	_num_play_in_asession = GAME()->GetInt16At(_buffer, SAVE_FORMAT_USER_NUM_PLAY_IN_ASESSION);
	_rate_cancel_count = GAME()->GetInt16At(_buffer, SAVE_FORMAT_USER_RATE_CANCEL_COUNT);


	//ver8
	for (int i = 0; i<128; i++)
	{
		_game_promo_deviceID[i] = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_DEVICE_ID);
		_game_promo_icon_id[i] = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_ICON_ID);
		_game_promo_link_store[i] = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_LINK_STORE);
	}
	_game_promo_delay_show = GAME()->GetInt64At(_buffer, SAVE_FORMAT_USER_GAME_PROMO_DELAY_SHOW);
	_game_promo_has_show = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_HAS_SHOW);
	_game_promo_order = GAME()->GetInt16At(_buffer, SAVE_FORMAT_USER_GAME_PROMO_ORDER);
	_game_promo_revision = GAME()->GetInt16At(_buffer, SAVE_FORMAT_USER_GAME_PROMO_REVISION);
	_has_touch_promo = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_HAS_TOUCH_PROMO);

	if (_versionFormat < CURRENT_SAVE_VERSION)
	{
		SynForNewVersion(_versionFormat + 1);
		DataSave();
	}

	printf("\n ========= END LOAD ========== \n");

}

int CUser::getCurrentDate() {
	time_t t = time(NULL);
	struct tm * now = localtime(&t);
	return now->tm_yday;
}

void CUser::AddCoin(INT32 value)
{
	if (value > 0)
	{
		_total_coin += value;
	}
	_coin += value;
	if (value > 0)
	{
		MAIN_GAME()->_increase = true;
		MAIN_GAME()->_descrease = false;
		MAIN_GAME()->_deltaBone = (_coin - MAIN_GAME()->_coin_scroll) / 40;
		if (MAIN_GAME()->_deltaBone == 0)
		{
			MAIN_GAME()->_deltaBone = 1;
		}
	}
	else
	{
		MAIN_GAME()->_increase = false;
		MAIN_GAME()->_descrease = true;
		MAIN_GAME()->_deltaBone = (MAIN_GAME()->_coin_scroll - _coin) / 40;
		if (MAIN_GAME()->_deltaBone == 0)
		{
			MAIN_GAME()->_deltaBone = 1;
		}
	}
	//DataSave();
}



void CUser::DataDefaultLocal()
{
	MAIN_GAME()->_isMusicEnable = true;
	MAIN_GAME()->_isSoundEnable = true;
	MAIN_GAME()->_isNotiEnable = true;

	MAIN_GAME()->_isNotifySpeed = NOTIFY_TYPE_NONE;
	MAIN_GAME()->_isNotifyRevolver = NOTIFY_TYPE_NONE;
	MAIN_GAME()->_isNotifyFastTrigger = NOTIFY_TYPE_NONE;
	MAIN_GAME()->_isNotifyMagnet = NOTIFY_TYPE_NONE;
	MAIN_GAME()->_isNotifyMachete = NOTIFY_TYPE_NONE;
	MAIN_GAME()->_isNotifyMule = NOTIFY_TYPE_NONE;
	MAIN_GAME()->_isNotifyCavalry = NOTIFY_TYPE_NONE;
}
void CUser::DataSaveLocal()
{
	DataToBufferLocal();

#if defined(MAC_OS) || defined(_WINDOWS_PHONE8) || defined (ANDROID_OS)
	OS_SaveAppData(SAVE_LOCAL_FILE_NAME, _buffer_local, SAVE_LOACL_FORMAT_END, false);
#else

	FILE * stream = NULL;

	stream = fopen(SAVE_LOCAL_FILE_NAME, "wb");

	if (stream != NULL)
	{
		fwrite((void *)_buffer_local, sizeof(BYTE), SAVE_LOACL_FORMAT_END, stream);
	}

	if (stream != NULL)
	{
		fclose(stream);
	}
#endif
}

void CUser::DataLoadLocal()
{
	int saved = 0;
	Buffer* buff = OS_LoadAppData(SAVE_LOCAL_FILE_NAME);
	if (buff != NULL)
	{
		saved = buff->Length();
		memcpy(_buffer_local, buff->Data(), saved);
		delete buff;
	}

	if (saved == 0)
	{
		DataDefaultLocal();
		DataSaveLocal();
	}
	else
	{
		DataFromBufferLocal();
	}
}

void CUser::DataToBufferLocal()
{
	GAME()->SetByteAt(_buffer_local, SAVE_LOACL_FORMAT_MUSIC, MAIN_GAME()->_isMusicEnable);
	GAME()->SetByteAt(_buffer_local, SAVE_LOACL_FORMAT_SOUND, MAIN_GAME()->_isSoundEnable);
	GAME()->SetByteAt(_buffer_local, SAVE_LOACL_FORMAT_NOTI, MAIN_GAME()->_isNotiEnable);

	GAME()->SetByteAt(_buffer_local, SAVE_LOACL_FORMAT_NOTIFY_SPEED, MAIN_GAME()->_isNotifySpeed);
	GAME()->SetByteAt(_buffer_local, SAVE_LOACL_FORMAT_NOTIFY_REVOLVER, MAIN_GAME()->_isNotifyRevolver);
	GAME()->SetByteAt(_buffer_local, SAVE_LOACL_FORMAT_NOTIFY_FAST_TRIGGER, MAIN_GAME()->_isNotifyFastTrigger);
	GAME()->SetByteAt(_buffer_local, SAVE_LOACL_FORMAT_NOTIFY_MAGNET, MAIN_GAME()->_isNotifyMagnet);
	GAME()->SetByteAt(_buffer_local, SAVE_LOACL_FORMAT_NOTIFY_MACHETE, MAIN_GAME()->_isNotifyMachete);
	GAME()->SetByteAt(_buffer_local, SAVE_LOACL_FORMAT_NOTIFY_MULE, MAIN_GAME()->_isNotifyMule);
	GAME()->SetByteAt(_buffer_local, SAVE_LOACL_FORMAT_NOTIFY_CAVALRY, MAIN_GAME()->_isNotifyCavalry);
}

void CUser::DataFromBufferLocal()
{
	MAIN_GAME()->_isMusicEnable = GAME()->GetByteAt(_buffer_local, SAVE_LOACL_FORMAT_MUSIC);
	MAIN_GAME()->_isSoundEnable = GAME()->GetByteAt(_buffer_local, SAVE_LOACL_FORMAT_SOUND);
	MAIN_GAME()->_isNotiEnable = GAME()->GetByteAt(_buffer_local, SAVE_LOACL_FORMAT_NOTI);

	MAIN_GAME()->_isNotifySpeed = GAME()->GetByteAt(_buffer_local, SAVE_LOACL_FORMAT_NOTIFY_SPEED);
	MAIN_GAME()->_isNotifyRevolver = GAME()->GetByteAt(_buffer_local, SAVE_LOACL_FORMAT_NOTIFY_REVOLVER);
	MAIN_GAME()->_isNotifyFastTrigger = GAME()->GetByteAt(_buffer_local, SAVE_LOACL_FORMAT_NOTIFY_FAST_TRIGGER);
	MAIN_GAME()->_isNotifyMagnet = GAME()->GetByteAt(_buffer_local, SAVE_LOACL_FORMAT_NOTIFY_MAGNET);
	MAIN_GAME()->_isNotifyMachete = GAME()->GetByteAt(_buffer_local, SAVE_LOACL_FORMAT_NOTIFY_MACHETE);
	MAIN_GAME()->_isNotifyMule = GAME()->GetByteAt(_buffer_local, SAVE_LOACL_FORMAT_NOTIFY_MULE);
	MAIN_GAME()->_isNotifyCavalry = GAME()->GetByteAt(_buffer_local, SAVE_LOACL_FORMAT_NOTIFY_CAVALRY);


}
void CUser::CheckLocalAndCloudSaveFile(bool isauto)
{
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
#if ANDROID_OS
	if (!Device::IsNetworkAvailable())
	{
		if (!isauto)
		{
			OS_ShowMessageBox("Error!", "Connection is not available!");
		}
		return;
	}

	if (!GAME()->_async_task_manager._is_login_google)
	{
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_IS_LOG_IN_GOOGLE_PLAY);
		GAME()->_async_task_manager._is_force_login_google_play = true;
		GAME()->_async_task_manager._on_process_sync_cloud = true;
		return;
	}
	Buffer * buff = OS_LoadAppData(CLOUD_SAVE_FILE_NAME);
	if (buff != NULL)
	{
		char* server_buff = (char*)buff->Data();
		if (!isauto)
		{
			GAME()->_async_task_manager._on_process_sync_cloud = true;
		}
		int svScore = GAME()->GetInt32At(server_buff, SAVE_FORMAT_HIGHT_SCORE);
		int svNumPlay = GAME()->GetInt16At(server_buff, SAVE_FORMAT_USER_PLAY_NUM);
		int location = GAME()->GetByteAt(server_buff, SAVE_FORMAT_LVL_LOCATION);
		PDEBUG("\n from cloud battle_cur = %d -- _highScore:%d", svScore, _highScore);
		INT_TIME ver = GAME()->GetInt64At(server_buff, SAVE_FORMAT_CURRENT_VERSION);
		PDEBUG("\n #icloud sv_version = %lld, local_version = %d", ver, CURRENT_SAVE_VERSION);
		PDEBUG("\n location = %d -- _lvlLocation:%d", location, _lvlLocation);

		if (ver < CURRENT_SAVE_VERSION)
		{
			if (svScore > _highScore)
			{
				if (!isauto)
				{
					ConfirmSyncSaveFromCloud();
				}
			}
			else
			{
				ConfirmSyncSaveToCloud(isauto);
			}
		}
		else
		{
			if (_highScore > 0)
			{
				if (svScore < _highScore)
				{
					ConfirmSyncSaveToCloud(isauto);
				}
				else if (svScore == _highScore)
				{
					if (!isauto)
					{
						if (svNumPlay > _play_game_num)
						{
							ConfirmSyncSaveFromCloud();
						}
						else if (svNumPlay < _play_game_num)
						{
							ConfirmSyncSaveToCloud(isauto);
						}
						else
						{
							OS_ShowMessageBox("Congratulations!", "Your data has been saved!");
							GAME()->_async_task_manager._on_process_sync_cloud = false;
						}
					}
				}
				else if (svScore > _highScore)
				{
					if (!isauto)
					{
						ConfirmSyncSaveFromCloud();
					}
				}
			}
			else
			{
				if (location > _lvlLocation)
				{
					if (!isauto)
					{
						ConfirmSyncSaveFromCloud();
					}
				}
				else if (location < _lvlLocation)
				{
					ConfirmSyncSaveToCloud(isauto);
				}
				else
				{
					if (!isauto)
					{
						if (svNumPlay > _play_game_num)
						{
							ConfirmSyncSaveFromCloud();
						}
						else if (svNumPlay < _play_game_num)
						{
							ConfirmSyncSaveToCloud(isauto);
						}
						else
						{
							OS_ShowMessageBox("Congratulations!", "Your data has been saved!");
							GAME()->_async_task_manager._on_process_sync_cloud = false;
						}
					}
				}
			}
		}

		buff->Release();
	}
	else
	{
		ConfirmSyncSaveToCloud(isauto);
	}
#endif
}
void CUser::ConfirmSyncSaveToCloud(bool isauto)
{
#if ANDROID_OS
	if (isauto)
	{
		UpdateCloudSaveFile();
		GAME()->_async_task_manager._is_force_sync_save_to_cloud = true;
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_UPDATE_CLOUND_SAVE);
	}
	else
	{
		sprintf(GAME()->_async_task_manager._message_box_title, "%s", "Google Drive sync");
		sprintf(GAME()->_async_task_manager._message_box_message, "%s", "Do you want to safeguard your progress to Google Drive?");
		GAME()->_async_task_manager._message_box_confirm_type = CAsyncTasks::enumCloudSaveConfirmType::USE_SAVE_FROM_LOCAL;
		GAME()->_async_task_manager._on_process_sync_cloud = true;
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_SHOW_MESSAGE_BOX);
	}
#endif
}
void CUser::ConfirmSyncSaveFromCloud()
{
#if ANDROID_OS
	sprintf(GAME()->_async_task_manager._message_box_title, "%s", "Google Drive backup");
	sprintf(GAME()->_async_task_manager._message_box_message, "%s", "Do you want to play with your latest Google Drive Backup?");
	GAME()->_async_task_manager._message_box_confirm_type = CAsyncTasks::enumCloudSaveConfirmType::USE_SAVE_FROM_CLOUD;
	GAME()->_async_task_manager._on_process_sync_cloud = true;
	GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_SHOW_MESSAGE_BOX);
#endif
}
void CUser::UpdateCloudSaveFile()
{
#if ANDROID_OS
	PDEBUG("UpdateCloudSaveFile with local");
	OS_SaveAppData(CLOUD_SAVE_FILE_NAME, _buffer, MAX_SAVE_BUFFER_CACHE, false);
#endif
}
void CUser::UpdateLocalSaveFile(Buffer* buffer)
{
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
#if ANDROID_OS
	PDEBUG("UpdateLocalSaveFile with cloud");
	memcpy(_buffer, buffer->Data(), SAVE_FORMAT_END);
	DataFromBuffer();
	DataToBuffer();
	//CheckUnlockAllAchievements();
	OS_SaveAppData(SAVE_FILE_NAME, _buffer, MAX_SAVE_BUFFER_CACHE, false);
	game->SetState(k_GS_MAINMENU);
	game->_is_ggplay_menu_close = true;
	game->_is_ggplay_menu_active = false;
#endif
}
void CUser::CheckUnlockAllAchievements()
{
	WildWildWestGame *game = (WildWildWestGame*)GAME()->_current_game;
#if ANDROID_OS
	bool has_achievement_need_unlock = false;
	//int total = GAME()->_poker_flat_game->_game_design.leveldesign.SheetAchievement.int32Value._totalData;
	if (GAME()->_async_task_manager._need_check_unlock_all_achievement)
	{
		GAME()->_async_task_manager._need_check_unlock_all_achievement = false;
		/*if (GAME()->num_big_flower >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[20] = true;
		}
		else if(GAME()->num_big_flower >= 500)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[19] = true;
		}
		if (GAME()->num_large_flower >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[22] = true;
		}
		else if (GAME()->num_large_flower >= 500)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[21] = true;
		}
		if (GAME()->num_lily >= 10000000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[4] = true;
		}
		else if (GAME()->num_lily >= 500000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[5] = true;
		}
		else if(GAME()->num_lily >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[6] = true;
		}

		if (GAME()->num_sunflower >= 10000000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[3] = true;
		}
		else if (GAME()->num_sunflower >= 500000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[2] = true;
		}
		else if (GAME()->num_sunflower >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[1] = true;
		}

		if (GAME()->num_violet >= 10000000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[9] = true;
		}
		else if (GAME()->num_violet >= 500000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[8] = true;
		}
		else if (GAME()->num_violet >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[7] = true;
		}

		if (GAME()->num_lotus >= 10000000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[12] = true;
		}
		else if (GAME()->num_lotus >= 500000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[11] = true;
		}
		else if (GAME()->num_lotus >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[10] = true;
		}

		if (GAME()->num_rose >= 10000000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[15] = true;
		}
		else if (GAME()->num_rose >= 500000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[14] = true;
		}
		else if (GAME()->num_rose >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[13] = true;
		}

		if (GAME()->num_orchild >= 10000000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[18] = true;
		}
		else if (GAME()->num_orchild >= 500000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[17] = true;
		}
		else if (GAME()->num_orchild >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[16] = true;
		}*/

		/*for (int i = 0; i < game->_hight_score; i++)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[i] = true;
		}*/
	}
	if (has_achievement_need_unlock && GAME()->_async_task_manager._is_login_google)
	{
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_UNLOCK_ACHIEVEMENT);
	}
#endif
}