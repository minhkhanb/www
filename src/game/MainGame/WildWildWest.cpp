#include "pch.h"
#include "WildWildWest.h"
#if defined (ANDROID_OS)
extern "C" void PurchaseProductWithType(const char *indentify, int type);
extern "C" void PurchaseProduct(const char *indentify);
#endif
void WildWildWestGame::Init()
{
	_check_syn_save_complete = 0;

	_online_town = 0;
	_online_howfar = 0;
	_online_total_coin = 0;
	_syn_save_state = SYN_SAVE_NONE;
	_can_submit = false;

	_countShowAdsMods = 0;
	_countShowAdsUnity = 1;
	_countShowPineAds = 0;
	_stateShowPineAds  =0;
	_notShowPineAds = false;

	_isProssesIap = false;

	_playWitchEndlest = false;
	_increase = false;
	_descrease = false;
	_newScore = false;
	_isGameOver = false;
	_playWitchEndlest = false;

	_battleMap._usedBG_1 = false;
	_battleMap._usedBG_2 = false;

	_counttoPlay = 3;
    _touchID = -1;
    
    _typeTouchShop = -1;
	num_playing_restart = 0;
	num_continue = 0;
	_count_frame_show_ads = 0;
	_time_delay_show_ads = (TIME_DELAY_SHOW_ADS * 60)/2;
	_show_ads_interrupt = false;
	_count_frame_waiting = 0;
	_purchase_iap_id = -1;
#if ANDROID_OS
	GAME()->_gameBoardScoreItem.AccountType = PineAccountType::GooglePlay;
	sprintf(GAME()->_gameBoardScoreItem.BoardId, "%s", GP_LEADERBOARD_STORIES);
#endif

	GAME()->is_reset_quest = false;

	
	playing_current_date_local = _user.getCurrentDate();
	_count_frame_delay_show_ads = 0;
}
void WildWildWestGame::UpdateAndRender()
{
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	G()->ClearClip();
	G()->SetOpacity(100);
	/*if (!GAME()->LoadGameForceground())
	{
		return;
	}*/
	CheckAndSynSave();
	//Utils::Loga("\nUpdate And Render");
	switch (_current_state)
	{
	case k_GS_TEST:
		//RenderTest();
		break;
	case k_GS_LOGO:
		RenderLogo();
		//UpdateLogo();
		break;
	case k_GS_PLAYGAME:
		RenderStatePlayGame();
		//UpdateStatePlayGame();
		break;
	case k_GS_RESULT:
		RenderStateResult();
		//UpdateStateResult();
		break;
	case k_GS_IGM:
		RenderStateIGM();
		//UpdateStateIGM();
		break;
	case k_GS_MAINMENU:
		RenderStateMainMenu();
		//UpdateStateMainMenu();
		break;
	case k_GS_TUTORIAL:
		RenderStateTutorial();
		//UpdateStateTutorial();
		break;
	case k_GS_CONFIRM:
		RenderStateConfirm();

		break;
	}

	if (_purchase_iap_id >= 0 && _purchase_iap_id < TOTAL_IAP_NAME_DES_ROW)
	{

#if defined (ANDROID_OS)
		if (_purchase_iap_id == 4 )
		{
			PurchaseProduct("www_adsremove");
		}
		else
		{
			PurchaseProductWithType(GDS_Exporter::iap_name_des[TOTAL_IAP_NAME_DES*_purchase_iap_id + IAP_NAME_DES_GOOGLE_ID], 3);
		}
#endif
		_purchase_iap_id = -1;
	}
		



#if ADD_ADVERT
	if (GAME()->_advert.HasPopup())
	{
		GAME()->_advert.Render();
		GAME()->_advert.Update();
		return;
	}
#endif

	if (_current_state >= k_GS_LOGO)
	{
		if (GAME()->time_get_remote_config > 0)
		{
			GAME()->time_get_remote_config--;
			if (GAME()->time_get_remote_config <= 0)
			{
				GAME()->time_get_remote_config = 0;
				GAME()->_topebox_game.GetTopeboxAdsInfoRemoteConfig();
			}

		}
	}

	if (_current_state > k_GS_LOGO)
	{

		time_t seconds;
		seconds = time(NULL);
		int daysSince1970 = seconds / (60 * 60 * 24);
		//PDEBUG("\n daysSince1970:%d,_user._dailyQuest_curDate:%d", daysSince1970, _user._dailyQuest_curDate);
		if (daysSince1970 > _user._dailyQuest_curDate)
		{
			_achievement.ResetInfoQuest();
			_user._dailyQuest_curDate = daysSince1970;
		}
		UpdateStatusAchievement();
		_achievement.UpdateInfoQuest();
		if (_time_delay_show_ads > 0)
		{
			_time_delay_show_ads--;
		}
		if (!_user._hadRemovedAds)
		{
		
			if (_count_frame_show_ads > 0)
			{
				_count_frame_show_ads--;
				if (_count_frame_show_ads <= 0)
				{
					_count_frame_show_ads = 0;
					_time_delay_show_ads = TIME_DELAY_SHOW_ADS * 60;
					PDEBUG("\n _topebox_game.show_banner_topebox_active:%d", GAME()->_topebox_game.show_banner_topebox_active);
					if (GAME()->_topebox_game.show_banner_topebox_active)
					{
						//show topebox ads
						if (GAME()->_topebox_game.typeAds == TopeBox_PromoGame::enumTypeTopeboxAds::TOPEBOX_ADS_BANNER)
						{
							GAME()->_topebox_game.count_show_close_banner_topebox = 0;
							GAME()->_topebox_game._is_close_ads = false;
							GAME()->_topebox_game.render_banner_topebox = true;
						}
					}
					else
					{
						GAME()->PrepareAndShowInterstitial();
//#if ANDROID_OS
//						if (GAME()->HasAds())
//						{
//							GAME()->ShowAds();
//						}
//#endif
					}
				}
			}
		}
		if (_is_waiting_for_reward && !_on_video_ads_reward)
		{
			G()->SetColor(0xaa000000);
			G()->FillFullScreen(true);
			_count_frame_waiting+=2;
			if (_count_frame_waiting >= 180)
			{
				_count_frame_waiting -= 180;
			}
			int sec = _count_frame_waiting / 60;
			switch (sec)
			{
			case 0:
				GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2 - 60, k_SCREEN_HEIGHT / 2, _LEFT | _VCENTER, "%s", "Loading.");
				break;
			case 1:
				GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2 - 60, k_SCREEN_HEIGHT / 2, _LEFT | _VCENTER, "%s", "Loading..");
				break;
			case 2:
				GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2 - 60, k_SCREEN_HEIGHT / 2, _LEFT | _VCENTER, "%s", "Loading...");
				break;
			default:
				break;
			}
			
		}
		if (_current_state == k_GS_PLAYGAME)
		{
			//PDEBUG("\n _time_delay_show_ads:%d --- _user._removeAdd:%d", _time_delay_show_ads, _user._removeAdd);
			if (_count_frame_delay_show_ads > 0)
			{
				_count_frame_delay_show_ads--;
				if (!_user._removeAdd && _time_delay_show_ads <= 0)
				{
					if (_countShowPineAds == 1)
					{
						_stateShowPineAds = 1;
					}
					else
					{
						if (_countShowAdsMods <= 1)
						{
							if (_countShowAdsUnity <= 0)
							{
#if defined (_WINDOWS_PHONE8) || defined(_WIN8)
								GAME()->_admob_manager.showInterstitial();
#elif defined (ANDROID_OS)
								//GAME()->PrepareAndShowVideoAds();
								//if (GAME()->HasAds())
								//{
								GAME()->PrepareAndShowInterstitial();
								//}
#elif defined (MAC_OS)
								int type = PineAdBox::HasVideoAds();
								if (type >= 0)
								{
									PineAdBox::ShowVideoAds(type);
								}
#endif
								_countShowAdsUnity = 2;
							}
							else
							{
#if defined (_WINDOWS_PHONE8) || defined(_WIN8)
								GAME()->_admob_manager.showInterstitial();
#elif defined (ANDROID_OS)
								//if (GAME()->HasAds())
								{
									//GAME()->ShowAds();
									GAME()->PrepareAndShowInterstitial();
								}
#elif defined (MAC_OS)
								PineAdBox::SetReactiveAdvert();
								PineAdBox::ShowAdvertFullScreen();
#endif
								_countShowAdsMods = 1;
								_countShowAdsUnity--;
							}
						}
					}
				}
			}
			
		}
#pragma region TOPEBOX_PROMO_GAME
		if (_user._game_promo_has_show)
		{

#if defined (ANDROID_OS)
			if (GAME()->_topebox_game._is_close_ads)
			{
				//_topebox_game.render_banner_topebox = false;
				GAME()->_topebox_game.render_banner_topebox = false;
				if (GAME()->_topebox_game.count_skip_banner >= 3)
				{
					_user._game_promo_has_show = false;
					_user._game_promo_delay_show = time(NULL) + TIME_DELAY_SHOW_TOPEBOX_ADS;
					GAME()->_topebox_game.show_banner_topebox_active = false;
					GAME()->_topebox_game.count_skip_banner = 3;
					_user.DataSave();
					PDEBUG("\n ###########Skip 3 lan");
				}
			}
#endif
			//PDEBUG("\n _topebox_game.GamePromoHasTouchDown():%d -- _canTouch:%d", _topebox_game.GamePromoHasTouchDown(), _canTouch);
			if (GAME()->_topebox_game.count_show_close_banner_topebox > 0)
			{
				GAME()->_topebox_game.count_show_close_banner_topebox--;
				//PDEBUG("\n --------------------->_topebox_game.count_show_close_banner_topebox:%d", _topebox_game.count_show_close_banner_topebox);
				if (GAME()->_topebox_game.count_show_close_banner_topebox <= 0)
				{
					GAME()->_topebox_game.count_show_close_banner_topebox = -10;
				}
			}
			if (GAME()->_topebox_game.GamePromoHasTouchDown())
			{
#if !defined (_WINDOWS)
				if (Device::IsNetworkAvailable())
#endif
				{
					_show_ads_interrupt = false;
					_user._game_promo_has_show = false;
					GAME()->_topebox_game.show_banner_topebox_active = false;
					GAME()->_topebox_game.render_banner_topebox = false;
					_user._game_promo_order++;
					_user._game_promo_delay_show = time(NULL) + TIME_DELAY_SHOW_TOPEBOX_ADS;
					//_topebox_game.ResponseRequest();
					PDEBUG("\n ##### Open URL:_link_store = %s \n", GAME()->_topebox_game._link_store);
#if defined (ANDROID_OS) || defined (MAC_OS)
					PDEBUG("\n ##### Open URL:linkStore = %s \n", GAME()->_topebox_game._link_store);
					PineSocial::OpenURL(GAME()->_topebox_game._link_store);


#endif
					/*char title[128];
					char decs[1024];
					sprintf(title, "%s Click", "Topebox-Ads");
					sprintf(decs, "%s Click", "Topebox-Ads");
					char type[128];
					sprintf(type, "%s", "topebox_ads");
					#if defined (MAC_OS) || (ANDROID_OS)
					PDEBUG("\n logContent title:%s --- decs:%s --- type:%s", title, decs, type);
					Device::GA_LogContents(title, decs, type);
					#endif*/
					_user.DataSave();
				}
			}
		}
		//if (!(_menu_button_active || _menu_button_scrolling))
		{
			if (_user._game_promo_has_show)
			{
				if (GAME()->_topebox_game.UpdateTouchGamePromo())
				{
					//return true;
				}
			}
		}

#pragma endregion
	}

	if (_current_state >= k_GS_MAINMENU)
	{
		
		if (!_is_ggplay_menu_close)
		{
			MenuGGPlayRender();
			
		}
		//QuestRender(0, 0, 100);
		
	}


	

	if (_isProssesIap || UpdateSynSave())
	{
		G()->SetColor(0xaa000000);
		G()->FillFullScreen(true);

		_iconLoading.DrawAnimObject(G());
		_iconLoading.UpdateAnimObject();
		return;
	}

	switch (_current_state)
	{
	case k_GS_TEST:
		//UpdateTest();
		break;
	case k_GS_LOGO:
		//RenderLogo();
		UpdateLogo();
		break;
	case k_GS_PLAYGAME:
		//RenderStatePlayGame();
		UpdateStatePlayGame();
		break;
	case k_GS_RESULT:
		//RenderStateResult();
		UpdateStateResult();
		break;
	case k_GS_IGM:
		//RenderStateIGM();
		UpdateStateIGM();
		break;
	case k_GS_MAINMENU:
		//RenderStateMainMenu();
		UpdateStateMainMenu();
		break;
	case k_GS_TUTORIAL:
		//RenderStateTutorial();
		UpdateStateTutorial();
		break;
	case k_GS_CONFIRM:
		UpdateStateConfirm();
		break;
	}



#if ADD_ADVERT
	if (_stateShowPineAds == 1)
	{
		GAME()->_advert.Init();
		_stateShowPineAds = 2;
	}
	else if (_stateShowPineAds == 2)
	{
		printf("\n CGame::_this->_advert.GetAppInstallState() =%d", CGame::_this->_advert.GetAppInstallState());
		if (CGame::_this->_advert.GetAppInstallState() == PINEADS_INSTALL_STATE_DOWNLOAD_DONE)
		{
			CGame::_this->_advert.SetAppInstallState(PINEADS_INSTALL_STATE_NONE);
			CGame::_this->_advert.SetDownloadCompete();
			_stateShowPineAds = 0;

		}
		else if (CGame::_this->_advert.GetAppInstallState() == PINEADS_INSTALL_STATE_DOWNLOAD_CANCEL)
		{
			CGame::_this->_advert.SetAppInstallState(PINEADS_INSTALL_STATE_NONE);
			_stateShowPineAds = 0;
		}
	}
#endif
}
//Update 27/11/2017
void WildWildWestGame::GooglePlayShowLeaderBoard()
{
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
#if ANDROID_OS
	if (GAME()->_async_task_manager._is_login_google)
	{
		PDEBUG("\nGAME()->_async_task_manager._on_process_show_leaderBoard: %d", GAME()->_async_task_manager._on_process_show_leaderBoard);
		if (!GAME()->_async_task_manager._on_process_show_leaderBoard)
		{

			//GAME()->_gameBoardScoreItem.Score = game->_hight_score;
			//GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_POST_SCORE);
			GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_SHOW_LEADERBOARD);
			GAME()->_async_task_manager._on_process_show_leaderBoard = true;
		}
	}
	else
	{
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_IS_LOG_IN_GOOGLE_PLAY);
		GAME()->_async_task_manager._is_force_login_google_play = true;
		GAME()->_async_task_manager._on_process_show_leaderBoard = true;
	}
#endif
}
void WildWildWestGame::GooglePlaySynDriver()
{
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;

#if ANDROID_OS
	if (!GAME()->_async_task_manager._on_process_sync_cloud)
	{
		game->_user.CheckLocalAndCloudSaveFile(false);
	}
#endif
}

void WildWildWestGame::GooglePlayShowAchievement()
{
#if ANDROID_OS
	if (GAME()->_async_task_manager._is_login_google)
	{
		if (!GAME()->_async_task_manager._on_process_show_achievement)
		{
			GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_SHOW_ACHIEVEMENTS);
			GAME()->_async_task_manager._on_process_show_achievement = true;
		}
	}
	else
	{
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_IS_LOG_IN_GOOGLE_PLAY);
		GAME()->_async_task_manager._is_force_login_google_play = true;
		GAME()->_async_task_manager._on_process_show_achievement = true;
	}
#endif
}

void WildWildWestGame::GooglePlaySubmitAchievement(int index)
{
#if defined (ANDROID_OS)
		if (GAME()->_async_task_manager._is_login_google)
		{
			GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[index] = true;
			GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_UNLOCK_ACHIEVEMENT);
		}
#endif
}

void WildWildWestGame::SubmitAchievement(int index)
{
	if (index < 0)
	{
		return;
	}
	_user._achievement_status[index] = CUser::ACHIEVEMENT_STATE_COMPLETED;
	GooglePlaySubmitAchievement(index);
}

void WildWildWestGame::UpdateStatusAchievement()
{
	bool issubmit = false;
	for (int i = 0; i < MAX_ACHIEVEMENT - 10; i++)
	{
		if (_user._achievement_status[i] == CUser::ACHIEVEMENT_STATE_COMPLETED)
		{
			issubmit = true;
			_user._achievement_status[i] = CUser::ACHIEVEMENT_STATE_SUBMITTING;
#if defined(ANDROID_OS)
			GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[i] = true;
#endif
		}
	}
	if (issubmit)
	{
#if defined(ANDROID_OS)
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_UNLOCK_ACHIEVEMENT);
#endif
	}
}
void WildWildWestGame::SetAchievementSubmitCompleted(int index)
{
	if (index < 0)
	{
		return;
	}
	_user._achievement_status[index] = CUser::ACHIEVEMENT_STATE_SUBMITED_SUCCESS;
	_user.DataSave();
}

int WildWildWestGame::GetPercent(int num, int max) {
	return num / (float)max * 100;
}
void WildWildWestGame::Logs_QuestCompleted(char *name, int number_quest) {
	if (number_quest <= 0)
	{
		return;
	}
	char title[128];
	char desc[1024];
	char type[128];

	sprintf(title, "%s_number_%d", name, number_quest);
	sprintf(desc, "%s_number_%d", name, number_quest);
	sprintf(type, "%s", "daily_quest_logs");

#if defined (MAC_OS) || (ANDROID_OS)
	PDEBUG("\n logContent title:%s --- desc:%s --- type:%s", title, desc, type);
	Device::GA_LogContents(title, desc, type);
#else
	PDEBUG("\n game_dailyquest_complete: title:%s --- desc:%s --- type:%s", title, desc, type);
#endif
}
void WildWildWestGame::Logs_GameStorePWLevel(char *name, int level) {
	if (level <= 0)
	{
		return;
	}
	char title[128];
	char desc[1024];
	char type[128];

	sprintf(title, "%s_lv_%d", name, level);
	sprintf(desc, "%s_lv_%d", name, level);
	sprintf(type, "%s", "power_up_logs");

#if defined (MAC_OS) || (ANDROID_OS)
	PDEBUG("\n logContent title:%s --- desc:%s --- type:%s", title, desc, type);
	Device::GA_LogContents(title, desc, type);
#else
	PDEBUG("\n game_store: title:%s --- desc:%s --- type:%s", title, desc, type);
#endif
}
void WildWildWestGame::Logs_LocationPass(int location) {
	char title[128];
	char desc[1024];
	char type[128];

	char _status[128];

	sprintf(title, "location %d", location);
	sprintf(desc, "location %d", location);
	sprintf(type, "%s", "location_logs");

#if defined (MAC_OS) || (ANDROID_OS)
	PDEBUG("\n logContent title:%s --- desc:%s --- type:%s", title, desc, type);
	Device::GA_LogContents(title, desc, type);
#else
	PDEBUG("\n ads_show: title:%s --- desc:%s --- type:%s", title, desc, type);
#endif
}
void WildWildWestGame::Logs_GameStoreCharacter(char *charname) {
	char title[128];
	char desc[1024];
	char type[128];

	sprintf(title, "character unlock %s", charname);
	sprintf(desc, "character unlock %s", charname);
	sprintf(type, "%s", "character_logs");


#if defined (MAC_OS) || (ANDROID_OS)
	PDEBUG("\n logContent title:%s --- desc:%s --- type:%s", title, desc, type);
	Device::GA_LogContents(title, desc, type);
#else
	PDEBUG("\n game_store: title:%s --- desc:%s --- type:%s", title, desc, type);
#endif
}
void WildWildWestGame::Logs_DailyQuest_Double(bool status) {
	char title[128];
	char desc[1024];
	char type[128];

	char _status[128];

	if (status)
	{
		sprintf(_status, "%s", "ok");
	}
	else {
		sprintf(_status, "%s", "fail");
	}

	sprintf(title, "view ads %s", _status);
	sprintf(desc, "view ads %s", _status);
	sprintf(type, "%s", "double_ads");

#if defined (MAC_OS) || (ANDROID_OS)
	PDEBUG("\n logContent title:%s --- desc:%s --- type:%s", title, desc, type);
	Device::GA_LogContents(title, desc, type);
#else
	PDEBUG("\n ads_show: title:%s --- desc:%s --- type:%s", title, desc, type);
#endif
}
void WildWildWestGame::Logs_GamePlaying_Restart(int restart_count, bool isFirstPlay) {
	char title[128];
	char desc[1024];
	char type[128];

	char type_play[128];

	if (isFirstPlay)
	{
		sprintf(type_play, "%s", "first_play");
	}
	else {
		sprintf(type_play, "%s", "continue_play");
	}
	_user._num_play_in_asession = 0;
	sprintf(title, "%s_%d", type_play, restart_count);
	sprintf(desc, "%s_%d", type_play, restart_count);
	sprintf(type, "%s", "playing_logs");


#if defined (MAC_OS) || (ANDROID_OS)
	PDEBUG("\n logContent title:%s --- desc:%s --- type:%s", title, desc, type);
	Device::GA_LogContents(title, desc, type);
#else
	PDEBUG("\n game_playing_restart: title:%s --- desc:%s --- type:%s", title, desc, type);
#endif
}

