#include "pch.h"
#include "../../GameCore/CGame.h"


#undef _DEC_BUTTON_NUMS
#undef _DEC_BUTTON_ARRAY
#undef _DEC_ANIM_BUTTON_BEIGN
#undef _DEC_SPRITE_BUTTON

#define _DEC_BUTTON_NUMS				(IGM_BUTTON_NUMS)
#define _DEC_BUTTON_ARRAY				_buttonIGM
#define _DEC_ANIM_BUTTON_BEIGN			4
#define _DEC_SPRITE_BUTTON				SPRITEID_HUD_INGAME


void WildWildWestGame::UpdateStateIGM()
{
	if (_sub_state == k_LOOP)
	{
		if (!TouchButtonIGM())
		{

		}

	}
}
void WildWildWestGame::RenderStateIGM()
{
	if (_sub_state == k_INIT)
	{
		_sub_state = k_LOOP;

		for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
		{
			_DEC_BUTTON_ARRAY[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER,  SFX_BUTTON);
			_DEC_BUTTON_ARRAY[i].SetAnim((_DEC_SPRITE_BUTTON), _DEC_ANIM_BUTTON_BEIGN + i * 4);
		}
		USER().DataSave();
	}
	
	if (_sub_state == k_LOOP)
	{
		BATTLE().Render();
		G()->SetColor(0xaa000000); 
		G()->FillFullScreen(true); 
		for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
		{
			_DEC_BUTTON_ARRAY[i].Render(G());
			_DEC_BUTTON_ARRAY[i].Update();
		}
	}
}

bool WildWildWestGame::TouchButtonIGM()
{
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
			case IGM_BUTTON_RESUM:
				SetState(k_GS_PLAYGAME);
				_sub_state = k_LOOP;
				num_continue++;
				break;

			case IGM_BUTTON_END:
				PDEBUG("\n IGM_BUTTON_END fire");
				_isGameOver = true;
				SetState(k_GS_RESULT);
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
