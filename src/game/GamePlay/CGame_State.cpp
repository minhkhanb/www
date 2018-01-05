#include "pch.h"
#include "../GameCore/CGame.h"
#include "../GameCore/Os_Functions.h"
#include <stdio.h>
extern "C" void ServerADVERTStart();
extern "C" bool isChartboostAvailable();
void CGame::OnScreenChange(PineSize size)
{
	_optimizeTexture = false;
	///////////////////////////////////////////
	// INIT CONTEXT
	///////////////////////////////////////////

	//define support version
#if defined(_WINDOWS_PHONE8)
	ResVersion support_version[] = {
		{ SupportVersion::RESVER_1136_640, Orientation::Landscape_90 }
	};
	int support_version_num = 1;
	
#elif defined(_WIN8)
	ResVersion support_version[] = {
		{ SupportVersion::RESVER_1366_768, Orientation::Landscape_90 }
	};
	int support_version_num = 1;
#elif defined(MAC_OS)
	ResVersion support_version[] = {
		//{ SupportVersion::RESVER_960_640, Orientation::Landscape_90 },
		//{ SupportVersion::RESVER_1024_768, Orientation::Landscape_90 },
		{ SupportVersion::RESVER_1136_640, Orientation::Landscape_90 }
	};
	int support_version_num = 1;
#elif defined(ANDROID_OS)
	ResVersion support_version[] = {
		//{ SupportVersion::RESVER_960_640, Orientation::Landscape_90 },
		//{ SupportVersion::RESVER_1024_768, Orientation::Landscape_90 },
		{ SupportVersion::RESVER_1136_640, Orientation::Landscape_90 },
		//{ SupportVersion::RESVER_800_480, Orientation::Landscape_90 },
	};
	int support_version_num = 1;
#else //windows
	ResVersion support_version[] = {
		//{ SupportVersion::RESVER_1280_768, Orientation::Landscape_90 },
		//{ SupportVersion::RESVER_800_480, Orientation::Landscape_90 },
		//{ SupportVersion::RESVER_1366_768, Orientation::Landscape_90 },
		//{ SupportVersion::RESVER_960_640, Orientation::Landscape_90 },
		//{ SupportVersion::RESVER_1024_768, Orientation::Landscape_90 },
		{ SupportVersion::RESVER_1136_640, Orientation::Landscape_90 }
	};
	int support_version_num = 1;
#endif
    //detect device version
    
	if (support_version_num > 0){
		PineSize dvsize = { _device.ScreenSize.W, _device.ScreenSize.H };
		ResVersion curr_ver = GetAppropriateVersion(dvsize, support_version, support_version_num);
		Alignment game_align = { HorizontalAlign::Center, VerticalAlign::Middle };
		AlignmentContext(game_align, dvsize, curr_ver,true,_VCENTER);

		switch (curr_ver.Version)
		{

		/*case SupportVersion::RESVER_800_480:
			_current_version = VERSION_480x800;

			break;
		case SupportVersion::RESVER_960_640:
			_current_version = VERSION_640x960;
			break;
		case SupportVersion::RESVER_1024_768:
			_current_version = VERSION_768x1024;
			break;*/
		case SupportVersion::RESVER_1136_640:
			_current_version = VERSION_640x1136;
			break;
		//case SupportVersion::RESVER_1366_768:
			//_current_version = VERSION_1366x768;
			//break;
		}
		if (_device.ScreenSize.W == 800 || _device.ScreenSize.H == 800) {
			_optimizeTexture = true;
			GAME()->SetTextureScale(true);
		}
	}
}
void CGame::InitEx()
{
    
	InitBuff(UNZIP_BUFFER_LENGTH_HIGH);
	_GameState =  k_GS_LOGO;
	_GameSubState = k_INIT;
	//Utils::Loga("run here\n");


#ifdef MAC_OS
    _pine_social = new PineGameCenterController();
    _pine_social->Init();
#endif



	_main_game = new WildWildWestGame();
	_main_game->Init();
	_main_game->SetState(k_GS_LOGO);
	_main_game->_game_id = APPLE_ID;
	_current_game = _main_game;


#if defined (_WINDOWS_PHONE8) || defined (_WIN8)
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	for (int i = 0; i < TOTAL_IAP_NAME_DES_ROW; i++)
	{

		sprintf(game->_inapp_price[i], "%s", GET_DES().getIAPNameDes(i, IAP_NAME_DES_PRICE));
	}
#endif


	setUseOptimizeDrawModule(1);
#if !defined (MAC_OS)
	_isBackkeyPress = false;
	_waitForProcessBackkeyCount = 0;
#endif

	configDownloaded = false;
	closeTopeboxAds = false;
	disableTopeboxAds = false;
	time_get_remote_config = 0;

	_point_o = GAME()->_game_context.MeasureAnchor(0, 0, ANCHOR_LEFT | ANCHOR_TOP);
	_point_right_bottom = GAME()->_game_context.MeasureAnchor(0, 0, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	_screen_width = _point_right_bottom.X - _point_o.X;
	_screen_height = _point_right_bottom.Y - _point_o.Y;
	_port_scale_x = _screen_width / k_SCREEN_WIDTH;
	_port_scale_y = _screen_height / k_SCREEN_HEIGHT;
}

void CGame::ReleaseEx()
{
	
}

void CGame::PauseEx()
{
#if defined(_WINDOWS_PHONE8) || defined(_WIN8)
	{
		
		Prepare_restore();
		_on_pause = true;
	}
#endif
	if (MAIN_GAME()->_current_state == k_GS_PLAYGAME)
	{
		MAIN_GAME()->SetState(k_GS_IGM);
	}
}
void CGame::ResumeEx()
{
	WildWildWestGame * game = (WildWildWestGame*)_current_game;
    if(game != NULL)
    {
		game->_countShowAdsMods = 0;
    }
#if defined(_WINDOWS_PHONE8) || defined(_WIN8)
	if (_on_pause){
		_GameState = k_GS_RESTORE;
		_GameSubState = k_INIT;
	}
	
#endif
	if (game->_show_ads_interrupt && game->_current_state != k_GS_LOGO)
	{
		game->_count_frame_show_ads = 30;
		game->_show_ads_interrupt = false;
	}
	if (!game->_show_ads_interrupt)
	{
		game->_show_ads_interrupt = true;
	}

	game->num_continue++;

#if ADD_ADVERT
	if (GAME()->_advert.IsAvailable() && !game->_isProssesIap && !game->_notShowPineAds &&  game->_stateShowPineAds == 0 && !game->_user._removeAdd)
	{		
		game->_stateShowPineAds = 1;
	}
#endif
}
bool CGame::OnBackKeyPress(){
    //Implement function when
#if !defined (MAC_OS)
    CGame::_this->_isBackkeyPress = true;
    CGame::_this->_waitForProcessBackkeyCount = 0;
    return true;
#else
    return false;
#endif
    
}
void CGame::UpdateAndRender()
{
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	G()->SetColor(0x000000);
	G()->FillFullScreen();
	G()->ClearClip();
	G()->SetOpacity(100);
	/*if (!LoadGameForceground())
	{
		return;
	}
*/
	//
	//Utils::Loga("\nUpdate And Render");
	/*if (_GameState == k_GS_RESTORE){
		Update_state_restore();
		Render_state_restore();
		return;
	}*/
#if defined(_WINDOWS_PHONE8)
	/*if (CGame::_device.ScreenSize.W == 800 || CGame::_device.ScreenSize.H == 800)
	{
	}
	else*/
	{
		_admob_manager.update();
	}
	
#endif
	_main_game->UpdateAndRender();

#if defined (ANDROID_OS)
	_async_task_manager.Run();
#endif
	//render
	//printf("\n GAME()->_num_touches = %d", GAME()->_num_touches);
	/*if (_num_touches > 1)
	{
		ClearAllTouch();
	}*/
#if !defined (MAC_OS)
	if (CGame::_this->_isBackkeyPress)
	{
		if (MAIN_GAME()->_isRenderSetting)
		{
			MAIN_GAME()->CloseSetting();
			CGame::_this->_isBackkeyPress = false;
		}
		if (MAIN_GAME()->_isRenderAbout && !MAIN_GAME()->_closeAbout)
		{
			MAIN_GAME()->_closeAbout = true;
			CGame::_this->_isBackkeyPress = false;
		}
		if (MAIN_GAME()->_isMoveButon)
		{
			MAIN_GAME()->_isMoveButon = false;
			MAIN_GAME()->_buttonMainMenu[MAINMENU_BUTTON_SHOP].SetState(BUTTON_STATE_NORMAL);
			CGame::_this->_isBackkeyPress = false;
		}
		if (MAIN_GAME()->_current_state == k_GS_PLAYGAME && MAIN_GAME()->_battle._timeBegin <= 0)
		{
			MAIN_GAME()->SetState(k_GS_IGM);
			CGame::_this->_isBackkeyPress = false;
		}
		else if (MAIN_GAME()->_current_state == k_GS_IGM)
		{
			MAIN_GAME()->SetState(k_GS_PLAYGAME);
			MAIN_GAME()->_sub_state = k_LOOP;
			MAIN_GAME()->num_continue++;
			CGame::_this->_isBackkeyPress = false;
		}
		if (MAIN_GAME()->_current_state == k_GS_MAINMENU && game->_is_ggplay_menu_active)
		{
			game->_is_ggplay_menu_active = false;
			game->_is_ggplay_menu_close = true;
			CGame::_this->_isBackkeyPress = false;
		}
		if (MAIN_GAME()->_current_state == k_GS_MAINMENU && game->_quest_popup_active)
		{
			game->_quest_popup_active = false;
			game->_quest_popup_isclose = true;
			CGame::_this->_isBackkeyPress = false;
		}
		if (CGame::_this->_isBackkeyPress && CGame::_this->_waitForProcessBackkeyCount >= 1)
		{
			CGame::_this->_isBackkeyPress = false;
			CGame::_this->_waitForProcessBackkeyCount = 0;
			WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
			OS_Exit(false);
		}
		CGame::_this->_waitForProcessBackkeyCount++;
	}
#if defined (ANDROID_OS)
	UpdateAds();
#endif
#endif

}

#if defined(_WINDOWS)
void CGame::ProcessCommandLine(const char* cmd)
{

}
#endif



void CGame::UpdateTouch(int xx, int yy, int touch_state, int touch_id, int tap_count)
{
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	if (game->_is_waiting_for_reward && !game->_on_video_ads_reward)
	{
		return;
	}
    switch (game->_current_state)
    {
    case k_GS_MAINMENU:
        if (!game->TouchButtonMainMenu(xx,yy,touch_state,touch_id))
        {
            if (game->_isRenderShop)
            {
                game->UpdateScrollShop(xx,yy,touch_state,touch_id);
            }
            else
            {
                game->UpdateChooseLocation(xx,yy,touch_state,touch_id);
            }
        }
		if (!game->TouchButtonGGPlay(xx, yy, touch_state, touch_id))
		{

		}
		if (game->_quest_popup_active)
		{
			game->UpdateScrollQuest(xx, yy, touch_state, touch_id);
		}
        break;
    }
    
    
}

void WildWildWestGame::UpdateScrollCoin()
{
	if (_increase)
	{
		_coin_scroll += _deltaBone;
		if (_coin_scroll >= USER()._coin)
		{
			_coin_scroll = USER()._coin;
			_increase = false;
		}
	}
	else
	{
		if (_descrease)
		{
			_coin_scroll -= _deltaBone;
			if (_coin_scroll <= USER()._coin)
			{
				_coin_scroll = USER()._coin;
				_descrease = false;
			}
		}
		else
		{
			_coin_scroll = USER()._coin;
		}
	}
}


void WildWildWestGame::CheckAndSynSave()
{
#ifdef MAC_OS
    if(_current_state != k_GS_LOGO && !_check_syn_save_complete && GAME()->_pine_social->GetFlag() == SOCIAL_FLAG_FULL_ACCESS && GAME()->_pine_social->GetState() == PINESOCIAL_STATE_COMPLETED)
    {
        ((PineGameCenterController*)GAME()->_pine_social)->LoadMyScore();
 
        _check_syn_save_complete = true;
        _syn_save_step = 0;
        _syn_save_state = SYN_SAVE_PROCESSING;
       
    }
#endif
}

bool  WildWildWestGame::UpdateSynSave()
{
	BYTE _tutorialStep;
	_tutorialStep = 0;
    if(_syn_save_state)
    {
        _syn_time_out -= 16;
        switch (_syn_save_step) {
            case 0:
                //_syn_save_step++;
                _syn_time_out = 5000;
                break;
            case 1:
                
                //_syn_save_step++;
                break;
            case 2:
#ifdef MAC_OS
                if(_syn_time_out < 0)
                {
                    _syn_save_state = SYN_SAVE_NONE;
                }
                else
                if(CGame::_this->_pine_social->GetState() != PINESOCIAL_STATE_COMPLETED)
                {
                    _syn_save_step--;
                }
#endif
                break;
            case 14:
#ifdef MAC_OS
                _online_town = PineGameCenterController::GetMyScore("WWW_ADVENTURER");
                _online_howfar = PineGameCenterController::GetMyScore("WWW_HOWFAR");
                _online_total_coin = PineGameCenterController::GetMyScore("WWW_COINS");
                 printf("\n_online_town = %d, _online_howfar = %d, _online_total_score = %d",_online_town,_online_howfar,_online_total_coin);
#endif
                break;
            case 15:
#ifdef MAC_OS
                //new user and submit
                //syn save
                _can_submit = true;
                if((USER()._userId == 0 &&  GAME()->_pine_social->GetUserID() > 0 && _online_total_coin <= 0) || (USER()._userId != 0 && USER()._userId == GAME()->_pine_social->GetUserID()))
                {
                    USER()._userId = GAME()->_pine_social->GetUserID();
                    PineGameCenterController::SubmitMyScore("WWW_ADVENTURER",USER()._lvlLocation);
					PineGameCenterController::SubmitMyScore("WWW_HOWFAR", USER()._highScore);
                    PineGameCenterController::SubmitMyScore("WWW_COINS",USER()._total_coin);
                }
                else if(USER()._userId != GAME()->_pine_social->GetUserID() && GAME()->_pine_social->GetUserID() > 0)
                {
                   
                    USER().DataDefault();
                    USER().DataFromBuffer();
                     USER()._userId = GAME()->_pine_social->GetUserID();
                    if(_online_total_coin <= 0)
                    {
                        USER()._lvlLocation = 0;
						USER()._highScore = 0;
                        USER()._total_coin = 0;
                        USER()._coin = 0;
                        PineGameCenterController::SubmitMyScore("WWW_ADVENTURER",USER()._lvlLocation);
						PineGameCenterController::SubmitMyScore("WWW_HOWFAR", USER()._highScore);
                        PineGameCenterController::SubmitMyScore("WWW_COINS",USER()._total_coin);
                    }
                    else
                    {

                        USER()._lvlLocation = _online_town;
						USER()._highScore = _online_howfar;
                        USER()._total_coin = _online_total_coin;
                        USER()._coin =  USER()._total_coin;
                    }
                }
                else
                {
                    _can_submit = false;
                }


#endif
                break;
			
            case 30:
                USER().DataSave();
                _syn_save_state = SYN_SAVE_NONE;
                break;

			case 50:// reset data				
				_tutorialStep = TUTORIAL()._step;
#ifdef MAC_OS
				_online_town = PineGameCenterController::GetMyScore("WWW_ADVENTURER");
				_online_howfar = PineGameCenterController::GetMyScore("WWW_HOWFAR");
				_online_total_coin = PineGameCenterController::GetMyScore("WWW_COINS");
				printf("\n_online_town = %d, _online_howfar = %d, _online_total_score = %d", _online_town, _online_howfar, _online_total_coin);
#endif
				break;
			case 51:
            {
                bool Machete = USER()._isUnlockMachete;
                bool Mule = USER()._isUnlockMule;
                bool Cavalry = USER()._isUnlockCavalry;
				USER().DataDefault();
				USER()._lvlLocation = _online_town;				
				USER()._highScore = _online_howfar;
				USER()._total_coin = _online_total_coin;
				USER()._coin = USER()._total_coin;
				TUTORIAL()._step = _tutorialStep;
                USER()._isUnlockMachete = Machete;
                USER()._isUnlockMule = Mule;
                USER()._isUnlockCavalry = Cavalry;
            }
				break;

			case 80:
				USER().DataSave();
				_syn_save_state = SYN_SAVE_NONE;
				break;


            default:
                break;
        }
        _syn_save_step++;
    }
    return _syn_save_state != SYN_SAVE_NONE;
}

void WildWildWestGame::SubmitAllScore()
{
    if(_can_submit)
    {
#ifdef MAC_OS
        PineGameCenterController::SubmitMyScore("WWW_ADVENTURER",USER()._lvlLocation);
        PineGameCenterController::SubmitMyScore("WWW_HOWFAR",USER()._highScore);
        PineGameCenterController::SubmitMyScore("WWW_COINS",USER()._total_coin);        
#endif
    }
}

void WildWildWestGame::SubmitTotalCoin()
{
	if (_can_submit)
	{		
#ifdef MAC_OS
		PineGameCenterController::SubmitMyScore("WWW_COINS", USER()._total_coin);
#endif
	}
}

void WildWildWestGame::SubmitHighScore()
{
	if (_can_submit)
	{
#ifdef MAC_OS
		PineGameCenterController::SubmitMyScore("WWW_HOWFAR", USER()._highScore);
#elif ANDROID_OS
		//PineGameCenterController::SubmitMyScore(GP_LEADERBOARD_ENDLEST, USER()._highScore);
		sprintf(GAME()->_gameBoardScoreItem.BoardId, "%s", GP_LEADERBOARD_ENDLEST);
		GAME()->_gameBoardScoreItem.Score = USER()._highScore;
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_POST_SCORE);
#endif
	}
}
void WildWildWestGame::SubmitTown()
{
	if (_can_submit)
	{
#ifdef MAC_OS
		PineGameCenterController::SubmitMyScore(GP_LEADERBOARD_STORIES, USER()._lvlLocation);
#elif ANDROID_OS
		
		sprintf(GAME()->_gameBoardScoreItem.BoardId, "%s", GP_LEADERBOARD_STORIES);
		GAME()->_gameBoardScoreItem.Score = USER()._lvlLocation;
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_POST_SCORE);
#endif
	}
}

bool CGame::InitAds()
{
#if defined (ANDROID_OS)
	PDEBUG("INIT ADS");
	_tapdaq_manager.setUnitId("DONT_NEED");
	_tapdaq_manager.prepareInterstitial();
	//_tapdaq_manager.prepareBanner();
	//_tapdaq_manager.prepareRewardedVideo();
	//_unityads_manager.prepareAds();
	//_unityads_manager.prepareVideo();
	//_adsVungle_manager.prepare();
	//_unityads_manager.prepareSkippableVideo();
	_on_show_video_ads = false;
	_admob_play_count = 0;
	_time_count_for_show_ads_banner = 0;
	return true;
#endif
	return false;
}
void CGame::ShowAds()
{
#if defined (ANDROID_OS)
	PDEBUG("\n call ShowAds");
	//_on_show_video_ads = true;
	//_is_waiting_for_reward = true;
	//_on_video_ads_reward = false;
	/*if (_unityads_manager.isVideoReady())
	{
	PDEBUG("SHOW UNITY VIDEO");
	_unityads_manager.showVideo();
	_admob_play_count = 0;
	}
	else if (_adsVungle_manager.isReady())
	{
	PDEBUG("SHOW VUNGLE ADS");
	_admob_play_count = 0;
	_adsVungle_manager.show();
	}
	else */if (_tapdaq_manager.isInterstitialReady())
	{
		PDEBUG("SHOW _admob showInterstitial");
		_tapdaq_manager.showInterstitial();
	}
#endif

}

void CGame::ShowBannerAds()
{
#if defined (ANDROID_OS)
	PDEBUG(" call _admob banner: %d %d", _tapdaq_manager.isBannerReady(), _tapdaq_manager.isShowBanner());
	if (_tapdaq_manager.isBannerReady() && !_tapdaq_manager.isShowBanner())
	{
		PDEBUG("SHOW _admob banner");
		_tapdaq_manager.showBanner();
		_has_show_ads_banner = true;
	}
	else {
		_time_count_for_show_ads_banner = 2 * FPS_MAX;
	}
#endif
}
void CGame::HideBannerAds()
{
#if defined (ANDROID_OS)
	_has_show_ads_banner = false;
	_tapdaq_manager.hideBanner();
#endif
}
void CGame::UpdateAds()
{
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
#if defined (ANDROID_OS)
	if (OS_IsNetworkAvailable())
	{
		//Admob
		PineTapdaqAsync::enumTapdaqUpdateResult result = _tapdaq_manager.update();
		//PDEBUG("\n enumAdmobUpdateResult: %d", result);
		//PDEBUG("\n enumAdmobUpdateResult: %d", result);
		switch (result)
		{
		case PineTapdaqAsync::enumTapdaqUpdateResult::SHOW_VIDEO_SUCCESS:
			if (_is_waiting_for_reward)
			{
				PDEBUG("\n Show Tapdaq success");
				_on_video_ads_reward = true;
				_is_waiting_for_reward = false;
				_isAdsProcessing = false;
			}
			game->_show_ads_interrupt = false;
			break;
		case PineTapdaqAsync::enumTapdaqUpdateResult::SHOW_ADS_SUCCESS:
		case PineTapdaqAsync::enumTapdaqUpdateResult::SHOW_ADS_FAIL:
		case PineTapdaqAsync::enumTapdaqUpdateResult::PREPARE_ADS_FAIL:
		case PineTapdaqAsync::enumTapdaqUpdateResult::SHOW_VIDEO_FAIL:
		case PineTapdaqAsync::enumTapdaqUpdateResult::PREPARE_VIDEO_FAIL:
			PDEBUG("\n Show Tapdaq false");
			_is_waiting_for_reward = false;
			_isAdsProcessing = false;
			break;
		default:
			break;
}
	}
	else {
		if (_is_waiting_for_reward)
		{
			_on_video_ads_reward = true;
			_is_waiting_for_reward = false;
			_isAdsProcessing = false;
		}
	}
#endif
}

void CGame::PrepareAndShowInterstitial()
{
#if defined (ANDROID_OS)
	_tapdaq_manager.prepareInterstitial(true);
#endif
}

void CGame::PrepareAndShowVideoAds()
{
#if defined (ANDROID_OS)
	_is_waiting_for_reward = true;
	_on_video_ads_reward = false;
	_isAdsProcessing = true;
	_tapdaq_manager.prepareRewardedVideo(true);
#endif
}