#include "pch.h"
#include "../../GameCore/CGame.h"




#undef _DEC_BUTTON_NUMS
#undef _DEC_BUTTON_ARRAY
#undef _DEC_ANIM_BUTTON_BEIGN
#undef _DEC_SPRITE_BUTTON

#define _DEC_BUTTON_NUMS				(MAINMENU_BUTTON_NUMS)
#define _DEC_BUTTON_ARRAY				_buttonMainMenu
#define _DEC_ANIM_BUTTON_BEIGN			0
#define _DEC_SPRITE_BUTTON				SPRITEID_HUD_SHOP

#define MIN_ITEM_SHOP				3
extern "C" void PurchaseProduct(const char *indentify);
extern "C" bool isChartboostAvailable();

#if defined (ANDROID_OS)
extern "C" void				OS_Rating();
extern "C" void PurchaseProductWithType(const char *indentify, int type);
#endif

void WildWildWestGame::PurchaseIAP(const char* strID, enumIAPResponseStatus status)
{
	if (status == enumIAPResponseStatus::IAP_RESPONSE_STATUS_SUCCESS || status == enumIAPResponseStatus::IAP_RESPONSE_STATUS_SUCCESS_RESTORE)
	{
		for (int i = 0; i < TOTAL_IAP_NAME_DES_ROW; i++)
		{
#if defined(ANDROID_OS)
			if (strcmp(GDS_Exporter::iap_name_des[TOTAL_IAP_NAME_DES*i + IAP_NAME_DES_GOOGLE_ID], strID) == 0)
			{

				if (strcmp(strID, "www_adsremove") == 0)
#else
			if (strcmp(GDS_Exporter::iap_name_des[TOTAL_IAP_NAME_DES*i + IAP_NAME_DES_APPLE_ID], strID) == 0)
			{
				if (strcmp(strID, "WWW_RemoveAd") == 0)
#endif
				/*if (strcmp(strID, "WWW_RemoveAd") == 0)*/
				{
					USER()._removeAdd = true;
					USER().DataSave();
				}
				else
				{
					USER().AddCoin(GDS_Exporter::iap_value_des[i]);
					SubmitTotalCoin();
				}

				break;
			}
		}
	}
	_isProssesIap = false;
}

void WildWildWestGame::MenuGGPlayUpdate() {
	ProcessingButtonGGPlay();
}

void WildWildWestGame::UpdateStateMainMenu()
{
	if (_sub_state == k_LOOP)
	{
		RatingUpdate();
		if (_rating_active)
		{
			return;
		}
		MenuGGPlayUpdate();
		if (_quest_popup_active)
		{
			QuestUpdate();
		}

		/*if (!TouchButtonMainMenu())
		{
			if (_isRenderShop)
			{
				UpdateScrollShop();
			}
			else
			{
				UpdateChooseLocation();
			}
		}*/

		ProcessingButton();
#if defined MAC_OS
		if (_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_MOREGAME]._state == BUTTON_STATE_HIDE)
		{
			_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_MOREGAME].SetState(BUTTON_STATE_NORMAL);
		}
		if (_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_RANKING]._state == BUTTON_STATE_HIDE)
		{
			_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_RANKING].SetState(BUTTON_STATE_NORMAL);
		}
#endif

		if (_maxButtonShop > MIN_ITEM_SHOP)
		{
			_scroll_shop.Update();
			_scroll_y = -_scroll_shop.GetCurrent();
		}

		if (_birdX < k_SCREEN_WIDTH)
		{
			_birdX++;

		}
		else
		{
			_birdX = 0;
		}
		_animBird.SetPos(_birdX, 0);
		UpdateScrollCoin();

		if (_isMoveButon)
		{
			_moveButtonPlay -= 200 / 16.0f;
			if (_moveButtonPlay <= -200)
			{
				_moveButtonPlay = -200;
			}
		}
		else
		{
			_opacityShop -= 100 / 16;
			if (_opacityShop < 0)
			{
				_opacityShop = 0;
			}
			_moveButtonPlay += 200 / 16.0f;
			if (_moveButtonPlay >= 0)
			{
				_moveButtonPlay = 0;
				_isRenderShop = false;
			}
		}
		EFFECT().Update();
	}
}

void WildWildWestGame::MenuGGPlayActive() {
	printf("\n Show menu ggplay");
	_is_ggplay_menu_close = false;
	_is_ggplay_menu_active = true;
}

void WildWildWestGame::MenuGGPlayInit() {
	for (int i = 0; i < GGPLAY_BUTTON_NUM; i++)
	{
		_buttonGGPlayMenu[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, 0, 0, SFX_BUTTON);
		_buttonGGPlayMenu[i].SetAnim(SPRITEID_HUD_ANDROID, i * 4);
	}
	_is_ggplay_menu_close = true;
	_is_ggplay_menu_active = false;
	_scale_leaderboard = 0.0f;
	_scale_achievement = 0.0f;
	_scale_sync = 0.0f;


}

void WildWildWestGame::MenuGGPlayRender() {

	G()->SetColor(0xaa000000);
	G()->FillFullScreen(true);

	GET_SPRITE(SPRITEID_HUD_ANDROID)->DrawFrame(G(), 8, 0, 0);
	float scale = 1.0f;
	for (int i = 0; i < GGPLAY_BUTTON_NUM; i++)
	{
		_buttonGGPlayMenu[i].SetPos(0, 0);
		_buttonGGPlayMenu[i].Render(G());
		_buttonGGPlayMenu[i].Update();
		if (_buttonGGPlayMenu[i]._state == BUTTON_STATE_SELECT)
		{
			scale = 1.0f;
		}
	}

	G()->SetScale(0.80f*scale, 0.80f*scale, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), GET_SPRITE(SPRITEID_HUD_ANDROID)->GetFModuleX(0, 0) + 230, GET_SPRITE(SPRITEID_HUD_ANDROID)->GetFModuleY(0, 0) - 12, _HCENTER | _VCENTER, "Leaderboards");
	G()->ClearScale();
	G()->SetScale(0.80f*scale, 0.80f*scale, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), GET_SPRITE(SPRITEID_HUD_ANDROID)->GetFModuleX(2, 0) + 230, GET_SPRITE(SPRITEID_HUD_ANDROID)->GetFModuleY(2, 0) + 25, _HCENTER | _VCENTER, "Achievements");
	G()->ClearScale();
	G()->SetScale(0.80f*scale, 0.80f*scale, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), GET_SPRITE(SPRITEID_HUD_ANDROID)->GetFModuleX(4, 0) + 230, GET_SPRITE(SPRITEID_HUD_ANDROID)->GetFModuleY(4, 0) + 55, _HCENTER | _VCENTER, "Cloud Sync");
	G()->ClearScale();

}

void WildWildWestGame::MainMenuInit()
{
	_animCoin.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_COIN), SPRITEID_HUD_COIN);
	_animCoin.SetAnim(0, true);
	//GAME()->disableTopeboxAds = true;
}

void WildWildWestGame::RenderStateMainMenu()
{
	if (_sub_state == k_INIT)
	{
		if (_last_state == k_GS_LOGO)
		{
			GAME()->LoadSpriteCurrentVer(SPRITEID_CHARATER_01, false, false);
			GAME()->LoadSpriteCurrentVer(SPRITEID_CHARATER_02, false, false);
			GAME()->LoadSpriteCurrentVer(SPRITEID_CHARATER_03, false, false);
			GAME()->LoadSpriteCurrentVer(SPRITEID_CHARATER_04, false, false);
			GAME()->LoadSpriteCurrentVer(SPRITEID_CHARATER_05, false, false);

		}
		/*if (_last_state == k_GS_RESULT)
		{
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_SHOP, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_BG_MAIN_MENU, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_EFFECT_DUST, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_COIN, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_FONT_DISTANCE, false, !_optimizeDate);
			GET_SPRITE(SPRITEID_FONT_DISTANCE)->SetCharSpacing(5);
		}*/



		

		_sub_state = k_LOOP;
		_isRenderShop = false;
		_isRenderSetting = false;
		_isRenderAbout = false;
		_closeAbout = true;
		_isMoveButon = false;
		_moveButtonPlay = 0;


		for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
		{
			_DEC_BUTTON_ARRAY[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, 0, 0, SFX_BUTTON);
			_DEC_BUTTON_ARRAY[i].SetAnim((_DEC_SPRITE_BUTTON), _DEC_ANIM_BUTTON_BEIGN + i * 4);
		}
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_PLAY].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, 0, 0, SFX_BUTTON_START);
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_YES].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, 0, 0, SFX_UPGRADE);

		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_YES].SetAnim((_DEC_SPRITE_BUTTON), 44);
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_NO].SetAnim((_DEC_SPRITE_BUTTON), 48);

		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_MUSIC].SetAnim((_DEC_SPRITE_BUTTON), 32);
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_SOUND].SetAnim((_DEC_SPRITE_BUTTON), 28);
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_NOTIFY].SetAnim((_DEC_SPRITE_BUTTON), 36);
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_ABOUT].SetAnim((_DEC_SPRITE_BUTTON), 40);
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_RESET].SetAnim((_DEC_SPRITE_BUTTON), 52);
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_ACHIEVEMENT].SetAnim((_DEC_SPRITE_BUTTON), 64);
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_GGPLAY].SetAnim((_DEC_SPRITE_BUTTON), 68);
		_DEC_BUTTON_ARRAY[QUEST_BUTTON_GIFT].SetAnim(SPRITEID_HUD_DAILY_REWARD, 8);
		_DEC_BUTTON_ARRAY[QUEST_BUTTON_TASK].SetAnim(SPRITEID_HUD_DAILY_REWARD, 12);

		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_YES].SetState(BUTTON_STATE_HIDE);
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_NO].SetState(BUTTON_STATE_HIDE);

		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_MUSIC].SetState(BUTTON_STATE_HIDE);
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_SOUND].SetState(BUTTON_STATE_HIDE);
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_NOTIFY].SetState(BUTTON_STATE_HIDE);
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_ABOUT].SetState(BUTTON_STATE_HIDE);
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_RESET].SetState(BUTTON_STATE_HIDE);
		//_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_RESET].SetState(BUTTON_STATE_HIDE);
		
		//NotifyInit();


#if defined (_WINDOWS_PHONE8)
		if (!_playWitchEndlest)
		{
			_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_RANKING].SetState(BUTTON_STATE_HIDE);
		}
		else
		{
			_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_RANKING].SetState(BUTTON_STATE_NORMAL);
		}
#endif

		_animShop.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_SHOP), SPRITEID_HUD_SHOP);
		_animSetting.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_SHOP), SPRITEID_HUD_SHOP);

		_coin_scroll = USER()._coin;

		_animBird.SetAnimSprite(GET_SPRITE(SPRITEID_BG_MAIN_MENU), SPRITEID_BG_MAIN_MENU);
		_animGrass1.SetAnimSprite(GET_SPRITE(SPRITEID_BG_MAIN_MENU), SPRITEID_BG_MAIN_MENU);
		_animGrass2.SetAnimSprite(GET_SPRITE(SPRITEID_BG_MAIN_MENU), SPRITEID_BG_MAIN_MENU);
		_animTree.SetAnimSprite(GET_SPRITE(SPRITEID_BG_MAIN_MENU), SPRITEID_BG_MAIN_MENU);

		_animBird.SetAnim(1, true);
		_animGrass1.SetAnim(2, true);
		_animGrass2.SetAnim(3, true);
		_animTree.SetAnim(4, true);

		_birdX = 0;
		_animGrass1.SetPos(0, 0);
		_animGrass2.SetPos(0, 0);
		_animTree.SetPos(0, 0);
		EFFECT().Init();
		EFFECT().AddParticalDust(SPRITEID_EFFECT_DUST, 4, 5, CHARATER_MOVE_DOWN_LEFT, 345, 360);

		_animCharater.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_SHOP), SPRITEID_HUD_SHOP);
		_animCharater.SetAnim(25, true);


		_rota1 = 0;
		_rota2 = 0;

		_animCoin.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_COIN), SPRITEID_HUD_COIN);
		_animCoin.SetAnim(0, true);

		if (_newScore)
		{
			GAME()->StopCurrentMusic();
			GAME()->LoadMusic(BGM_RESULT_SCREEN_WITH_NEW_HIGHSCORE);
			GAME()->PlayCurrentMusic(BGM_RESULT_SCREEN_WITH_NEW_HIGHSCORE);
		}
		else
		{
			GAME()->StopCurrentMusic();
			GAME()->LoadMusic(BGM_MAIN_MENU);
			GAME()->PlayCurrentMusic(BGM_MAIN_MENU);
		}

		//USER()._lvlLocation = TOTAL_CHECK_POINT_DES_ROW - 1;
		if (USER()._currentLocation == TOTAL_CHECK_POINT_DES_ROW - 1)
		{
			_playWitchEndlest = true;
			if (_isGameOver)
			{
				_stateScore = 0;
				_scaleScore = 0;
				_scoreX = k_SCREEN_WIDTH / 2;
				_scoreY = k_SCREEN_HEIGHT / 2;
				_opacityNew = 0;
			}
		}
		_opacityMainMenu = 0;
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_RANKING].SetState(BUTTON_STATE_HIDE);
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_MOREGAME].SetState(BUTTON_STATE_HIDE);
		//USER()._currentUseCharater = CMath::RANDOM(0,3);

	}

	if (_sub_state == k_LOOP)
	{

		float  offsetY = 0;

		//bg
		float zoom = 1.0f;
		if (GAME()->_device._version == SupportVersion::RESVER_960_640)
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
		G()->SetScale(zoom, zoom, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);

		GET_SPRITE(SPRITEID_BG_MAIN_MENU)->DrawFrame(G(), 0, 0, 0);

		G()->ClearScale();
		_animBird.DrawAnimObject(G());
		_animBird.UpdateAnimObject();

		_animGrass1.DrawAnimObject(G());
		_animGrass1.UpdateAnimObject();

		_animGrass2.DrawAnimObject(G());
		_animGrass2.UpdateAnimObject();

		_animTree.DrawAnimObject(G());
		_animTree.UpdateAnimObject();

		EFFECT().Render();
		if (_isMoveButon || _isRenderSetting)
		{
			int opacity = 100 + _moveButtonPlay / 2.8f;
			if (_isRenderSetting)
			{
				opacity = 28;
			}
			G()->SetOpacity(opacity);
		}
		else
		{
			G()->SetOpacity(_opacityMainMenu);
		}

		if (_isGameOver && _playWitchEndlest)
		{
			RenderResult();
		}
		else
		{
			GET_SPRITE(SPRITEID_BG_MAIN_MENU)->DrawFrame(G(), 1, 0, 0 + offsetY);
		}
		RenderLocation();
		G()->SetOpacity(_opacityAbout);
		if (_isRenderAbout)
		{

			RenderAbout();
		}
		G()->SetOpacity(100);
		if (_closeAbout)
		{
			_opacityMainMenu += 5;
			if (_opacityMainMenu > 100)
			{
				_opacityMainMenu = 100;
			}
			_opacityAbout -= 5;
			if (_opacityAbout < 0)
			{
				_opacityAbout = 0;
				_isRenderAbout = false;
			}

		}

		G()->SetOpacity(_opacityMainMenu);
		if (_isRenderSetting)
		{
			RenderSetting();
		}
		NotifyRender(0, 0, 100);
		if (_isRenderShop)
		{
			
			RenderShop(_moveButtonPlay + 200, 0);
		}
		for (int i = 0; i <= MAINMENU_BUTTON_IAP; i++)
		{
			if (i == MAINMENU_BUTTON_PLAY || i == MAINMENU_BUTTON_SHOP)
			{
				_DEC_BUTTON_ARRAY[i].SetPos(_moveButtonPlay, offsetY);
			}
			else if (i == MAINMENU_BUTTON_IAP)
			{
				_DEC_BUTTON_ARRAY[i].SetPos(_moveButtonPlay * 3, offsetY);
			}
			else
			{
				_DEC_BUTTON_ARRAY[i].SetPos(0, offsetY);
			}

			if (i != MAINMENU_BUTTON_IAP)
			{
				_DEC_BUTTON_ARRAY[i].Render(G());
			}
			_DEC_BUTTON_ARRAY[i].Update();
		}




#if GGPLAY_IMPLEMENT
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_GGPLAY].SetPos(0, 0 - GET_SPRITE(SPRITEID_HUD_SHOP)->GetFModuleY(118,0) + GET_SPRITE(SPRITEID_HUD_SHOP)->GetFModuleY(2, 0));
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_GGPLAY].Render(G());
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_GGPLAY].Update();
#endif



#if defined MAC_OS
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_ACHIEVEMENT].SetPos(0, offsetY);
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_ACHIEVEMENT].Render(G());
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_ACHIEVEMENT].Update();
#else
		if (_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_ACHIEVEMENT]._state != BUTTON_STATE_HIDE)
		{
			_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_ACHIEVEMENT].SetState(BUTTON_STATE_HIDE);
		}
#endif


		/*NotifyRender(0, 0, 100);*/
		QuestRender(0, 0, 100);

		//GAME()->RenderStringVsWidth(SPRITEID_FONT_MONEY, ALIG_MONEY_M_X + _moveButtonPlay * 3, ALIG_MONEY_M_Y, _coin_scroll, 120);
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_IAP].Render(G());
		RenderMoney(ALIG_MONEY_M_X + _moveButtonPlay * 3, ALIG_MONEY_M_Y + offsetY, 130, _coin_scroll, 1);

		G()->SetOpacity(100);
		RatingRender(0, 0);

		if (GAME()->configDownloaded)
		{
#pragma region TOPEBOX_PROMO_GAME
			PDEBUG("\n _game->_game_promo_has_show:%d", _user._game_promo_has_show);
			if (_user._game_promo_has_show)
			{

				if (GAME()->disableTopeboxAds)
				{
					float max = -GAME()->_topebox_game._icon_w*1.5f;
					if (GAME()->_topebox_game_scrollEff > max)
					{
						GAME()->_topebox_game_scrollEff += -10;
						if (GAME()->_topebox_game_scrollEff <= max)
						{
							GAME()->_topebox_game_scrollEff = max;

						}
					}
				}
				else {
					if (GAME()->_topebox_game.GamePromoIsReady())
					{
						//PDEBUG("\n _game->_user.GamePromoHasTouchDown: %d", _game->_user._game_promo_has_show);
						if (!GAME()->_topebox_game.GamePromoHasTouchDown())
						{
							float max = GAME()->_topebox_game._icon_w*1.3f;
							if (GAME()->_topebox_game_scrollEff < max)
							{
								GAME()->_topebox_game_scrollEff += 10.0f;
								if (GAME()->_topebox_game_scrollEff >= max)
								{
									GAME()->_topebox_game_scrollEff = max;
								}
							}
						}
					}
				}
				//PDEBUG("\n _game->_user.GamePromoIsReady: %d", _game->_user._game_promo_has_show);
			}
			else
			{
				float max = -GAME()->_topebox_game._icon_w*1.5f;
				if (GAME()->_topebox_game_scrollEff == 0)
				{
					GAME()->_topebox_game_scrollEff = max;
				}
				if (GAME()->_topebox_game_scrollEff > max)
				{
					GAME()->_topebox_game_scrollEff += -10;
					if (GAME()->_topebox_game_scrollEff <= max)
					{
						GAME()->_topebox_game_scrollEff = max;
						GAME()->_topebox_game.ResetGamePromoInfo();

					}
				}
				GAME()->_topebox_game.show_banner_topebox_active = false;
				//PDEBUG("\n _game->_topebox_game.show_banner_topebox_active  => Gameplay render");
			}
			int fw = 165;//GET_SPRITE(SPRITEID_HUD_PAUSE_01)->GetFrameWidth(82);
			char textname[128];
			if (GAME()->_topebox_game.GamePromoIsReady())
			{
				int lenght = sprintf(textname, "%s", GAME()->_topebox_game._des);
				float text_w = GET_SPRITE(SPRITEID_FONT_RESULT)->GetAnsiTextWidth(textname, 0, lenght);

				float scalex = text_w / fw + 0.2f;
				//PDEBUG("\n _game->_topebox_game._des:%s -- lenght:%d -- text_w:%f -- scalex:%f", _game->_topebox_game._des, lenght, text_w, scalex);
				G()->SetColor(0xff000000);
				G()->SetOpacity(90);
				G()->SetScale(1.0f, 1.0f, GAME_PROMO_RENDER_X + -GAME()->_topebox_game_scrollEff + 0 - 35, GAME_PROMO_RENDER_Y - GAME()->_topebox_game._icon_h + 0 - 20);
				//GET_SPRITE(SPRITEID_HUD_PAUSE_01)->DrawFrame(G(), 82, GAME_PROMO_RENDER_X + -_game->_topebox_game_scrollEff + x - 30, GAME_PROMO_RENDER_Y - _game->_topebox_game._icon_h + y - 20);
				if (GAME()->_topebox_game.typeAds == TopeBox_PromoGame::enumTypeTopeboxAds::TOPEBOX_ADS_ICON)
				{
					G()->FillRect(GAME_PROMO_RENDER_X + -GAME()->_topebox_game_scrollEff + 0 - 25, GAME_PROMO_RENDER_Y - GAME()->_topebox_game._icon_h + 0 + 10, fw, fw, true);
				}
				else if (GAME()->_topebox_game.typeAds == TopeBox_PromoGame::enumTypeTopeboxAds::TOPEBOX_ADS_BANNER && GAME()->_topebox_game.render_banner_topebox) {
					//_gp->FillRect(0, 0, k_SCREEN_WIDTH, k_SCREEN_HEIGHT, true);
					G()->FillFullScreen(true);
				}

				G()->ClearScale();
				G()->SetOpacity(100);
			}
			if (GAME()->_topebox_game.typeAds == TopeBox_PromoGame::enumTypeTopeboxAds::TOPEBOX_ADS_ICON)
			{
				GAME()->_topebox_game.RenderGamePromo(GAME_PROMO_RENDER_X + -GAME()->_topebox_game_scrollEff + 0 - 20, GAME_PROMO_RENDER_Y - GAME()->_topebox_game._icon_h + 0);
			}
			else if (GAME()->_topebox_game.typeAds == TopeBox_PromoGame::enumTypeTopeboxAds::TOPEBOX_ADS_BANNER && GAME()->_topebox_game.render_banner_topebox)
			{
				GAME()->_topebox_game.RenderGamePromo(k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
			}



#pragma endregion
		}
	}
}
bool WildWildWestGame::TouchButtonMainMenu()
{
	if (_isRenderAbout)
	{
		return false;
	}
	bool ret = false;
	int index = -1;
	for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
	{
		if (_DEC_BUTTON_ARRAY[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}
	if (ret)
	{
		if (_DEC_BUTTON_ARRAY[index].NeedAction())
		{
			switch (index)
			{
			case MAINMENU_BUTTON_ACHIEVEMENT:
#ifdef MAC_OS
				PineGameCenterController::ShowLeaderAchivement();
#endif
				break;
			case MAINMENU_BUTTON_SETING:
				InitSetting();
				break;
			case MAINMENU_BUTTON_RANKING:
				if (USER()._lvlLocation < MAX_LOCATION)
				{
#ifdef MAC_OS
					PineGameCenterController::ShowLeaderBoard("WWW_ADVENTURER");
#endif
				}
				else
				{
#ifdef MAC_OS
					PineGameCenterController::ShowLeaderBoard("WWW_HOWFAR");
#endif
				}
#if defined (_WINDOWS_PHONE8) || defined (_WIN8)
				if (_playWitchEndlest)
				{
					//_user._highScore = _user._score;
					_social.UpdateNewScore(&_svc_post_score, _user._highScore);
					if (_svc_post_score.Status == PineServiceResult::Responsed)
					{
						if (_svc_post_score.ResponseStatus == 0)
						{
							//succced post score

						}
						else
						{
							//error

						}
					}
				}

#endif
				break;
			case MAINMENU_BUTTON_MOREGAME:
#ifdef MAC_OS
				PineAdBox::ShowMoreGame();
#endif
				break;
				/*case MAINMENU_BUTTON_GGPLAY:

					break;*/
			case MAINMENU_BUTTON_PLAY:
				SetState(k_GS_PLAYGAME);
				break;
			case MAINMENU_BUTTON_SHOP:
				IniteShop();
				break;
			case MAINMENU_BUTTON_IAP:
				if (!_isRenderShop)
				{
					IniteShop();
					SwitchTypeShop(SHOP_TYPE_IAP);
				}
				else
				{
					SwitchTypeShop(SHOP_TYPE_IAP);
				}
				break;
			case MAINMENU_BUTTON_SHOP_YES:
				ProccessTouch(_ChosseIndex);
				_itemShop[_ChosseIndex]._isAcctive = false;
				_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_YES].SetState(BUTTON_STATE_HIDE);
				_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_NO].SetState(BUTTON_STATE_HIDE);
				break;
			case MAINMENU_BUTTON_SHOP_NO:
				_itemShop[_ChosseIndex]._isAcctive = false;
				_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_YES].SetState(BUTTON_STATE_HIDE);
				_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_NO].SetState(BUTTON_STATE_HIDE);
				break;

				//setting 
			case MAINMENU_BUTTON_SETTING_MUSIC:
				_isMusicEnable = !_isMusicEnable;
				GAME()->_musicEnable = _isMusicEnable;
				USER().DataSaveLocal();

				if (GAME()->_musicEnable)
				{
					if (_newScore)
					{
						GAME()->StopCurrentMusic();
						GAME()->LoadMusic(BGM_RESULT_SCREEN_WITH_NEW_HIGHSCORE);
						GAME()->PlayCurrentMusic(BGM_RESULT_SCREEN_WITH_NEW_HIGHSCORE);
					}
					else
					{
						GAME()->StopCurrentMusic();
						GAME()->LoadMusic(BGM_MAIN_MENU);
						GAME()->PlayCurrentMusic(BGM_MAIN_MENU);
					}
				}
				else
				{
					GAME()->StopCurrentMusic();
				}
				break;
			case MAINMENU_BUTTON_SETTING_SOUND:
				_isSoundEnable = !_isSoundEnable;
				GAME()->_soundEnable = _isSoundEnable;
				USER().DataSaveLocal();
				break;
			case MAINMENU_BUTTON_SETTING_NOTIFY:
				_isNotiEnable = !_isNotiEnable;
				GAME()->_notificationEnable = _isNotiEnable;
				USER().DataSaveLocal();
				break;
			case MAINMENU_BUTTON_SETTING_ABOUT:
				_scrollLogo = false;
				_closeAbout = false;
				CloseSetting();
				_isRenderAbout = true;
				_opacityAbout = 0;

				_aboutScrollY = 270 + OFFSET_I4(100) + OFFSET_IPAD(100);
				break;
			case MAINMENU_BUTTON_SETTING_RESET:
				_typeConfirm = CONFIRM_RESET_DATE;
				sprintf(_stringConfirm, "%s", GET_DES().getString(27));
				SetState(k_GS_CONFIRM);
				break;
			}
			for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
			{
				if (_DEC_BUTTON_ARRAY[i]._state != BUTTON_STATE_LOCK && _DEC_BUTTON_ARRAY[i]._state != BUTTON_STATE_HIDE)
				{
					_DEC_BUTTON_ARRAY[i].SetState(BUTTON_STATE_NORMAL);
				}
			}
			//GAME()->ClearAllTouch();
		}
		return true;
	}
	bool pressed = false;
	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		//if (TOUCHS(i)->_flag)
		{
			for (int touch = 0; touch < _DEC_BUTTON_NUMS; touch++)
			{
				if (_DEC_BUTTON_ARRAY[touch]._state == BUTTON_STATE_LOCK || _DEC_BUTTON_ARRAY[touch]._state == BUTTON_STATE_HIDE)
				{
					continue;
				}
				int x = _DEC_BUTTON_ARRAY[touch]._fixedX - 20;
				int y = _DEC_BUTTON_ARRAY[touch]._fixedY - 10;
				int w = _DEC_BUTTON_ARRAY[touch]._fixedW + 40;
				int h = _DEC_BUTTON_ARRAY[touch]._fixedH + 20;
				Button *button = &_DEC_BUTTON_ARRAY[touch];
				if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
				{
					pressed = true;
					if (TOUCHS(i)->_state == TOUCH_DOWN)
					{
						if (button->_touch_id == -1)
						{
							button->_touch_id = TOUCHS(i)->_id;
							button->SetState(BUTTON_STATE_SELECT);
						}
					}
					else if (TOUCHS(i)->_state == TOUCH_DRAG)
					{
						/*if (button->_state != BUTTON_STATE_SELECT && Button::GetButtonActive() == NULL)
						{
							button->SetState(BUTTON_STATE_SELECT);
						}*/
					}
					else if (TOUCHS(i)->_state == TOUCH_UP)
					{
						if (button->_touch_id == TOUCHS(i)->_id)
						{
							button->SetState(BUTTON_STATE_ACTIVE);
							button->_touch_id = -1;
						}
					}
				}
				else
				{
					if (TOUCHS(i)->_state == TOUCH_UP)
					{
						if (button->_touch_id == TOUCHS(i)->_id)
						{
							button->SetState(BUTTON_STATE_NORMAL);
							button->_touch_id = -1;
						}
					}
				}
			}

		}
	}
	if (pressed == false)
	{
		return false;
	}
	else
	{

		return true;
	}

	return false;
}

void WildWildWestGame::RenderLocation()
{
	float zoom = 1.0f;
	float offsetY = 0;
	if (GAME()->_device._version == SupportVersion::RESVER_960_640)
	{
		zoom = 1.20f;
		offsetY = 0.15 * 640 / 2;
	}
	if (CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
	{
		zoom = IPAD_ZOOM;
		offsetY = -IPAD_OFFSET;
	}
	GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 48, 0, 0 + offsetY);
	for (int i = 0; i < TOTAL_CHECK_POINT_DES_ROW; i++)
	{
		if (i <= USER()._lvlLocation)
		{
			if (USER()._currentLocation != i)
			{
				GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 49 + i, 0, 0 + offsetY);

				int x = GET_SPRITE(SPRITEID_HUD_SHOP)->GetFModuleX(48, i * 2);
				int y = GET_SPRITE(SPRITEID_HUD_SHOP)->GetFModuleY(48, i * 2);

				GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 58, x, y + offsetY);

				if (i < USER()._lvlLocation)
				{
					x = GET_SPRITE(SPRITEID_HUD_SHOP)->GetFModuleX(48, i * 2 + 1);
					y = GET_SPRITE(SPRITEID_HUD_SHOP)->GetFModuleY(48, i * 2 + 1) + offsetY;
					GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 59, x, y);
				}
			}
			else
			{
				GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 58 + i, 0, 0 + offsetY);

				int x = GET_SPRITE(SPRITEID_HUD_SHOP)->GetFModuleCenterX(49 + i, 0) + 1;
				int y = GET_SPRITE(SPRITEID_HUD_SHOP)->GetFModuleCenterY(49 + i, 0) - 60;


				_animCharater.SetPos(x, y + 2 + offsetY);
				_animCharater.DrawAnimObject(G());
				_animCharater.UpdateAnimObject();
			}
		}
	}
}
void WildWildWestGame::RenderStringVsWidth(int SpriteFont, float x, float y, INT32 value, float maxWidth, const char* ch)
{
	char string[20];
	sprintf(string, "%s%d", ch, value);
	float stringW = GET_SPRITE(SpriteFont)->GetAnsiTextWidth(string, 0, strlen(string));

	float deltaScale = 1;
	if (stringW > maxWidth)
	{
		deltaScale = maxWidth / stringW;
	}

	G()->SetScale(1.0f*deltaScale, 1.0f*deltaScale, x, y + 18);
	GET_SPRITE(SpriteFont)->DrawAnsiTextFormat(G(), x, y, _HCENTER | _VCENTER, string);
	G()->ClearScale();
}
void WildWildWestGame::RenderMoney(float x, float y, float maxW, INT32 value, float scale, bool updateAnim)
{
	char string[20];
	sprintf(string, "%d", value);
	float stringW = GET_SPRITE(SPRITEID_FONT_MONEY)->GetAnsiTextWidth(string, 0, strlen(string));

	float deltaScale = 1;
	float maxWidth = maxW;
	if (stringW > maxWidth)
	{
		deltaScale = maxWidth / stringW;
	}
	else
	{
		maxWidth = stringW;
	}

	G()->SetScale(deltaScale*scale, deltaScale*scale, x, y + 18);
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), x, y, _HCENTER | _VCENTER, string);

	_animCoin.SetPos(x + 15 + maxWidth*scale / 2, y + (36 - 15 * scale));
	G()->SetScale(deltaScale*scale, deltaScale*scale, x + 5 + maxWidth*scale / 2, y + (36 - 15 * scale));
	_animCoin.DrawAnimObject(G());

	if (updateAnim)
	{
		_animCoin.UpdateAnimObject();
	}


	G()->ClearScale();
}
void WildWildWestGame::RenderResult()
{

	float zoom = 1.0f;
	float offsetY = 0;
	if (GAME()->_device._version == SupportVersion::RESVER_960_640)
	{
		zoom = 1.20f;
		offsetY = -0.15 * 640 / 2;
	}
	if (CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
	{
		zoom = IPAD_ZOOM;
		offsetY = IPAD_OFFSET;
	}

	_rota1 += 0.2f;
	if (_rota1 >= 360)
	{
		_rota1 -= 360;
	}

	_rota2 -= 0.3f;
	if (_rota2 <= -360)
	{
		_rota2 += 360;
	}
	//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);
	G()->SetRotate(_rota1, k_SCREEN_WIDTH / 2, SCORE_Y + offsetY);
	GET_SPRITE(SPRITEID_HUD_RESULT)->DrawFrame(G(), 1, k_SCREEN_WIDTH / 2, SCORE_Y + offsetY);

	G()->SetRotate(_rota2, k_SCREEN_WIDTH / 2, SCORE_Y + offsetY);
	GET_SPRITE(SPRITEID_HUD_RESULT)->DrawFrame(G(), 2, k_SCREEN_WIDTH / 2, SCORE_Y + offsetY);
	G()->ClearRotate();

	//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);

	GET_SPRITE(SPRITEID_HUD_RESULT)->DrawFrame(G(), 0, k_SCREEN_WIDTH / 2, 0 + offsetY);

	char string[20];
	sprintf(string, "%d", (int)USER()._score);

	//_newScore = true;
	if (_newScore)
	{
		float stringW = GET_SPRITE(SPRITEID_FONT_RESULT)->GetAnsiTextWidth(string, 0, strlen(string));

		GET_SPRITE(SPRITEID_FONT_RESULT)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, SCORE_Y + offsetY, _VCENTER | _HCENTER, string);
		GET_SPRITE(SPRITEID_HUD_RESULT)->DrawFrame(G(), 3, k_SCREEN_WIDTH / 2 + stringW / 2 + 3, SCORE_Y + offsetY);
	}
	else
	{
		GET_SPRITE(SPRITEID_FONT_RESULT)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, SCORE_Y + offsetY, _VCENTER | _HCENTER, string);
	}


	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, COIN_COLLECT_Y + offsetY, _VCENTER | _HCENTER, "%d", USER()._highScore);

	//RenderMoney(k_SCREEN_WIDTH / 2 - 17, COIN_COLLECT_Y+offsetY, 500, BATTLE()._coinCollect, 1,false);

//	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, COIN_COLLECT_Y , _VCENTER | _HCENTER, "%d^", BATTLE()._coinCollect);

}

void WildWildWestGame::UpdateEffectScore()
{
	switch (_stateScore)
	{
	case 0:
		break;
	}
}
//Shop
void WildWildWestGame::IniteShop()
{
	for (int i = 0; i < 4; i++)
	{
		_effect_y[i] = k_SCREEN_HEIGHT;
		_effect_opa[i] = 0;
		_effect_sta[i] = 0;

		_effect_accel[i] = 1 + i*0.1;
	}
	_effect_sta[0] = 1;
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP].SetState(BUTTON_STATE_HIDE);

	_opacityShop = 100;
	_animShop.SetAnim(24, false);
	_animShop.SetPos(0, 0);
	if (_isRenderSetting)
	{
		CloseSetting();
	}
	_isRenderShop = true;
	_isMoveButon = true;
	_moveButtonPlay = 0;

	_maxButtonShop = 0;
	_numShopPower = 8;
	_numShopIap = 6;
	_numShopCharacter = 4;
	_touchDrag = false;

	_beginEffect = 0;
	SwitchTypeShop(SHOP_TYPE_POWER);
}
void WildWildWestGame::ProccessTouch(BYTE index)
{
	PDEBUG("NTD DEBUG ProcessTouch(index = %d)", index);
	bool has_pw = false;
	switch (_itemShop[index]._type)
	{
	case TYPE_SHOP_REVOLVER:
		ACHIEVEMENT().SetCompleteAchivement(19);
		if (CHARATER().GetLVLRevolver() < TOTAL_GUN_DES_ROW - 1)
		{
			CHARATER().SetLVLRevolver(CHARATER().GetLVLRevolver() + 1);
			USER().AddCoin(-_itemShop[index]._money);
			_itemShop[index]._money = GET_DES().getGunDes(CHARATER().GetLVLRevolver() + 1, GUN_DES_UPGRADE_COST);
		}
		_itemShop[index]._currnetLvl = CHARATER().GetLVLRevolver();
		if (CHARATER().GetLVLRevolver() >= TOTAL_GUN_DES_ROW - 1)
		{
			_itemShop[index]._state = ITEM_CAN_MAX;
			ACHIEVEMENT().SetCompleteAchivement(20);
		}
		has_pw = true;
		break;

	case TYPE_SHOP_FAST_TRIGGER:
		ACHIEVEMENT().SetCompleteAchivement(19);
		if (CHARATER().GetLVLFastTrigger() < TOTAL_GUN_DES_ROW - 1)
		{
			CHARATER().SetLVLFastTrigger(CHARATER().GetLVLFastTrigger() + 1);
			USER().AddCoin(-_itemShop[index]._money);
			_itemShop[index]._money = GET_DES().getGunDes(CHARATER().GetLVLFastTrigger() + 1, GUN_DES_UPGRADE_COST);
		}
		_itemShop[index]._currnetLvl = CHARATER().GetLVLFastTrigger();
		if (CHARATER().GetLVLFastTrigger() >= TOTAL_GUN_DES_ROW - 1)
		{
			_itemShop[index]._state = ITEM_CAN_MAX;
			ACHIEVEMENT().SetCompleteAchivement(20);
		}
		has_pw = true;
		break;

	case TYPE_SHOP_SPEED:
		ACHIEVEMENT().SetCompleteAchivement(19);
		if (CHARATER().GetLVLSpeed() < TOTAL_CHARATER_DES_ROW - 1)
		{
			CHARATER().SetLVLSpeed(CHARATER().GetLVLSpeed() + 1);
			USER().AddCoin(-_itemShop[index]._money);
			_itemShop[index]._money = GET_DES().getCharaterDes(CHARATER().GetLVLSpeed() + 1, CHARATER_DES_UPGRADE_COST);
		}
		_itemShop[index]._currnetLvl = CHARATER().GetLVLSpeed();
		if (CHARATER().GetLVLSpeed() >= TOTAL_CHARATER_DES_ROW - 1)
		{
			_itemShop[index]._state = ITEM_CAN_MAX;
			ACHIEVEMENT().SetCompleteAchivement(20);
		}
		has_pw = true;
		break;

	case TYPE_SHOP_MAGNET:
		ACHIEVEMENT().SetCompleteAchivement(19);
		if (CHARATER().GetLVLMagnet() < TOTAL_MAGNET_DES_ROW - 1)
		{
			CHARATER().SetLVLMagnet(CHARATER().GetLVLMagnet() + 1);
			USER().AddCoin(-_itemShop[index]._money);
			_itemShop[index]._money = GET_DES().getMagnetDes(CHARATER().GetLVLMagnet() + 1, MAGNET_DES_UPGRADE_COST);
		}
		_itemShop[index]._currnetLvl = CHARATER().GetLVLMagnet();
		if (CHARATER().GetLVLMagnet() >= TOTAL_MAGNET_DES_ROW - 1)
		{
			_itemShop[index]._state = ITEM_CAN_MAX;
			ACHIEVEMENT().SetCompleteAchivement(20);
		}
		has_pw = true;
		break;

	case TYPE_SHOP_MACHETE:
		ACHIEVEMENT().SetCompleteAchivement(19);
		if (CHARATER().GetLVLMachete() < TOTAL_MACHETE_DES_ROW - 1)
		{
			CHARATER().SetLVLMachete(CHARATER().GetLVLMachete() + 1);
			USER().AddCoin(-_itemShop[index]._money);
			_itemShop[index]._money = GET_DES().getMacheteDes(CHARATER().GetLVLMachete() + 1, MACHETE_DES_UPGRADE_COST);
		}
		_itemShop[index]._currnetLvl = CHARATER().GetLVLMachete();
		if (CHARATER().GetLVLMachete() >= TOTAL_MACHETE_DES_ROW - 1)
		{
			_itemShop[index]._state = ITEM_CAN_MAX;
			ACHIEVEMENT().SetCompleteAchivement(20);
		}
		has_pw = true;
		break;

	case TYPE_SHOP_MULE:
		ACHIEVEMENT().SetCompleteAchivement(19);
		if (CHARATER().GetLVLMule() < TOTAL_MULE_DES_ROW - 1)
		{
			CHARATER().SetLVLMule(CHARATER().GetLVLMule() + 1);
			USER().AddCoin(-_itemShop[index]._money);
			_itemShop[index]._money = GET_DES().getMuleDes(CHARATER().GetLVLMule() + 1, MULE_DES_UPGRADE_COST);
		}
		_itemShop[index]._currnetLvl = CHARATER().GetLVLMule();
		if (CHARATER().GetLVLMule() >= TOTAL_MULE_DES_ROW - 1)
		{
			_itemShop[index]._state = ITEM_CAN_MAX;
			ACHIEVEMENT().SetCompleteAchivement(20);
		}
		has_pw = true;
		break;

	case TYPE_SHOP_CAVALRY:
		ACHIEVEMENT().SetCompleteAchivement(19);
		if (CHARATER().GetLVLCavalry() < TOTAL_CAVALRY_DES_ROW - 1)
		{
			CHARATER().SetLVLCavalry(CHARATER().GetLVLCavalry() + 1);
			USER().AddCoin(-_itemShop[index]._money);
			_itemShop[index]._money = GET_DES().getCavalryDes(CHARATER().GetLVLCavalry() + 1, CAVALRY_DES_UPGRADE_COST);
		}
		_itemShop[index]._currnetLvl = CHARATER().GetLVLCavalry();
		if (CHARATER().GetLVLCavalry() >= TOTAL_CAVALRY_DES_ROW - 1)
		{
			_itemShop[index]._state = ITEM_CAN_MAX;
			ACHIEVEMENT().SetCompleteAchivement(20);
		}
		has_pw = true;
		break;

	case TYPE_SHOP_LIVE:
		ACHIEVEMENT().SetCompleteAchivement(19);
		if (CHARATER().GetLVLAlive() < TOTAL_CAVALRY_DES_ROW - 1)
		{
			CHARATER().SetLVLAlive(CHARATER().GetLVLAlive() + 1);
			USER().AddCoin(-_itemShop[index]._money);
			_itemShop[index]._money = GET_DES().getAliveDes(CHARATER().GetLVLAlive() + 1, LIVE_DES_UPGRADE_COST);
		}
		_itemShop[index]._currnetLvl = CHARATER().GetLVLAlive();
		if (CHARATER().GetLVLAlive() >= TOTAL_CAVALRY_DES_ROW - 1)
		{
			_itemShop[index]._state = ITEM_CAN_MAX;
			ACHIEVEMENT().SetCompleteAchivement(20);
		}
		has_pw = true;
		break;

		//iap
	case TYPE_SHOP_IAP_PACK_1:
#if defined MAC_OS
		if (!PineSocial::IsDeviceHacked() && !_isProssesIap)
		{
			_isProssesIap = true;
			PurchaseProduct(GDS_Exporter::iap_name_des[TOTAL_IAP_NAME_DES*(_itemShop[index]._type - TYPE_SHOP_IAP_PACK_1) + IAP_NAME_DES_APPLE_ID]);
		}
#elif defined(_WINDOWS_PHONE8) || defined(_WIN8)
		{
			_isProssesIap = true;
			PurchaseProduct(GDS_Exporter::iap_name_des[TOTAL_IAP_NAME_DES*(_itemShop[index]._type - TYPE_SHOP_IAP_PACK_1) + IAP_NAME_DES_APPLE_ID]);
		}
#elif defined(ANDROID_OS)
		_isProssesIap = true;
		//PurchaseProductWithType(GDS_Exporter::iap_name_des[TOTAL_IAP_NAME_DES*((int)(_itemShop[(int)index]._type) - TYPE_SHOP_IAP_PACK_1) + IAP_NAME_DES_GOOGLE_ID], 3);
		_purchase_iap_id = ((int)(_itemShop[(int)index]._type) - TYPE_SHOP_IAP_PACK_1);
#else
		USER().AddCoin(_itemShop[index]._money);
#endif
		break;

	case TYPE_SHOP_IAP_PACK_2:
#if defined MAC_OS
		if (!PineSocial::IsDeviceHacked() && !_isProssesIap)
		{
			_isProssesIap = true;
			_notShowPineAds = true;
			PurchaseProduct(GDS_Exporter::iap_name_des[TOTAL_IAP_NAME_DES*(_itemShop[index]._type - TYPE_SHOP_IAP_PACK_1) + IAP_NAME_DES_APPLE_ID]);
		}

#elif defined(_WINDOWS_PHONE8) || defined(_WIN8)
		{
			_isProssesIap = true;
			PurchaseProduct(GDS_Exporter::iap_name_des[TOTAL_IAP_NAME_DES*(_itemShop[index]._type - TYPE_SHOP_IAP_PACK_1) + IAP_NAME_DES_APPLE_ID]);
		}
#elif defined(ANDROID_OS)
		_isProssesIap = true;
		//PurchaseProductWithType(GDS_Exporter::iap_name_des[TOTAL_IAP_NAME_DES*((int)(_itemShop[(int)index]._type) - TYPE_SHOP_IAP_PACK_1) + IAP_NAME_DES_GOOGLE_ID], 3);
		_purchase_iap_id = ((int)(_itemShop[(int)index]._type) - TYPE_SHOP_IAP_PACK_1);
#else
		USER().AddCoin(_itemShop[index]._money);
#endif		
		break;

	case TYPE_SHOP_IAP_PACK_3:
#if defined MAC_OS
		if (!PineSocial::IsDeviceHacked() && !_isProssesIap)
		{
			_isProssesIap = true;
			_notShowPineAds = true;
			PurchaseProduct(GDS_Exporter::iap_name_des[TOTAL_IAP_NAME_DES*(_itemShop[index]._type - TYPE_SHOP_IAP_PACK_1) + IAP_NAME_DES_APPLE_ID]);
		}

#elif defined(_WINDOWS_PHONE8) || defined(_WIN8)
		{
			_isProssesIap = true;
			PurchaseProduct(GDS_Exporter::iap_name_des[TOTAL_IAP_NAME_DES*(_itemShop[index]._type - TYPE_SHOP_IAP_PACK_1) + IAP_NAME_DES_APPLE_ID]);
		}
#elif defined(ANDROID_OS)
		_isProssesIap = true;
		//PurchaseProductWithType(GDS_Exporter::iap_name_des[TOTAL_IAP_NAME_DES*((int)(_itemShop[(int)index]._type) - TYPE_SHOP_IAP_PACK_1) + IAP_NAME_DES_GOOGLE_ID], 3);
		_purchase_iap_id = ((int)(_itemShop[(int)index]._type) - TYPE_SHOP_IAP_PACK_1);
#else
		USER().AddCoin(_itemShop[index]._money);
#endif		
		break;

	case TYPE_SHOP_IAP_PACK_4:
#if defined MAC_OS
		if (!PineSocial::IsDeviceHacked() && !_isProssesIap)
		{
			_isProssesIap = true;
			_notShowPineAds = true;
			PurchaseProduct(GDS_Exporter::iap_name_des[TOTAL_IAP_NAME_DES*(_itemShop[index]._type - TYPE_SHOP_IAP_PACK_1) + IAP_NAME_DES_APPLE_ID]);
		}

#elif defined(_WINDOWS_PHONE8) || defined(_WIN8)
		{
			_isProssesIap = true;
			PurchaseProduct(GDS_Exporter::iap_name_des[TOTAL_IAP_NAME_DES*(_itemShop[index]._type - TYPE_SHOP_IAP_PACK_1) + IAP_NAME_DES_APPLE_ID]);
		}
#elif defined(ANDROID_OS)
		_isProssesIap = true;
		//PurchaseProductWithType(GDS_Exporter::iap_name_des[TOTAL_IAP_NAME_DES*((int)(_itemShop[(int)index]._type) - TYPE_SHOP_IAP_PACK_1) + IAP_NAME_DES_GOOGLE_ID], 3);
		_purchase_iap_id = ((int)(_itemShop[(int)index]._type) - TYPE_SHOP_IAP_PACK_1);
#else
		USER().AddCoin(_itemShop[index]._money);
#endif		
		break;

	case TYPE_SHOP_IAP_REMOVE:
#if defined MAC_OS
		if (!PineSocial::IsDeviceHacked() && !_isProssesIap)
		{
			_isProssesIap = true;
			_notShowPineAds = true;
			PurchaseProduct("WWW_RemoveAd");
		}
#elif defined(_WINDOWS_PHONE8) || defined(_WIN8)
		PurchaseProduct("WWW_RemoveAd");
#elif defined(ANDROID_OS)
		_isProssesIap = true;
		//PurchaseProduct("www_adsremove");
		_purchase_iap_id = 4;
#endif
		break;

	case TYPE_SHOP_IAP_RESTORE:
#if defined MAC_OS
		if (!PineSocial::IsDeviceHacked() && !_isProssesIap)
		{
			_isProssesIap = true;
			_notShowPineAds = true;
			PurchaseProduct("WWW_RemoveAd");
		}
#elif defined(ANDROID_OS)
		_isProssesIap = true;
		//PurchaseProduct("www_adsremove");
		_purchase_iap_id = 4;
#endif
		break;


	case TYPE_SHOP_CHARATER_0:
	case TYPE_SHOP_CHARATER_1:
	case TYPE_SHOP_CHARATER_2:
	case TYPE_SHOP_CHARATER_3:
		USER().AddCoin(-_itemShop[index]._money);
		_itemShop[index]._unlock = true;
		USER()._unlockCharater[index] = true;
		_itemShop[index]._state = ITEM_UNLOCK;
		Logs_GameStoreCharacter(_itemShop[index]._itemName);
		break;


	}
	if (has_pw)
	{
		Logs_GameStorePWLevel(_itemShop[index]._itemName, _itemShop[index]._currnetLvl);
	}
	
	USER().DataSave();
}

void WildWildWestGame::UpdateScrollShop()
{

	G()->SetColor(0xaaff0000);
	G()->FillRect(SCROLL_X, SCROLL_Y, SCROLL_W, SCROLL_H, true);

	//shop
	G()->SetColor(0xaaffff00);
	G()->FillRect(SHOP_X, SHOP_Y, SHOP_W, SHOP_H, true);

	//charater
	G()->SetColor(0xaaff00ff);
	G()->FillRect(CHARATER_X, CHARATER_Y, CHARATER_W, CHARATER_H, true);

	//power
	G()->SetColor(0xaa00ffff);
	G()->FillRect(IAP_X, IAP_Y, IAP_W, IAP_H, true);

	//ability
	G()->SetColor(0xaa0000ff);
	G()->FillRect(ABILITY_X, ABILITY_Y, ABILITY_W, ABILITY_H, true);


	float zoom = 1.0f;
	float offsetY = 0;
	if (GAME()->_device._version == SupportVersion::RESVER_960_640)
	{
		zoom = 1.20f;
		offsetY = -0.15 * 640 / 2;
	}
	if (CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
	{
		zoom = IPAD_ZOOM;
		offsetY = IPAD_OFFSET;
	}
	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		//if (TOUCHS(i)->_flag)
		{
			if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, CHARATER_X, CHARATER_Y + offsetY, CHARATER_W, CHARATER_H))
			{
				printf("\n 111111111111111111");
				/*if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					if (_typeShop != SHOP_TYPE_POWER)
					{
						GAME()->PlaySFX(SFX_BUTTON);
						SwitchTypeShop(SHOP_TYPE_POWER);
					}
					GAME()->ClearAllTouch();
				}
				else if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (!_scroll_shop._collided)
					{
						_scroll_shop.End();
					}
					_scroll_shop.SetState(2);
					_touchDrag = false;
					GAME()->ClearAllTouch();
				}*/
			}
			else if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, ABILITY_X, ABILITY_Y + offsetY, ABILITY_W, ABILITY_H))
			{
				printf("\n 222222222222");
				/*if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					if (_typeShop != SHOP_TYPE_CHARATER)
					{
						GAME()->PlaySFX(SFX_BUTTON);
						SwitchTypeShop(SHOP_TYPE_CHARATER);
					}
					GAME()->ClearAllTouch();
				}
				else if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (!_scroll_shop._collided)
					{
						_scroll_shop.End();
					}
					_scroll_shop.SetState(2);
					_touchDrag = false;
					GAME()->ClearAllTouch();
				}*/
			}
			else if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, IAP_X, IAP_Y + offsetY, IAP_W, IAP_H))
			{
				printf("\n 3333333333333");
				/*if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					if (_typeShop != SHOP_TYPE_IAP)
					{
						GAME()->PlaySFX(SFX_BUTTON);
						SwitchTypeShop(SHOP_TYPE_IAP);
					}
					GAME()->ClearAllTouch();
				}
				else if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (!_scroll_shop._collided)
					{
						_scroll_shop.End();
					}
					_scroll_shop.SetState(2);
					_touchDrag = false;
					GAME()->ClearAllTouch();
				}*/
			}
			else if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, SCROLL_X, SCROLL_Y + offsetY, SCROLL_W, SCROLL_H*zoom) ||
				CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, SHOP_X, SHOP_Y + offsetY, SHOP_W, SHOP_H))
			{
				/*printf("\n 44444444444");
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					_touchDrag = false;
					_offsetDrag = 0;
					GAME()->ClearAnotherTouch(i);
					_touch_drag_y = TOUCHS()->y;
					_scroll_shop.Set();
					_scroll_shop.SetState(0);
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					_touchDrag = true;
					_offsetDrag = TOUCHS(i)->y - _touch_drag_y;
					_touch_drag_y = TOUCHS()->y;
					_scroll_shop.AddOffset(-_offsetDrag);
					_scroll_shop.ValidateRangeExt();
					_scroll_shop.ForceToTarget();
					_scroll_shop.SetState(1);
				}
				else if (TOUCHS(i)->_state == TOUCH_UP)
				{
					int index = -1;
					if (CMath::ABSOLUTE_VALUE(_offsetDrag) < 1 && !_touchDrag)
					{
						for (int j = 0; j < _maxButtonShop; j++)
						{

							if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, SCROLL_X, _itemShop[j]._y + _scroll_y+ offsetY, SCROLL_W, _broadWidth - 10) && !_itemShop[j]._isAcctive)
							{
								if (_itemShop[j]._type >= TYPE_SHOP_IAP_PACK_1 && _itemShop[j]._type <= TYPE_SHOP_IAP_RESTORE)
								{
									_itemShop[j]._stateScale = 1;
									index = j;
									break;
								}
								else
								{
									if (!_itemShop[j]._isLock && _itemShop[j]._state == ITEM_CAN_BUY)
									{
										if (USER()._coin >= _itemShop[j]._money )
										{
											_itemShop[j]._stateScale = 1;
											index = j;
											break;
										}
										else
										{
											_itemShop[j]._stateScale = 1;
											if (_typeShop != SHOP_TYPE_IAP)
											{
												GAME()->PlaySFX(SFX_BUTTON);
												SwitchTypeShop(SHOP_TYPE_IAP);
												CGame::_this->ClearAllTouch();
											}
											//_scroll_shop.SetExt(5 * 150);
										}
									}

									if (_itemShop[j]._state == ITEM_UNLOCK && !_itemShop[j]._isLock)
									{
										if (j != _user._currentUseCharater)
										{
											_itemShop[j]._isuse = !_itemShop[j]._isuse;
											_itemShop[j]._stateScale = 1;
											GAME()->PlaySFX(SFX_BUTTON);
										}
										for (int k = 0; k < _maxButtonShop; k++)
										{
											if (k != j)
											{
												_itemShop[k]._isuse = false;
											}
											_user._useCharacter[k] = _itemShop[k]._isuse;
										}
										if (_itemShop[j]._isuse)
										{
											_user._currentUseCharater = j;
										}

										_user.DataSave();
									}
								}

							}
						}
					}

					if (index == -1)
					{

					}
					else
					{

						GAME()->PlaySFX(SFX_BUTTON);
						if (_typeShop == SHOP_TYPE_IAP)
						{
							ProccessTouch(index);
						}
						else
						{
							_ChosseIndex = index;
							for (int j = 0; j < _maxButtonShop; j++)
							{
								_itemShop[j]._isAcctive = false;
							}
							_itemShop[index]._isAcctive = true;

							_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_YES].SetState(BUTTON_STATE_NORMAL);
							_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_NO].SetState(BUTTON_STATE_NORMAL);
						}


						///ProccessTouch(index);
					}

					if (!_scroll_shop._collided)
					{
						_scroll_shop.End();
					}
					_scroll_shop.SetState(2);
					_touchDrag = false;
					GAME()->ClearAllTouch();
				}*/
			}
			else
			{
				printf("\n 555555555555");
				/*if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (!_touchDrag)
					{
						if (_isMoveButon)
						{
							_isMoveButon = false;
							_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP].SetState(BUTTON_STATE_NORMAL);
						}
					}

					if (!_scroll_shop._collided)
					{
						_scroll_shop.End();
					}
					_scroll_shop.SetState(2);
					_touchDrag = false;
					GAME()->ClearAllTouch();
				}*/
			}
		}
	}
}
void WildWildWestGame::UpdateChooseLocation()
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
	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		//if (TOUCHS(i)->_flag)
		{

			if (TOUCHS(i)->_state == TOUCH_UP)
			{
				for (int j = 0; j < TOTAL_CHECK_POINT_DES_ROW; j++)
				{
					int x = GET_SPRITE(SPRITEID_HUD_SHOP)->GetFModuleCenterX(49 + j, 0) - 50;
					int y = GET_SPRITE(SPRITEID_HUD_SHOP)->GetFModuleCenterY(49 + j, 0) - 50 - offsetY;
					int w = 100;
					int h = 100;

					if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
					{
						if (j <= USER()._lvlLocation && j != USER()._currentLocation)
						{
							GAME()->PlaySFX(SFX_BUTTON);
							USER()._currentLocation = j;

							if (USER()._currentLocation == TOTAL_CHECK_POINT_DES_ROW - 1)
							{
								_playWitchEndlest = true;
							}
							USER().DataSave();
						}
					}
				}

				if (_isRenderSetting)
				{
					CloseSetting();
				}
				if (_isRenderAbout && !_closeAbout)
				{
					_closeAbout = true;
				}

				//GAME()->ClearAllTouch();


			}
		}
	}
}
void WildWildWestGame::UpdateChooseLocation(int xx, int yy, int touch_state, int touch_id)
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
	if (touch_state == TOUCH_UP)
	{
		for (int j = 0; j < TOTAL_CHECK_POINT_DES_ROW; j++)
		{
			int x = GET_SPRITE(SPRITEID_HUD_SHOP)->GetFModuleCenterX(49 + j, 0) - 50;
			int y = GET_SPRITE(SPRITEID_HUD_SHOP)->GetFModuleCenterY(49 + j, 0) - 50 - offsetY;
			int w = 100;
			int h = 100;

			if (CHECK_POINT_IN_RECT(xx, yy, x, y, w, h))
			{
				if (j <= USER()._lvlLocation && j != USER()._currentLocation)
				{
					GAME()->PlaySFX(SFX_BUTTON);
					USER()._currentLocation = j;

					if (USER()._currentLocation == TOTAL_CHECK_POINT_DES_ROW - 1)
					{
						_playWitchEndlest = true;
					}
					USER().DataSave();
				}
			}
		}

		if (_isRenderSetting)
		{
			CloseSetting();
		}
		if (_isRenderAbout && !_closeAbout)
		{
			_closeAbout = true;
		}
	}
}

void WildWildWestGame::SwitchTypeShop(BYTE type)
{
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_YES].SetState(BUTTON_STATE_HIDE);
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_NO].SetState(BUTTON_STATE_HIDE);
	int BeginY = 90;
	if (type == SHOP_TYPE_CHARATER)
	{
		_broadWidth = 200;
		for (int i = 0; i < _numShopCharacter; i++)
		{
			_itemShop[i]._stateScale = 0;
			_itemShop[i]._scale = 1;
			_itemShop[i]._y = BeginY;
			_itemShop[i]._state = ITEM_CAN_BUY;
			_itemShop[i]._isAcctive = false;
			_itemShop[i]._isLock = false;

			sprintf(_itemShop[i]._itemName, "%s", GET_DES().getShopCharaterName(i, SHOP_CHARATER_NAME_NAME));
			sprintf(_itemShop[i]._itemDescrip, "%s", GET_DES().getShopCharaterName(i, SHOP_CHARATER_NAME_DESCRIPTION));
			int town = GET_DES().getLevlCheckPointDes(GET_DES().getShopCharaterDes(i, SHOP_CHARATER_DISTANCE_UNLOCK)) + 1;
			sprintf(_itemShop[i]._itemDescrip_2, GET_DES().getString(16), town);
			if (USER()._lvlLocation < town - 1)
			{
				_itemShop[i]._isLock = true;
			}

			_itemShop[i]._money = GET_DES().getShopCharaterDes(i, SHOP_CHARATER_PRICE);
			_itemShop[i]._type = TYPE_SHOP_CHARATER_0 + i;
			_itemShop[i]._spriteID = GET_DES().getShopCharaterDes(i, SHOP_CHARATER_SPRITE_ID);
			_itemShop[i]._unlock = USER()._unlockCharater[i];
			_itemShop[i]._isuse = USER()._useCharacter[i];

			_itemShop[i]._animCharater.SetAnimSprite(GET_SPRITE(_itemShop[i]._spriteID), _itemShop[i]._spriteID);
			_itemShop[i]._animCharater.SetAnim(12, true);
			if (USER()._unlockCharater[i])
			{
				_itemShop[i]._state = ITEM_UNLOCK;
			}


			BeginY += _broadWidth;
		}
		_maxButtonShop = _numShopCharacter;
	}
	else if (type == SHOP_TYPE_POWER)
	{
		_broadWidth = 200;
		for (int i = 0; i < _numShopPower; i++)
		{
			_itemShop[i]._stateScale = 0;
			_itemShop[i]._scale = 1;
			_itemShop[i]._y = BeginY;
			_itemShop[i]._state = ITEM_CAN_BUY;
			_itemShop[i]._isAcctive = false;
			_itemShop[i]._isLock = false;

			switch (i)
			{
			case 2:
				_itemShop[i]._type = TYPE_SHOP_SPEED;
				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getString(4));
				sprintf(_itemShop[i]._itemDescrip, "%s", GET_DES().getString(5));

				_itemShop[i]._currnetLvl = CHARATER().GetLVLSpeed();
				if (CHARATER().GetLVLSpeed() >= TOTAL_CHARATER_DES_ROW - 1)
				{
					_itemShop[i]._state = ITEM_CAN_MAX;
				}
				else
				{
					_itemShop[i]._money = GET_DES().getCharaterDes(CHARATER().GetLVLSpeed() + 1, CHARATER_DES_UPGRADE_COST);
				}
				break;

			case 0:
				_itemShop[i]._type = TYPE_SHOP_FAST_TRIGGER;
				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getString(0));
				sprintf(_itemShop[i]._itemDescrip, "%s", GET_DES().getString(1));
				_itemShop[i]._currnetLvl = CHARATER().GetLVLFastTrigger();
				if (CHARATER().GetLVLFastTrigger() >= TOTAL_GUN_DES_ROW - 1)
				{
					_itemShop[i]._state = ITEM_CAN_MAX;
				}
				else
				{
					_itemShop[i]._money = GET_DES().getGunDes(CHARATER().GetLVLFastTrigger() + 1, GUN_DES_UPGRADE_COST);
				}
				break;

			case 1:
				_itemShop[i]._type = TYPE_SHOP_REVOLVER;
				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getString(2));
				sprintf(_itemShop[i]._itemDescrip, "%s", GET_DES().getString(3));
				_itemShop[i]._currnetLvl = CHARATER().GetLVLRevolver();
				if (CHARATER().GetLVLRevolver() >= TOTAL_GUN_DES_ROW - 1)
				{
					_itemShop[i]._state = ITEM_CAN_MAX;
				}
				else
				{
					_itemShop[i]._money = GET_DES().getGunDes(CHARATER().GetLVLRevolver() + 1, GUN_DES_UPGRADE_COST);
				}
				break;

			case 3:
				_itemShop[i]._type = TYPE_SHOP_MAGNET;
				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getString(6));
				sprintf(_itemShop[i]._itemDescrip, "%s", GET_DES().getString(7));
				_itemShop[i]._currnetLvl = CHARATER().GetLVLMagnet();
				if (CHARATER().GetLVLMagnet() >= TOTAL_MAGNET_DES_ROW - 1)
				{
					_itemShop[i]._state = ITEM_CAN_MAX;
				}
				else
				{
					_itemShop[i]._money = GET_DES().getMagnetDes(CHARATER().GetLVLMagnet() + 1, MAGNET_DES_UPGRADE_COST);
				}
				break;

			case 4:
				_itemShop[i]._type = TYPE_SHOP_MACHETE;
				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getString(8));
				sprintf(_itemShop[i]._itemDescrip, "%s", GET_DES().getString(9));
				_itemShop[i]._currnetLvl = CHARATER().GetLVLMachete();
				if (CHARATER().GetLVLMachete() >= TOTAL_MAGNET_DES_ROW - 1)
				{
					_itemShop[i]._state = ITEM_CAN_MAX;
				}
				else
				{
					_itemShop[i]._money = GET_DES().getMacheteDes(CHARATER().GetLVLMachete() + 1, MACHETE_DES_UPGRADE_COST);
				}
				break;

			case 5:
				_itemShop[i]._type = TYPE_SHOP_MULE;
				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getString(10));
				if (!USER()._isUnlockMule)
				{
					int town = GET_DES().getLevlCheckPointDes(GET_DES().getMuleDes(CHARATER().GetLVLMule(), MULE_DES_DISTANCE_UNLOCK)) + 1;
					sprintf(_itemShop[i]._itemDescrip, GET_DES().getString(16), town);
					_itemShop[i]._isLock = true;
				}
				else
				{
					sprintf(_itemShop[i]._itemDescrip, "%s", GET_DES().getString(11));
				}

				_itemShop[i]._currnetLvl = CHARATER().GetLVLMule();
				if (CHARATER().GetLVLMule() >= TOTAL_MULE_DES_ROW - 1)
				{
					_itemShop[i]._state = ITEM_CAN_MAX;
				}
				else
				{
					_itemShop[i]._money = GET_DES().getMuleDes(CHARATER().GetLVLMule() + 1, MULE_DES_UPGRADE_COST);
				}
				break;

			case 6:
				_itemShop[i]._type = TYPE_SHOP_CAVALRY;
				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getString(12));
				if (!USER()._isUnlockCavalry)
				{
					int town = GET_DES().getLevlCheckPointDes(GET_DES().getCavalryDes(CHARATER().GetLVLCavalry(), CAVALRY_DES_DISTANCE_UNLOCK)) + 1;
					sprintf(_itemShop[i]._itemDescrip, GET_DES().getString(16), town);


					_itemShop[i]._isLock = true;
				}
				else
				{
					sprintf(_itemShop[i]._itemDescrip, "%s", GET_DES().getString(13));
				}
				_itemShop[i]._currnetLvl = CHARATER().GetLVLCavalry();
				if (CHARATER().GetLVLCavalry() >= TOTAL_CAVALRY_DES_ROW - 1)
				{
					_itemShop[i]._state = ITEM_CAN_MAX;
				}
				else
				{
					_itemShop[i]._money = GET_DES().getCavalryDes(CHARATER().GetLVLCavalry() + 1, CAVALRY_DES_UPGRADE_COST);
				}
				break;

			case 7:
				_itemShop[i]._type = TYPE_SHOP_LIVE;
				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getString(14));
				sprintf(_itemShop[i]._itemDescrip, "%s", GET_DES().getString(15));
				_itemShop[i]._currnetLvl = CHARATER().GetLVLAlive();
				if (CHARATER().GetLVLAlive() >= TOTAL_CHARATER_DES_ROW - 1)
				{
					_itemShop[i]._state = ITEM_CAN_MAX;
				}
				else
				{
					_itemShop[i]._money = GET_DES().getAliveDes(CHARATER().GetLVLAlive() + 1, LIVE_DES_UPGRADE_COST);
				}
				break;
			}

			BeginY += _broadWidth;
		}
		_maxButtonShop = _numShopPower;
	}
	else if (type == SHOP_TYPE_IAP)
	{
		_broadWidth = 150;
		for (int i = 0; i < _numShopIap; i++)
		{
			_itemShop[i]._stateScale = 0;
			_itemShop[i]._scale = 1;
			_itemShop[i]._y = BeginY;
			_itemShop[i]._state = ITEM_CAN_BUY;
			_itemShop[i]._isAcctive = false;
			switch (i)
			{
			case 0:
				_itemShop[i]._type = TYPE_SHOP_IAP_PACK_1;
				_itemShop[i]._state = ITEM_CAN_BUY;
#if defined (_WINDOWS_PHONE8) || defined (_WIN8)
				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getIAPNameDes(i, IAP_NAME_DES_NAME));
				sprintf(_itemShop[i]._itemDescrip, "%s", _inapp_price[i]);
				_itemShop[i]._money = GET_DES().getIAPValuDes(i, IAP_VALUE_DES_VALUE);
#else
				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getIAPNameDes(i, IAP_NAME_DES_NAME));
				sprintf(_itemShop[i]._itemDescrip, "%s$", GET_DES().getIAPNameDes(i, IAP_NAME_DES_PRICE));
				_itemShop[i]._money = GET_DES().getIAPValuDes(i, IAP_VALUE_DES_VALUE);
#endif


				break;

			case 1:
				_itemShop[i]._type = TYPE_SHOP_IAP_PACK_2;
				_itemShop[i]._state = ITEM_CAN_BUY;
#if defined (_WINDOWS_PHONE8) || defined (_WIN8)
				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getIAPNameDes(i, IAP_NAME_DES_NAME));
				sprintf(_itemShop[i]._itemDescrip, "%s", _inapp_price[i]);
				_itemShop[i]._money = GET_DES().getIAPValuDes(i, IAP_VALUE_DES_VALUE);
#else
				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getIAPNameDes(i, IAP_NAME_DES_NAME));
				sprintf(_itemShop[i]._itemDescrip, "%s$", GET_DES().getIAPNameDes(i, IAP_NAME_DES_PRICE));
				_itemShop[i]._money = GET_DES().getIAPValuDes(i, IAP_VALUE_DES_VALUE);
#endif

				break;

			case 2:
				_itemShop[i]._type = TYPE_SHOP_IAP_PACK_3;
				_itemShop[i]._state = ITEM_CAN_BUY;
#if defined (_WINDOWS_PHONE8) || defined (_WIN8)
				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getIAPNameDes(i, IAP_NAME_DES_NAME));
				sprintf(_itemShop[i]._itemDescrip, "%s", _inapp_price[i]);
				_itemShop[i]._money = GET_DES().getIAPValuDes(i, IAP_VALUE_DES_VALUE);
#else
				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getIAPNameDes(i, IAP_NAME_DES_NAME));
				sprintf(_itemShop[i]._itemDescrip, "%s$", GET_DES().getIAPNameDes(i, IAP_NAME_DES_PRICE));
				_itemShop[i]._money = GET_DES().getIAPValuDes(i, IAP_VALUE_DES_VALUE);
#endif

				break;

			case 3:
				_itemShop[i]._type = TYPE_SHOP_IAP_PACK_4;
				_itemShop[i]._state = ITEM_CAN_BUY;
#if defined (_WINDOWS_PHONE8) || defined (_WIN8)

				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getIAPNameDes(i, IAP_NAME_DES_NAME));
				sprintf(_itemShop[i]._itemDescrip, "%s", _inapp_price[i]);
				_itemShop[i]._money = GET_DES().getIAPValuDes(i, IAP_VALUE_DES_VALUE);
#else
				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getIAPNameDes(i, IAP_NAME_DES_NAME));
				sprintf(_itemShop[i]._itemDescrip, "%s$", GET_DES().getIAPNameDes(i, IAP_NAME_DES_PRICE));
				_itemShop[i]._money = GET_DES().getIAPValuDes(i, IAP_VALUE_DES_VALUE);
#endif

				break;

			case 4:
				_itemShop[i]._type = TYPE_SHOP_IAP_REMOVE;
				_itemShop[i]._state = ITEM_CAN_BUY;
#if defined (_WINDOWS_PHONE8) || defined (_WIN8)
				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getIAPNameDes(i, IAP_NAME_DES_NAME));
				sprintf(_itemShop[i]._itemDescrip, "%s", _inapp_price[i]);
				_itemShop[i]._money = GET_DES().getIAPValuDes(i, IAP_VALUE_DES_VALUE);
#else
				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getIAPNameDes(i, IAP_NAME_DES_NAME));
				//sprintf(_itemShop[i]._itemDescrip, "%s$", GET_DES().getIAPNameDes(i, IAP_NAME_DES_PRICE));
				_itemShop[i]._money = GET_DES().getIAPValuDes(i, IAP_VALUE_DES_VALUE);
#endif

				break;

			case 5:
				_itemShop[i]._type = TYPE_SHOP_IAP_RESTORE;
				_itemShop[i]._state = ITEM_CAN_BUY;
				sprintf(_itemShop[i]._itemName, "%s", GET_DES().getIAPNameDes(4, IAP_NAME_DES_NAME));
				sprintf(_itemShop[i]._itemDescrip, "%s$", GET_DES().getIAPNameDes(4, IAP_NAME_DES_PRICE));
				_itemShop[i]._money = 15000;
				break;

			}

			BeginY += _broadWidth;
		}
		_maxButtonShop = _numShopIap;
	}
	if (_maxButtonShop <= MIN_ITEM_SHOP)
	{
		_scroll_y = 0;
	}
	else
	{

		float offsetY = 0;
		if (CGame::_this->_device._version == SupportVersion::RESVER_960_640)
		{
			offsetY = -100;
		}
		if (CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
		{
			offsetY = IPAD_OFFSET - 100;
		}

		if (type == SHOP_TYPE_IAP)
		{
			_scroll_shop.Init(0, (_maxButtonShop - MIN_ITEM_SHOP) * _broadWidth - 100 + offsetY, 3, 2);
		}
		else
		{
			_scroll_shop.Init(0, 50 + (_maxButtonShop - MIN_ITEM_SHOP) * _broadWidth + offsetY, 3, 2);
		}

	}


	_typeShop = type;

}
void WildWildWestGame::CheckNewNotify()
{
	//check speed
	if (_isNotifySpeed == NOTIFY_TYPE_NONE)
	{
		if (USER()._coin >= GET_DES().getCharaterDes(CHARATER().GetLVLSpeed() + 1, CHARATER_DES_UPGRADE_COST))
		{
			_isNotifySpeed = NOTIFY_TYPE_NEW;
		}
	}

	//check revoler	
	if (_isNotifyRevolver == NOTIFY_TYPE_NONE)
	{
		if (USER()._coin >= GET_DES().getGunDes(CHARATER().GetLVLRevolver() + 1, GUN_DES_UPGRADE_COST))
		{
			_isNotifyRevolver = NOTIFY_TYPE_NEW;
		}
	}

	//check fast trigger
	if (_isNotifyFastTrigger == NOTIFY_TYPE_NONE)
	{
		if (USER()._coin >= GET_DES().getGunDes(CHARATER().GetLVLFastTrigger() + 1, GUN_DES_UPGRADE_COST))
		{
			_isNotifyFastTrigger = NOTIFY_TYPE_NEW;
		}
	}

	//check magnet
	if (_isNotifyMagnet == NOTIFY_TYPE_NONE)
	{
		if (USER()._coin >= GET_DES().getMagnetDes(CHARATER().GetLVLMagnet() + 1, MAGNET_DES_UPGRADE_COST))
		{
			_isNotifyMagnet = NOTIFY_TYPE_NEW;
		}
	}

	//check machete
	if (_isNotifyMachete == NOTIFY_TYPE_NONE)
	{
		if (USER()._coin >= GET_DES().getMacheteDes(CHARATER().GetLVLMachete() + 1, MACHETE_DES_UPGRADE_COST))
		{
			_isNotifyMachete = NOTIFY_TYPE_NEW;
		}
	}

	//check mule
	if (_isNotifyMule == NOTIFY_TYPE_NONE)
	{
		if (USER()._coin >= GET_DES().getMuleDes(CHARATER().GetLVLMule() + 1, MULE_DES_UPGRADE_COST))
		{
			_isNotifyMule = NOTIFY_TYPE_NEW;
		}
	}

	//check cavalry
	if (_isNotifyCavalry == NOTIFY_TYPE_NONE)
	{
		if (USER()._coin >= GET_DES().getCavalryDes(CHARATER().GetLVLCavalry() + 1, TOTAL_CAVALRY_DES_ROW))
		{
			_isNotifyCavalry = NOTIFY_TYPE_NEW;
		}
	}

	//check shop
}
void WildWildWestGame::RenderShop(float x, float y)
{
	float  offsetY = 0;
	float zoom = 1.0f;
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
	y = y + offsetY;

	G()->SetOpacity(_opacityShop);
	_animShop.SetPos(x, y);
	_animShop.DrawAnimObject(G());
	_animShop.UpdateAnimObject();

	for (int i = 0; i < 4; i++)
	{
		if (_effect_sta[i] > 0)
		{
			_effect_opa[i] += (int)10 * _effect_accel[i];
			if (_effect_opa[i] >= 100)
			{
				_effect_opa[i] = 100;
			}

			if (_effect_opa[i] >= 50)
			{
				if (i < 3 && _effect_sta[i + 1] == 0)
				{
					_effect_sta[i + 1] = 1;
				}
			}

			switch (_effect_sta[i])
			{
			case 1:
				_effect_y[i] -= 30 * _effect_accel[i];
				if (_effect_y[i] <= -8)
				{
					_effect_y[i] = -8;
					_effect_sta[i] = 2;
				}
				break;
			case 2:
				_effect_y[i] += 2 * _effect_accel[i];
				if (_effect_y[i] >= 3)
				{
					_effect_y[i] = 3;
					_effect_sta[i] = 3;
				}
				break;
			case 3:
				_effect_y[i] -= 1 * _effect_accel[i];
				if (_effect_y[i] <= 0)
				{
					_effect_y[i] = 0;
					_effect_sta[i] = -1;
				}
				break;

			}
		}
	}

	if (_animShop.IsCurrentAnimStop())
	{
		int numButton = _maxButtonShop;
		if (_typeShop == SHOP_TYPE_CHARATER)
		{
			GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 113, x, y);
		}
		else if (_typeShop == SHOP_TYPE_POWER)
		{
			GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 27, x, y);

		}
		else if (_typeShop == SHOP_TYPE_IAP)
		{
			GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 28, x, y);
			if (USER()._removeAdd)
			{
				numButton = _maxButtonShop - 2;

				float offsetY = 0;
				if (CGame::_this->_device._version == SupportVersion::RESVER_960_640)
				{
					offsetY = -100;
				}
				if (CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
				{
					offsetY = IPAD_OFFSET - 100;
				}
				_scroll_shop.SetMax((numButton - MIN_ITEM_SHOP) * _broadWidth - 100 + offsetY);
			}
		}

		G()->SetClip(SCROLL_X + x, SCROLL_Y + y, SCROLL_W, SCROLL_H*zoom);

		for (int i = 0; i < numButton; i++)
		{
			//update touch scale
			switch (_itemShop[i]._stateScale)
			{
			case 1:
				_itemShop[i]._scale -= 0.02;
				if (_itemShop[i]._scale <= 0.95)
				{
					_itemShop[i]._stateScale = 2;
				}
				break;

			case 2:
				_itemShop[i]._scale += 0.02;
				if (_itemShop[i]._scale >= 1.0f)
				{
					_itemShop[i]._scale = 1;
					_itemShop[i]._stateScale = 0;
				}
				break;
			}

			float eff_y = 0;

			//if
			if (i >= _beginEffect && i < 4 + _beginEffect)
			{
				if (_effect_sta[i - _beginEffect] > 0)
				{
					G()->SetOpacity(_effect_opa[i - _beginEffect]);
				}
				eff_y = _effect_y[i - _beginEffect];
			}

			//render
			float centerScaleX = GET_SPRITE(SPRITEID_HUD_SHOP)->GetFModuleX(29, 1) + x;
			float centerScaleY = GET_SPRITE(SPRITEID_HUD_SHOP)->GetFModuleCenterY(29, 1) + y + _itemShop[i]._y + _scroll_y + eff_y;

			G()->SetScale(_itemShop[i]._scale, _itemShop[i]._scale, centerScaleX, centerScaleY);
			if (_typeShop == SHOP_TYPE_POWER || _typeShop == SHOP_TYPE_CHARATER)
			{
				if (_itemShop[i]._state == ITEM_CAN_BUY)
				{
					GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 29, x, y + _itemShop[i]._y + _scroll_y + eff_y);
				}
				else
				{
					GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 30, x, y + _itemShop[i]._y + _scroll_y + eff_y);
				}
			}
			else
			{
				GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 102, x, y + _itemShop[i]._y + _scroll_y + eff_y);
			}

			if (_itemShop[i]._type >= TYPE_SHOP_CHARATER_0 && _itemShop[i]._type <= TYPE_SHOP_CHARATER_3)
			{
				_itemShop[i]._animCharater.SetPos(x + k_SCREEN_WIDTH / 2 + 80, y + 150 + _itemShop[i]._y + _scroll_y + eff_y);
				G()->SetScale(_itemShop[i]._scale*0.8, _itemShop[i]._scale*0.8, centerScaleX + 80, centerScaleY + 150);
				_itemShop[i]._animCharater.DrawAnimObject(G());

				_itemShop[i]._animCharater.UpdateAnimObject();
				//GET_SPRITE(_itemShop[i]._spriteID)->DrawFrame(G(), 11, );
			}

			if (_itemShop[i]._type < TYPE_SHOP_IAP_PACK_1)
			{
				GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), _itemShop[i]._type, x, y + _itemShop[i]._y + _scroll_y + eff_y);


				G()->SetScale(_itemShop[i]._scale*0.55, _itemShop[i]._scale*0.55, centerScaleX, centerScaleY + 50);
				GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiPageFormat(G(), centerScaleX, centerScaleY + 50, 700, _HCENTER | _VCENTER, _itemShop[i]._itemDescrip);

			}




			G()->SetScale(_itemShop[i]._scale, _itemShop[i]._scale, centerScaleX, centerScaleY);

			if (_itemShop[i]._isAcctive)
			{
				int offsetY = 0;
				if (_typeShop == SHOP_TYPE_CHARATER)
				{
					offsetY = 20;
				}
				_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_YES].SetPos(x, y + offsetY + _itemShop[i]._y + _scroll_y);
				_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_YES].Render(G());
				_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_YES].Update();

				_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_NO].SetPos(x, y + offsetY + _itemShop[i]._y + _scroll_y);
				_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_NO].Render(G());
				_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_NO].Update();

				G()->SetScale(_itemShop[i]._scale*0.8, _itemShop[i]._scale*0.8, centerScaleX, centerScaleY + offsetY);
				if (_typeShop == SHOP_TYPE_CHARATER)
				{
					GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiPageFormat(G(), x + k_SCREEN_WIDTH - 120, y + offsetY + 5 + _itemShop[i]._y + _scroll_y, 650, _HCENTER | _VCENTER, "Are you sure?");
				}
				else
				{
					GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiPageFormat(G(), x + k_SCREEN_WIDTH - 120, y + 5 + _itemShop[i]._y + _scroll_y, 650, _HCENTER | _VCENTER, "Are you sure?");
				}

			}
			else
			{
				if (_itemShop[i]._type < TYPE_SHOP_IAP_PACK_1)
				{
					GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), _itemShop[i]._type, x, y + _itemShop[i]._y + _scroll_y + eff_y);
					G()->SetScale(_itemShop[i]._scale*0.5, _itemShop[i]._scale*0.5, centerScaleX, centerScaleY);
					G()->SetColorMask(0xffff00);
					GET_SPRITE(SPRITEID_FONT_DISTANCE)->DrawAnsiTextFormat(G(), x + k_SCREEN_WIDTH + 230, y + 15 + _itemShop[i]._y + _scroll_y + eff_y, _RIGHT | _TOP, _itemShop[i]._itemName);
					G()->ClearColorMask();
				}
			}

			G()->SetScale(_itemShop[i]._scale, _itemShop[i]._scale, centerScaleX, centerScaleY);

			if (_itemShop[i]._type >= TYPE_SHOP_CHARATER_0 && _itemShop[i]._type <= TYPE_SHOP_CHARATER_3)
			{
				int offsetYYYY = 0;
				if (!_itemShop[i]._isAcctive)
				{
					if (_itemShop[i]._state == ITEM_CAN_BUY)
					{
						if (_itemShop[i]._isLock)
						{
							offsetYYYY = 20;
							G()->SetScale(_itemShop[i]._scale*0.55, _itemShop[i]._scale*0.55, centerScaleX + 105, centerScaleY + offsetYYYY);
							GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiPageFormat(G(), centerScaleX + 105, y + offsetYYYY + 130 + _itemShop[i]._y + _scroll_y + eff_y, 700, _HCENTER | _VCENTER, _itemShop[i]._itemDescrip_2);
						}
						else
						{
							char stringMoney[20];
							sprintf(stringMoney, "%d^", _itemShop[i]._money);
							if (_coin_scroll < _itemShop[i]._money || _itemShop[i]._isLock)
							{
								sprintf(stringMoney, "<P>%d<C>^", _itemShop[i]._money);
							}
							GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextPageFormat(G(), x + k_SCREEN_WIDTH - 160, y + 130 + _itemShop[i]._y + _scroll_y + eff_y, 200, _VCENTER | _HCENTER, stringMoney);
						}
					}
					else
					{
						GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 114, x - 10, y + 45 + _itemShop[i]._y + _scroll_y + eff_y);
						if (_itemShop[i]._isuse)
						{
							GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 115, x - 10, y + 45 + _itemShop[i]._y + _scroll_y + eff_y);
						}
					}

					G()->SetScale(_itemShop[i]._scale*0.5, _itemShop[i]._scale*0.5, centerScaleX + 100, centerScaleY + offsetYYYY);
					G()->SetColorMask(0xffff00);
					GET_SPRITE(SPRITEID_FONT_DISTANCE)->DrawAnsiTextFormat(G(), centerScaleX + 100, y + offsetYYYY + 10 + _itemShop[i]._y + _scroll_y + eff_y, _HCENTER | _TOP, _itemShop[i]._itemName);
					G()->ClearColorMask();

					//if (_itemShop[i]._type >= TYPE_SHOP_CHARATER_0 && _itemShop[i]._type <= TYPE_SHOP_CHARATER_3)
					{
						G()->SetScale(_itemShop[i]._scale*0.55, _itemShop[i]._scale*0.55, centerScaleX + 105, centerScaleY - 20 + offsetYYYY);
						GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiPageFormat(G(), centerScaleX + 105, centerScaleY - 20 + offsetYYYY, 700, _HCENTER | _VCENTER, _itemShop[i]._itemDescrip);
					}
				}


			}
			else if (_itemShop[i]._type >= TYPE_SHOP_IAP_PACK_1 && _itemShop[i]._type <= TYPE_SHOP_IAP_RESTORE)//render IAP
			{
				if (_itemShop[i]._type < TYPE_SHOP_IAP_REMOVE)
				{
					G()->SetScale(_itemShop[i]._scale*0.7, _itemShop[i]._scale*0.7, centerScaleX, y + 25 + _itemShop[i]._y + _scroll_y + eff_y);
					G()->SetColorMask(0xffff00);
					GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextPageFormat(G(), centerScaleX, y + 25 + _itemShop[i]._y + _scroll_y + eff_y, 200, _VCENTER | _HCENTER, _itemShop[i]._itemName);
					G()->ClearColorMask();
					G()->ClearScale();

					G()->SetScale(_itemShop[i]._scale, _itemShop[i]._scale, centerScaleX, centerScaleY);
					GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextPageFormat(G(), x + k_SCREEN_WIDTH - 20, y + 95 + _itemShop[i]._y + _scroll_y + eff_y, 200, _VCENTER | _RIGHT, _itemShop[i]._itemDescrip);

					GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), x + k_SCREEN_WIDTH / 2 + 260, y + 95 + _itemShop[i]._y + _scroll_y + eff_y, _RIGHT | _VCENTER, "%d^", _itemShop[i]._money);
				}
				else
				{
					if (_itemShop[i]._type == TYPE_SHOP_IAP_REMOVE)
					{
						//GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextPageFormat(G(), x + k_SCREEN_WIDTH - 20, y + 70 + _itemShop[i]._y + _scroll_y + eff_y, 200, _VCENTER | _RIGHT, _itemShop[i]._itemDescrip);
					}
					GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), _itemShop[i]._type, x, y + _itemShop[i]._y + _scroll_y + eff_y);
				}

			}
			else //render power
			{
				if (!_itemShop[i]._isAcctive)
				{
					//PDEBUG("\n _itemShop[%d]._state:%d", i, _itemShop[i]._state);
					if (_itemShop[i]._state == ITEM_CAN_BUY)
					{
						char stringMoney[20];
						sprintf(stringMoney, "%d^", _itemShop[i]._money);
						//PDEBUG("\n _itemShop[%d]._state:%d -- money:%d -- islock:%d --- _coin_scroll:%d -- town:%d", i, _itemShop[i]._state, _itemShop[i]._money, _itemShop[i]._isLock, _coin_scroll, USER()._lvlLocation);
						if (_coin_scroll < _itemShop[i]._money || _itemShop[i]._isLock)
						{
							sprintf(stringMoney, "<P>%d<C>^", _itemShop[i]._money);
						}
						GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextPageFormat(G(), x + k_SCREEN_WIDTH - 20, y + 95 + _itemShop[i]._y + _scroll_y + eff_y, 200, _VCENTER | _RIGHT, stringMoney);

					}
					else
					{
						G()->SetColorMask(0xffff00);
						GET_SPRITE(SPRITEID_FONT_DISTANCE)->DrawAnsiTextFormat(G(), x + k_SCREEN_WIDTH - 20, y + 95 + _itemShop[i]._y + _scroll_y + eff_y, _RIGHT | _VCENTER, "MAX");
						G()->ClearColorMask();
					}

				}


				if (_itemShop[i]._type == TYPE_SHOP_FAST_TRIGGER || _itemShop[i]._type == TYPE_SHOP_REVOLVER)
				{
					GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 31, x, y + _itemShop[i]._y + _scroll_y + eff_y);
					for (int j = 0; j < _itemShop[i]._currnetLvl; j++)
					{
						GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 32, x + j * 48, y + _itemShop[i]._y + _scroll_y + eff_y);
					}
				}
				else
				{
					GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 100, x, y + _itemShop[i]._y + _scroll_y + eff_y);
					for (int j = 0; j < _itemShop[i]._currnetLvl; j++)
					{
						GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 101, x + j * 80, y + _itemShop[i]._y + _scroll_y + eff_y);
					}
				}


				G()->ClearColorMask();
			}
			G()->ClearScale();
		}

		G()->ClearClip();
	}

	G()->SetOpacity(100);
}

//setting 
void WildWildWestGame::InitSetting()
{
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETING].SetState(BUTTON_STATE_LOCK);
	/*_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_MUSIC].SetState(BUTTON_STATE_NORMAL);
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_SOUND].SetState(BUTTON_STATE_NORMAL);
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_NOTIFY].SetState(BUTTON_STATE_NORMAL);
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_ABOUT].SetState(BUTTON_STATE_NORMAL);*/
	for (int i = MAINMENU_BUTTON_SETTING_MUSIC; i <= MAINMENU_BUTTON_SETTING_ABOUT; i++)
	{
		_DEC_BUTTON_ARRAY[i].SetState(BUTTON_STATE_NORMAL);
	}
	//_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_RESET].SetState(BUTTON_STATE_NORMAL);

	_isRenderSetting = true;
	_animSetting.SetAnim(27, false);
	_animSetting.SetPos(0, 0);
	if (_isMoveButon)
	{
		_isMoveButon = false;
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP].SetState(BUTTON_STATE_NORMAL);
	}
}
void WildWildWestGame::CloseSetting()
{
	for (int i = MAINMENU_BUTTON_SETTING_MUSIC; i <= MAINMENU_BUTTON_SETTING_ABOUT; i++)
	{
		_DEC_BUTTON_ARRAY[i].SetState(BUTTON_STATE_HIDE);
	}
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETING].SetState(BUTTON_STATE_NORMAL);
	/*_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_MUSIC].SetState(BUTTON_STATE_HIDE);
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_SOUND].SetState(BUTTON_STATE_HIDE);
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_NOTIFY].SetState(BUTTON_STATE_HIDE);
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_ABOUT].SetState(BUTTON_STATE_HIDE);*/
	//_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SETTING_RESET].SetState(BUTTON_STATE_HIDE);
	_isRenderSetting = false;
}
void WildWildWestGame::RenderSetting()
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
	}    _animSetting.SetPos(0, offsetY);
	_animSetting.DrawAnimObject(G());
	_animSetting.UpdateAnimObject();

	if (_animSetting.IsCurrentAnimStop())
	{
		GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 106, 0, offsetY);

		for (int i = MAINMENU_BUTTON_SETTING_MUSIC; i <= MAINMENU_BUTTON_SETTING_RESET; i++)
		{
			_DEC_BUTTON_ARRAY[i].SetPos(0 + 45, offsetY);
			_DEC_BUTTON_ARRAY[i].Render(G());
			_DEC_BUTTON_ARRAY[i].Update();
		}

		if (!_isMusicEnable)
		{
			GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 91, 0 + 45, offsetY);
		}

		if (!_isSoundEnable)
		{
			GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 89, 0 + 45, offsetY);
		}

		if (!_isNotiEnable)
		{
			GET_SPRITE(SPRITEID_HUD_SHOP)->DrawFrame(G(), 93, 0 + 45, offsetY);
		}
	}
}

void WildWildWestGame::RenderAbout()
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
	if (!_closeAbout)
	{
		_opacityMainMenu -= 5;
		if (_opacityMainMenu <= 0)
		{
			_opacityMainMenu = 0;
		}
		_opacityAbout += 5;
		if (_opacityAbout >= 100)
		{
			_opacityAbout = 100;
		}
	}
	G()->SetColor(0xaa000000);
	G()->FillFullScreen(true);
	_aboutScrollY -= 1.5f;
	if (_aboutScrollY <= -1540 - 50)
	{
		_aboutScrollY = 650 + OFFSET_I4(100);
	}

	if (_aboutScrollY < 0)
	{
		_scrollLogo = true;
	}

	float beginY = _aboutScrollY + offsetY;
	if (_scrollLogo)
	{
		GET_SPRITE(SPRITEID_BG_MAIN_MENU)->DrawFrame(G(), 1, 0, beginY);
	}
	else
	{
		GET_SPRITE(SPRITEID_BG_MAIN_MENU)->DrawFrame(G(), 1, 0, offsetY);
	}


	//Producer
	beginY += 380;
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, beginY, _HCENTER | _VCENTER, "PRODUCER");

	G()->SetColorMask(0xffff00);
	beginY += 100;
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, beginY, _HCENTER | _VCENTER, "LIEM THAI THANH");

	beginY += 50;
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, beginY, _HCENTER | _VCENTER, "NGHIA DO THANH");
	G()->ClearColorMask();

	//Design
	beginY += 100;
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, beginY, _HCENTER | _VCENTER, "GAME DESIGN");


	G()->SetColorMask(0xffff00);
	beginY += 100;
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, beginY, _HCENTER | _VCENTER, "VINH NGUYEN THE");
	G()->ClearColorMask();
	//Artist
	beginY += 100;
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, beginY, _HCENTER | _VCENTER, "ARTIST");

	G()->SetColorMask(0xffff00);
	beginY += 100;
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, beginY, _HCENTER | _VCENTER, "VU HOANG TRAN ANH");

	beginY += 50;
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, beginY, _HCENTER | _VCENTER, "DUY LE");

	beginY += 50;
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, beginY, _HCENTER | _VCENTER, "VU NGUYEN");
	G()->ClearColorMask();

	//Programmer
	beginY += 100;
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, beginY, _HCENTER | _VCENTER, "PROGRAMMER");

	G()->SetColorMask(0xffff00);
	beginY += 100;
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, beginY, _HCENTER | _VCENTER, "QUOC NGUYEN TRONG");
	beginY += 50;
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, beginY, _HCENTER | _VCENTER, "LAM NGUYEN VIET");
	beginY += 50;
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, beginY, _HCENTER | _VCENTER, "KHA LE MINH");
	beginY += 50;
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, beginY, _HCENTER | _VCENTER, "KHOA HO KY");
	G()->ClearColorMask();

	//Copy right
	beginY += 100;
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, beginY, _HCENTER | _VCENTER, "# 2017 topebox");

}

////////////////////////////
void WildWildWestGame::SetTouchProcessing(int touchID)
{
	_touchID = touchID;
}
void WildWildWestGame::ClearTouchProcessing()
{
	_touchID = -1;
}
bool WildWildWestGame::CheckTouchProcessing(int touchID)
{
	if (_touchID == touchID)
	{
		return true;
	}
	return false;
}

bool WildWildWestGame::TouchButtonGGPlay(int touch_x, int touch_y, int touch_state, int touch_id) {
	bool pressed = false;
	if (!ProcessingButtonGGPlay())
	{
		for (int touch = 0; touch < GGPLAY_BUTTON_NUM; touch++)
		{
			/*if (CheckTouchProcessing(touch_id) && touch_state == TOUCH_DOWN)
			{
				continue;
			}*/
			if (_buttonGGPlayMenu[touch]._state == BUTTON_STATE_LOCK || _buttonGGPlayMenu[touch]._state == BUTTON_STATE_HIDE)
			{
				continue;
			}

			int x = _buttonGGPlayMenu[touch].TZ_GetX() - 20;
			int y = _buttonGGPlayMenu[touch].TZ_GetY() - 20;
			int w = _buttonGGPlayMenu[touch].TZ_GetWidth() + 40;
			int h = _buttonGGPlayMenu[touch].TZ_GetHeight() + 40;


			Button *button = &_buttonGGPlayMenu[touch];

			//printf("\n[%d]: touchX = %d, touchY = %d => x= %d, y = %d, w = %d, h = %d", touch, touch_x, touch_y, x, y, w, h);
			if (CHECK_POINT_IN_RECT(touch_x, touch_y, x, y, w, h))
			{
				if (touch_state == TOUCH_DOWN)
				{
					if (button->_touch_id == -1)
					{
						button->SetState(BUTTON_STATE_SELECT, touch_id, TOUCH_DOWN);
						SetTouchProcessing(touch_id);
						button->_touch_id = touch_id;
						pressed = true;
					}
					/*else if (button->CheckState(BUTTON_STATE_SELECT, touch_id))
					{

						pressed = true;
					}*/
				}
				else if (touch_state == TOUCH_DRAG)
				{
					/*if (button->CheckState(BUTTON_STATE_SELECT, touch_id))
					{

						pressed = true;
					}*/
				}
				else if (touch_state == TOUCH_UP)
				{
					if (button->_touch_id == touch_id)
					{
						button->SetState(BUTTON_STATE_ACTIVE);
						button->_touch_id = -1;
						//break;
						pressed = true;
					}
				}
				if (button->_touch_id == touch_id)
				{
					pressed = true;
				}
			}
			else
			{
				if (touch_state == TOUCH_UP)
				{
					if (button->_touch_id == touch_id)
					{
						button->SetState(BUTTON_STATE_NORMAL);
						button->_touch_id = -1;
						//break;
						pressed = true;
					}
				}
			}
		}
	}
	return pressed;
}

bool WildWildWestGame::TouchButtonMainMenu(int touch_x, int touch_y, int touch_state, int touch_id)
{

	bool pressed = false;
	//PDEBUG("\n _quest_popup_active:%d -- _is_ggplay_menu_active:%d", _quest_popup_active, _is_ggplay_menu_active);
	if (_is_ggplay_menu_active || _quest_popup_active || _rating_active)
	{
		return true;
	}
	if (!ProcessingButton())
	{
		//PDEBUG("\n mainmenu ProcessingButton");
		if (_isRenderAbout)
		{
			return false;
		}
		for (int touch = 0; touch < _DEC_BUTTON_NUMS; touch++)
		{
			/*if (CheckTouchProcessing(touch_id) && touch_state == TOUCH_DOWN)
			{
				continue;
			}*/
			if (_DEC_BUTTON_ARRAY[touch]._state == BUTTON_STATE_LOCK || _DEC_BUTTON_ARRAY[touch]._state == BUTTON_STATE_HIDE)
			{
				continue;
			}
			if (_isRenderShop)
			{
				if (touch != MAINMENU_BUTTON_SHOP_YES && touch != MAINMENU_BUTTON_SHOP_NO)
				{
					continue;
				}
			}
			int x = _DEC_BUTTON_ARRAY[touch].TZ_GetX() - 20;
			int y = _DEC_BUTTON_ARRAY[touch].TZ_GetY() - 20;
			int w = _DEC_BUTTON_ARRAY[touch].TZ_GetWidth() + 40;
			int h = _DEC_BUTTON_ARRAY[touch].TZ_GetHeight() + 40;


			Button *button = &_DEC_BUTTON_ARRAY[touch];

			//printf("\n[%d]: touchX = %d, touchY = %d => x= %d, y = %d, w = %d, h = %d", touch, touch_x, touch_y, x, y, w, h);
			if (CHECK_POINT_IN_RECT(touch_x, touch_y, x, y, w, h))
			{
				if (touch_state == TOUCH_DOWN)
				{
					if (button->_touch_id == -1)
					{
						//PDEBUG("\n Mainbt index:%d", touch);
						button->SetState(BUTTON_STATE_SELECT, touch_id, TOUCH_DOWN);
						SetTouchProcessing(touch_id);
						button->_touch_id = touch_id;
						pressed = true;
					}
					/*else if (button->CheckState(BUTTON_STATE_SELECT, touch_id))
					{
						pressed = true;
					}*/
				}
				else if (touch_state == TOUCH_DRAG)
				{
					/*if (button->CheckState(BUTTON_STATE_SELECT, touch_id))
					{
						pressed = true;
					}*/
				}
				else if (touch_state == TOUCH_UP)
				{
					if (button->_touch_id == touch_id)
					{
						//PDEBUG("\n bt up index:%d", touch);
						button->SetState(BUTTON_STATE_ACTIVE);
						button->_touch_id = -1;
						//break;
						pressed = true;
					}
				}
				if (button->_touch_id == touch_id)
				{
					pressed = true;
				}
			}
			else {
				if (touch_state == TOUCH_UP) {
					if (button->_touch_id == touch_id)
					{
						button->SetState(BUTTON_STATE_NORMAL);
						button->_touch_id = -1;
						pressed = true;
					}
				}
			}

		}
	}
	return pressed;
}
void WildWildWestGame::NotifyRender(float x, float y, int opac) {

	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	int pos = 0;
	isCloseQuest = false;
	x += 50;
	if (_DEC_BUTTON_ARRAY[QUEST_BUTTON_GIFT]._state != BUTTON_STATE_HIDE)
	{
		_DEC_BUTTON_ARRAY[QUEST_BUTTON_GIFT].SetState(BUTTON_STATE_HIDE);
	}
	for (int i = QUEST_BUTTON_GIFT; i < _DEC_BUTTON_NUMS; i++)
	{
		if (_DEC_BUTTON_ARRAY[QUEST_BUTTON_GIFT]._state == BUTTON_STATE_HIDE)
		{
			y += -60;
			if (i != QUEST_BUTTON_GIFT)
			{
				GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->DrawFrame(G(), 7, x, y + pos * 134);
			}
			
		}
		else
		{
			GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->DrawFrame(G(), 7, x, y + pos * 134);
		}
		
		pos++;
		_DEC_BUTTON_ARRAY[i].SetPos(x , y);
		_DEC_BUTTON_ARRAY[i].Render(G());
		_DEC_BUTTON_ARRAY[i].Update();
	}

	if (NumQuestCompleted() > 0)
	{
		GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->DrawFrame(G(), 11, x, y /*+ (pos-1) * 134*/);
		G()->SetColorMask(0xffff0000);
		G()->SetScale(0.7f, 0.7f, x + GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleCenterX(11, 0), y + GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleCenterY(11, 0) + 10);
		GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), x + GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleCenterX(11,0), y + GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleCenterY(11, 0) + 10 /*+ pos * 134*/, _HCENTER, "%d", NumQuestCompleted());
		G()->ClearScale();
		G()->ClearColorMask();
	}
}
void WildWildWestGame::CQuest::Init() {
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	for (int i = 0; i < NUM_BUTTON_QUEST; i++) {
		_quest_button[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, 0);
		_quest_button[i].SetAnim(SPRITEID_HUD_DAILY_REWARD, i * 4);
		_quest_button[i].SetPos(0, 0);
	}
	_animCoin.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_COIN), SPRITEID_HUD_COIN);
	_animCoin.SetAnim(0, true);
	_readyCollect = false;
	isComplete = false;
	_isDoubleReward = false;
}
void WildWildWestGame::CQuest::Render(float x, float y, int opac) {
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	//PDEBUG("\n quest _id:%d", _id);
	GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->DrawFrame(G(), 10, x, y);
	G()->SetScale(0.7f, 0.7f, GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleX(10, 0) + 20, y + GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleY(10, 0) + 55);
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleX(10, 0) + 20, y + GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleY(10, 0) + 55, _LEFT, "%s",_name);
	G()->ClearScale();
	G()->SetScale(0.7f, 0.7f, GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleX(10, 0) + 20, y + GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleY(10, 0) + 55 + 65);
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiPageFormat(G(), GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleX(10, 0) + 20, y + GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleY(10, 0) + 25 + 65, 660, _LEFT | _TOP,"%s", _title);
	G()->ClearScale();
	G()->SetScale(0.7f, 0.7f, GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleX(10, 0) + 20, y + GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleY(10, 0) + 55 + 60 + 95 - 40);
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleX(10, 0) + 20, y + GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleY(10, 0) + 55 + 60 + 95 - 40, _LEFT | _TOP,"%s%%", _progress);
	G()->ClearScale();
	G()->SetScale(0.7f, 0.7f, GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleX(10, 0) + 20, y + GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleY(10, 0) + 55 + 60 + 95);
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleX(10, 0) + 20, y + GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleY(10, 0) + 55 + 60 + 95, _LEFT,"Reward: %d", _reward_quest);
	G()->ClearScale();
	char rw_text[512];
	sprintf(rw_text, "Reward: %d", _reward_quest);
	int strW = GET_SPRITE(SPRITEID_FONT_MONEY)->GetAnsiTextWidth(rw_text, 0, strlen(rw_text));
	_animCoin.SetPos(GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleX(10, 0) - 26 + strW, y + GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleY(10, 0) + 55 + 60 + 98);
	//G()->SetScale(1, 1, GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleX(10, 0) + 20 + strW, y + GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleY(10, 0) + 55 + 60 + 95);
	_animCoin.DrawAnimObject(G());

	if (true)
	{
		_animCoin.UpdateAnimObject();
	}
	//G()->ClearScale();
#if ADS_DISABLE
	if (_quest_button[BUTTON_QUEST_X2REWARD]._state != BUTTON_STATE_HIDE)
	{
		_quest_button[BUTTON_QUEST_X2REWARD].SetState(BUTTON_STATE_HIDE);
	}
#endif
	for (int i = 0; i < NUM_BUTTON_QUEST; i++)
	{
		if (this->isComplete)
		{
			GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->DrawFrame(G(), 14, 0, y - GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleY(3, 0) - 95);
			break;
		}
		if (!_readyCollect)
		{
			if (_quest_button[i]._state != BUTTON_STATE_LOCK && _quest_button[i]._state != BUTTON_STATE_HIDE)
			{
				_quest_button[i].SetState(BUTTON_STATE_LOCK);
			}
		}
		else
		{
			if (i == 1)
			{
				if (!_isDoubleReward)
				{
					if (_quest_button[i]._state == BUTTON_STATE_LOCK)
					{
						_quest_button[i].SetState(BUTTON_STATE_NORMAL);
					}
				}
				else
				{
					if (_quest_button[i]._state != BUTTON_STATE_LOCK && _quest_button[i]._state != BUTTON_STATE_HIDE)
					{
						_quest_button[i].SetState(BUTTON_STATE_LOCK);
					}
				}
			}
			else
			{
				if (_quest_button[i]._state == BUTTON_STATE_LOCK)
				{
					_quest_button[i].SetState(BUTTON_STATE_NORMAL);
				}
			}
		}
		

		_quest_button[i].SetPos(x, y);
		float claim_y = 0;
		if (_quest_button[BUTTON_QUEST_X2REWARD]._state == BUTTON_STATE_HIDE)
		{
			claim_y = 70;
			_quest_button[BUTTON_QUEST_CLAIM].SetPos(x, y + claim_y);
		}
		_quest_button[i].Render(G());
		_quest_button[i].Update();
		float scale = 1.0f;
		if (_quest_button[i]._state == BUTTON_STATE_SELECT)
		{
			scale = 0.95f;
		}
		switch (i) {
		case BUTTON_QUEST_CLAIM:
			G()->SetScale(0.7f*scale, 0.7f*scale, GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleX(1, 0) + 95, y + GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleY(1, 0) + 22 + claim_y);
			GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleX(1, 0) + 95, y + GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleY(1, 0) + 22 + claim_y, _HCENTER | _VCENTER, "Claim");
			G()->ClearScale();
			break;

		case BUTTON_QUEST_X2REWARD:
			if (_quest_button[BUTTON_QUEST_X2REWARD]._state != BUTTON_STATE_HIDE)
			{
				G()->SetScale(0.7f*scale, 0.7f*scale, GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleX(3, 0) + 95, y + GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleY(3, 0) + 26);
				GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextPageFormat(G(), GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleX(3, 0) + 95, y + GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleY(3, 0) + 26, 256, _HCENTER | _VCENTER, "X2\nReward");
				G()->ClearScale();
			}
			break;
		}
	}


}

void WildWildWestGame::CQuest::Update() {
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	if (!UpdateTouch())
	{
		int rectQuest_x = GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleX(0, 0);
		int rectQuest_y = GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleY(0, 0);
		int reactQuest_w = GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFrameWidth(0);
		int reactQuest_h = GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFrameHeight(0);
		for (int i = GAME()->_num_touches - 1; i >= 0; i--)
		{
			if (!CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, rectQuest_x, rectQuest_y, reactQuest_w, reactQuest_h))
			{
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					if (game->_quest_popup_active)
					{
						game->_quest_popup_active = false;
						game->_quest_popup_isclose = true;
						TOUCHS(i)->_state = TOUCH_DRAG;
					}
				}
			}
		}
	}
}

void WildWildWestGame::QuestInit() {
	_quest_popup_active = false;
	_quest_popup_isclose = false;
	_quest_info_num = MAX_QUEST;
	for (int i = 0; i < _quest_info_num; i++) {
		_quest_info[i].Init();
	}
	_scroll_quest.Init(0, _quest_info_num*QUEST_ITEM_HEIGHT, 3);
	_numQuestCompleted = 0;
}

bool WildWildWestGame::CQuest::UpdateTouch() {
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	bool press = false;
	bool ret = false;
	int index = -1;
	if (game->_is_waiting_for_reward && !game->_on_video_ads_reward)
	{
		return true;
	}
	if (isComplete || !_readyCollect)
	{
		return ret;
	}
#if defined (MAC_OS)
	if ((PineAdBox::showAdState == SHOW_AD_PROCESSING || PineAdBox::showAdState == SHOW_AD_COMPLETED) && _has_watched_ads)
	{
		if (PineAdBox::showAdState == SHOW_AD_COMPLETED)
		{
			game->_has_watched_ads = false;
			game->_has_double_reward = false;
			_isDoubleReward = true;
			_quest_button[1].SetState(BUTTON_STATE_LOCK);
			_reward_quest = _reward_quest * 2;
			game->Logs_DailyQuest_Double(true);
			return  true;
		}
		else
		{
			game->_has_watched_ads = false;
			game->_has_double_reward = false;
		}
	}
#elif defined (ANDROID_OS)
	PDEBUG("\n _is_waiting_for_reward:%d -- _on_video_ads_reward:%d", game->_is_waiting_for_reward, game->_on_video_ads_reward);
	if (game->_is_waiting_for_reward)
	{
		if (game->_on_video_ads_reward)
		{
			game->_has_watched_ads = false;
			game->_on_video_ads_reward = false;
			game->_is_waiting_for_reward = false;
			_isDoubleReward = true;
			_quest_button[1].SetState(BUTTON_STATE_LOCK);
			//GameDataSave();
			PDEBUG("\n _reward_quest:%d", _reward_quest);
			_reward_quest = _reward_quest * 2;
			PDEBUG("\n 2222 _reward_quest:%d", _reward_quest);
			game->Logs_DailyQuest_Double(true);
			return true;
		}

	}
	else
	{
		if (game->_has_watched_ads)
		{
			game->_has_watched_ads = false;
			game->_on_video_ads_reward = false;
			game->_is_waiting_for_reward = false;
		}
	}
#endif
	for (int i = 0; i < NUM_BUTTON_QUEST; i++)
	{
		//PDEBUG("\n button state[%d]: %d", i, _quest_button[i]._state);
		if (_quest_button[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}
	
	//PDEBUG("\n button idx: %d", index);
	if (ret)
	{
		PDEBUG("\n true button idx: %d", index);
		if (_quest_button[index].NeedAction())
		{
			//PDEBUG("\n index = %d", index);
			switch (index)
			{
			case BUTTON_QUEST_CLAIM:
				//PDEBUG("\n touch---->BUTTON_QUEST_CLAIM quest %d", _index);
				game->_user.AddCoin(_reward_quest);
				isComplete = true;
				game->Logs_QuestCompleted(_name, ACHIEVEMENT()._dailyQuestCurrent+1);
				ACHIEVEMENT()._isQuestComplete[index] = QUEST_END;
				ACHIEVEMENT().DailyQuestNext();
				game->_user.DataSave();
				
				break;
			case BUTTON_QUEST_X2REWARD:
				
				//PDEBUG("\n touch----->BUTTON_QUEST_X2REWARD quest %d", _index);
				//game->Logs_DailyQuest_Double(true);
#if !ADS_DISABLE
#if defined (MAC_OS)
				{

					int adboxtype = PineAdBox::HasVideoAds();
					printf("\n adboxtype: %d", adboxtype);
					if (adboxtype >= 0)
					{
						game->_has_watched_ads = true;
						PineAdBox::ShowVideoAds(adboxtype);
					}

				}
#elif defined (ANDROID_OS)
				if (Device::IsNetworkAvailable())
				{
					game->_has_watched_ads = true;
					GAME()->PrepareAndShowVideoAds();
				}
#else
				_reward_quest = _reward_quest * 2;
#endif
#endif
				break;
			default:
				break;
			}
			for (int i = 0; i < NUM_BUTTON_QUEST; i++)
			{
				if (_quest_button[i]._state != BUTTON_STATE_LOCK && _quest_button[i]._state != BUTTON_STATE_HIDE)
				{
					_quest_button[i].SetState(BUTTON_STATE_NORMAL);
				}
			}
			return true;
		}

	}
	//PDEBUG("\n GAME()->_num_touches: %d", GAME()->_num_touches);
	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		for (int touch = 0; touch < NUM_BUTTON_QUEST; touch++)
		{
			if (_quest_button[touch]._state == BUTTON_STATE_LOCK || _quest_button[touch]._state == BUTTON_STATE_HIDE)
			{
				continue;
			}
			int x = _quest_button[touch].TZ_GetX();
			int y = _quest_button[touch].TZ_GetY();
			int w = _quest_button[touch].TZ_GetWidth();
			int h = _quest_button[touch].TZ_GetHeight();
			Button *button = &_quest_button[touch];



			if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
			{
				//PDEBUG("\n touch:%d x = %d, y = %d, w = %d, h = %d ---- touchx:%d,touchy:%d", touch, x, y, w, h, TOUCHS(i)->x, TOUCHS(i)->y);
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					//PDEBUG("\n TOUCH_DOWN: %d", TOUCHS(i)->_state);
					if (button->_touch_id == -1)
					{
						button->SetState(BUTTON_STATE_SELECT);
						button->_touch_id = TOUCHS(i)->_id;
						//TOUCHS(i)->_state = TOUCH_DRAG;
						press = true;
						PLAY_SFX(SFX_BUTTON);
						//PDEBUG("\n button->_touch_id: %d", button->_touch_id);
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						//button->SetState(BUTTON_STATE_SELECT);
						//button->_touch_id = TOUCHS(i)->_id;
						//TOUCHS(i)->_state = TOUCH_DRAG;
						press = true;
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_UP)
				{

					if (button->_touch_id == TOUCHS(i)->_id)
					{
						//PDEBUG("\n in touch up id: %d", button->_touch_id);
						//button->SetState(BUTTON_STATE_NORMAL);
						button->SetState(BUTTON_STATE_ACTIVE);
						button->_touch_id = -1;
						press = true;
					}
				}
			}
			else
			{
				if (TOUCHS(i)->_state == TOUCH_UP)
				{

					if (button->_touch_id == TOUCHS(i)->_id)
					{
						//PDEBUG("\n in touch up quest bt id: %d", touch);
						//button->SetState(BUTTON_STATE_NORMAL);
						button->SetState(BUTTON_STATE_NORMAL);
						button->_touch_id = -1;
						press = true;
					}
				}
			}
		}

	}
	return press;
}
void WildWildWestGame::QuestUpdate() {
	for (int i = 0; i < _quest_info_num; i++)
	{
		_quest_info[i].Update();
	}
	_scroll_quest.Update();
	_quest_scroll_y = _scroll_quest.GetCurrent();
}
int WildWildWestGame::NumQuestCompleted()
{
	int count = 0;
	for (int i = 0; i < _quest_info_num; i++)
	{
		if (_quest_info[i]._readyCollect && !_quest_info[i].isComplete)
		{
			count++;
		}
	}
	return count;
}

void WildWildWestGame::QuestActive() {
	_quest_popup_active = true;
	_quest_popup_isclose = false;
}
void WildWildWestGame::QuestRender(float x, float y, int opac) {
	if (!_quest_popup_active)
	{
		return;
	}
	G()->SetColor(0x88000000);
	G()->FillFullScreen(true);
	GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->DrawFrame(G(), 0, x, y);
	GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->DrawFrame(G(), 13, x, y);
	int top_each = GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFrameHeight(10) + 20;
	for (int i = 0; i < MAX_QUEST; i++)
	{
		G()->SetClip(GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleX(0, 0) + 20, GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleY(0, 0) + 60, GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFrameWidth(0) - 40, GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFrameHeight(0) - 80);
		_quest_info[i].Render(x, y + i*top_each - _quest_scroll_y, opac);
		_quest_info[i]._index = i;
		G()->ClearClip();

	}
	_scroll_quest.SetMax(_quest_info_num * 145);
}
bool WildWildWestGame::ProcessingButtonQuest() {
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	bool ret = false;
	int index = -1;
	/*if (!_quest_popup_active)
	{
		return false;
	}
	for (int i = 0; i < NUM_BUTTON_QUEST * MAX_QUEST; i++)
	{
		if (_quest_button[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}
	if (ret)
	{
		if (_quest_button[index].NeedAction())
		{
			switch (index)
			{

			case BUTTON_QUEST_CLAIM:
				PDEBUG("\n====================> claim pressed");
				break;
			case BUTTON_QUEST_X2REWARD:
				PDEBUG("\n====================> x2 reward pressed");
				break;

			default:
				break;
			}
			for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
			{
				PDEBUG("\n reset button");
				if (_DEC_BUTTON_ARRAY[i]._state != BUTTON_STATE_LOCK && _DEC_BUTTON_ARRAY[i]._state != BUTTON_STATE_HIDE)
				{
					_DEC_BUTTON_ARRAY[i].SetState(BUTTON_STATE_NORMAL, -1);
				}
			}
		}
		return true;
	}*/
	return false;
}
bool WildWildWestGame::ProcessingButtonGGPlay() {
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	bool ret = false;
	int index = -1;

	for (int i = 0; i < GGPLAY_BUTTON_NUM; i++)
	{
		if (_buttonGGPlayMenu[i]._state == BUTTON_STATE_ACTIVE)
		{
			//PDEBUG("\n _buttonGGPlayMenu[i]._state:%d", _buttonGGPlayMenu[i]._state);
			ret = true;
			index = i;
			break;
		}
	}


	if (ret)
	{
		if (_buttonGGPlayMenu[index].NeedAction())
		{
			switch (index)
			{
			case GGPLAY_BUTTON_CLOSE:
				_is_ggplay_menu_active = false;
				_is_ggplay_menu_close = true;
				break;

			case GGPLAY_BUTTON_LEADERBOARD:
				printf("\n Show leaderboard");
				game->GooglePlayShowLeaderBoard();
				break;

			case GGPLAY_BUTTON_ACHIEVEMENT:
				printf("\n Show achievement");
				game->GooglePlayShowAchievement();
				break;

			case GGPLAY_BUTTON_SYNC:
				printf("\n Sync cloud");
				game->GooglePlaySynDriver();
				break;

			default:
				break;
			}
			for (int i = 0; i < GGPLAY_BUTTON_NUM; i++)
			{
				if (_buttonGGPlayMenu[i]._state != BUTTON_STATE_LOCK && _buttonGGPlayMenu[i]._state != BUTTON_STATE_HIDE)
				{
					_buttonGGPlayMenu[i].SetState(BUTTON_STATE_NORMAL, -1);
				}
			}
		}
		return true;
	}
	return false;
}

bool WildWildWestGame::ProcessingButton()
{
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	bool ret = false;
	int index = -1;

	if (_rating_active)
	{
		return true;
	}

	//PDEBUG("\n start ProcessingButton");
	for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
	{
		if (_DEC_BUTTON_ARRAY[i]._state == BUTTON_STATE_ACTIVE)
		{
			//PDEBUG("\n ProcessingButton");
			ret = true;
			index = i;
			break;
		}
	}



	if (ret)
	{
		//PDEBUG("\n\n\n\n\n\n\n\n\n\n start ProcessingButton   index:%d", index);
		if (_DEC_BUTTON_ARRAY[index].NeedAction())
		{
			switch (index)
			{
			case QUEST_BUTTON_GIFT:

				break;
			case QUEST_BUTTON_TASK:
				QuestActive();
				//PDEBUG("\n QUEST_BUTTON_TASK");
				break;
			case MAINMENU_BUTTON_ACHIEVEMENT:
#ifdef MAC_OS
				PineGameCenterController::ShowLeaderAchivement();
#endif
				break;
			case MAINMENU_BUTTON_SETING:
				InitSetting();
				break;
			case MAINMENU_BUTTON_RANKING:
				if (USER()._lvlLocation < MAX_LOCATION)
				{
#ifdef MAC_OS
					PineGameCenterController::ShowLeaderBoard("WWW_ADVENTURER");
#endif
				}
				else
				{
#ifdef MAC_OS
					PineGameCenterController::ShowLeaderBoard("WWW_HOWFAR");
#endif
			}
#if defined (_WINDOWS_PHONE8) || defined (_WIN8)
				if (_playWitchEndlest)
				{
					//_user._highScore = _user._score;
					_social.UpdateNewScore(&_svc_post_score, _user._highScore);
					if (_svc_post_score.Status == PineServiceResult::Responsed)
					{
						if (_svc_post_score.ResponseStatus == 0)
						{
							//succced post score

						}
						else
						{
							//error

						}
					}
				}

#endif
				break;
			case MAINMENU_BUTTON_MOREGAME:
#ifdef MAC_OS
				PineAdBox::ShowMoreGame();
#endif
				break;

			case MAINMENU_BUTTON_GGPLAY:
				//PDEBUG("\n MAINMENU_BUTTON_GGPLAY");
#if GGPLAY_IMPLEMENT
#if ANDROID_OS
				if (!GAME()->_async_task_manager._is_login_google)
				{
					//PDEBUG("\n Init ggplay login callback: %d", GAME()->_async_task_manager._is_login_google);
					GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_IS_LOG_IN_GOOGLE_PLAY);
					GAME()->_async_task_manager._is_force_login_google_play = true;
					GAME()->_async_task_manager._on_process_sync_cloud = true;
					GAME()->_async_task_manager._is_show_game_services_menu = true;
					//_user.CheckLocalAndCloudSaveFile(false);
				}
				else {
					MenuGGPlayActive();
				}
#endif
#if _WINDOWS
				MenuGGPlayActive();
#endif
#endif
				break;

			case MAINMENU_BUTTON_PLAY:
				SetState(k_GS_PLAYGAME);
				_count_frame_delay_show_ads = 20;
				_user._num_play_in_asession++;
				GAME()->disableTopeboxAds = true;
				break;
			case MAINMENU_BUTTON_SHOP:
				IniteShop();
				break;
			case MAINMENU_BUTTON_IAP:
				if (!_isRenderShop)
				{
					IniteShop();
					SwitchTypeShop(SHOP_TYPE_IAP);
				}
				else
				{
					SwitchTypeShop(SHOP_TYPE_IAP);
				}
				break;
			case MAINMENU_BUTTON_SHOP_YES:
				ProccessTouch(_ChosseIndex);
				_itemShop[_ChosseIndex]._isAcctive = false;
				_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_YES].SetState(BUTTON_STATE_HIDE);
				_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_NO].SetState(BUTTON_STATE_HIDE);
				break;
			case MAINMENU_BUTTON_SHOP_NO:
				_itemShop[_ChosseIndex]._isAcctive = false;
				_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_YES].SetState(BUTTON_STATE_HIDE);
				_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_NO].SetState(BUTTON_STATE_HIDE);
				break;

				//setting
			case MAINMENU_BUTTON_SETTING_MUSIC:
				_isMusicEnable = !_isMusicEnable;
				GAME()->_musicEnable = _isMusicEnable;
				USER().DataSaveLocal();

				if (GAME()->_musicEnable)
				{
					if (_newScore)
					{
						GAME()->StopCurrentMusic();
						GAME()->LoadMusic(BGM_RESULT_SCREEN_WITH_NEW_HIGHSCORE);
						GAME()->PlayCurrentMusic(BGM_RESULT_SCREEN_WITH_NEW_HIGHSCORE);
					}
					else
					{
						GAME()->StopCurrentMusic();
						GAME()->LoadMusic(BGM_MAIN_MENU);
						GAME()->PlayCurrentMusic(BGM_MAIN_MENU);
					}
				}
				else
				{
					GAME()->StopCurrentMusic();
				}
				break;
			case MAINMENU_BUTTON_SETTING_SOUND:
				_isSoundEnable = !_isSoundEnable;
				GAME()->_soundEnable = _isSoundEnable;
				USER().DataSaveLocal();
				break;
			case MAINMENU_BUTTON_SETTING_NOTIFY:
				_isNotiEnable = !_isNotiEnable;
				GAME()->_notificationEnable = _isNotiEnable;
				USER().DataSaveLocal();
				break;
			case MAINMENU_BUTTON_SETTING_ABOUT:
				_scrollLogo = false;
				_closeAbout = false;
				CloseSetting();
				_isRenderAbout = true;
				_opacityAbout = 0;

				_aboutScrollY = 270 + OFFSET_I4(100) + OFFSET_IPAD(100);
				break;
			case MAINMENU_BUTTON_SETTING_RESET:
				_typeConfirm = CONFIRM_RESET_DATE;
				sprintf(_stringConfirm, "%s", GET_DES().getString(27));
				SetState(k_GS_CONFIRM);
				break;
				}
			for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
			{
				if (_DEC_BUTTON_ARRAY[i]._state != BUTTON_STATE_LOCK && _DEC_BUTTON_ARRAY[i]._state != BUTTON_STATE_HIDE)
				{
					_DEC_BUTTON_ARRAY[i].SetState(BUTTON_STATE_NORMAL, -1);
				}
			}
			ClearTouchProcessing();
		}
		return true;
	}
	return false;
}
void WildWildWestGame::UpdateScrollQuest(int xx, int yy, int touch_state, int touch_id)
{
	float zoom = 1.0f;
	float offsetY = 0;
	if (GAME()->_device._version == SupportVersion::RESVER_960_640)
	{
		zoom = 1.20f;
		offsetY = -0.15 * 640 / 2;
	}
	if (CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
	{
		zoom = IPAD_ZOOM;
		offsetY = IPAD_OFFSET;
	}
	//printf("\n 111111111111111111 = %f",offsetY);

	int rectQuest_x = GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleX(0, 0);
	int rectQuest_y = GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFModuleY(0, 0);
	int reactQuest_w = GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFrameWidth(0);
	int reactQuest_h = GET_SPRITE(SPRITEID_HUD_DAILY_REWARD)->GetFrameHeight(0);

	if (touch_state == TOUCH_DOWN)
	{
		//PDEBUG("\n TOUCH_DOWN");
		if (CHECK_POINT_IN_RECT(xx, yy, rectQuest_x, rectQuest_y + offsetY, reactQuest_w, reactQuest_h*zoom) ||
			CHECK_POINT_IN_RECT(xx, yy, SHOP_X, SHOP_Y + offsetY, SHOP_W, SHOP_H))
		{
			ClearTouchProcessing();
			if (!_scroll_quest.CheckTouch(touch_id) && !CheckTouchProcessing(touch_id))
			{
				//printf("\n begin scroll");
				_typeTouchQuest = 100;
				_quest_touchDrag = false;
				_quest_offsetDrag = 0;
				_quest_touch_drag_y = yy;
				_scroll_quest.Set();
				_scroll_quest.SetState(0);
				_scroll_quest.SetBeginScorll(touch_id);
			}
		}
		else {
			_typeTouchQuest = -1;
		}

	}
	if (touch_state == TOUCH_DRAG)
	{
		//PDEBUG("\n TOUCH_DRAG");
		if (_scroll_quest.CheckTouch(touch_id))
		{
			if (CHECK_POINT_IN_RECT(xx, yy, rectQuest_x, rectQuest_y + offsetY, reactQuest_w, reactQuest_h*zoom) ||
				CHECK_POINT_IN_RECT(xx, yy, SHOP_X, SHOP_Y + offsetY, SHOP_W, SHOP_H))
			{
				_quest_touchDrag = true;
				_quest_offsetDrag = yy - _quest_touch_drag_y;
				_quest_touch_drag_y = yy;
				_scroll_quest.AddOffset(-_quest_offsetDrag);
				_scroll_quest.ValidateRangeExt();
				_scroll_quest.ForceToTarget();
				_scroll_quest.SetState(1);
				PDEBUG("\n _quest_touch_drag_y: %f", _quest_touch_drag_y);
			}
		}
	}
	if (touch_state == TOUCH_UP)
	{
		//PDEBUG("\n TOUCH_UP");
		if (_scroll_quest.CheckTouch(touch_id))
		{
			ClearTouchProcessing();
			if (!_scroll_quest._collided)
			{
				_scroll_quest.End();
			}
			_scroll_quest.ClearTouchScroll();
			_scroll_quest.SetState(2);
			_quest_touchDrag = false;

		}
		else {
			if (_typeTouchQuest = -1)
			{
				_scroll_shop.ClearTouchScroll();
				ClearTouchProcessing();
			}
		}
	}
}

void WildWildWestGame::UpdateScrollShop(int xx, int yy, int touch_state, int touch_id)
{
	float zoom = 1.0f;
	float offsetY = 0;
	if (GAME()->_device._version == SupportVersion::RESVER_960_640)
	{
		zoom = 1.20f;
		offsetY = -0.15 * 640 / 2;
	}
	if (CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
	{
		zoom = IPAD_ZOOM;
		offsetY = IPAD_OFFSET;
	}
	//printf("\n 111111111111111111 = %f",offsetY);
	if (touch_state == TOUCH_DOWN)
	{
		if (CHECK_POINT_IN_RECT(xx, yy, CHARATER_X, CHARATER_Y + offsetY, CHARATER_W, CHARATER_H))
		{
			printf("\n 111111111111111111");
			if (_typeShop != SHOP_TYPE_POWER)
			{
				_typeTouchShop = SHOP_TYPE_POWER;
				GAME()->PlaySFX(SFX_BUTTON);
				SwitchTypeShop(SHOP_TYPE_POWER);
			}
			//GAME()->ClearAllTouch();
		}
		else if (CHECK_POINT_IN_RECT(xx, yy, ABILITY_X, ABILITY_Y + offsetY, ABILITY_W, ABILITY_H))
		{
			printf("\n 222222222222");
			if (_typeShop != SHOP_TYPE_CHARATER)
			{
				_typeTouchShop = SHOP_TYPE_CHARATER;
				GAME()->PlaySFX(SFX_BUTTON);
				SwitchTypeShop(SHOP_TYPE_CHARATER);
			}

		}
		else if (CHECK_POINT_IN_RECT(xx, yy, IAP_X, IAP_Y + offsetY, IAP_W, IAP_H))
		{
			printf("\n 3333333333333");
			if (_typeShop != SHOP_TYPE_IAP)
			{
				_typeTouchShop = SHOP_TYPE_IAP;
				GAME()->PlaySFX(SFX_BUTTON);
				SwitchTypeShop(SHOP_TYPE_IAP);
			}
		}
		else if (CHECK_POINT_IN_RECT(xx, yy, SCROLL_X, SCROLL_Y + offsetY, SCROLL_W, SCROLL_H*zoom) ||
			CHECK_POINT_IN_RECT(xx, yy, SHOP_X, SHOP_Y + offsetY, SHOP_W, SHOP_H))
		{
			if (!_scroll_shop.CheckTouch(touch_id) && !CheckTouchProcessing(touch_id))
			{
				printf("\n 44444444444");
				_typeTouchShop = 100;
				_touchDrag = false;
				_offsetDrag = 0;
				_touch_drag_y = yy;
				_scroll_shop.Set();
				_scroll_shop.SetState(0);
				_scroll_shop.SetBeginScorll(touch_id);
			}

		}
		else
		{
			_typeTouchShop = -1;
		}
	}

	else if (touch_state == TOUCH_DRAG)
	{
		if (_scroll_shop.CheckTouch(touch_id))
		{
			if (CHECK_POINT_IN_RECT(xx, yy, SCROLL_X, SCROLL_Y + offsetY, SCROLL_W, SCROLL_H*zoom) ||
				CHECK_POINT_IN_RECT(xx, yy, SHOP_X, SHOP_Y + offsetY, SHOP_W, SHOP_H))
			{
				_touchDrag = true;
				_offsetDrag = yy - _touch_drag_y;
				_touch_drag_y = yy;
				_scroll_shop.AddOffset(-_offsetDrag);
				//_scroll_shop.SetBeginScorll(touch_id);
				_scroll_shop.ValidateRangeExt();
				_scroll_shop.ForceToTarget();
				_scroll_shop.SetState(1);
			}
		}
	}

	else if (touch_state == TOUCH_UP)
	{
		if (_scroll_shop.CheckTouch(touch_id))
		{
			ClearTouchProcessing();
			if (!_scroll_shop._collided)
			{
				_scroll_shop.End();
			}
			_scroll_shop.ClearTouchScroll();
			_scroll_shop.SetState(2);
			_touchDrag = false;


			int index = -1;
			if (CMath::ABSOLUTE_VALUE(_offsetDrag) < 1)
			{
				for (int j = 0; j < _maxButtonShop; j++)
				{
					//PDEBUG("\n _itemShop[%d]._isAcctive:%d", j,_itemShop[j]._isAcctive);
					if (CHECK_POINT_IN_RECT(xx, yy, SCROLL_X, _itemShop[j]._y + _scroll_y + offsetY, SCROLL_W, _broadWidth - 10))
					{
						if (_itemShop[j]._isAcctive)
						{
							index = j;
							/*_ChosseIndex = index;
							for (int j = 0; j < _maxButtonShop; j++)
							{
								_itemShop[j]._isAcctive = false;
							}
							_itemShop[index]._isAcctive = true;
							_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_YES].SetState(BUTTON_STATE_NORMAL);
							_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_NO].SetState(BUTTON_STATE_NORMAL);*/
						}
						else
						{
							if (_itemShop[j]._type >= TYPE_SHOP_IAP_PACK_1 && _itemShop[j]._type <= TYPE_SHOP_IAP_RESTORE)
							{
								_itemShop[j]._stateScale = 1;
								index = j;
								break;
							}
							else
							{
								if (!_itemShop[j]._isLock && _itemShop[j]._state == ITEM_CAN_BUY)
								{
									if (USER()._coin >= _itemShop[j]._money)
									{
										_itemShop[j]._stateScale = 1;
										index = j;
										break;
									}
									else
									{
										_itemShop[j]._stateScale = 1;
										if (_typeShop != SHOP_TYPE_IAP)
										{
											GAME()->PlaySFX(SFX_BUTTON);
											SwitchTypeShop(SHOP_TYPE_IAP);
											_typeTouchShop = SHOP_TYPE_IAP;
										}
										//_scroll_shop.SetExt(5 * 150);
									}
								}

								if (_itemShop[j]._state == ITEM_UNLOCK && !_itemShop[j]._isLock)
								{
									if (j != _user._currentUseCharater)
									{
										_itemShop[j]._isuse = !_itemShop[j]._isuse;
										_itemShop[j]._stateScale = 1;
										GAME()->PlaySFX(SFX_BUTTON);
									}
									for (int k = 0; k < _maxButtonShop; k++)
									{
										if (k != j)
										{
											_itemShop[k]._isuse = false;
										}
										_user._useCharacter[k] = _itemShop[k]._isuse;
									}
									if (_itemShop[j]._isuse)
									{
										_user._currentUseCharater = j;
									}

									_user.DataSave();
								}
							}
						}
					}
				}
			}

			if (index == -1)
			{

			}
			else if (_typeTouchShop == 100)
			{
				//PDEBUG("\n touch bt buy confirm -- _typeShop:%d", _typeShop);
				GAME()->PlaySFX(SFX_BUTTON);
				if (_typeShop == SHOP_TYPE_IAP)
				{
					ProccessTouch(index);
				}
				else
				{
					_ChosseIndex = index;
					for (int j = 0; j < _maxButtonShop; j++)
					{
						_itemShop[j]._isAcctive = false;
					}
					_itemShop[index]._isAcctive = true;

					_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_YES].SetState(BUTTON_STATE_NORMAL);
					_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP_NO].SetState(BUTTON_STATE_NORMAL);
				}
				///ProccessTouch(index);
			}
		}
		else if (_typeTouchShop == -1)
		{
			_scroll_shop.ClearTouchScroll();
			ClearTouchProcessing();
			printf("\n 555555555555");
			if (!_touchDrag)
			{
				if (_isMoveButon)
				{
					_isMoveButon = false;
					_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHOP].SetState(BUTTON_STATE_NORMAL);
				}
			}
		}
	}
}

#pragma region RATING
void WildWildWestGame::RatingRender(float x, float y)
{
	if (!_rating_active)
	{
		return;
	}
	G()->SetOpacity(_rating_opac);
	G()->SetColor(0xaa000000);
	G()->FillFullScreen(true);
	G()->SetScale(_rating_scale, _rating_scale, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
	if (_rating_state < 10)
	{
		
		GET_SPRITE(SPRITEID_HUD_TUTORIAL)->DrawFrame(G(), 0, x, y);
		GET_SPRITE(SPRITEID_HUD_TUTORIAL)->DrawFrame(G(), 11, x, y);
		for (int i = 0; i < _rating_num_star; i++)
		{
			int sx = GET_SPRITE(SPRITEID_HUD_TUTORIAL)->GetFModuleCenterX(11, 2 + i);
			int sy = GET_SPRITE(SPRITEID_HUD_TUTORIAL)->GetFModuleCenterY(11, 2 + i);

			GET_SPRITE(SPRITEID_HUD_TUTORIAL)->DrawFrame(G(), 13, x + sx, y + sy);
		}
		for (int i = 0; i < RATING_NUM_BUTTON; i++)
		{
			_rating_button[i].Render(G());
			_rating_button[i].Update();
		}
	}
	else
	{
		GET_SPRITE(SPRITEID_HUD_TUTORIAL)->DrawFrame(G(), 10, x, y);
		GET_SPRITE(SPRITEID_HUD_TUTORIAL)->DrawFrame(G(), 12, x, y);
	}
	G()->ClearScale();
	G()->SetOpacity(100);
}
bool WildWildWestGame::RatingUpdateTouch()
{

	bool pressed = false;
	bool ret = false;
	int index = -1;
	if (CGame::_this->_isBackkeyPress)
	{
		CGame::_this->_isBackkeyPress = false;
		if (_rating_state < 5)
		{
			USER()._rate_cancel_count++;
			_rating_is_close = true;
			_rating_state = 5;
		}
		else if (_rating_state < 15)
		{
			_rating_is_close = true;
			_rating_state = 15;
		}
		return true;
	}
	for (int i = 0; i < RATING_NUM_BUTTON; i++)
	{
		if (_rating_button[i]._state == BUTTON_STATE_ACTIVE)
		{
			//PDEBUG("\n ProcessingButton");
			ret = true;
			index = i;
			break;
		}
	}

	if (ret)
	{
		//PDEBUG("\n\n\n\n\n\n\n\n\n\n start ProcessingButton   index:%d", index);
		if (_rating_button[index].NeedAction())
		{
			switch (index)
			{
			case RATING_BUTTON_LATER:
				USER()._rate_cancel_count++;
				_rating_is_close = true;
				_rating_state = 5;
				break;
			case RATING_BUTTON_RATE:
				if (_rating_num_star >= 5)
				{
#if defined(ANDROID_OS)
					OS_Rating();
#endif
					_user._hasRate = true;
					_rating_is_close = true;
				}
				_rating_state = 5;
				break;
			}
			for (int i = 0; i < RATING_NUM_BUTTON; i++)
			{
				if (_rating_button[i]._state != BUTTON_STATE_LOCK && _rating_button[i]._state != BUTTON_STATE_HIDE)
				{
					_rating_button[i].SetState(BUTTON_STATE_NORMAL);
				}
			}
		}
		return ret;
	}
	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		//if (TOUCHS(i)->_flag)
		{
			for (int touch = 0; touch < RATING_NUM_BUTTON; touch++)
			{
				if (_rating_button[touch]._state == BUTTON_STATE_LOCK || _rating_button[touch]._state == BUTTON_STATE_HIDE)
				{
					continue;
				}
				int x = _rating_button[touch].TZ_GetX() - 20;
				int y = _rating_button[touch].TZ_GetY() - 10;
				int w = _rating_button[touch].TZ_GetWidth() + 40;
				int h = _rating_button[touch].TZ_GetHeight() + 20;
				Button *button = &_rating_button[touch];
				if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
				{
					
					if (TOUCHS(i)->_state == TOUCH_DOWN)
					{
						if (button->_touch_id == -1)
						{
							button->_touch_id = TOUCHS(i)->_id;
							button->SetState(BUTTON_STATE_SELECT);
						}
					}
					else if (TOUCHS(i)->_state == TOUCH_DRAG)
					{
						/*if (button->_state != BUTTON_STATE_SELECT && Button::GetButtonActive() == NULL)
						{
						button->SetState(BUTTON_STATE_SELECT);
						}*/
					}
					else if (TOUCHS(i)->_state == TOUCH_UP)
					{
						if (button->_touch_id == TOUCHS(i)->_id)
						{
							button->SetState(BUTTON_STATE_ACTIVE);
							button->_touch_id = -1;
							pressed = true;
						}
					}
					if (button->_touch_id != -1)
					{
						pressed = true;
					}
				}
				else
				{
					if (TOUCHS(i)->_state == TOUCH_UP)
					{
						if (button->_touch_id == TOUCHS(i)->_id)
						{
							button->SetState(BUTTON_STATE_NORMAL);
							button->_touch_id = -1;
							pressed = true;
						}
					}
				}
			}

		}
	}
	return pressed;
}
void WildWildWestGame::RatingUpdate()
{
	
	if (!_rating_active)
	{
		return;
	}
	switch (_rating_state)
	{
	case 1:
		if (_rating_scale < 1.1f)
		{
			_rating_scale += 0.05f;
			if (_rating_scale >= 1.1f)
			{
				_rating_state++;
			}
		}
		if (_rating_opac < 100)
		{
			_rating_opac += 5;
		}
		break;
	case 2:
		if (_rating_scale > 1.0f)
		{
			_rating_scale += -0.02f;
			if (_rating_scale <= 1.0f)
			{
				_rating_scale = 1.0f;
				_rating_state++;
			}
		}
		break;
	case 5:
		if (_rating_scale < 1.1f)
		{
			_rating_scale += 0.02f;
			if (_rating_scale >= 1.1f)
			{
				_rating_state++;
			}
		}
		break;
	case 6:
		if (_rating_scale > 0)
		{
			_rating_scale += -0.05f;
			if (_rating_scale <= 0)
			{
				_rating_scale = 0;
				if (_rating_is_close)
				{
					_rating_state = 0;
					_rating_active = false;
				}
				else
				{
					_rating_state = 10;
				}
				
			}
		}
		if (_rating_is_close)
		{
			if (_rating_opac > 0)
			{
				_rating_opac += -5;
			}
		}
		break;
	case 10:
		if (_rating_scale < 1.1f)
		{
			_rating_scale += 0.05f;
			if (_rating_scale >= 1.1f)
			{
				_rating_state++;
				_rating_touch_id = -1;
			}
		}
		break;
	case 11:
		if (_rating_scale > 1.0f)
		{
			_rating_scale += -0.02f;
			if (_rating_scale <= 1.0f)
			{
				_rating_scale = 1.0f;
				_rating_state++;
			}
		}
		break;
	case 15:
		if (_rating_scale < 1.1f)
		{
			_rating_scale += 0.02f;
			if (_rating_scale >= 1.1f)
			{
				_rating_state++;
			}
		}
		else
		{
			_rating_state++;
		}
		break;
	case 16:
		if (_rating_scale > 0)
		{
			_rating_scale += -0.05f;
			if (_rating_scale <= 0)
			{
				_rating_scale = 0;
				_rating_state = 0;
				_rating_active = false;
			}
		}
		if (_rating_opac > 0)
		{
			_rating_opac += -5;
		}
		break;
	default:
		break;
	}
	
	if (!RatingUpdateTouch())
	{
		//PDEBUG("\n update touch rate");
		bool pressed = false;
		for (int i = GAME()->_num_touches - 1; i >= 0; i--)
		{
			for (int touch = 0; touch < 5; touch++)
			{
				if (_rating_state < 10)
				{
					int w = GET_SPRITE(SPRITEID_HUD_TUTORIAL)->GetFModuleWidth(11,2);
					int h = GET_SPRITE(SPRITEID_HUD_TUTORIAL)->GetFModuleHeight(11, 2);
					int x = GET_SPRITE(SPRITEID_HUD_TUTORIAL)->GetFModuleX(11, 2 + touch);
					int y = GET_SPRITE(SPRITEID_HUD_TUTORIAL)->GetFModuleY(11, 2 + touch);
					//PDEBUG("\n x:%d -- y:%d -- w:%d -- h:%d  <<>> touch x:%d -- y:%d", x, y, w,h, TOUCHS(i)->x, TOUCHS(i)->y);
					if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
					{
						pressed = true;
						if (TOUCHS(i)->_state == TOUCH_DOWN)
						{
							
							if (_rating_touch_id == -1)
							{
								_rating_touch_id = TOUCHS(i)->_id;
								_rating_num_star = touch + 1;
							}
							//PDEBUG("\n 111 _rating_touch_id:%d -- _rating_num_star:%d -- _rating_state:%d", _rating_touch_id, _rating_num_star, _rating_state);
						}
						else if (TOUCHS(i)->_state == TOUCH_DRAG)
						{
							if (_rating_touch_id == TOUCHS(i)->_id)
							{
								_rating_num_star = touch + 1;
							}
							
						}
						else if (TOUCHS(i)->_state == TOUCH_UP)
						{
							if (_rating_touch_id == TOUCHS(i)->_id)
							{
								_rating_num_star = touch + 1;
								_rating_touch_id = -1;
							}
						}
					}
					else
					{
						if (_rating_touch_id == TOUCHS(i)->_id)
						{
							if (TOUCHS(i)->_state == TOUCH_UP)
							{
								_rating_touch_id = -1;
							}
						}
					}
				}
				else if(_rating_state > 10)
				{
					 if (TOUCHS(i)->_state == TOUCH_DOWN)
					{
						if (_rating_touch_id == -1)
						{
							_rating_is_close = true;
							_rating_state = 15;
							_rating_touch_id = -1;
						}
					}
				}
			}
		}
	}
}
void WildWildWestGame::RatingInit()
{
	_rating_active = false;
	_rating_is_close = true;
	_rating_opac = 0;
	_rating_scale = 0;
	_rating_state = 0;
	_rating_num_star = 0;
	_rating_touch_id = -1;
	for (int i = 0; i < RATING_NUM_BUTTON; i++)
	{
		_rating_button[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER);
		_rating_button[i].SetAnim(SPRITEID_HUD_TUTORIAL, 4 + i * 4);
	}
}
void WildWildWestGame::RatingActive()
{
	_rating_num_star = 0;
	_rating_active = true;
	_rating_is_close = false;
	_rating_opac = 0;
	_rating_scale = 0;
	_rating_state = 1;
	_rating_touch_id = -1;
}
#pragma endregion
