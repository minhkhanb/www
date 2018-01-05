#include "pch.h"
#include "../../GameCore/CGame.h"

void CEffectManager::Init()
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		_state[i] = EFFECT_MANAGER_STATE_NONE;
	}
}

bool CEffectManager::CompleteEffect(BYTE type)
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_type[i] == type && _state[i] != EFFECT_MANAGER_STATE_NONE)
		{
			return false;
		}
	}
	return true;
}

bool CEffectManager::CompleteEffect()
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_state[i] != EFFECT_MANAGER_STATE_NONE)
		{
			return false;
		}
	}
	return true;
}
void CEffectManager::SetStateEffect(BYTE type, BYTE state)
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_type[i] == type)
		{
			_state[i] = state;
		}
	}	
}
void CEffectManager::StopEffect(BYTE type)
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_type[i] == type && _state[i] != EFFECT_MANAGER_STATE_NONE)
		{
			_state[i] = EFFECT_MANAGER_STATE_NONE;
		}
	}

}
bool CEffectManager::CompleteStatePartical(int i, int num, BYTE state)
{
	bool res = true;
	for (int j = 0; j < num; j++)
	{
		if (_par_state[i * EFFECT_MANAGER_MAX_CACHE + j] != state)
		{
			res = false;
			break;
		}
	}
	return res;
}
bool CEffectManager::isMoveToTarget(float &angle, float &x, float &y, float targetX, float targetY, float speed)
{
	bool ret = false;
	float distance = CMath::DistancePOW2(targetX, targetY, x, y);
	if (distance < (speed * speed))
	{
		targetX = x;
		targetY = y;
		ret = true;
	}
	else
	{
		float angle_move = CMath::ANGLE(targetX, targetY, x, y) - 180;
		angle = angle_move + 180;
		float vy = speed*MSIN(angle_move);
		float vx = speed*MCOS(angle_move);
		x += vx;
		y += vy;

	}
	return ret;
}
void CEffectManager::Update()
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_state[i] != EFFECT_MANAGER_STATE_NONE)
		{
			switch (_type[i])
			{
			case EFFECT_MANAGER_TYPE_ROCKET_SMOKE:
				UpdateEffectSmoke(i);
				break;
			case EFFECT_MANAGER_TYPE_PATICAL_DUST:
				UpdateParticalDust(i);
				break;
			case EFFECT_MANAGER_TYPE_PATICAL_RAIN:
				UpdateParticalRain(i);
				break;
			case EFFECT_MANAGER_TYPE_TEXT_FLY:
				UpdateEffectTextFly(i);
				break;
			}
		}
	}
}
void CEffectManager::Render()
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_state[i] != EFFECT_MANAGER_STATE_NONE)
		{
			switch (_type[i])
			{
			case EFFECT_MANAGER_TYPE_ROCKET_SMOKE:
				RenderEffectSmoke(i);
				break;
			case EFFECT_MANAGER_TYPE_PATICAL_DUST:
				RenderParticalDust(i);
				break;
			case EFFECT_MANAGER_TYPE_PATICAL_RAIN:
				RenderParticalRain(i);
				break;
			case EFFECT_MANAGER_TYPE_TEXT_FLY:
				RenderEffectTextFly(i);
				break;
			}
		}
	}
}

void CEffectManager::AddEffectSmoke(float x, float y, float speed)
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_state[i] == EFFECT_MANAGER_STATE_NONE)
		{
			_type[i] = EFFECT_MANAGER_TYPE_ROCKET_SMOKE;
			_state[i] = 1;

			_x[i] = x;
			_y[i] = y;
			_varF2[i] = speed;
			_varF1[i] = 0.3;
			_var1[i] = 0;

			_var2[i] = CMath::RANDOM(1, 3);

			break;
		}
	}
}
void CEffectManager::UpdateEffectSmoke(int i)
{
	switch (_state[i])
	{
	case 1:
		_varF1[i] += _varF2[i];
		
		if (_varF1[i] >= 1)
		{
			_var1[i] -= 5;
		}
		else
		{
			_var1[i] += 10;
			if (_var1[i] >= 100)
			{
				_var1[i] = 100;
			}
		}
		if (_varF1[i] >= 2)
		{
			_state[i] = 2;
		}
		_x[i] -= BATTLE_MAP()._speed_scroll_map;
		break;
	case 2:
		_x[i] -= BATTLE_MAP()._speed_scroll_map;
		_var1[i] -= 7;
		if (_var1[i] <= 0)
		{
			_state[i] = EFFECT_MANAGER_STATE_NONE;
		}
		break;
	}
}
void CEffectManager::RenderEffectSmoke(int i)
{
	if (_state[i] != EFFECT_MANAGER_STATE_NONE)
	{	
		G()->SetOpacity(_var1[i]);
		G()->SetScale(_varF1[i], _varF1[i], _x[i], _y[i]);
		GET_SPRITE(SPRITEID_OBSTACLE_ROCKET)->DrawFrame(G(), _var2[i]*2 - 1, _x[i], _y[i]);

		G()->SetScale(_varF1[i], _varF1[i], _x[i], _y[i] + 70);
		GET_SPRITE(SPRITEID_OBSTACLE_ROCKET)->DrawFrame(G(), _var2[i] * 2, _x[i], _y[i]);

		G()->SetOpacity(100);
		G()->ClearScale();
	}
}

void CEffectManager::AddParticalDust(int sprite, int numFrame, int time, int direction, int beginAngle, int endAngle)
{
	
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_state[i] == EFFECT_MANAGER_STATE_NONE)
		{
			_type[i] = EFFECT_MANAGER_TYPE_PATICAL_DUST;
			_state[i] = 1;

			_var1[i] = EFFECT_PATICAL_MAX_CACHE;
			_var2[i] = 0;

			_var3[i] = sprite;
			_var4[i] = numFrame;
			_var5[i] = direction;
			_var6[i] = beginAngle;
			_var7[i] = endAngle;
			_var8[i] = time;

			for (int j = 0; j < _var1[i]; j++)
			{
				_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j] = EFFECT_MANAGER_STATE_NONE;
		
				if (j < 10)
				{
					_par_x[i*EFFECT_MANAGER_MAX_CACHE + j] = CMath::RANDOM(0, k_SCREEN_WIDTH);
					_par_y[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(0, k_SCREEN_HEIGHT);



					_par_angle[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(_var6[i], _var7[i]) - 180;
					_par_frame[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(0, _var4[i]);

					_par_speed[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(50, 70)*0.1;
					_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j] = 2;
                    if (_var3[i] == SPRITEID_EFFECT_FAST)
                    {
                        _par_speed[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(200, 250)*0.1;
                    }
				}			

			}

			break;
		}
	}
}
void CEffectManager::UpdateParticalDust(int i)
{
	if (_state[i] == 1)
	{
		_var2[i] ++;
		for (int j = 0; j < _var1[i]; j++)
		{
			if (_var2[i] % _var8[i] == 0)
			{
				if (_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j] == EFFECT_MANAGER_STATE_NONE)
				{
					_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j] = 1;
					break;
				}
			}
		}

		for (int j = 0; j < _var1[i]; j++)
		{
			switch (_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j])
			{
			case 1:
				if (_var5[i] == CHARATER_MOVE_LEFT)
				{
					_par_x[i*EFFECT_MANAGER_MAX_CACHE+ j] = k_SCREEN_WIDTH;
					_par_y[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(0, k_SCREEN_HEIGHT);
					_par_angle[i*EFFECT_MANAGER_MAX_CACHE + j] = CMath::RANDOM(_var6[i], _var7[i]);
				}
				else if (_var5[i] == CHARATER_MOVE_DOWN_LEFT)
				{
					if (CMath::RANDOM(0, 3) == 0)
					{
						_par_x[i*EFFECT_MANAGER_MAX_CACHE + j] = CMath::RANDOM(0, k_SCREEN_WIDTH);
						_par_y[i*EFFECT_MANAGER_MAX_CACHE+ j] = 0;

					}
					else
					{
						_par_x[i*EFFECT_MANAGER_MAX_CACHE+ j] = k_SCREEN_WIDTH;
						_par_y[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(0, k_SCREEN_HEIGHT);
					}
				}
				else if (_var5[i] == CHARATER_MOVE_DOWN_RIGHT)
				{
					if (CMath::RANDOM(0, 3) > 0)
					{
						_par_x[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(0, k_SCREEN_WIDTH);
						_par_y[i*EFFECT_MANAGER_MAX_CACHE+ j] = 0;

					}
					else
					{
						_par_x[i*EFFECT_MANAGER_MAX_CACHE+ j] = 0;
						_par_y[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(0, k_SCREEN_HEIGHT);

					}
				}

				_par_angle[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(_var6[i], _var7[i]) - 180;
				_par_frame[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(0, _var4[i]);

				_par_speed[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(50, 80)*0.1;
				if (_var3[i] == SPRITEID_EFFECT_FAST)
				{
					_par_speed[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(200, 250)*0.1;
				}
				_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j] = 2;
				break;

			case 2:
				_par_x[i*EFFECT_MANAGER_MAX_CACHE+ j] += _par_speed[i*EFFECT_MANAGER_MAX_CACHE+ j] * MCOS(_par_angle[i*EFFECT_MANAGER_MAX_CACHE+ j]);
				_par_y[i*EFFECT_MANAGER_MAX_CACHE+ j] += _par_speed[i*EFFECT_MANAGER_MAX_CACHE+ j] * MSIN(_par_angle[i*EFFECT_MANAGER_MAX_CACHE+ j]);

				if (!CHECK_POINT_IN_RECT(_par_x[i*EFFECT_MANAGER_MAX_CACHE+ j], _par_y[i*EFFECT_MANAGER_MAX_CACHE+ j], 0, 0, k_SCREEN_WIDTH, k_SCREEN_HEIGHT))
				{
					_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j] = EFFECT_MANAGER_STATE_NONE;
				}
				break;
			}
		}
	}
	else
	{
		for (int j = 0; j < _var1[i]; j++)
		{
			switch (_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j])
			{
			case 2:
				_par_x[i*EFFECT_MANAGER_MAX_CACHE+ j] += _par_speed[i*EFFECT_MANAGER_MAX_CACHE+ j] * MCOS(_par_angle[i*EFFECT_MANAGER_MAX_CACHE+ j]);
				_par_y[i*EFFECT_MANAGER_MAX_CACHE+ j] += _par_speed[i*EFFECT_MANAGER_MAX_CACHE+ j] * MSIN(_par_angle[i*EFFECT_MANAGER_MAX_CACHE+ j]);

				if (!CHECK_POINT_IN_RECT(_par_x[i*EFFECT_MANAGER_MAX_CACHE+ j], _par_y[i*EFFECT_MANAGER_MAX_CACHE+ j], 0, 0, k_SCREEN_WIDTH, k_SCREEN_HEIGHT))
				{
					_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j] = EFFECT_MANAGER_STATE_NONE;
				}
				break;
			}
		}
		if (CompleteStatePartical(i, _var1[i], EFFECT_MANAGER_STATE_NONE))
		{
			_state[i] = EFFECT_MANAGER_STATE_NONE;
		}
	}
}
void CEffectManager::RenderParticalDust(int i)
{
	for (int j = 0; j < _var1[i]; j++)
	{
		if (_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j] != EFFECT_MANAGER_STATE_NONE)
		{
			G()->SetOpacity(70);
			GET_SPRITE(_var3[i])->DrawFrame(G(),_par_frame[i*EFFECT_MANAGER_MAX_CACHE+ j], _par_x[i*EFFECT_MANAGER_MAX_CACHE+ j], _par_y[i*EFFECT_MANAGER_MAX_CACHE+ j]);
			G()->SetOpacity(100);
		}
	}
}

void CEffectManager::AddParticalRain(int time)
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_state[i] == EFFECT_MANAGER_STATE_NONE)
		{
			_type[i] = EFFECT_MANAGER_TYPE_PATICAL_RAIN;
			_state[i] = 1;

			_var1[i] = EFFECT_PATICAL_MAX_CACHE;			
			_var2[i] = 0;
			_var3[i] = SPRITEID_EFFECT_DUST;
			_var8[i] = time;

			for (int j = 0; j < _var1[i]; j++)
			{
				_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j] = EFFECT_MANAGER_STATE_NONE;

				if (j < 10)
				{
					_par_x[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(0, k_SCREEN_WIDTH);
					_par_y[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(0, k_SCREEN_HEIGHT);



					_par_angle[i*EFFECT_MANAGER_MAX_CACHE+ j] = 45;
					_par_frame[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(5, 6);

					_par_speed[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(50, 70)*0.1;
					_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j] = 2;

					if (_var3[i] == SPRITEID_EFFECT_FAST)
					{
						_par_speed[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(200, 250)*0.1;
					}
				}


			}

			break;
		}
	}
}
void CEffectManager::UpdateParticalRain(int i)
{
	if (_state[i] == 1)
	{
		_var2[i] ++;
	
		for (int j = 0; j < _var1[i]; j++)
		{
			if (_var2[i] % _var8[i] == 0)
			{
				if (_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j] == EFFECT_MANAGER_STATE_NONE)
				{
					_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j] = 1;
					break;
				}
			}
		}

		for (int j = 0; j < _var1[i]; j++)
		{
			switch (_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j])
			{
			case 1:

				if (CMath::RANDOM(0, 3) == 0)
				{

					_par_x[i*EFFECT_MANAGER_MAX_CACHE+ j] = k_SCREEN_WIDTH;
					_par_y[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(0, k_SCREEN_HEIGHT);
				}
				else
				{
					_par_x[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(0, k_SCREEN_WIDTH);
					_par_y[i*EFFECT_MANAGER_MAX_CACHE+ j] = 0;
				}

				_par_opacity[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(70, 80);
				_par_angle[i*EFFECT_MANAGER_MAX_CACHE+ j] = 45;
				_par_frame[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(5, 6);

				_par_speed[i*EFFECT_MANAGER_MAX_CACHE+ j] = CMath::RANDOM(50, 80)*0.1 * 2;
				_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j] = 2;
				break;

			case 2:
				_par_opacity[i*EFFECT_MANAGER_MAX_CACHE+ j]--;
				_par_x[i*EFFECT_MANAGER_MAX_CACHE+ j] -= _par_speed[i*EFFECT_MANAGER_MAX_CACHE+ j] * MCOS(_par_angle[i*EFFECT_MANAGER_MAX_CACHE+ j]);
				_par_y[i*EFFECT_MANAGER_MAX_CACHE+ j] += _par_speed[i*EFFECT_MANAGER_MAX_CACHE+ j] * MSIN(_par_angle[i*EFFECT_MANAGER_MAX_CACHE+ j]);

				if (_par_opacity[i*EFFECT_MANAGER_MAX_CACHE+ j] < 0)
					//if (!CHECK_POINT_IN_RECT(_par_x[i*EFFECT_MANAGER_MAX_CACHE+ j], _par_y[i*EFFECT_MANAGER_MAX_CACHE+ j], 0, 0, k_SCREEN_WIDTH, k_SCREEN_HEIGHT))
				{
					_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j] = EFFECT_MANAGER_STATE_NONE;
				}
				break;
			}

		}
	}
	else
	{
		for (int j = 0; j < _var1[i]; j++)
		{
			switch (_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j])
			{
			case 2:
				_par_opacity[i*EFFECT_MANAGER_MAX_CACHE+ j]--;
				_par_x[i*EFFECT_MANAGER_MAX_CACHE+ j] -= _par_speed[i*EFFECT_MANAGER_MAX_CACHE+ j] * MCOS(_par_angle[i*EFFECT_MANAGER_MAX_CACHE+ j]);
				_par_y[i*EFFECT_MANAGER_MAX_CACHE+ j] += _par_speed[i*EFFECT_MANAGER_MAX_CACHE+ j] * MSIN(_par_angle[i*EFFECT_MANAGER_MAX_CACHE+ j]);

				if (_par_opacity[i*EFFECT_MANAGER_MAX_CACHE+ j] < 0)
				{
					_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j] = EFFECT_MANAGER_STATE_NONE;
				}
			}
		}

			
		if (BATTLE()._opacityDay <= 0)
		{
			_state[i] = EFFECT_MANAGER_STATE_NONE;
			MAIN_GAME()->_isRain = false;
		}
	}
}
void CEffectManager::RenderParticalRain(int i)
{
	/*int min = 30 * 60;
	int mod = _var2[i] % min;

	if (mod > min - 7)
	{
		G()->SetColor(0xeeffffff);
		G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);
		G()->FillFullScreen(true);
		G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
	}

	if (mod > min - 25 && mod < min - 18)
	{
		G()->SetColor(0xaaffffff);
		G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);
		G()->FillFullScreen(true);
		G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
	}
	*/
	for (int j = 0; j < _var1[i]; j++)
	{
		if (_par_state[i*EFFECT_MANAGER_MAX_CACHE+ j] != EFFECT_MANAGER_STATE_NONE)
		{ 
			G()->SetOpacity(_par_opacity[i*EFFECT_MANAGER_MAX_CACHE+ j]);
			GET_SPRITE(_var3[i])->DrawFrame(G(), _par_frame[i*EFFECT_MANAGER_MAX_CACHE+ j], _par_x[i*EFFECT_MANAGER_MAX_CACHE+ j], _par_y[i*EFFECT_MANAGER_MAX_CACHE+ j]);
			G()->SetOpacity(100);
		}
	}
}

void CEffectManager::AddEffectTextFly(float x, float y, const char* string)
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_state[i] == EFFECT_MANAGER_STATE_NONE)
		{
			_type[i] = EFFECT_MANAGER_TYPE_TEXT_FLY;
			_state[i] = 1;
			_x[i] = x;
			_y[i] = y;
			_var1[i] = 150;
			_string[i] = string;

			break;
		}
	}
}
void CEffectManager::UpdateEffectTextFly(int i)
{
	switch (_state[i])
	{
	case 1:
		_var1[i] -= 1;
		_y[i] -= 2;
		if (_var1[i] < 0)
		{
			_state[i] = EFFECT_MANAGER_STATE_NONE;
		}
		break;
	}
}
void CEffectManager::RenderEffectTextFly(int i)
{
	G()->SetOpacity(_var1[i]);
	GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), _x[i], _y[i], _HCENTER | _VCENTER, _string[i]);
	G()->SetOpacity(100);
}