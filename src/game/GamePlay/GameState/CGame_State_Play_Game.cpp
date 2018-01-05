#include "pch.h"
#include "../../GameCore/CGame.h"


#undef _DEC_BUTTON_NUMS
#undef _DEC_BUTTON_ARRAY
#undef _DEC_ANIM_BUTTON_BEIGN
#undef _DEC_SPRITE_BUTTON

#define _DEC_BUTTON_NUMS				(PLAYGAME_BUTTON_NUMS)
#define _DEC_BUTTON_ARRAY				_buttonPlayGame
#define _DEC_ANIM_BUTTON_BEIGN			0
#define _DEC_SPRITE_BUTTON				SPRITEID_HUD_INGAME


void WildWildWestGame::UpdateStatePlayGame()
{
	if (_sub_state == k_LOOP)
	{
        if (!TouchButtonPlayGame())
        {
			//PDEBUG("\n GamePlay update BATTLE()._timeBegin:%d", BATTLE()._timeBegin);
			if (BATTLE()._timeBegin <= 0)
			{
				if (CHARATER().GetState() != CHARATER_STATE_DIE)
				{
					CHARATER().UpdateTouchMove();
				}
				else
				{
					CHARATER().SetRenderStick(false);

				}
			}
			
        }
		BATTLE().Update();	
		
	}
}
void WildWildWestGame::RenderStatePlayGame()
{
	if (_sub_state == k_INIT)
	{
		_sub_state = k_LOOP;
		isLogged = false;
        for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
		{
			_DEC_BUTTON_ARRAY[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, SFX_BUTTON);
			_DEC_BUTTON_ARRAY[i].SetAnim((_DEC_SPRITE_BUTTON), _DEC_ANIM_BUTTON_BEIGN + i * 4);
		}
        


		GAME()->StopCurrentMusic();
		GAME()->LoadMusic(BGM_INGAME);
		GAME()->PlayCurrentMusic(BGM_INGAME);

		


		GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_STICK, false, false);
		GAME()->LoadSpriteCurrentVer(SPRITEID_OBSTACLE_ROCKET, false, false);
		GAME()->LoadSpriteCurrentVer(SPRITEID_OBSTACLE, false, false);
		GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_INGAME, false, false);
		GAME()->LoadSpriteCurrentVer(SPRITEID_EFFECT_EXPLOSION, false, false);

		GET_SPRITE(SPRITEID_HUD_SHOP)->unloadTexture(G());
		GET_SPRITE(SPRITEID_BG_MAIN_MENU)->unloadTexture(G());
		GET_SPRITE(SPRITEID_FONT_DISTANCE)->unloadTexture(G());
		//GET_SPRITE(SPRITEID_EFFECT_DUST)->unloadTexture(G());
		BATTLE().Init();

		
		
	}

	if (_sub_state == k_LOOP)
	{
        float zoom = 1.0f;
        float offsetY = 0;
		if (CGame::_this->_device._version == SupportVersion::RESVER_960_640)
        {
            zoom = 1.20f;
            offsetY = -0.15*640/2;
        }
        if(CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
        {
            zoom = IPAD_ZOOM;
            offsetY = IPAD_OFFSET;
        }
		BATTLE().Render();
		//QuestRender(0, 0, 100);
        
		if (BATTLE()._timeBegin <= 0)
		{
			for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
			{
                _DEC_BUTTON_ARRAY[i].SetPos(0, -offsetY);
				_DEC_BUTTON_ARRAY[i].Render(G());
				_DEC_BUTTON_ARRAY[i].Update();
			}
		}
		
	}
}

bool WildWildWestGame::TouchButtonPlayGame()
{
	/*if (CHARATER().GetStick())
	{
		return false;
	}*/
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
			SetState(k_GS_IGM);
            
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
					if (button->_touch_id == TOUCHS(i)->_id)
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
