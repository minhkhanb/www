#include "pch.h"
#include "../../GameCore/CGame.h"

#if defined (ANDROID_OS)
extern "C" void				OS_Rating();//open rating page
#endif


void WildWildWestGame::UpdateStateResult()
{
	if (_sub_state == k_LOOP)
	{
		if (!TouchButtonStateResult())
		{

		}

		if (_timeGunOver == 65)
		{

			GAME()->PlaySFX(SFX_GUNSHOT_GAMEOVER);

		}
		_timeGunOver++;
	}
}
void WildWildWestGame::RenderStateResult()
{
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	if (_sub_state == k_INIT)
	{
		_has_new_location = false;
		_sub_state = k_LOOP;
		GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_RESULT, false, false);
		GAME()->LoadSpriteCurrentVer(SPRITEID_FONT_RESULT, false, false);
		GET_SPRITE(SPRITEID_FONT_RESULT)->SetCharSpacing(3);
		GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_GAME_OVER);
		_animGameOver.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_GAME_OVER), SPRITEID_HUD_GAME_OVER);
		_animGameOver.SetAnim(0, false);
		_animGameOver.SetPos(0, 0);

		_newScore = false;
		if (_user._score > _user._highScore && _playWitchEndlest)
		{
			_user._highScore = _user._score;
			_user._score = _user._highScore;
			_newScore = true;
			SubmitHighScore();
		}
		if (_user._lvlLocation > _oldLocation)
		{
			_has_new_location = true;
			SubmitTown();
			game->Logs_LocationPass(USER()._lvlLocation + 1);
		}
		SubmitTotalCoin();
		_isGameOver = true;

		GAME()->PlaySFX(SFX_GAMEOVER);
		_timeGunOver = 0;



		_achievement.CheckAchievementComplete();
		_achievement.CheckQuestComplete();


		USER().DataSave();
	}

	if (!isLogged)
	{
		isLogged = true;
		//game->Logs_GamePlaying_Restart(game->num_playing_restart, USER().isFirstPlay);
	}
	

	if (_sub_state == k_LOOP)
	{
		RenderStatePlayGame();

		G()->SetColor(0xdd000000);
		G()->FillFullScreen(true);

		CHARATER().GetAnimCharater().DrawAnimObject(G());
		CHARATER().RenderEffect();


		if (CHARATER().GetAnimCharater().IsCurrentAnimStop())
		{
			GET_SPRITE(GET_DES().getShopCharaterDes(USER()._currentUseCharater, SHOP_CHARATER_SPRITE_ID))->DrawFrame(G(), 117, CHARATER().GetAnimCharater()._x, CHARATER().GetAnimCharater()._y);
		}


		_animGameOver.DrawAnimObject(G());
		_animGameOver.UpdateAnimObject();

		if (_animGameOver.IsCurrentAnimStop())
		{
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_SHOP, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_BG_MAIN_MENU, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_EFFECT_DUST, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_COIN, false, !_optimizeDate);
			GAME()->LoadSpriteCurrentVer(SPRITEID_FONT_DISTANCE, false, !_optimizeDate);
			GET_SPRITE(SPRITEID_FONT_DISTANCE)->SetCharSpacing(5);

			/*GAME()->RegUnloadSpriteVer(SPRITEID_HUD_STICK);
			GAME()->RegUnloadSpriteVer(SPRITEID_OBSTACLE);
			GAME()->RegUnloadSpriteVer(SPRITEID_OBSTACLE_ROCKET);
			GAME()->RegUnloadSpriteVer(SPRITEID_EFFECT_EXPLOSION);*/



			//GET_SPRITE(SPRITEID_HUD_GAME_OVER)->unloadTexture(G());
			//GET_SPRITE(SPRITEID_HUD_STICK)->unloadTexture(G());
			//GET_SPRITE(SPRITEID_OBSTACLE)->unloadTexture(G());
			//GET_SPRITE(SPRITEID_OBSTACLE_ROCKET)->unloadTexture(G());
			//GET_SPRITE(SPRITEID_EFFECT_EXPLOSION)->unloadTexture(G());

			SetState(k_GS_MAINMENU);

			//////////
			_counttoPlay++;
			//num_playing_restart++;
			
			//GA_WLogContent(PLAYING_RESTART);
			
			PDEBUG("\n _rate_cancel_count:%d -- _user._hasRate:%d -- _user._lvlLocation:%d -- _has_new_location:%d", _user._rate_cancel_count, _user._hasRate, _user._lvlLocation, _has_new_location);
			if (_user._lvlLocation >= 1 && _user._rate_cancel_count < 3 && !_user._hasRate && _has_new_location)
			{
#if MAC_OS
				PineSocial::ShowPopUpRating(APPIRATER_MESSAGE_TITLE, APPIRATER_MESSAGE);
#elif ANDROID_OS
				/*sprintf(GAME()->_async_task_manager._message_box_title, "%s", APPIRATER_MESSAGE_TITLE);
				sprintf(GAME()->_async_task_manager._message_box_message, "%s", APPIRATER_MESSAGE);
				GAME()->_async_task_manager._message_box_confirm_type = CAsyncTasks::enumCloudSaveConfirmType::CONFIRM_TYPE_RATE;
				GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_SHOW_MESSAGE_BOX);*/
				RatingActive();
#else
				RatingActive();
#endif
			
			}
			else
			{
//				if (!_user._removeAdd && _time_delay_show_ads <= 0)
//				{
//					if (_countShowPineAds == 1)
//					{
//						_stateShowPineAds = 1;
//					}
//					else
//					{
//						if (_countShowAdsMods <= 1)
//						{
//							if (_countShowAdsUnity <= 0)
//							{
//#if defined (_WINDOWS_PHONE8) || defined(_WIN8)
//								GAME()->_admob_manager.showInterstitial();
//#elif defined (ANDROID_OS)
//								//GAME()->PrepareAndShowVideoAds();
//								if (GAME()->HasAds())
//								{
//									GAME()->ShowAds();
//								}
//#elif defined (MAC_OS)
//								int type = PineAdBox::HasVideoAds();
//								if (type >= 0)
//								{
//									PineAdBox::ShowVideoAds(type);
//								}
//#endif
//								_countShowAdsUnity = 2;
//							}
//							else
//							{
//#if defined (_WINDOWS_PHONE8) || defined(_WIN8)
//								GAME()->_admob_manager.showInterstitial();
//#elif defined (ANDROID_OS)
//								if (GAME()->HasAds())
//								{
//									GAME()->ShowAds();
//								}
//#elif defined (MAC_OS)
//								PineAdBox::SetReactiveAdvert();
//								PineAdBox::ShowAdvertFullScreen();
//#endif
//								_countShowAdsMods = 1;
//								_countShowAdsUnity--;
//							}
//						}
//					}
//				}
			}
		}
	}
}
bool WildWildWestGame::TouchButtonStateResult()
{
	for (int i = 0; i < CGame::_this->_num_touches; i++)
	{
		if (TOUCHS(i)->_flag)
		{
			if (TOUCHS(i)->_state == TOUCH_DOWN)
			{

			}
			else if (TOUCHS(i)->_state == TOUCH_DRAG)
			{

			}
			else if (TOUCHS(i)->_state == TOUCH_UP)
			{
				if (_animGameOver.IsCurrentAnimStop())
				{
					SetState(k_GS_PLAYGAME);
				}
			}
		}
	}
	return true;
}

