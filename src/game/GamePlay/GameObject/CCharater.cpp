#include "pch.h"
#include "../../GameCore/CGame.h"

void CCharater::SetState(BYTE state)
{
	//return;
	switch (state)
	{
	case CHARATER_STATE_BE_ATTACK:
		_timeDeplay = 120;

		_stateColor = 0;
		_isUndead = true;
		break;
	case CHARATER_STATE_DIE:
		_isTargetFocus = false;
		if (_posX < 200)
		{
			_posX = 200;
		}
		AddParticalDie(_posX, _posY - 30, 30, 0);
		CHARATER().SetAnimChater(16, false);
		StopEffect(EFFECT_TYPE_HAT_FALL);
		AddEffectHatFall(_posX, _posY - 100, 0, 3);
		_isUndead = false;
		break;
	case CHARATER_STATE_JUMP:
		_animHorse.SetAnimSprite(GET_SPRITE(_spriteID), _spriteID);
		_animHorse.SetAnim(17, true);
		_isRenderHorse = true;
		_posHorseX = _posX;
		_posHorseY = _posY;

		_g = 10;
		_y0 = _posY;
		_v0 = 25;
		_t0 = 0;
		_angle = 80;

		_timeDeplay = 60;
		_isUndead = true;

		GAME()->PlaySFX(SFX_HORSE_UP_DOWN);
		break;
	}

	_state = state;
}

void CCharater::SetTypePower(BYTE type)
{

	BATTLE()._speedIncreaseMap = 0;
	BATTLE()._speedIncreaseMove = 0;
	BATTLE()._isUsePower = true;

	if (type != POWER_NONE)
	{
		GAME()->PlaySFX(SFX_POWERUP_COLLECTED);

	}
	switch (type)
	{
	case POWER_NONE:
		if (_typePower == POWER_MULE || _typePower == POWER_MEDAL)
		{
			SetState(CHARATER_STATE_JUMP);

		}
		if (_typePower == POWER_MEDAL)
		{
			EFFECT().StopEffect(EFFECT_MANAGER_TYPE_PATICAL_DUST);
			if (BATTLE()._lvlCheckPiont % 2 == 0)
			{
				EFFECT().AddParticalDust(SPRITEID_EFFECT_DUST, 4, 5, CHARATER_MOVE_DOWN_LEFT, 345, 360);
			}
			else
			{
				EFFECT().AddParticalRain(1);
			}
		}
		BATTLE()._isUsePower = false;
		break;
	case POWER_MACHETE:
		MAIN_GAME()->_timeKnife = 60;
		GAME()->PlaySFX(SFX_KNIFE);
		_armyAttack1 = NULL;
		_armyAttack2 = NULL;
		_timePower = GET_DES().getMacheteDes(_lvlMachete, MACHETE_DES_TIME) / 1000.0f * 60;
		_timeDeplay = _timePower;
		_isUndead = true;
		break;
	case POWER_MULE:
		if (BATTLE()._timeBegin <= 0)
		{
			GAME()->PlaySFX(SFX_HORSE_UP_DOWN);
		}
		GAME()->PlaySFX(SFX_HORSE_RUN);
		_isUndead = true;
		_timePower = GET_DES().getMuleDes(_lvlMule, MULE_DES_TIME) / 1000.0f * 60;
		_timeDeplay = _timePower;
		BATTLE()._speedIncreaseMap = GET_DES().getMuleDes(_lvlMule, MULE_DES_SPEED_SCROLL_MAP);
		BATTLE()._speedIncreaseMove = GET_DES().getMuleDes(_lvlMule, MULE_DES_MOVEMENT_SPEED);
		_timeSoundHore = 6 * 60;
		break;
	case POWER_MEDAL:
		GAME()->PlaySFX(SFX_HORSE_UP_DOWN);
		GAME()->PlaySFX(SFX_HORSE_RUN);
		EFFECT().StopEffect(EFFECT_MANAGER_TYPE_PATICAL_DUST);
		EFFECT().AddParticalDust(SPRITEID_EFFECT_FAST, 3, 10, CHARATER_MOVE_LEFT, 0, 0);
		_timePower = GET_DES().getCavalryDes(_lvlCavalry, CAVALRY_DES_TIME) / 1000.0f * 60;
		_timeDeplay = _timePower;
		_isUndead = true;
		BATTLE()._speedIncreaseMap = GET_DES().getCavalryDes(_lvlCavalry, CAVALRY_DES_SPEED_SCROLL_MAP);
		BATTLE()._speedIncreaseMove = GET_DES().getCavalryDes(_lvlCavalry, MULE_DES_MOVEMENT_SPEED);
		_timeSoundHore = 6 * 60;
		break;
	}
	if (type != POWER_MEDAL)
	{
		if (_typePower == POWER_MEDAL)
		{
			for (int i = 0; i < BATTLE()._numCavalry; i++)
			{
				BATTLE()._cavalry[i] = NULL;
			}
		}
	}
	SetAnimPower(type);
	_typePower = type;

	if (_typePower != POWER_MULE && _typePower != POWER_MEDAL)
	{
		_collisionW = GET_SPRITE(_spriteID)->GetFModuleWidth(1, 0);
		_collisionH = GET_SPRITE(_spriteID)->GetFModuleHeight(1, 0);

		_colMoveW = GET_SPRITE(_spriteID)->GetFModuleWidth(0, 0);
		_colMoveH = GET_SPRITE(_spriteID)->GetFModuleHeight(0, 0);

	}
	else
	{
		_collisionW = GET_SPRITE(_spriteID)->GetFModuleWidth(5, 0);
		_collisionH = GET_SPRITE(_spriteID)->GetFModuleHeight(5, 0);

		_colMoveW = GET_SPRITE(_spriteID)->GetFModuleWidth(4, 0);
		_colMoveH = GET_SPRITE(_spriteID)->GetFModuleHeight(4, 0);
	}
}

void CCharater::SetAnimPower(BYTE type)
{
	if (type == POWER_MEDAL)
	{
		type = POWER_MULE;
	}
	switch (_directionMove)
	{
	case CHARATER_MOVE_LEFT:
		_animCharater.SetAnim(2 + type * 5, true);
		break;
	case CHARATER_MOVE_TOP_LEFT:
		_animCharater.SetAnim(1 + type * 5, true);
		break;
	case CHARATER_MOVE_TOP:
		_animCharater.SetAnim(0 + type * 5, true);
		break;
	case CHARATER_MOVE_TOP_RIGHT:
		_animCharater.SetAnim(1 + type * 5, true);
		break;
	case CHARATER_MOVE_RIGHT:
		_animCharater.SetAnim(2 + type * 5, true);
		break;
	case CHARATER_MOVE_DOWN_RIGHT:
		_animCharater.SetAnim(3 + type * 5, true);
		break;
	case CHARATER_MOVE_DOWN:
		_animCharater.SetAnim(4 + type * 5, true);
		break;
	case CHARATER_MOVE_DOWN_LEFT:
		_animCharater.SetAnim(3 + type * 5, true);
		break;
	}
}
void CCharater::Init(float x, float y, BYTE type, BYTE initBy)
{
	_autoShot = false;
	_forceShot = false;
	_touchIDShot = -1;
	_touchID = -1;
	_type = type;
	_live = GET_DES().getAliveDes(_lvlAlive, LIVE_DES_TIME);
	_live += BATTLE()._numLife;
	_spriteID = GET_DES().getShopCharaterDes(USER()._currentUseCharater, SHOP_CHARATER_SPRITE_ID);// GET_DES()._charterSpriteID[USER()._currentUseCharater];

	GAME()->LoadSpriteCurrentVer(SPRITEID_EFFECT_BULLET, false, false);



	_posY = y;
	_posX = x;

	_typePower = POWER_NONE;

	_animCharater.SetAnimSprite(GET_SPRITE(_spriteID), _spriteID);

	int indexPower = 0;
	if (_typePower == POWER_NONE)
	{
		indexPower = 0;
	}
	else if (_typePower == POWER_MACHETE)
	{
		indexPower = 1;
	}
	else
	{
		indexPower = 2;
	}

	_animCharater.SetAnim(2 + indexPower * 5, true);

	_armyBeAttack = NULL;
	_armyAttack1 = NULL;
	_armyAttack2 = NULL;


	_isRenderHorse = false;
	if (_typePower != POWER_MULE && _typePower != POWER_MEDAL)
	{
		_collisionW = GET_SPRITE(_spriteID)->GetFModuleWidth(1, 0);
		_collisionH = GET_SPRITE(_spriteID)->GetFModuleHeight(1, 0);

		_colMoveW = GET_SPRITE(_spriteID)->GetFModuleWidth(0, 0);
		_colMoveH = GET_SPRITE(_spriteID)->GetFModuleHeight(0, 0);

	}
	else
	{
		_collisionW = GET_SPRITE(_spriteID)->GetFModuleWidth(5, 0);
		_collisionH = GET_SPRITE(_spriteID)->GetFModuleHeight(5, 0);

		_colMoveW = GET_SPRITE(_spriteID)->GetFModuleWidth(4, 0);
		_colMoveH = GET_SPRITE(_spriteID)->GetFModuleHeight(4, 0);
	}
	_isUndead = false;

	InitEffect();

	_angleMove = 180;
	_directionMove = CHARATER_MOVE_RIGHT;
	_scaleX = 1;

	//from design
	_distanceGun = GET_DES().getGunDes(_lvlRevolver, GUN_DES_SHOOT_DISTANCE);
	_distanceGun = k_SCREEN_WIDTH;
	_dame = GET_DES().getGunDes(_lvlRevolver, GUN_DES_DAME);
	/*_timeGun = GET_DES().getGunDes(_lvlFastTrigger, GUN_DES_TIME_GUN);
	_timeGun2 = GET_DES().getGunDes(_lvlFastTrigger, GUN_DES_TIME_GUN);*/
	_timeGunSpeed = GET_DES().getGunDes(_lvlFastTrigger, GUN_DES_TIME_GUN)*60;
	_timeGun2Speed = GET_DES().getGunDes(_lvlFastTrigger, GUN_DES_TIME_BULLET_DELAY)*60;
	_timeGun = _timeGunSpeed;
	_timeGun2 = _timeGun2Speed;
	_speedGun = GET_DES().getGunDes(_lvlFastTrigger, GUN_DES_SPEED);

	_speedStart = GET_DES().getCharaterDes(_lvlSpeed, CHARATER_DES_MOVE_SPEED);// SPEED_CHARATER_MOVE;

	_distanceMagnet = GET_DES().getMagnetDes(_lvlMagnet, MAGNET_DES_DISTANCE_EFFECT);


	SetPosStick();
	_state = CHARATER_STATE_MOVE;
	_angleGunX = 50;
	_angleGunY = -50;
	SetPosStickShoot();
	_stickShootScale = 1.0f;
	_isTargetFocus = false;

}
bool CCharater::CheckCollisonCoin(float coinX, float coinY, float coinRad)
{
	float dd = CMath::DISTANCE(_posX, _posY, coinX, coinY);
	if (dd <= coinRad + 20 + _distanceMagnet)
	{
		return true;
	}
	return false;
}
void CCharater::SetPosStick()
{
	int xx = 150;
	int yy = k_SCREEN_HEIGHT - 95;

	_stickX = xx;
	_stickY = yy;

	_stickCenterX = xx;
	_stickCenterY = yy;

	_stickRenderX = xx;
	_stickRenderY = yy;

	_stickCenterRenderX = _stickRenderX;
	_stickCenterRenderY = _stickRenderY;


	_isRenderStick = true;
}
void CCharater::UpdateTouchMove()
{
	//PDEBUG("\n char UpdateTouchMove");
	for (int i = 0; i < CGame::_this->_num_touches; i++)
	{
		int x = 0;
		int y = 0;
		int w = k_SCREEN_WIDTH/2;
		int h = k_SCREEN_HEIGHT;
		if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
		{
			//PDEBUG("\n char UpdateTouchMove --- _touchID:%d", _touchID);
			if (TOUCHS(i)->_state == TOUCH_DOWN)
			{
				if (_touchID == -1)
				{
					_touchID = TOUCHS(i)->_id;

					_touch_drag_x = TOUCHS(i)->x;
					_touch_drag_y = TOUCHS(i)->y;

					_stickX = _touch_drag_x;
					_stickY = _touch_drag_y;

					_stickCenterX = _touch_drag_x;
					_stickCenterY = _touch_drag_y;

					_stickRenderX = _touch_drag_x;
					_stickRenderY = _touch_drag_y;

					if (_stickRenderX < 100)
					{
						_stickRenderX = 100;
					}

					if (_stickRenderX > k_SCREEN_WIDTH - 100)
					{
						_stickRenderX = k_SCREEN_WIDTH - 100;
					}

					if (_stickRenderY < 100)
					{
						_stickRenderY = 100;
					}

					if (_stickRenderY > k_SCREEN_HEIGHT - 100)
					{
						_stickRenderY = k_SCREEN_HEIGHT - 100;
					}

					_stickCenterRenderX = _stickRenderX;
					_stickCenterRenderY = _stickRenderY;

					_isRenderStick = true;
				}

			}
			else if (TOUCHS(i)->_state == TOUCH_DRAG)
			{
				if (_isRenderStick && _touchID == TOUCHS(i)->_id)
				{
					float dd = CMath::DISTANCE(_touch_drag_x, _touch_drag_y, _stickX, _stickY);
					_touch_drag_x = TOUCHS(i)->x;
					_touch_drag_y = TOUCHS(i)->y;

					float angle = CMath::ANGLE(_stickX, _stickY, TOUCHS(i)->x, TOUCHS(i)->y) + 180;
					if (dd > 30)
					{
						_angleMove = CMath::ANGLE(_stickX, _stickY, TOUCHS(i)->x, TOUCHS(i)->y) + 180;
					}

					if (_armyAttack1 == NULL && _armyAttack2 == NULL)
					{
						CheckDirectionMove(_angleMove);
					}


					_stickCenterX = _stickX + dd * CMath::COS(angle + 180);
					_stickCenterY = _stickY + dd * CMath::SIN(angle + 180);

					_stickCenterRenderX = _stickRenderX + dd * CMath::COS(angle + 180);
					_stickCenterRenderY = _stickRenderY + dd * CMath::SIN(angle + 180);

					if (dd > 50)
					{
						_stickCenterX = _stickX + 50 * CMath::COS(angle + 180);
						_stickCenterY = _stickY + 50 * CMath::SIN(angle + 180);

						_stickCenterRenderX = _stickRenderX + 50 * CMath::COS(angle + 180);
						_stickCenterRenderY = _stickRenderY + 50 * CMath::SIN(angle + 180);

					}

					if (dd > 65)
					{
						_stickX = _touch_drag_x + 65 * CMath::COS(angle);
						_stickY = _touch_drag_y + 65 * CMath::SIN(angle);
						dd = 65;
					}

					float acell = ((dd) / (65))*1.0f;
					if (acell > 0.7)
					{
						acell = 1.0f;
					}
					if (acell > 0.05f)
					{
						float offset = _stickCenterX - _stickX < 0 ? (BATTLE_MAP()._speed_scroll_map*1.2) : 0;
						_posX -= (_speedMove + offset)*acell*CMath::COS(angle);
						_posY -= (_speedMove + BATTLE_MAP()._speed_scroll_map / 2)*acell*CMath::SIN(angle);
					}

				}
			}
			else if (TOUCHS(i)->_state == TOUCH_UP)
			{
				if (_isRenderStick && _touchID == TOUCHS(i)->_id)
				{
					_touchID = -1;
					_isRenderStick = false;
					_angleMove = 180;
					_directionMove = CHARATER_MOVE_RIGHT;
					_scaleX = 1;
					int indexPower = 0;
					if (_typePower == POWER_NONE)
					{
						indexPower = 0;
					}
					else if (_typePower == POWER_MACHETE)
					{
						indexPower = 1;
					}
					else
					{
						indexPower = 2;
					}

					_animCharater.SetAnim(2 + indexPower * 5, true);
					SetPosStick();
					//GAME()->ClearAllTouch();
				}
				else if (_touchIDShot == TOUCHS(i)->_id)
				{
					_touchIDShot = -1;
					_autoShot = false;
					_forceShot = false;
					SetPosStickShoot();
				}
			}
		}
		else
		{
			if (_touchID == TOUCHS(i)->_id)
			{
				if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					if (_isRenderStick)
					{
						float dd = CMath::DISTANCE(_touch_drag_x, _touch_drag_y, _stickX, _stickY);
						_touch_drag_x = TOUCHS(i)->x;
						_touch_drag_y = TOUCHS(i)->y;

						float angle = CMath::ANGLE(_stickX, _stickY, TOUCHS(i)->x, TOUCHS(i)->y) + 180;
						if (dd > 30)
						{
							_angleMove = CMath::ANGLE(_stickX, _stickY, TOUCHS(i)->x, TOUCHS(i)->y) + 180;
						}

						if (_armyAttack1 == NULL && _armyAttack2 == NULL)
						{
							CheckDirectionMove(_angleMove);
						}


						_stickCenterX = _stickX + dd * CMath::COS(angle + 180);
						_stickCenterY = _stickY + dd * CMath::SIN(angle + 180);

						_stickCenterRenderX = _stickRenderX + dd * CMath::COS(angle + 180);
						_stickCenterRenderY = _stickRenderY + dd * CMath::SIN(angle + 180);

						if (dd > 50)
						{
							_stickCenterX = _stickX + 50 * CMath::COS(angle + 180);
							_stickCenterY = _stickY + 50 * CMath::SIN(angle + 180);

							_stickCenterRenderX = _stickRenderX + 50 * CMath::COS(angle + 180);
							_stickCenterRenderY = _stickRenderY + 50 * CMath::SIN(angle + 180);

						}

						if (dd > 65)
						{
							_stickX = _touch_drag_x + 65 * CMath::COS(angle);
							_stickY = _touch_drag_y + 65 * CMath::SIN(angle);
							dd = 65;
						}

						float acell = ((dd) / (65))*1.0f;
						if (acell > 0.7)
						{
							acell = 1.0f;
						}
						if (acell > 0.05f)
						{
							float offset = _stickCenterX - _stickX < 0 ? (BATTLE_MAP()._speed_scroll_map*1.2) : 0;
							_posX -= (_speedMove + offset)*acell*CMath::COS(angle);
							_posY -= (_speedMove + BATTLE_MAP()._speed_scroll_map / 2)*acell*CMath::SIN(angle);
						}

					}
				}
				else if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (_isRenderStick )
					{
						_touchID = -1;
						_isRenderStick = false;
						_angleMove = 180;
						_directionMove = CHARATER_MOVE_RIGHT;
						_scaleX = 1;
						int indexPower = 0;
						if (_typePower == POWER_NONE)
						{
							indexPower = 0;
						}
						else if (_typePower == POWER_MACHETE)
						{
							indexPower = 1;
						}
						else
						{
							indexPower = 2;
						}
						_animCharater.SetAnim(2 + indexPower * 5, true);
						SetPosStick();
					}
				}
			}
			else
			{
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					if (_touchIDShot == -1)
					{
						/*_stickShootRenderX = TOUCHS(i)->x;
						_stickShootRenderY = TOUCHS(i)->y;*/
						_touchIDShot = TOUCHS(i)->_id;
						//_autoShot = true;
						_forceShot = true;
						_timeGun = 0;
						//_timeGun2 = 0;
						_stickShootScale = 0.8f;
						TOUCHS(i)->_state = TOUCH_DRAG;
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					/*if (_touchIDShot == TOUCHS(i)->_id)
					{
						_stickShootRenderX = TOUCHS(i)->x;
						_stickShootRenderY = TOUCHS(i)->y;
					}*/
				}
				else if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (_touchIDShot == TOUCHS(i)->_id)
					{
						_touchIDShot = -1;
						_autoShot = false;
						_forceShot = false;
						SetPosStickShoot();
					}
				}
			}
		}
	}
	if (CGame::_this->_num_touches <= 0)
	{
		if (_isRenderStick && _touchID != -1)
		{
			_touchID = -1;
			_isRenderStick = false;
			_angleMove = 180;
			_directionMove = CHARATER_MOVE_RIGHT;
			_scaleX = 1;
			int indexPower = 0;
			if (_typePower == POWER_NONE)
			{
				indexPower = 0;
			}
			else if (_typePower == POWER_MACHETE)
			{
				indexPower = 1;
			}
			else
			{
				indexPower = 2;
			}

			_animCharater.SetAnim(2 + indexPower * 5, true);
		}
	}
}

void CCharater::SetPosStickShoot()
{
	_stickShootRenderX = k_SCREEN_WIDTH - 150;
	_stickShootRenderY = k_SCREEN_HEIGHT - 100;
}
void CCharater::RenderStickShoot()
{
	if (_stickShootScale < 1.0f)
	{
		_stickShootScale += 0.02f;
	}
	G()->SetScale(_stickShootScale, _stickShootScale, _stickShootRenderX, _stickShootRenderY);
	GET_SPRITE(SPRITEID_HUD_STICK)->DrawFrame(G(), 0, _stickShootRenderX, _stickShootRenderY);
	GET_SPRITE(SPRITEID_HUD_STICK)->DrawFrame(G(), 2, _stickShootRenderX, _stickShootRenderY);
	G()->ClearScale();
}

void CCharater::RenderStick()
{
	GET_SPRITE(SPRITEID_HUD_STICK)->DrawFrame(G(), 0, _stickRenderX, _stickRenderY);

	GET_SPRITE(SPRITEID_HUD_STICK)->DrawFrame(G(), 1, _stickCenterRenderX, _stickCenterRenderY);
}
void CCharater::CheckDirectionMove(float angle)
{

	if (_state == CHARATER_STATE_DIE)
	{
		return;
	}
	if (angle < 0)
	{
		angle += 360;
	}
	int Angle = CMath::ABSOLUTE_VALUE(angle);

	int indexPower = 0;
	if (_typePower == POWER_NONE)
	{
		indexPower = 0;
	}
	else if (_typePower == POWER_MACHETE)
	{
		indexPower = 1;
	}
	else
	{
		indexPower = 2;
	}
	if (Angle <= 10 || Angle >= 350)
	{
		if (_directionMove != CHARATER_MOVE_LEFT)
		{
			_directionMove = CHARATER_MOVE_LEFT;
			_animCharater.SetAnim(2 + indexPower * 5, true);
			_scaleX = -1;
			//printf("\n move left");
		}
	}
	else if (Angle < 80)
	{
		if (_directionMove != CHARATER_MOVE_TOP_LEFT)
		{
			_directionMove = CHARATER_MOVE_TOP_LEFT;
			_animCharater.SetAnim(1 + indexPower * 5, true);
			_scaleX = -1;
			_angleGunX = -40;
			_angleGunY = -70;
			//printf("\n move top-left");
		}

	}
	else if (Angle < 100)
	{
		if (_directionMove != CHARATER_MOVE_TOP)
		{
			_directionMove = CHARATER_MOVE_TOP;
			_animCharater.SetAnim(0 + indexPower * 5, true);
			_scaleX = 1;
			//printf("\n move top");

		}
	}
	else if (Angle < 170)
	{
		if (_directionMove != CHARATER_MOVE_TOP_RIGHT)
		{
			_directionMove = CHARATER_MOVE_TOP_RIGHT;
			_animCharater.SetAnim(1 + indexPower * 5, true);
			_scaleX = 1;
			//printf("\n move top-right");
		}
	}
	else if (Angle < 190)
	{
		if (_directionMove != CHARATER_MOVE_RIGHT)
		{
			_directionMove = CHARATER_MOVE_RIGHT;
			_animCharater.SetAnim(2 + indexPower * 5, true);
			_scaleX = 1;
			//printf("\n move right");
		}
	}
	else if (Angle < 260)
	{
		if (_directionMove != CHARATER_MOVE_DOWN_RIGHT)
		{
			_directionMove = CHARATER_MOVE_DOWN_RIGHT;
			_animCharater.SetAnim(3 + indexPower * 5, true);
			_scaleX = 1;
			//printf("\n move down-right");
		}
	}
	else if (Angle < 280)
	{
		if (_directionMove != CHARATER_MOVE_DOWN)
		{
			_directionMove = CHARATER_MOVE_DOWN;
			_animCharater.SetAnim(4 + indexPower * 5, true);
			_scaleX = 1;
			//printf("\n move down");
		}
	}
	else
	{
		if (_directionMove != CHARATER_MOVE_DOWN_LEFT)
		{
			_directionMove = CHARATER_MOVE_DOWN_LEFT;
			_animCharater.SetAnim(3 + indexPower * 5, true);
			_scaleX = -1;
			//printf("\n move down-left");
		}
	}

}
void CCharater::UpdateAngleGun()
{
	switch (_directionMove)
	{
	case CHARATER_MOVE_LEFT:
		_angleGunX = -60;
		_angleGunY = -50;
		break;
	case CHARATER_MOVE_RIGHT:
		_angleGunX = 60;
		_angleGunY = -50;
		break;
	case CHARATER_MOVE_TOP:
		_angleGunX = 0;
		_angleGunY = -100;
		break;
	case CHARATER_MOVE_DOWN:
		_angleGunX = 0;
		_angleGunY = -30;
		break;
	case CHARATER_MOVE_TOP_LEFT:
		_angleGunX = -35;
		_angleGunY = -80;
		break;
	case CHARATER_MOVE_TOP_RIGHT:
		_angleGunX = 35;
		_angleGunY = -80;
		break;
	case CHARATER_MOVE_DOWN_LEFT:
		_angleGunX = -40;
		_angleGunY = -30;
		break;
	case CHARATER_MOVE_DOWN_RIGHT:
		_angleGunX = 40;
		_angleGunY = -30;
		break;
	}
}
void CCharater::SetAttackArmy1(CArmy *army)
{
	_armyAttack1 = army; 
}
void CCharater::SetAttackArmy2(CArmy *army)
{
	_armyAttack2 = army;
}

void CCharater::Update()
{
	if (_state == CHARATER_STATE_DIE)
	{
		
	}
	else
	{
		//_isUndead = true;
		_speedMove = _speedStart + BATTLE()._speedIncreaseMove;

		if (_posX <= 50)
		{
			_posX = 50;
		}

		if (_posX >= k_SCREEN_WIDTH - 50)
		{
			_posX = k_SCREEN_WIDTH - 50;
		}

		if (_posY <= MIN_MOVE_Y + BATTLE()._offsetMoveY)
		{
			_posY = MIN_MOVE_Y + BATTLE()._offsetMoveY;
		}

		if (_posY >= MAX_MOVE_Y)
		{
			_posY = MAX_MOVE_Y;
		}

		if (_typePower != POWER_NONE)
		{
			if (_timePower > 0)
			{
				_timePower--;

				if (_timePower <= 0)
				{
					SetTypePower(POWER_NONE);
				}

				if (_typePower != POWER_MACHETE)
				{
					if (_timePower <= 1 * 60)
					{
						BATTLE()._speedIncreaseMap -= 0.4;
						if (BATTLE()._speedIncreaseMap <= 0)
						{
							BATTLE()._speedIncreaseMap = 0;
						}
					}
				}
			}

			if (_timeSoundHore > 0 && _typePower != POWER_MACHETE)
			{
				_timeSoundHore--;
				if (_timeSoundHore == 0)
				{
					GAME()->PlaySFX(SFX_HORSE_RUN);
					_timeSoundHore = 6 * 60;
				}
				if (_typePower == POWER_MEDAL)
				{
					if (_timeSoundHore % 60 == 0)
					{
						GAME()->PlaySFX(SFX_HORSE_UP_DOWN);
					}
				}
			}
			else
			{
				MAIN_GAME()->_timeKnife--;
				if (MAIN_GAME()->_timeKnife <= 0)
				{
					GAME()->PlaySFX(SFX_KNIFE);
					MAIN_GAME()->_timeKnife = 1 * 50;
				}

			}
		}
		if (_timeDeplay > 0 && _isUndead)
		{
			_timeDeplay--;
			if (_timeDeplay <= 0)
			{
				_state = CHARATER_STATE_MOVE;
				_isUndead = false;
			}
		}
		if (_state == CHARATER_STATE_JUMP)
		{
			_t0 += 16;

			float ft = _t0 / 80.0f;
			float x = 20 * CMath::COS(90)*ft;
			float y = 20 * CMath::SIN(90)*ft - _g*ft*ft*0.5f;

			_posX += x;
			_posY -= y;

			if (_posY >= _y0)
			{
				_timeDeplay = 60;
				_isUndead = true;
				SetState(CHARATER_STATE_MOVE);
				if (TUTORIAL()._step == 0)
				{
					BATTLE()._addTutorial = true;
				}

			}
		}

		if (_isRenderHorse)
		{
			_posHorseX += 10;
			if (_posHorseX >= k_SCREEN_WIDTH + 50)
			{
				_isRenderHorse = false;
				GAME()->StopAllSFX();

			}
			_animHorse.SetPos(_posHorseX, _posHorseY + BATTLE()._vibrateY);
		}
		if (_timeGun <= 0)
		{
			if (_armyAttack1 != NULL)
			{
				/*_timeGun2 = GET_DES().getGunDes(_lvlFastTrigger, GUN_DES_TIME_BULLET_DELAY) * 60;
					_timeGun = GET_DES().getGunDes(_lvlFastTrigger, GUN_DES_TIME_GUN) * 60;*/
				_timeGun = _timeGunSpeed;
				_timeGun2 = _timeGun2Speed;
				_angleMove = CMath::ANGLE(_posX, _posY, _armyAttack1->PosX(), _armyAttack1->PosY()) + 180;
				CheckDirectionMove(_angleMove);
				UpdateAngleGun();
				AddEffectGun2(_posX, _posY, _armyAttack1, _speedGun);
				PDEBUG("\n shot by gun 1");
			}
			else if(GetTypePower() == POWER_NONE)
			{
				if (_autoShot || _forceShot)
				{
					_timeGun = _timeGunSpeed;
					_timeGun2 = _timeGun2Speed;
					//_angleMove += 180;
					CheckDirectionMove(_angleMove);
					UpdateAngleGun();
					float tarx = _posX + k_SCREEN_HEIGHT * CMath::COS(180);
					float tary = _posY + k_SCREEN_HEIGHT * CMath::SIN(180);
					AddEffectGun2(_posX, _posY, tarx, tary, _speedGun);
					_forceShot = false;
				}
			}
		}
		if (_timeGun > 0)
		{
			_timeGun--;
		}
		/*if (_timeGun2 <= 0)
		{
			if (_armyAttack2 != NULL)
			{
				//_timeGun2 = GET_DES().getGunDes(_lvlFastTrigger, GUN_DES_TIME_GUN) * 60;
				_timeGun = _timeGunSpeed;
				_timeGun2 = _timeGun2Speed;
				_angleMove = CMath::ANGLE(_posX, _posY, _armyAttack2->PosX(), _armyAttack2->PosY()) + 180;
				CheckDirectionMove(_angleMove);
				UpdateAngleGun();
				AddEffectGun2(_posX, _posY, _armyAttack2, _speedGun);
				PDEBUG("\n shot by gun 2");
			}
			else if (GetTypePower() == POWER_NONE)
			{
				if (_autoShot || _forceShot)
				{
					_timeGun = _timeGunSpeed;
					_timeGun2 = _timeGun2Speed;
					//_angleMove += 180;
					CheckDirectionMove(_angleMove);
					UpdateAngleGun();
					float tarx = _posX + k_SCREEN_HEIGHT * CMath::COS(180);
					float tary = _posY + k_SCREEN_HEIGHT * CMath::SIN(180);
					AddEffectGun2(_posX, _posY, tarx, tary, _speedGun);
					_forceShot = false;
				}
			}
		}
		if (_timeGun2 > 0)
		{
			_timeGun2--;
		}*/
	}
	_animCharater.SetPos(_posX, _posY + BATTLE()._vibrateY);
	if (_typePower != POWER_MULE && _typePower != POWER_MEDAL)
	{
		_collisionX = GET_SPRITE(_spriteID)->GetFModuleX(1, 0) + _posX;
		_collisionY = GET_SPRITE(_spriteID)->GetFModuleY(1, 0) + _posY;

		_colMoveX = GET_SPRITE(_spriteID)->GetFModuleX(0, 0) + _posX;
		_colMoveY = GET_SPRITE(_spriteID)->GetFModuleX(0, 0) + _posY;

		_attackX = GET_SPRITE(_spriteID)->GetFModuleCenterX(1, 0) + _posX;
		_attackY = GET_SPRITE(_spriteID)->GetFModuleCenterY(1, 0) + _posY;

	}
	else
	{
		_collisionX = GET_SPRITE(_spriteID)->GetFModuleX(5, 0) + _posX;
		_collisionY = GET_SPRITE(_spriteID)->GetFModuleY(5, 0) + _posY;

		_colMoveX = GET_SPRITE(_spriteID)->GetFModuleX(4, 0) + _posX;
		_colMoveY = GET_SPRITE(_spriteID)->GetFModuleX(4, 0) + _posY;

		_attackX = GET_SPRITE(_spriteID)->GetFModuleCenterX(5, 0) + _posX;
		_attackY = GET_SPRITE(_spriteID)->GetFModuleCenterY(5, 0) + _posY;
	}


	UpdateEffect();

}

void CCharater::RenderUnDie()
{

	if (_isRenderHorse)
	{
		GET_SPRITE(_spriteID)->DrawFrame(G(), 3, _animHorse._x, _animHorse._y);
		_animHorse.DrawAnimObject(G());
		_animHorse.UpdateAnimObject();
	}


	if (_isUndead && BATTLE()._timeBegin <= 0 && BATTLE()._timePlay > 5 * 60)
	{
		if (_timeDeplay % 20 < 10)
		{
			//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);
		}

		G()->SetColorMask(0xffb800);
		_animCharater.DrawAnimObject(G());
		//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
		G()->ClearColorMask();
	}
}

void CCharater::Render()
{


	if (_state != CHARATER_STATE_JUMP)
	{
		if (_typePower != POWER_MULE && _typePower != POWER_MEDAL)
		{
			if (_state != CHARATER_STATE_DIE)
			{
				GET_SPRITE(_spriteID)->DrawFrame(G(), 2, _animCharater._x, _animCharater._y);
			}
		}
		else
		{
			GET_SPRITE(_spriteID)->DrawFrame(G(), 3, _animCharater._x, _animCharater._y);
		}
	}

	if (_state == CHARATER_STATE_DIE)
	{
		if (_animCharater.IsCurrentAnimStop())
		{
			GET_SPRITE(_spriteID)->DrawFrame(G(), 117, _animCharater._x, _animCharater._y);
		}
	}

	BATTLE()._animManager.Add(&_animCharater, 100, _animCharater._x, _animCharater._y);

	//_animCharater.DrawAnimObject(G());
	G()->ClearScale();

	_animCharater.UpdateAnimObject();


	G()->SetOpacity(100);



	if (_typePower != POWER_NONE && BATTLE()._timeBegin <= 0)
	{
		int offsetY = 145;
		if (_typePower == POWER_MULE || _typePower == POWER_MEDAL)
		{
			offsetY = 220;
		}
		if (_timePower / 60 > 0)
		{
			GET_SPRITE(SPRITEID_FONT_POWER)->DrawAnsiTextFormat(G(), _posX - 5, _posY - offsetY, _HCENTER | _VCENTER, "%d", (int)(_timePower / 60));
		}
		else
		{
			if (_timePower % 20 <= 10)
			{
				G()->SetScale(1.2, 1.2, _posX - 5, _posY - offsetY);
				GET_SPRITE(SPRITEID_FONT_POWER)->DrawAnsiTextFormat(G(), _posX - 5, _posY - offsetY, _HCENTER | _VCENTER, "%d", (int)(_timePower / 60));
				G()->ClearScale();
			}
		}
	}




}
bool CCharater::IsMoveToTarget(float &x, float &y, float targetX, float targetY, float speed)
{
	bool ret = false;
	float distance = CMath::DistancePOW2(x, y, targetX, targetY);
	if (distance < (speed * speed))
	{
		x = targetX;
		y = targetY;
		ret = true;
	}
	else
	{
		float _angle = CMath::ANGLE(targetX, targetY, x, y) - 180;
		float vy = speed*CMath::SIN(_angle);
		float vx = speed*CMath::COS(_angle);

		x += vx;
		y += vy;

	}
	return ret;
}
void CCharater::BeDame(int dame)
{
	SetState(CHARATER_STATE_BE_ATTACK);
	GAME()->PlaySFX(SFX_PLAYER_HURT);
	{
		if (_live > 0)
		{
			//printf("\n\n========\n========\n bi trung dan \n========\n========\n\n");
			_live -= dame;
			if (_live <= 0)
			{
				if (_state != CHARATER_STATE_DIE)
				{
					SetState(CHARATER_STATE_DIE);
				}
			}
		}
	}
}
void CCharater::AddLive()
{
	if (_live < 5 + BATTLE()._numLife)
	{
		_live++;
	}
}
void CCharater::RenderLive()
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
	if (BATTLE()._timeBegin <= 0)
	{
		for (int i = 0; i < _live; i++)
		{
			int yy = -60;
			if (MAIN_GAME()->_playWitchEndlest)
			{
				yy = 0;
			}
			GET_SPRITE(SPRITEID_HUD_INGAME)->DrawFrame(G(), 9, 20 + 55 * i, yy + offsetY);
		}
	}
}
#pragma region effect attack
//effect

void CCharater::InitEffect()
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		_eff_state[i] = EFFECT_STATE_ATTACK_NONE;
	}
}
bool CCharater::CompleteEffect(BYTE type, BYTE state)
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		if (_eff_type[i] == type && _eff_state[i] != state)
		{
			return false;
		}
	}
	return true;
}
bool CCharater::CompleteEffect(BYTE type)
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		if (_eff_type[i] == type && _eff_state[i] != EFFECT_STATE_ATTACK_NONE)
		{
			return false;
		}
	}
	return true;
}
bool CCharater::CompleteEffect()
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		if (_eff_state[i] != EFFECT_STATE_ATTACK_NONE)
		{
			return false;
		}
	}
	return true;
}
void CCharater::StopEffect(BYTE type)
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		if (_eff_type[i] == type && _eff_state[i] != EFFECT_STATE_ATTACK_NONE)
		{
			_eff_state[i] = EFFECT_STATE_ATTACK_NONE;
		}
	}

}
void CCharater::SetStateEffect(BYTE type, BYTE state)
{
	for (int i = 0; i < EFFECT_STATE_ATTACK_NONE; i++)
	{
		if (_eff_type[i] == type && _eff_state[i] != EFFECT_STATE_ATTACK_NONE)
		{
			_eff_state[i] = state;
		}
	}
}

bool CCharater::CompleteStatePartical(int i, int num, BYTE state)
{
	bool res = true;
	for (int j = 0; j < num; j++)
	{
		if (_par_state[i * MAX_EFFECT_PARTICAL + j] != state)
		{
			res = false;
			break;
		}
	}
	return res;
}

void CCharater::AddEffectGun(float x, float y, float speed)
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		if (_eff_state[i] == EFFECT_STATE_ATTACK_NONE)
		{
			GAME()->PlaySFX(SFX_GUNSHOT);
			_eff_type[i] = EFFECT_TYPE_ATTACK_GUN;
			_eff_state[i] = 1;

			_eff_anim[i].SetAnimSprite(GET_SPRITE(SPRITEID_EFFECT_BULLET), SPRITEID_EFFECT_BULLET);
			_eff_anim[i].SetAnim(0, true);

			_eff_anim2[i].SetAnimSprite(GET_SPRITE(SPRITEID_EFFECT_BULLET), SPRITEID_EFFECT_BULLET);
			_eff_anim2[i].SetAnim(2, false);
			_eff_anim2[i].SetSubTime(1);

			_varF1[i] = speed;

			_varF2[i] = _armyBeAttack->PosAttackX();
			_varF3[i] = _armyBeAttack->PosAttackY();

			if (_typePower != POWER_MULE && _typePower != POWER_MEDAL)
			{
				_eff_x[i] = x + _angleGunX;
				_eff_y[i] = y + _angleGunY;
			}
			else
			{
				_eff_x[i] = x + _angleGunX;
				_eff_y[i] = y + _angleGunY - 100;
			}

			_eff_anim2[i].SetPos(_eff_x[i], _eff_y[i]);
			_varF4[i] = CMath::ANGLE(_eff_x[i], _eff_y[i], _varF2[i], _varF3[i]);
			break;
		}
	}
}
void CCharater::UpdateEffectGun(int i)
{
	if (!CHECK_POINT_IN_RECT(_eff_x[i], _eff_y[i], 0, 0, k_SCREEN_WIDTH, k_SCREEN_HEIGHT))
	{
		_eff_state[i] = EFFECT_STATE_ATTACK_NONE;
	}
	switch (_eff_state[i])
	{
	case 1:
		_eff_y[i] += _varF1[i] * CMath::SIN(_varF4[i]);
		_eff_x[i] += _varF1[i] * CMath::COS(_varF4[i]);

		for (int j = 0; j < BATTLE()._numArmy; j++)
		{
			if (BATTLE()._army[j] != NULL && BATTLE()._army[j]->GetType() != TYPE_ENEMY_BOSS_2)
			{
				if (BATTLE()._army[j]->GetState() != CHARATER_STATE_DIE)
				{
					if (CHECK_POINT_IN_RECT(_eff_x[i], _eff_y[i], BATTLE()._army[j]->CollisionX(), BATTLE()._army[j]->CollisionY(), BATTLE()._army[j]->CollisionW(), BATTLE()._army[j]->CollisionH()))
					{
						_eff_state[i] = 2;
						BATTLE()._army[j]->BeDame(_dame);
						BATTLE()._army[j]->SetAngleBullet(_varF4[i]);
						_eff_state[i] = EFFECT_STATE_ATTACK_NONE;
					}
				}
			}
		}

		break;

	case 2:
		for (int j = 0; j < BATTLE()._numArmy; j++)
		{
			if (BATTLE()._army[j] != NULL)
			{
				//if (BATTLE()._army[j]->GetState() != ARMY_STATE_DIE)
				{
					if (BATTLE()._army[j]->GetState() == CHARATER_STATE_DIE)
					{
						if (BATTLE()._army[j]->CompleteEffect() && BATTLE()._army[j]->GetScale() == 0)
						{
							//BATTLE()._army[j]->SetState(CHARATER_STATE_NULL);
							if (_armyBeAttack == BATTLE()._army[j])
							{
								_armyBeAttack = NULL;
							}
							BATTLE()._army[j] = NULL;
							_eff_state[i] = EFFECT_STATE_ATTACK_NONE;
						}
					}
				}
			}
		}
		break;

	case 3:
		break;
	}
}
void CCharater::RenderEffectGun(int i)
{
	_eff_anim[i].SetPos(_eff_x[i], _eff_y[i]);
	G()->SetRotate(_varF4[i], _eff_anim2[i]._x, _eff_anim2[i]._y);

	//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);

	_eff_anim2[i].DrawAnimObject(G());
	_eff_anim2[i].UpdateAnimObject();
	//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
	G()->ClearRotate();

	_eff_anim[i].DrawAnimObject(G());
	_eff_anim[i].UpdateAnimObject();
}

void CCharater::AddParticalDie(float x, float y, int numPar, float angle)
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		if (_eff_state[i] == EFFECT_STATE_ATTACK_NONE)
		{

			GAME()->PlaySFX(SFX_ENEMY_DIE_1 + CMath::RANDOM(0, 2));

			_eff_type[i] = EFFECT_TYPE_PARTICAL_DIE;
			_eff_state[i] = 1;
			_eff_x[i] = x;
			_eff_y[i] = y;
			_var1[i] = numPar;

			for (int j = 0; j < _var1[i]; j++)
			{
				_par_x0[i*MAX_EFFECT_PARTICAL + j] = _eff_x[i] + CMath::RANDOM(-20, 20);
				_par_y0[i*MAX_EFFECT_PARTICAL + j] = _eff_y[i] + CMath::RANDOM(-20, 20);
				_par_target_y0[i*MAX_EFFECT_PARTICAL + j] = _eff_y[i] + 50;

				_par_v0[i*MAX_EFFECT_PARTICAL + j] = 50 + CMath::RANDOM(-20, 0);
				_par_g0[i*MAX_EFFECT_PARTICAL + j] = 20 + CMath::RANDOM(-5, 0);
				_par_t0[i*MAX_EFFECT_PARTICAL + j] = 0;

				_par_angle[i*MAX_EFFECT_PARTICAL + j] = 100 + CMath::RANDOM(-10, 15);

				_par_state[i*MAX_EFFECT_PARTICAL + j] = 1;
				_par_frame[i*MAX_EFFECT_PARTICAL + j] = CMath::RANDOM(0, 2);

				_par_opacity[i*MAX_EFFECT_PARTICAL + j] = 100;
				_par_count[i*MAX_EFFECT_PARTICAL + j] = 0;

				if (CHARATER().PosX() > x)
				{
					_par_v1[i*MAX_EFFECT_PARTICAL + j] = _LEFT;
				}
				else
				{
					_par_v1[i*MAX_EFFECT_PARTICAL + j] = _RIGHT;
				}

			}

			break;
		}
	}
}
void CCharater::RenderParticalDie(int i)
{
	for (int j = 0; j < _var1[i]; j++)
	{
		if (_par_state[i*MAX_EFFECT_PARTICAL + j] != EFFECT_STATE_ATTACK_NONE)
		{
			G()->SetOpacity(_par_opacity[i*MAX_EFFECT_PARTICAL + j]);
			GET_SPRITE(SPRITEID_EFFECT_PARTICAL)->DrawFrame(G(), _par_frame[i*MAX_EFFECT_PARTICAL + j], _par_x[i*MAX_EFFECT_PARTICAL + j], _par_y[i*MAX_EFFECT_PARTICAL + j]);
			G()->SetOpacity(100);
		}
	}
}
void CCharater::UpdateParticalDie(int i)
{
	switch (_eff_state[i])
	{
	case 1:
		for (int j = 0; j < _var1[i]; j++)
		{
			switch (_par_state[i*MAX_EFFECT_PARTICAL + j])
			{
			case 1:
			{
				_par_t0[i*MAX_EFFECT_PARTICAL + j] += 16;// DT();

				float ft = _par_t0[i*MAX_EFFECT_PARTICAL + j] / 40.0f;
				float x = _par_v0[i*MAX_EFFECT_PARTICAL + j] * CMath::COS(_par_angle[i*MAX_EFFECT_PARTICAL + j])*ft;
				float y = _par_v0[i*MAX_EFFECT_PARTICAL + j] * CMath::SIN(_par_angle[i*MAX_EFFECT_PARTICAL + j])*ft - _par_g0[i*MAX_EFFECT_PARTICAL + j] * ft*ft*0.5f;


				if (_par_v1[i*MAX_EFFECT_PARTICAL + j] == _RIGHT)
				{
					_par_x[i*MAX_EFFECT_PARTICAL + j] = _par_x0[i*MAX_EFFECT_PARTICAL + j] - x;
				}
				else
				{
					_par_x[i*MAX_EFFECT_PARTICAL + j] = _par_x0[i*MAX_EFFECT_PARTICAL + j] + x;
				}
				_par_y[i*MAX_EFFECT_PARTICAL + j] = _par_y0[i*MAX_EFFECT_PARTICAL + j] - y;


				_par_opacity[i*MAX_EFFECT_PARTICAL + j] -= 4;
				if (_par_opacity[i*MAX_EFFECT_PARTICAL + j] <= 0)
				{

				}
				if (_par_y[i*MAX_EFFECT_PARTICAL + j] > _par_target_y0[i*MAX_EFFECT_PARTICAL + j])
				{
					_par_count[i*MAX_EFFECT_PARTICAL + j]++;
					_par_g0[i*MAX_EFFECT_PARTICAL + j] = 25;
					_par_t0[i*MAX_EFFECT_PARTICAL + j] = 0;
					_par_v0[i*MAX_EFFECT_PARTICAL + j] *= 0.8;
					_par_y0[i*MAX_EFFECT_PARTICAL + j] = _par_y[i*MAX_EFFECT_PARTICAL + j];
					_par_x0[i*MAX_EFFECT_PARTICAL + j] = _par_x[i*MAX_EFFECT_PARTICAL + j];

				}

				if (_par_count[i*MAX_EFFECT_PARTICAL + j] > 1)
				{
					_par_state[i*MAX_EFFECT_PARTICAL + j] = EFFECT_STATE_ATTACK_NONE;
				}



				if (CompleteStatePartical(i, _var1[i], EFFECT_STATE_ATTACK_NONE))
				{
					_eff_state[i] = EFFECT_STATE_ATTACK_NONE;
				}

			}
			break;
			}
		}
		break;
	}
}

void CCharater::AddParticalWagon(float x, float y, int sprite, int beginFarme, int numFrame, int numPar)
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		if (_eff_state[i] == EFFECT_STATE_ATTACK_NONE)
		{
			_eff_type[i] = EFFECT_TYPE_PARTICAL_WAGON;
			_eff_state[i] = 1;
			_eff_x[i] = x;
			_eff_y[i] = y;
			_var1[i] = numPar;

			_var2[i] = sprite;


			for (int j = 0; j < _var1[i]; j++)
			{
				_par_x0[i*MAX_EFFECT_PARTICAL + j] = _eff_x[i] + CMath::RANDOM(-20, 20);
				_par_y0[i*MAX_EFFECT_PARTICAL + j] = _eff_y[i] + CMath::RANDOM(-20, 20);
				_par_target_y0[i*MAX_EFFECT_PARTICAL + j] = _eff_y[i] + 50;

				_par_v0[i*MAX_EFFECT_PARTICAL + j] = 90 + CMath::RANDOM(-20, 0);
				_par_g0[i*MAX_EFFECT_PARTICAL + j] = 20 + CMath::RANDOM(-5, 0);
				_par_t0[i*MAX_EFFECT_PARTICAL + j] = 0;

				_par_angle[i*MAX_EFFECT_PARTICAL + j] = CMath::RANDOM(70, 90);

				_par_state[i*MAX_EFFECT_PARTICAL + j] = 1;
				_par_frame[i*MAX_EFFECT_PARTICAL + j] = beginFarme + CMath::RANDOM(0, numFrame - 1);

				_par_opacity[i*MAX_EFFECT_PARTICAL + j] = 200;
				_par_count[i*MAX_EFFECT_PARTICAL + j] = 0;

				_par_v1[i*MAX_EFFECT_PARTICAL + j] = CMath::RANDOM(0, 1) ? _LEFT : _RIGHT;

			}

			break;
		}
	}
}

void CCharater::RenderEffectTarget()
{
	
	
}
void CCharater::RenderParticalWagon(int i)
{
	for (int j = 0; j < _var1[i]; j++)
	{
		if (_par_state[i*MAX_EFFECT_PARTICAL + j] != EFFECT_STATE_ATTACK_NONE)
		{
			G()->SetOpacity(_par_opacity[i*MAX_EFFECT_PARTICAL + j]);
			GET_SPRITE(_var2[i])->DrawFrame(G(), _par_frame[i*MAX_EFFECT_PARTICAL + j], _par_x[i*MAX_EFFECT_PARTICAL + j], _par_y[i*MAX_EFFECT_PARTICAL + j]);
			G()->SetOpacity(100);
		}
	}
}
void CCharater::UpdateParticalWagon(int i)
{
	switch (_eff_state[i])
	{
	case 1:
		for (int j = 0; j < _var1[i]; j++)
		{
			switch (_par_state[i*MAX_EFFECT_PARTICAL + j])
			{
			case 1:
			{
				_par_t0[i*MAX_EFFECT_PARTICAL + j] += 7;// DT();

				float ft = _par_t0[i*MAX_EFFECT_PARTICAL + j] / 40.0f;
				float x = _par_v0[i*MAX_EFFECT_PARTICAL + j] * CMath::COS(_par_angle[i*MAX_EFFECT_PARTICAL + j])*ft;
				float y = _par_v0[i*MAX_EFFECT_PARTICAL + j] * CMath::SIN(_par_angle[i*MAX_EFFECT_PARTICAL + j])*ft - _par_g0[i*MAX_EFFECT_PARTICAL + j] * ft*ft*0.5f;


				if (_par_v1[i*MAX_EFFECT_PARTICAL + j] == _RIGHT)
				{
					_par_x[i*MAX_EFFECT_PARTICAL + j] = _par_x0[i*MAX_EFFECT_PARTICAL + j] - x;
				}
				else
				{
					_par_x[i*MAX_EFFECT_PARTICAL + j] = _par_x0[i*MAX_EFFECT_PARTICAL + j] + x;
				}
				_par_y[i*MAX_EFFECT_PARTICAL + j] = _par_y0[i*MAX_EFFECT_PARTICAL + j] - y;


				_par_opacity[i*MAX_EFFECT_PARTICAL + j] -= 1;
				if (_par_opacity[i*MAX_EFFECT_PARTICAL + j] <= 0)
				{

				}
				if (_par_y[i*MAX_EFFECT_PARTICAL + j] > _par_target_y0[i*MAX_EFFECT_PARTICAL + j])
				{
					_par_count[i*MAX_EFFECT_PARTICAL + j]++;
					_par_g0[i*MAX_EFFECT_PARTICAL + j] = 25;
					_par_t0[i*MAX_EFFECT_PARTICAL + j] = 0;
					_par_v0[i*MAX_EFFECT_PARTICAL + j] *= 0.8;
					_par_y0[i*MAX_EFFECT_PARTICAL + j] = _par_y[i*MAX_EFFECT_PARTICAL + j];
					_par_x0[i*MAX_EFFECT_PARTICAL + j] = _par_x[i*MAX_EFFECT_PARTICAL + j];

				}

				if (_par_count[i*MAX_EFFECT_PARTICAL + j] > 1)
				{
					_par_state[i*MAX_EFFECT_PARTICAL + j] = EFFECT_STATE_ATTACK_NONE;
				}



				if (CompleteStatePartical(i, _var1[i], EFFECT_STATE_ATTACK_NONE))
				{
					_eff_state[i] = EFFECT_STATE_ATTACK_NONE;
				}

			}
			break;
			}
		}
		break;
	}
}

void CCharater::AddEffectGun2(float x, float y, float tarx, float tary, float speed)
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		if (_eff_state[i] == EFFECT_STATE_ATTACK_NONE)
		{
			GAME()->PlaySFX(SFX_GUNSHOT);
			_eff_type[i] = EFFECT_TYPE_ATTACK_GUN_2;
			_eff_state[i] = 1;

			_eff_anim[i].SetAnimSprite(GET_SPRITE(SPRITEID_EFFECT_BULLET), SPRITEID_EFFECT_BULLET);
			_eff_anim[i].SetAnim(0, true);

			_eff_anim2[i].SetAnimSprite(GET_SPRITE(SPRITEID_EFFECT_BULLET), SPRITEID_EFFECT_BULLET);
			_eff_anim2[i].SetAnim(2, false);
			_eff_anim2[i].SetSubTime(1);

			_varF1[i] = speed;

			_varF2[i] = tarx;
			_varF3[i] = tary;

			if (_typePower != POWER_MULE && _typePower != POWER_MEDAL)
			{
				_eff_x[i] = x + _angleGunX;
				_eff_y[i] = y + _angleGunY;
			}
			else
			{
				_eff_x[i] = x + _angleGunX;
				_eff_y[i] = y + _angleGunY;
			}

			_eff_anim2[i].SetPos(_eff_x[i], _eff_y[i]);
			_varF4[i] = CMath::ANGLE(_eff_x[i], _eff_y[i], _varF2[i], _varF3[i]) - 180;
			break;
		}
	}
}

void CCharater::AddEffectGun2(float x, float y, CArmy *army, float speed)
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		if (_eff_state[i] == EFFECT_STATE_ATTACK_NONE)
		{
			GAME()->PlaySFX(SFX_GUNSHOT);
			_eff_type[i] = EFFECT_TYPE_ATTACK_GUN_2;
			_eff_state[i] = 1;

			_eff_anim[i].SetAnimSprite(GET_SPRITE(SPRITEID_EFFECT_BULLET), SPRITEID_EFFECT_BULLET);
			_eff_anim[i].SetAnim(0, true);

			_eff_anim2[i].SetAnimSprite(GET_SPRITE(SPRITEID_EFFECT_BULLET), SPRITEID_EFFECT_BULLET);
			_eff_anim2[i].SetAnim(2, false);
			_eff_anim2[i].SetSubTime(1);

			_varF1[i] = speed;
			_eff_army[i] = army;
			
			_varF2[i] = _eff_army[i]->PosAttackX();
			_varF3[i] = _eff_army[i]->PosAttackY();

			if (_typePower != POWER_MULE && _typePower != POWER_MEDAL)
			{
				_eff_x[i] = x + _angleGunX;
				_eff_y[i] = y + _angleGunY;
			}
			else
			{
				_eff_x[i] = x + _angleGunX;
				_eff_y[i] = y + _angleGunY - 100;
			}

			_eff_anim2[i].SetPos(_eff_x[i], _eff_y[i]);
			_varF4[i] = CMath::ANGLE(_eff_x[i], _eff_y[i], _varF2[i], _varF3[i]);
			break;
		}
	}
}
void CCharater::UpdateEffectGun2(int i)
{
	if (!CHECK_POINT_IN_RECT(_eff_x[i], _eff_y[i], 0, 0, k_SCREEN_WIDTH, k_SCREEN_HEIGHT))
	{
		_eff_state[i] = EFFECT_STATE_ATTACK_NONE;
	}
	switch (_eff_state[i])
	{
	case 1:
		_eff_y[i] += _varF1[i] * CMath::SIN(_varF4[i]);
		_eff_x[i] += _varF1[i] * CMath::COS(_varF4[i]);

		for (int j = 0; j < BATTLE()._numArmy; j++)
		{
			if (BATTLE()._army[j] != NULL && BATTLE()._army[j]->GetType() != TYPE_ENEMY_BOSS_2)
			{
				if (BATTLE()._army[j]->GetState() != CHARATER_STATE_DIE)
				{
					if (CHECK_POINT_IN_RECT(_eff_x[i], _eff_y[i], BATTLE()._army[j]->CollisionX(), BATTLE()._army[j]->CollisionY(), BATTLE()._army[j]->CollisionW(), BATTLE()._army[j]->CollisionH()))
					{
						_eff_state[i] = 2;
						BATTLE()._army[j]->BeDame(_dame);
						BATTLE()._army[j]->SetAngleBullet(_varF4[i]);
						_eff_state[i] = EFFECT_STATE_ATTACK_NONE;

						//if (BATTLE()._army[j]->GetType() == TYPE_ENEMY_BOSS_1)
						{
							AddEffectBulet(_eff_x[i], _eff_y[i], _varF4[i] + 180);
						}
					}
				}
			}
		}

		break;

	case 2:
		for (int j = 0; j < BATTLE()._numArmy; j++)
		{
			if (BATTLE()._army[j] != NULL)
			{
				//if (BATTLE()._army[j]->GetState() != ARMY_STATE_DIE)
				{
					if (BATTLE()._army[j]->GetState() == CHARATER_STATE_DIE)
					{
						if (BATTLE()._army[j]->CompleteEffect() && BATTLE()._army[j]->GetScale() == 0)
						{
							//BATTLE()._army[j]->SetState(CHARATER_STATE_NULL);
							if (_eff_army[i] == BATTLE()._army[j])
							{
								_eff_army[i] = NULL;
							}
							BATTLE()._army[j] = NULL;
							_eff_state[i] = EFFECT_STATE_ATTACK_NONE;
						}
					}
				}
			}
		}
		break;
	case 3:
		break;
	}
}
void CCharater::RenderEffectGun2(int i)
{
	_eff_anim[i].SetPos(_eff_x[i], _eff_y[i]);
	G()->SetRotate(_varF4[i], _eff_anim2[i]._x, _eff_anim2[i]._y);

	//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);

	_eff_anim2[i].DrawAnimObject(G());
	_eff_anim2[i].UpdateAnimObject();
	//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
	G()->ClearRotate();

	_eff_anim[i].DrawAnimObject(G());
	_eff_anim[i].UpdateAnimObject();
}


#define RADS 0.037453293
#define K 80


void CCharater::AddEffectHatFall(float x, float y, int typeHat, float speed)
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		if (_eff_state[i] == EFFECT_STATE_ATTACK_NONE)
		{
			_eff_type[i] = EFFECT_TYPE_HAT_FALL;
			_eff_state[i] = 1;


			_eff_x[i] = x;
			_eff_y[i] = y;
			_varF4[i] = y;

			_varF1[i] = speed;

			_var2[i] = 1;
			_varF2[i] = 0;

			_varF3[i] = 0;
			_varF5[i] = 0;
			break;

		}
	}
}
void CCharater::UpdateEffectHatFall(int i)
{
	switch (_eff_state[i])
	{
	case 1:
		_eff_y[i] -= _varF1[i] * 2;
		if (_eff_y[i] < _varF4[i] - 50)
		{
			_eff_state[i] = 2;
		}
		break;

	case 2:

		_varF3[i] = _varF5[i] * sin(_eff_y[i] * RADS);
		_varF5[i] ++;
		_eff_y[i] += _varF1[i] * 1.5;
		if (_eff_y[i] > CHARATER().PosY())
		{
			_eff_y[i] = CHARATER().PosY();
			_eff_state[i] = 3;
			_varF5[i] = 0;
		}

		break;
	case 3:
		_varF5[i] ++;
		if (_varF5[i] >= 90)
		{
			_eff_state[i] = 4;
			MAIN_GAME()->SetState(k_GS_RESULT);
		}
		break;
	}

	if (_eff_state[i] != EFFECT_STATE_ATTACK_NONE && _eff_state[i] < 3)
	{
		_eff_x[i] -= BATTLE_MAP()._speed_scroll_map;
		if (_var2[i] == 1)
		{
			_varF2[i] += 0.6;
			if (_varF2[i] > 20)
			{
				_var2[i] = -1;
			}
		}

		if (_var2[i] == -1)
		{
			_varF2[i] -= 0.6;
			if (_varF2[i] < -20)
			{
				_var2[i] = 1;
			}
		}
	}

}
void CCharater::RenderEffectHatFall(int i)
{
	if (_eff_state[i] != EFFECT_STATE_ATTACK_NONE)
	{
		G()->SetRotate(_varF2[i], _eff_x[i] + _varF3[i], _eff_y[i]);
		GET_SPRITE(_spriteID)->DrawFrame(G(), 118, _eff_x[i] + _varF3[i], _eff_y[i]);
		G()->ClearRotate();
	}
}

void CCharater::AddEffectBulet(float x, float y, float angle)
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		if (_eff_state[i] == EFFECT_STATE_ATTACK_NONE)
		{
			_eff_type[i] = EFFECT_TYPE_HAT_BULET;
			_eff_state[i] = 1;

			_eff_x[i] = x;
			_eff_y[i] = y;

			_varF1[i] = angle;
			_eff_anim[i].SetAnimSprite(GET_SPRITE(SPRITEID_EFFECT_BULLET), SPRITEID_EFFECT_BULLET);
			_eff_anim[i].SetAnim(2, false);
			_eff_anim[i].SetSubTime(1);

			break;

		}
	}
}
void CCharater::UpdateEffectBulet(int i)
{
	switch (_eff_state[i])
	{
	case 1:
		if (_eff_anim[i].IsCurrentAnimStop())
		{
			_eff_state[i] = EFFECT_STATE_ATTACK_NONE;
		}
		break;
	}

}
void CCharater::RenderEffectBulet(int i)
{
	G()->SetRotate(_varF1[i], _eff_anim[i]._x, _eff_anim[i]._y);
	//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);
	_eff_anim[i].DrawAnimObject(G());
	_eff_anim[i].UpdateAnimObject();
	//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
	G()->ClearRotate();
}

void CCharater::UpdateEffect()
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		if (_eff_state[i] != EFFECT_STATE_ATTACK_NONE)
		{
			switch (_eff_type[i])
			{
			case EFFECT_TYPE_ATTACK_GUN:
				UpdateEffectGun(i);
				break;
			case EFFECT_TYPE_PARTICAL_DIE:
				UpdateParticalDie(i);
				break;
			case EFFECT_TYPE_ATTACK_GUN_2:
				UpdateEffectGun2(i);
				break;
			case EFFECT_TYPE_HAT_FALL:
				UpdateEffectHatFall(i);
				break;
			case EFFECT_TYPE_HAT_BULET:
				UpdateEffectBulet(i);
				break;
			case EFFECT_TYPE_PARTICAL_WAGON:
				UpdateParticalWagon(i);
				break;
			}
		}
	}
}
void CCharater::RenderEffect()
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		if (_eff_state[i] != EFFECT_STATE_ATTACK_NONE)
		{
			switch (_eff_type[i])
			{
			case EFFECT_TYPE_ATTACK_GUN:
				RenderEffectGun(i);
				break;
			case EFFECT_TYPE_PARTICAL_DIE:
				RenderParticalDie(i);
				break;
			case EFFECT_TYPE_ATTACK_GUN_2:
				RenderEffectGun2(i);
				break;
			case EFFECT_TYPE_HAT_FALL:
				RenderEffectHatFall(i);
				break;
			case EFFECT_TYPE_HAT_BULET:
				RenderEffectBulet(i);
				break;
			case EFFECT_TYPE_PARTICAL_WAGON:
				RenderParticalWagon(i);
				break;
			}
		}
	}
}

#pragma endregion 