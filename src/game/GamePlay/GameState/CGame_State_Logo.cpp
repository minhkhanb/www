#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"

#include "../Game_ExternFunctions.h"

//extern "C" void InitADManager();
//extern "C" void InitADBanner();
//extern "C" void InitTakePicture();
//extern "C" void takeAPicture();
//extern "C" void LoadImageToPngFile(const char* url,const char *file_name);
extern "C" void AuthTwitter();
extern "C" void CaptureScreenshot();
extern "C" void PostScreenToTwitter();
extern "C" void OS_ResetAllAchievement();

#define LOGO_STEP_LOAD_DEFAULT				0
#define LOGO_STEP_CHECK_NETWORK				1
#define LOGO_STEP_LOAD_IAP					2
#define LOGO_STEP_CHECK_LOGIN				3
#define LOGO_STEP_WAIT_LOGIN_COMPLETE		4
#define LOGO_STEP_LOAD_ACCOUNT_INFO			5
#define LOGO_STEP_CHECK_ACCOUNT				6
#define LOGO_STEP_CREATE_NEW_ACCOUNT		7
#define LOGO_STEP_SYN_SAVE_TO_SERVER		8
#define LOGO_STEP_SYN_SAVE_FROM_SERVER		9

#define LOGO_STEP_BEGIN_LOAD_RESOURCE		10


void WildWildWestGame::UpdateLogo()
{
	printf("\n _loading_step = %d", _loading_step);
	if (_sub_state == k_LOOP)
	{
		switch (_loading_step)
		{
		case LOGO_STEP_LOAD_DEFAULT:
			_user.DataLoad();
			_loading_step++;
#if defined (_WINDOWS_PHONE8) || defined(_WIN8)
			/*if (CGame::_device.ScreenSize.W == 800 || CGame::_device.ScreenSize.H == 800)
			{

			}
			else*/
			{
				GAME()->_admob_manager.prepareInterstitial();
			}

#endif
			break;
		case LOGO_STEP_CHECK_NETWORK:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8)
			if (OS_IsNetworkAvailable()) {
				AsynCheckServerRealIsAble(&_svc_check_server_able);
				if (_svc_check_server_able.Status == PineServiceResult::Responsed)
				{
					if (_svc_check_server_able.ResponseStatus == 0)
						_loading_step = LOGO_STEP_LOAD_IAP;
					else
					{
						_loading_step = LOGO_STEP_BEGIN_LOAD_RESOURCE;
					}
				}

			}
			else {
				_loading_step = LOGO_STEP_BEGIN_LOAD_RESOURCE;
			}
#elif defined (ANDROID_OS)
			GAME()->InitAds();
			GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_LOAD_IAP);
			_loading_step = LOGO_STEP_BEGIN_LOAD_RESOURCE;
#else
			_loading_step = BEGIN_LOADING_STEP_RESOURCE;
#endif
			break;
		case LOGO_STEP_LOAD_IAP:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8)
			_social.LoadIAP(&_svc_inapp_load);
			if (_svc_inapp_load.Status == PineServiceResult::Responsed)
			{
				if (_svc_inapp_load.ResponseStatus == 0)
				{
					_loaded_store = true;
				}
				_loading_step = LOGO_STEP_CHECK_LOGIN;
			}
#endif
			//_loading_step = LOGO_STEP_CHECK_LOGIN;
			break;

		case LOGO_STEP_CHECK_LOGIN:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8) || defined(ANDROID_OS)
			_account_mng.Type = PineAccountType::Facebook;
			_svc_acc_login.reset();
			_loading_step = LOGO_STEP_WAIT_LOGIN_COMPLETE;
#else
			_loading_step = BEGIN_LOADING_STEP_RESOURCE;
#endif
			break;
		case LOGO_STEP_WAIT_LOGIN_COMPLETE:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8)
			_social.LoginAccount(&_svc_acc_login, _account_mng.Type);
			if (_svc_acc_login.Status == PineServiceResult::Responsed)
			{
				if (_svc_acc_login.ResponseStatus == 0)
				{
					_account_mng.IsLogin = true;
					_svc_acc_load.reset();
					_loading_step = LOGO_STEP_LOAD_ACCOUNT_INFO;
				}
				else
				{
					_loading_step = BEGIN_LOADING_STEP_RESOURCE;
				}
			}
#else
			_loading_step = BEGIN_LOADING_STEP_RESOURCE;
#endif
			break;
		case LOGO_STEP_LOAD_ACCOUNT_INFO:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8)
			_social.LoadAccountInfo(&_svc_acc_load, _account_mng.Type, &_account_mng.Info);
			if (_svc_acc_load.Status == PineServiceResult::Responsed)
			{
				if (_svc_acc_load.ResponseStatus == 0)
				{
					_user._socialId = PINE_ACCOUNT_SOCIAL_ID(_account_mng.Info.Id);
					_user._userId = _account_mng.Info.Id;
					sprintf(_user._userName, "%s", _account_mng.Info.Name);
					sprintf(_user._avatarURL, "%s", _account_mng.Info.AvatarUrl);

					//_pine_social->SetInfomation(_user._socialId, _user._userName);
					_loading_step = LOGO_STEP_CHECK_ACCOUNT;

				}
				else
				{
					_account_mng.IsLogin = false;
					_loading_step = BEGIN_LOADING_STEP_RESOURCE;
				}
			}

#else
			_loading_step = BEGIN_LOADING_STEP_RESOURCE;
#endif
			break;
			//		case LOGO_STEP_LOAD_ACHIVEMENT_FB:
			//#if defined (_WINDOWS_PHONE8) || defined(_WIN8) || defined(ANDROID_OS)
			//			//_social.LoadAchievementsFB(&_svc_load_achivement_fb);
			//			_social.UpdateAchievementsFB(&_svc_update_achievement_fb, "699158856865846");
			//
			//			if (_svc_update_achievement_fb.Status == PineServiceResult::Responsed)
			//			{
			//				if (_svc_update_achievement_fb.ResponseStatus == 0)
			//				{
			//					//succced load achievements
			//					_loading_step = BEGIN_LOADING_STEP_RESOURCE;
			//				}
			//				else
			//				{
			//					//error
			//					_loading_step = BEGIN_LOADING_STEP_RESOURCE;
			//				}
			//			}
			//#else
			//			_loading_step = BEGIN_LOADING_STEP_RESOURCE;
			//#endif
			//			break;
			//		case LOGO_STEP_POST_HIGH_SCORE:
			//			{
			//#if defined (_WINDOWS_PHONE8) || defined(_WIN8) || defined(ANDROID_OS)
			//
			//				 _social.UpdateNewScore(&_svc_post_score,100);
			//				 if (_svc_post_score.Status == PineServiceResult::Responsed)
			//				 {
			//					 if (_svc_post_score.ResponseStatus == 0)
			//					 {
			//						 //succced post score
			//						 _loading_step = BEGIN_LOADING_STEP_RESOURCE;
			//					 }
			//					 else
			//					 {
			//						 //error
			//						 _loading_step = BEGIN_LOADING_STEP_RESOURCE;
			//					 }
			//				 }
			//#else
			//				_loading_step = BEGIN_LOADING_STEP_RESOURCE;
			//#endif
			//			}
			//			break;
		case LOGO_STEP_CHECK_ACCOUNT:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8)
			if (_user._userId > 0)
			{
				AsynCheckDataFromPineServer(&_svc_check_acc, _user._userId);
				if (_svc_check_acc.Status == PineServiceResult::Responsed)
				{
					if (_svc_check_acc.ResponseStatus == CHECKING_STATUS_NO_ACCOUNT)
					{
						//no data -> create new account
						_user._ower_save = _user._userId;
						_user.DataSave();
						_svc_create_new_acc.reset();
						_loading_step = LOGO_STEP_CREATE_NEW_ACCOUNT;
					}
					else if (_svc_check_acc.ResponseStatus == CHECKING_STATUS_NORMAL)
					{
						_svc_sync_save_to_server.reset();
						_loading_step = LOGO_STEP_SYN_SAVE_TO_SERVER;
					}
					else if (_svc_check_acc.ResponseStatus == CHECKING_STATUS_SWAP_ACCOUNT)
					{
						_svc_sync_save_from_server.reset();
						_loading_step = LOGO_STEP_SYN_SAVE_FROM_SERVER;
					}
					else if (_svc_check_acc.ResponseStatus == CHECKING_STATUS_OLD_DATA)
					{
						_svc_sync_save_from_server.reset();
						_loading_step = LOGO_STEP_SYN_SAVE_FROM_SERVER;
					}
					else
					{
						//_user.DataLoad();
						_loading_step = LOGO_STEP_BEGIN_LOAD_RESOURCE;
					}
				}
			}
			else
			{
				_loading_step = LOGO_STEP_BEGIN_LOAD_RESOURCE;
			}
#else
			_loading_step = LOGO_STEP_BEGIN_LOAD_RESOURCE;
#endif
			break;
		case LOGO_STEP_CREATE_NEW_ACCOUNT:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8)
			if (_user._socialId > 0)
			{
				AsynCreateNewAccount(&_svc_create_new_acc);
				if (_svc_create_new_acc.Status == PineServiceResult::Responsed)
				{
					_loading_step = LOGO_STEP_SYN_SAVE_TO_SERVER;
					_svc_sync_save_to_server.reset();
				}
			}
			else
			{
				_loading_step = LOGO_STEP_BEGIN_LOAD_RESOURCE;
			}
#endif
			break;
		case LOGO_STEP_SYN_SAVE_TO_SERVER:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8)
			AsynSyncSaveToServer(&_svc_sync_save_to_server);
			if (_svc_sync_save_to_server.Status == PineServiceResult::Responsed)
			{
				if (_svc_sync_save_to_server.ResponseStatus == 0)
				{
					_user._isLogin = true;
				}
				else
				{
					_user._isLogin = false;
				}
				_loading_step = LOGO_STEP_BEGIN_LOAD_RESOURCE;
			}
			else
			{
				_loading_step = LOGO_STEP_BEGIN_LOAD_RESOURCE;
			}
#else
			_loading_step = LOGO_STEP_BEGIN_LOAD_RESOURCE;
#endif
			break;
		case LOGO_STEP_SYN_SAVE_FROM_SERVER:
#if defined (_WINDOWS_PHONE8) || defined(_WIN8)
			AsynSyncSaveFromServer(&_svc_sync_save_from_server);
			if (_svc_sync_save_from_server.Status == PineServiceResult::Responsed)
			{
				if (_svc_sync_save_from_server.ResponseStatus == 0)
				{
					_user._isLogin = true;
				}
				else
				{
					_user._isLogin = false;
				}
				_user.DataToBuffer();
				_user._ower_save = _user._userId;
				_loading_step = LOGO_STEP_BEGIN_LOAD_RESOURCE;
			}




#else
			_loading_step = LOGO_STEP_BEGIN_LOAD_RESOURCE;
#endif
			break;
		case BEGIN_LOADING_STEP_RESOURCE:
			_autoLoadingStep = true;
#ifdef MAC_OS
			if (_game_id == APPLE_ID && !PineSocial::IsDeviceHacked())
			{
				PineSocial::LoadIAP();
			}
#endif
			break;
			//Load Menu Resources
		case 11:
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_SHOP, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_BG_MAIN_MENU, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_EFFECT_DUST, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_COIN, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_FONT_DISTANCE, false, !_optimizeDate);
			GET_SPRITE(SPRITEID_FONT_DISTANCE)->SetCharSpacing(5);
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_TOPEBOX_ADS, false, !_optimizeDate);
			break;

			//Load GamePlay Resources
			//Load Character
		case 12:
		{
			if (GAME()->_optimizeTexture) {
				GAME()->SetTextureScale(false);
			}
			GAME()->LoadSpriteCurrentVer(SPRITEID_CHARATER_01, false, !_optimizeDate);
			GET_SPRITE(SPRITEID_CHARATER_01)->unloadTexture(G());
			GAME()->LoadSpriteCurrentVer(SPRITEID_CHARATER_02, false, !_optimizeDate);
			GET_SPRITE(SPRITEID_CHARATER_02)->unloadTexture(G());
			GAME()->LoadSpriteCurrentVer(SPRITEID_CHARATER_03, false, !_optimizeDate);
			GET_SPRITE(SPRITEID_CHARATER_03)->unloadTexture(G());
			GAME()->LoadSpriteCurrentVer(SPRITEID_CHARATER_04, false, !_optimizeDate);
			GET_SPRITE(SPRITEID_CHARATER_04)->unloadTexture(G());
			GAME()->LoadSpriteCurrentVer(SPRITEID_CHARATER_05, false, !_optimizeDate);

			GET_SPRITE(SPRITEID_CHARATER_05)->unloadTexture(G());
			if (GAME()->_optimizeTexture) {
				GAME()->SetTextureScale(true);
			}
		}
		//Load Result Resources	
		break;
		case 13:
			if (GAME()->_optimizeTexture) {
				GAME()->SetTextureScale(true);
			}
			GAME()->LoadSpriteCurrentVer(SPRITEID_BG_01_PALYER_3, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_BG_01_PALYER_1, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_BG_01_PALYER_2, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_BG_01_PALYER_4, false, !_optimizeDate);
			if (GAME()->_optimizeTexture) {
				GAME()->SetTextureScale(false);
			}
			break;
		case 14:

			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_INGAME, false, !_optimizeDate);
			//GET_SPRITE(SPRITEID_HUD_INGAME)->unloadTexture(G());
			break;
		case 15:
			if (GAME()->_optimizeTexture) {
				GAME()->SetTextureScale(false);
			}
			GAME()->LoadSpriteCurrentVer(SPRITEID_EFFECT_PARTICAL, false, !_optimizeDate);
			if (GAME()->_optimizeTexture) {
				GAME()->SetTextureScale(true);
			}
			break;
		case 16:
			if (GAME()->_optimizeTexture) {
				GAME()->SetTextureScale(false);
			}
			GAME()->LoadSpriteCurrentVer(SPRITEID_FROM_COIN, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_FROM_OBSTACLE, false, !_optimizeDate);
			if (GAME()->_optimizeTexture) {
				GAME()->SetTextureScale(true);
			}
			break;
		case 17:
			if (GAME()->_optimizeTexture) {
				GAME()->SetTextureScale(false);
			}
			GAME()->LoadSpriteCurrentVer(SPRITEID_OBSTACLE, false, !_optimizeDate);
			if (GAME()->_optimizeTexture) {
				GAME()->SetTextureScale(true);
			}
			//GET_SPRITE(SPRITEID_OBSTACLE)->unloadTexture(G());
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_STICK, false, !_optimizeDate);
			//GET_SPRITE(SPRITEID_HUD_STICK)->unloadTexture(G());
			break;
		case 18:
			if (GAME()->_optimizeTexture) {
				GAME()->SetTextureScale(false);
			}
			GAME()->LoadSpriteCurrentVer(SPRITEID_OBSTACLE_ROCKET, false, !_optimizeDate);
			//GET_SPRITE(SPRITEID_OBSTACLE_ROCKET)->unloadTexture(G());
			GAME()->LoadSpriteCurrentVer(SPRITEID_EFFECT_EXPLOSION, false, !_optimizeDate);
			//GET_SPRITE(SPRITEID_EFFECT_EXPLOSION)->unloadTexture(G());
			if (GAME()->_optimizeTexture) {
				GAME()->SetTextureScale(true);
			}
			break;
		case 19:
			GAME()->LoadSpriteCurrentVer(SPRITEID_BG_01_PALYER_2A, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_MACHETE, false, !_optimizeDate);
			break;
		case 20:
			GAME()->LoadSpriteCurrentVer(SPRITEID_FONT_POWER, false, !_optimizeDate);

			break;
		case 21:

			GAME()->LoadSpriteCurrentVer(SPRITEID_BOSS_01, false, !_optimizeDate);
			break;
		case 22:
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_MULE, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_MEDAL, false, !_optimizeDate);
			break;
		case 23:


			break;
		case 24:
			GAME()->LoadSpriteCurrentVer(SPRITEID_EFFECT_FAST, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_TRESURE_CHESS, false, !_optimizeDate);
			break;
		case 25:
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_CHECK_POINT, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_FROM_ROCKET, false, !_optimizeDate);
			break;
		case 26:
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_RESULT, false, !_optimizeDate);
			//GET_SPRITE(SPRITEID_HUD_RESULT)->unloadTexture(G());
			GAME()->LoadSpriteCurrentVer(SPRITEID_FONT_RESULT, false, !_optimizeDate);
			//GET_SPRITE(SPRITEID_FONT_RESULT)->unloadTexture(G());
			GET_SPRITE(SPRITEID_FONT_RESULT)->SetCharSpacing(3);
			break;
		case 27:
			if (GAME()->_optimizeTexture) {
				GAME()->SetTextureScale(false);
			}
			for (int i = SPRITEID_ENEMY_01; i <= SPRITEID_ENEMY_05; i++)
			{
				GAME()->LoadSpriteCurrentVer(i, false, !_optimizeDate);
			}
			if (GAME()->_optimizeTexture) {
				GAME()->SetTextureScale(true);
			}
			break;
		case 28:
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_APPLE, false, !_optimizeDate);
			break;

		case 29:
			GAME()->LoadMusic(BGM_INGAME);
			GAME()->LoadMusic(BGM_MAIN_MENU);
			GAME()->LoadMusic(BGM_RESULT_SCREEN_WITH_NEW_HIGHSCORE);
			GAME()->LoadMusic(BGM_BOSS_FIGHT);

			GAME()->LoadSFX(SFX_BOMB);
			GAME()->LoadSFX(SFX_BUTTON);
			GAME()->LoadSFX(SFX_BUTTON_START);
			GAME()->LoadSFX(SFX_COIN);
			GAME()->LoadSFX(SFX_ENEMY_DIE_1);
			GAME()->LoadSFX(SFX_ENEMY_DIE_2);
			GAME()->LoadSFX(SFX_ENEMY_DIE_3);
			GAME()->LoadSFX(SFX_GUNSHOT);
			GAME()->LoadSFX(SFX_HORSE_UP_DOWN);
			GAME()->LoadSFX(SFX_MISSILES);
			GAME()->LoadSFX(SFX_MISSION_COMPLETED);
			GAME()->LoadSFX(SFX_MISSION_FAILED);
			GAME()->LoadSFX(SFX_POWERUP_COLLECTED);
			GAME()->LoadSFX(SFX_SCORE_COUNTING);
			GAME()->LoadSFX(SFX_BOSS_DEFEATED);
			GAME()->LoadSFX(SFX_UPGRADE);
			GAME()->LoadSFX(SFX_WARNING);
			GAME()->LoadSFX(SFX_GAMEOVER);
			GAME()->LoadSFX(SFX_GUNSHOT_BOSS);
			GAME()->LoadSFX(SFX_HORSE_RUN);
			GAME()->LoadSFX(SFX_TRAIN_START_STOP);
			GAME()->LoadSFX(SFX_TRAIN_WARNING);

			GAME()->LoadSFX(SFX_GUNSHOT_GAMEOVER);
			GAME()->LoadSFX(SFX_KNIFE);
			GAME()->LoadSFX(SFX_PLAYER_HURT);
			GAME()->LoadSFX(SFX_TRAIN_GOIN);
			GAME()->LoadSFX(SFX_BOSS_THROWING_BOMB);
			GAME()->LoadSFX(SFX_WARNING_BOSS);
			break;
		case 30:
			GAME()->LoadSpriteCurrentVer(SPRITEID_BG_SWAP_LAYER_1, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_BG_SWAP_LAYER_2, false, !_optimizeDate);
			break;

		case 31:
			GAME()->LoadSpriteCurrentVer(SPRITEID_BG_02_PALYER_1, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_BG_02_PALYER_2, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_BG_02_PALYER_3, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_BG_02_PALYER_4, false, !_optimizeDate);
			break;
		case 32:
			GAME()->LoadSpriteCurrentVer(SPRITEID_BOSS_02, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_BOSS_03, false, !_optimizeDate);
			break;
		case 33:
			GAME()->LoadSpriteCurrentVer(SPRITEID_BOSS_1_A, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_EFFECT_WAGON_PARTICAL, false, !_optimizeDate);
			break;
		case 34:
			GAME()->LoadSpriteCurrentVer(SPRITEID_EFFECT_TRAIN_SMOKE, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_EFFECT_GOLD_PARTICAL, false, !_optimizeDate);

			break;
		case 35:
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_TUTORIAL, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_BOSS_3_A, false, !_optimizeDate);
			break;
		case 36:
			//GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_ICON_LOADING, false, !_optimizeDate);
#if defined (ANDROID_OS)
			GAME()->_topebox_game.GetTopeboxAdsInfoRemoteConfig();
#endif
			break;
		case 37:
			//for (int i = SPRITEID_CHARATER_01; i <= SPRITEID_CHARATER_05; i++)
		{
#if defined (ANDROID_OS)
			if (GAME()->configDownloaded)
			{
				_autoLoadingStep = true;
				PDEBUG("\n configDownloaded:%d", GAME()->configDownloaded);
				_loading_step = 38;
			}
			else
			{
				_autoLoadingStep = false;
			}
#endif
		}
		break;
		}

		CGame::_this->Sleep(100);
		if (_autoLoadingStep)
		{
			_loading_step++;
			/*if (_loading_step == 39)
			{
				_loading_step = 50;
			}*/
		}

		if (_loading_step >= MAX_LOADING_STEP)
		{
			// _user.DataLoad();
			_user.DataLoadLocal();
			GAME()->_musicEnable = _isMusicEnable;
			GAME()->_soundEnable = _isSoundEnable;
			GAME()->_notificationEnable = _isNotiEnable;
			_achievement.LoadInfoAchievement();
			_achievement.LoadInfoQuestFromSave();
			_achievement.CheckQuestComplete();
			MenuGGPlayInit();
			RatingInit();
			if (TUTORIAL()._step == 0)
			{
				GAME()->LoadSpriteCurrentVer(SPRITEID_CHARATER_01, false, false);
				GAME()->LoadSpriteCurrentVer(SPRITEID_CHARATER_02, false, false);
				GAME()->LoadSpriteCurrentVer(SPRITEID_CHARATER_03, false, false);
				GAME()->LoadSpriteCurrentVer(SPRITEID_CHARATER_04, false, false);
				GAME()->LoadSpriteCurrentVer(SPRITEID_CHARATER_05, false, false);
				_animCoin.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_COIN), SPRITEID_HUD_COIN);
				_animCoin.SetAnim(0, true);
				SetState(k_GS_PLAYGAME);
			}
			else
			{
				SetState(k_GS_MAINMENU);
				Logs_GamePlaying_Restart(_user._num_play_in_asession, _user.isFirstPlay);
				_user.isFirstPlay = false;
			}
			if (_user._game_promo_delay_show < time(NULL))
			{
				GAME()->_topebox_game.SetDeviceID(_user._game_promo_deviceID);

			}
		}
	}
}

void WildWildWestGame::RenderLogo()
{
	if (_sub_state == k_INIT)
	{
		_optimizeDate = true;
		GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_ICON_LOADING, false, !_optimizeDate);
		GAME()->LoadSpriteCurrentVer(SPRITEID_BG_LOADING, false, !_optimizeDate);
		GAME()->LoadSpriteCurrentVer(SPRITEID_FONT_MONEY, false, !_optimizeDate);
		GET_SPRITE(SPRITEID_FONT_MONEY)->SetCharSpacing(5);

		_loading_step = LOGO_STEP_LOAD_DEFAULT;
		_autoLoadingStep = false;
		_sub_state = k_LOOP;
		_iconLoading.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_ICON_LOADING), SPRITEID_HUD_ICON_LOADING);
		_iconLoading.SetAnim(0, true);
		_iconLoading.SetPos(k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
		GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_ANDROID, false, !_optimizeDate);
		GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_DAILY_REWARD, false, !_optimizeDate);
		GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_DAILY_REWARD, false, !_optimizeDate);
		GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_COIN, false, !_optimizeDate);
		QuestInit();
		//topebox ads
		GAME()->_topebox_game.InitGamePromo(0, 0, 64, 64, TopeBox_PromoGame::enumTypeTopeboxAds::TOPEBOX_ADS_BANNER);
		GAME()->_topebox_game_scrollEff = 0;
	}
	if (_sub_state == k_LOOP)
	{
		float zoom = 1.0f;
		float offsetY = 0;
		if (CGame::_this->_device._version == SupportVersion::RESVER_960_640)
		{
			zoom = 1.20f;
			offsetY = -0.15 * 640 / 2;
		}
		if (CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
		{
			zoom = IPAD_ZOOM;
			offsetY = IPAD_OFFSET;
		}
#if defined (ANDROID_OS)
		if (CGame::_this->_current_version == VERSION_640x1136)
		{
			float zoomx = GAME()->_device.ScreenSize.W / 1136;
			float zoomy = GAME()->_device.ScreenSize.H / 640;
			zoom = zoomx > zoomy ? zoomx : zoomy;
			if (zoom < 1.0f) zoom = 1.0f;
		}
#endif
		//G()->SetScale(zoom*0.5f, zoom*0.5f, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
		GET_SPRITE(SPRITEID_BG_LOADING)->DrawFrame(G(), (int)0, (int)0, 0);
		//G()->ClearScale();
		int percent = _loading_step / (float)MAX_LOADING_STEP * 100;

		G()->SetScale(0.8f, 0.8f, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT - 70);
		GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT - 70 - offsetY, _HCENTER | _VCENTER, "Loading %d%%", percent);
		G()->ClearScale();

	}
}



void CGame::UnloadGameBackground()
{
	//printf("\nUnload Game Back ground");
	if (!_compete_backup_stock)
	{
		_step_force_loading = 0;
		//BackupAnimStock();
		for (int i = 0; i < TOTAL_SPRITE_INGAME; i++)
		{
			if (GET_SPRITE(i) != NULL)
				//if(_sprite_loaded[i])
			{
				//printf("\nUnload SpriteID = %d",i);
				//UnloadSpriteVer(i);
				//_sprite_loaded[i] = 1;
				GET_SPRITE(i)->unloadTexture(G());
			}
		}
		_compete_backup_stock = true;
		// printf("\nUnload background");
	}

}
bool CGame::LoadGameForceground()
{

	if (_step_force_loading >= TOTAL_SPRITE_INGAME + 10)
	{
		return true;
	}

	_compete_backup_stock = false;
	//printf("\nloading force ground = %d",_step_force_loading);
	//G()->SetColor(0);
	//G()->FillRect(0,0,k_SCREEN_WIDTH,k_SCREEN_HEIGHT);


	if (_step_force_loading >= 10)
	{
		for (int i = 0; i < 100; i++)
		{

			if (_sprite_loaded[_step_force_loading - 10])
			{
				//printf("\n_step_force_loading-10 = %d",(_step_force_loading-10));
				LoadSpriteCurrentVer(_step_force_loading - 10, _sprite_antialias_loaded[_step_force_loading - 10], _sprite_optimized_loaded[_step_force_loading - 10]);
			}
			_step_force_loading++;
			if (_step_force_loading >= TOTAL_SPRITE_INGAME + 10)
			{
				//RestoreAnimStock();

				return true;
			}

		}
	}
	else
	{
		_step_force_loading++;
	}
	return false;
}

