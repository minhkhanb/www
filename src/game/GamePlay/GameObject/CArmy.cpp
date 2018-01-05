#include "pch.h"
#include "../../GameCore/CGame.h"

void CArmy::SetState(BYTE state)
{	
 	switch (state)
	{
	case CHARATER_STATE_DIE:
		_isTargetFocus = false;
		if (_type < TYPE_ENEMY_BOSS_1)
		{			
			int numCoin = GET_DES().getEnemyDes(_type, ENEMY_DES_COIN_VALUE);
#if DISABLE_COIN
			numCoin = NUM_COIN;
#endif // DIS
			//PDEBUG("\n _dieBy:%d", _dieBy);
			BATTLE().AddCoin(numCoin, _posX, _posY - 40, 0, 75 + CMath::RANDOM(-5,3), COIN_STATE_INIT_FALL, _posX + CMath::RANDOM(30,70), _posY + CMath::RANDOM(-30,0));
			AddParticalDie(_posX, _posY - 30, 30, _angleMove);
			//return;
		}		
        break;
	case CHARATER_STATE_BE_ATTACK:
        _timeDeplay = 20;
		_color = 0x770000;
		_stateColor = 0;
		break;
	case CHARATER_STATE_BOSS_DIE:
		_isTargetFocus = false;
		_animCharater.SetAnim(7, true);
		//_animHorse1.StopAnim();		
		_animArmy.StopAnim();
		_countEx = 0;
		AddParticalWagon(_posX, _posY - 50, SPRITEID_EFFECT_WAGON_PARTICAL, 0, 6, 10);
		break;

	case CHARATER_STATE_JUMP:	

		_g = 10 ;
		_y0 = _posY;
		_v0 = 45 +  CMath::RANDOM(50,100)*0.1;
		_t0 = 0;
		_angle = 90 +  CMath::RANDOM(-7,7);
	
		break;
	case CHARATER_STATE_MOVE_TOP:
		_posX = _targetX;
		_directionMove = CHARATER_MOVE_TOP;
		_animCharater.SetAnim(0 , true);
		_scaleX = 1;
		break;
	case CHARATER_STATE_MOVE_RIGHT:
		_directionMove = CHARATER_MOVE_RIGHT;
		_animCharater.SetAnim(2 , true);
		_scaleX = 1;

		break;
	}
	_state = state;
}

void CArmy::Init(float x, float y, BYTE type, BYTE initBy)
{
	_initBy = initBy;
	_type =  type;
	_posX = x;
	_posY = y;
	_renderTop = false;
	InitEffect();

	if (_type == TYPE_ENEMY_BOSS_1)
	{
		InitBoss_1(_type);
	}
	else if (_type == TYPE_ENEMY_BOSS_2)
	{
		InitBoss_2(_type);
	}
	else if (_type == TYPE_ENEMY_BOSS_3)
	{
		InitBoss_3(_type);
	}
	else
	{
        //_type = 4;
		
		_targetX = CHARATER().PosX();
		_targetY = CHARATER().PosY();

		_speedStart = MAIN_GAME()->_design_util.getEnemyDes(_type, ENEMY_DES_MOVEMENT_SPEED);

		_animCharater.SetAnimSprite(GET_SPRITE(SPRITEID_ENEMY_01 + _type), SPRITEID_ENEMY_01 + _type);
		_animCharater.SetAnim(1, true);

		_state = CHARATER_STATE_MOVE;

		_hp = MAIN_GAME()->_design_util.getEnemyDes(_type, ENEMY_DES_HP);

		_collisionW = GET_SPRITE(SPRITEID_ENEMY_01 + _type)->GetFModuleWidth(1, 0);
		_collisionH = GET_SPRITE(SPRITEID_ENEMY_01 + _type)->GetFModuleHeight(1, 0);

		_colMoveW = GET_SPRITE(SPRITEID_ENEMY_01 + _type)->GetFModuleWidth(0, 0);
		_colMoveH = GET_SPRITE(SPRITEID_ENEMY_01 + _type)->GetFModuleHeight(0, 0);
		
		_isMoveStraight = false;

		_timeGun = 60;
		
		_speedGun = GET_DES().getEnemyDes(_type, ENEMY_DES_BULLET_SPEED);
		
		_distanceRunPower = CMath::RANDOM(200, 400);
	}
	_isTargetFocus = false;
}

void CArmy::BeDame(int dame)
{
 	if (_type < TYPE_ENEMY_BOSS_1)
	{
		if (_state != CHARATER_STATE_MOVE_TOP )
		{
			SetState(CHARATER_STATE_BE_ATTACK);
		}			
	}
	else
	{
		if (_type != TYPE_ENEMY_BOSS_3)
		{
			_timeDeplay = 0;
			_counttttt = 30;
		}
	
	
	}
	
 	_hp -= dame;
	if (_hp <= 0)
	{		
		_hp = 0;
		if (_type == TYPE_ENEMY_BOSS_1)
		{
			if (USER()._stateBoss == BOSS_STATE_ACTIVE_3)
			{				
				SetState(CHARATER_STATE_BOSS_DIE);
				USER()._stateBoss = BOSS_STATE_NONE;
				BATTLE()._lvlBosse = 1;
				BATTLE()._isCalculateDistance = true;				
			}
		}
		else if (_type == TYPE_ENEMY_BOSS_2)
		{
			GAME()->PlaySFX(SFX_TRAIN_START_STOP);
			_timeDeplay = 30;
			BATTLE()._isTargetTrain = false;
			_state = CHARATER_STATE_MOVE_LEFT;

             GAME()->PlaySFX(SFX_MISSION_FAILED);
		}
		else if (_type == TYPE_ENEMY_BOSS_3 && _state != CHARATER_STATE_BOSS_DIE)
		{	
 			_timeCoin = 0;
			int min = GET_DES().getCheckPointDes(BATTLE()._lvlCheckPiont, CHECK_POINT_DES_MIN_COIN_REWARD);
			int max = GET_DES().getCheckPointDes(BATTLE()._lvlCheckPiont, CHECK_POINT_DES_MAX_COIN_REWARD);

			_numCoin = CMath::RANDOM(min, max);
			printf("\n _numCoin = %d", _numCoin);
			if (MAIN_GAME()->_playWitchEndlest)
			{
				_distaneBoss = BATTLE()._distanceRun + 500;
				BATTLE()._lvlBosse = CMath::RANDOM(0, 1) ? 0 : 2;

			}
			else
			{
				BATTLE()._lvlBosse = 1;
			}
			_state = CHARATER_STATE_BOSS_DIE;
			USER()._stateBoss = BOSS_STATE_NONE;

			GAME()->StopCurrentMusic();
			GAME()->LoadMusic(BGM_INGAME);
			GAME()->PlayCurrentMusic(BGM_INGAME);

			GAME()->PlaySFX(SFX_BOSS_DEFEATED);
			BATTLE()._isCalculateDistance = true;

			_animCharater.SetAnim(5, false);
		}
		else
		{
			if (_state != CHARATER_STATE_BOSS_DIE )
			{
				printf("\n die by GUN");
				SetState(CHARATER_STATE_DIE);
				SetDieBy(ENEMY_DIE_BY_GUN);

				//set Achivement
				ACHIEVEMENT()._numEnemyDie++;
				ACHIEVEMENT()._totalEnemyDie++;
				if (_initBy == ENEMY_INIT_BY_BARREL)
				{
					ACHIEVEMENT()._numEnemyJumpBarrelDie++;
					ACHIEVEMENT()._totalEnemyJumpBarrelDie++;
					ACHIEVEMENT()._numDailyEnemyJumpBarrel++;
				}
			}			
		}
	}
	else
	{
		//printf("\n hp = %d",_hp);
	//	if (BATTLE()._isTargetTrain)
		{
			if (_state == CHARATER_STATE_MOVE_TOP)
			{
				//printf("\n hp = %d", _hp);
				_posY = MIN_MOVE_Y + CMath::RANDOM(10,50) + BATTLE()._offsetMoveY;
				_posX += CMath::RANDOM(-40, 40);
				SetState(CHARATER_STATE_MOVE);
			}
		}
	}
	
	if (_type == TYPE_ENEMY_BOSS_1)
	{		
		if (USER()._stateBoss == BOSS_STATE_ACTIVE_1)
		{
			if (_hp <= 0)
			{
				_timeCoin = 0;
				int min = GET_DES().getCheckPointDes(BATTLE()._lvlCheckPiont, CHECK_POINT_DES_MIN_COIN_REWARD);
				int max = GET_DES().getCheckPointDes(BATTLE()._lvlCheckPiont, CHECK_POINT_DES_MAX_COIN_REWARD);

				_numCoin = CMath::RANDOM(min, max);
				printf("\n _numCoin = %d", _numCoin);
				if (MAIN_GAME()->_playWitchEndlest)
				{
					_distaneBoss = BATTLE()._distanceRun + 500;
					BATTLE()._lvlBosse = CMath::RANDOM(0,1)?0:2;
					
				}
				else
				{
					BATTLE()._lvlBosse = 1;
				}
				
				SetState(CHARATER_STATE_BOSS_DIE);
				USER()._stateBoss = BOSS_STATE_NONE;

				GAME()->StopCurrentMusic();
				GAME()->LoadMusic(BGM_INGAME);
				GAME()->PlayCurrentMusic(BGM_INGAME);

				GAME()->PlaySFX(SFX_BOSS_DEFEATED);
				BATTLE()._isCalculateDistance = true;
			}
		}
	}
	else 	if (_type == TYPE_ENEMY_BOSS_2)
	{

	}
}
void CArmy::AddCoinBossDie()
{
	for (int i = 0; i < _numCoin; i++)
	{
		if (_timeCoin == i*2)
		{
			if (_state == CHARATER_STATE_BOSS_DIE)
			{
				BATTLE().AddCoin(1, _posX + CMath::RANDOM(-70, 70), _posY - 70 + CMath::RANDOM(-50, 50), 0, 75 + CMath::RANDOM(-5, 3), COIN_STATE_INIT_FALL, CMath::RANDOM(100, k_SCREEN_WIDTH - 100), CMath::RANDOM(MIN_MOVE_Y, MAX_MOVE_Y));
			}
			else
			{
#if DISABLE_COIN
				int numCoin = 0;
#else
				int numCoin = 1;
#endif // DIS

				BATTLE().AddCoin(numCoin, _posX + CMath::RANDOM(-70, 70), _posY - 200 + CMath::RANDOM(-50, 50), 0, 75 + CMath::RANDOM(-5, 3), COIN_STATE_INIT_FALL, CMath::RANDOM(100, k_SCREEN_WIDTH - 100), CMath::RANDOM(MIN_MOVE_Y, MAX_MOVE_Y));
			}

		}
	}

	if (CHARATER().GetLive() < 5 + BATTLE()._numLife)
	{
		if (_timeCoin == _numCoin * 2 / 2)
		{
			BATTLE().AddCoin(1, _posX + CMath::RANDOM(-70, 70), _posY - 200 + CMath::RANDOM(-50, 50), 1, 75 + CMath::RANDOM(-5, 3), COIN_STATE_INIT_FALL, CMath::RANDOM(k_SCREEN_WIDTH / 2, k_SCREEN_WIDTH - 100), CMath::RANDOM(MIN_MOVE_Y, MAX_MOVE_Y));
		}
	}

	_timeCoin++;
}
void CArmy::InitBoss_1(BYTE type)
{
	_fastBome = false;
	_opactiyBoss = 100;
	if (BATTLE()._lvlCheckPiont == 0)
	{
		_spriteBoss = SPRITEID_BOSS_01;
	}
	else
	{
		_spriteBoss = SPRITEID_BOSS_1_A;
	}
	

	_collisionW = GET_SPRITE(_spriteBoss)->GetFModuleWidth(0, 0);
	_collisionH = GET_SPRITE(_spriteBoss)->GetFModuleHeight(0, 0);

	_colMoveW = GET_SPRITE(_spriteBoss)->GetFModuleWidth(0, 0);
	_colMoveH = GET_SPRITE(_spriteBoss)->GetFModuleHeight(0, 0);

	_maxHP = GET_DES().getCheckPointDes(BATTLE()._lvlCheckPiont, CHECK_POINT_DES_BOSS_HP);
	_hpEnd = 0;
	if (USER()._stateBoss == BOSS_STATE_ACTIVE_1)
	{		
		_hp = _maxHP;
	}
	else if (USER()._stateBoss == BOSS_STATE_ACTIVE_2)
	{		
		_hp = _maxHP;
	}
	else if (USER()._stateBoss == BOSS_STATE_ACTIVE_3)
	{
		_hp = _maxHP;
	}
	
	_lastHPRocket = _hp;
	_lastHPBome = _hp;

	int percent = _hp / (float)_maxHP * 100;
	if (percent >= 70 )
	{
		_typeDiff = 0;
	}
	else if (percent >= 30)
	{
		_typeDiff = 1;
	}
	else
	{
		_typeDiff = 2;
	}

	SetValueBose();

	_speedStart = 5;

	_animCharater.SetAnimSprite(GET_SPRITE(_spriteBoss), _spriteBoss);
	_animCharater.SetAnim(2, true);

	_animHorse1.SetAnimSprite(GET_SPRITE(_spriteBoss), _spriteBoss);
	_animHorse1.SetAnim(3, true);


	_animArmy.SetAnimSprite(GET_SPRITE(_spriteBoss), _spriteBoss);
	_animArmy.SetAnim(4, true);


	_scaleX = 1;

	_state = CHARATER_STATE_INIT_BOSS;
	_color = 0;
	_timeDeplay = 50;
	_addgun = false;
	_isRocketGun = false;
	_numRocket = 0;

	for (int i = 0; i < 6; i++)
	{
		_animFire[i].SetAnimSprite(GET_SPRITE(_spriteBoss), _spriteBoss);
		_animFire[i].SetAnim(6, true);
	}
	
    _timeGun = 0;
	_addgun1 = false;
	_addRocket = false;

	InitEffect();
}
void CArmy::SetValueBose()
{
	_maxBome = GET_DES().getAIBossDes(_typeDiff, AI_BOSS_DES_NUM_BOME);

	_distaneBoss = GET_DES().getAIBossDes(_typeDiff, AI_BOSS_DES_DISTANCE);
	_timeDeplayBome = GET_DES().getAIBossDes(_typeDiff, AI_BOSS_DES_TIME_DEPLAY_BOME)*60;

	if (USER()._stateBoss == BOSS_STATE_ACTIVE_1)
	{
		int hp = _maxHP;
		_perHPRocket = hp*GET_DES().getAIBossDes(_typeDiff, AI_BOSS_DES_PERCENT_ROCKET) / 100.0f;
		_perHPBome = hp*GET_DES().getAIBossDes(_typeDiff, AI_BOSS_DES_PERCENT_BOME) / 100.0f;
	}
	else if (USER()._stateBoss == BOSS_STATE_ACTIVE_2)
	{
		_perHPRocket = _lastHPRocket*GET_DES().getAIBossDes(_typeDiff, AI_BOSS_DES_PERCENT_ROCKET) / 100.0f;
		_perHPBome = _lastHPBome*GET_DES().getAIBossDes(_typeDiff, AI_BOSS_DES_PERCENT_BOME) / 100.0f;
	}
	else if (USER()._stateBoss == BOSS_STATE_ACTIVE_2)
	{
		_perHPRocket = _lastHPRocket*GET_DES().getAIBossDes(_typeDiff, AI_BOSS_DES_PERCENT_ROCKET) / 100.0f;
		_perHPBome = _lastHPBome*GET_DES().getAIBossDes(_typeDiff, AI_BOSS_DES_PERCENT_BOME) / 100.0f;
	}


	_numBome = 0;
}
void CArmy::RenderBoss_1()
{
    float zoom = 1.0f;
    float offsetY = 0;
    if(CGame::_this->_device._version == SupportVersion::RESVER_960_640)
    {
        zoom = 1.20f;
        offsetY = -0.15*640/2;
    }
    if(CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
    {
        zoom = IPAD_ZOOM;
        offsetY = IPAD_OFFSET;
    }
	G()->SetOpacity(_opactiyBoss);
	_animCharater.DrawAnimObject(G());
	_animCharater.UpdateAnimObject();
	G()->SetOpacity(100);

	for (int i = 0; i < 4; i++)
	{

		//printf("\n x = %f, y = %f", _animEx[i]._x, _animEx[i]._y);
		_animEx[i].DrawAnimObject(G());
		_animEx[i].UpdateAnimObject();
	}

	_animHorse1.DrawAnimObject(G());
	_animHorse1.UpdateAnimObject();

	if (_state != CHARATER_STATE_BOSS_DIE)
	{
		
        

		if (CHARATER().PosX() > _animArmy._x && (_addgun || _addgun1))
		{
			G()->SetScale(-1, 1, _animArmy._x, _animArmy._y);
		}
		_animArmy.DrawAnimObject(G());
		G()->ClearScale();
		_animArmy.UpdateAnimObject();

		if (_counttttt > 0)
		{
			_counttttt--;
			if (_counttttt % 30 > 15)
			{
				G()->SetColorMask(0xFF4500);
				//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);
				_animCharater.DrawAnimObject(G());
				_animHorse1.DrawAnimObject(G());
				if (CHARATER().PosX() > _animArmy._x && (_addgun || _addgun1))
				{
					G()->SetScale(-1, 1, _animArmy._x, _animArmy._y);
				}
				_animArmy.DrawAnimObject(G());
				G()->ClearScale();
			}
		}

		G()->ClearColorMask();
		//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);



		GET_SPRITE(_spriteBoss)->DrawFrame(G(), 1, 0, 0+offsetY);

		int clipX = GET_SPRITE(_spriteBoss)->GetFModuleX(2, 0);
		int clipY = GET_SPRITE(_spriteBoss)->GetFModuleY(2, 0)+offsetY;
		int clipH = GET_SPRITE(_spriteBoss)->GetFModuleHeight(2, 0);

		int clipW = (_hp / (float)_maxHP)*GET_SPRITE(_spriteBoss)->GetFModuleWidth(2, 0);

		G()->SetClip(clipX, clipY, clipW, clipH);
		GET_SPRITE(_spriteBoss)->DrawFrame(G(), 2, 0, 0+offsetY);
		G()->ClearClip();

		for (int i = 0; i < _numFire; i++)
		{
			_animFire[i].DrawAnimObject(G());
			_animFire[i].UpdateAnimObject();
		}


	}

	RenderEffect();

}
void CArmy::UpdateBoss_1()
{
	_collisionX = GET_SPRITE(_spriteBoss)->GetFModuleX(0, 0) + _posX;
	_collisionY = GET_SPRITE(_spriteBoss)->GetFModuleY(0, 0) + _posY;

	_colMoveX = GET_SPRITE(_spriteBoss)->GetFModuleX(0, 0) + _posX;
	_colMoveY = GET_SPRITE(_spriteBoss)->GetFModuleY(0, 0) + _posY;

	_attackX = _posX + GET_SPRITE(_spriteBoss)->GetFModuleX(0, 0) + 50;
	_attackY = _posY + GET_SPRITE(_spriteBoss)->GetFModuleCenterY(0, 0);

	_timeDeplay++;	

	UpdateEffect();
	//printf("\n _state = %d", _state);
	switch (_state)
	{
	case CHARATER_STATE_INIT_BOSS:
		_posX -= BATTLE_MAP()._speed_scroll_map*2;
		if (_posX <= k_SCREEN_WIDTH/5*3)
		{
			_stateMoveBoss = 1;
			//_state = CHARATER_STATE_MOVE_RIGHT;
			_animCharater.SetAnim(2, true);
			_animHorse1.SetAnim(3, true);
			_state = CHARATER_STATE_BOSS_NORMAL;
			
		}
		break;

	case CHARATER_STATE_STANDING:
		_timeDeplay++;
		if (_timeDeplay >= CMath::RANDOM(60, 90))
		{
			int ran = CMath::RANDOM(1, 4);			
			if (ran == 1 )
			{	
				_state = CHARATER_STATE_MOVE_TOP;
				_timeDeplay = 0;

				_tagetMove = _posY - CMath::RANDOM(0, _posY - MIN_BOSS_Y);
				if (_tagetMove < MIN_BOSS_Y)
				{
					_tagetMove = MIN_BOSS_Y;
				}
			}
			else if (ran == 2 )
			{
				_state = CHARATER_STATE_MOVE_DOWN;
				_timeDeplay = 0;
				_tagetMove = _posY + CMath::RANDOM(0, MAX_BOSS_Y - _posY);
				if (_tagetMove > MAX_BOSS_Y)
				{
					_tagetMove = MAX_BOSS_Y;
				}
			}			
			else if (ran == 4)
			{
				_state = CHARATER_STATE_MOVE_RIGHT;
				_timeDeplay = 0;
			}
			else if (ran == 3)
			{
				if (_numRocket == 0)
				{
					bool res = true;
					for (int i = 0; i < BATTLE()._numRocket; i++)
					{
						if (BATTLE()._rocket[i] != NULL)
						{
							if (BATTLE()._rocket[i]->_state == ROCKET_STATE_INIT_GUN)
							{
								res = false;
								break;
							}
						}
					}

					if (res)
					{
						_state = CHARATER_STATE_MOVE_LEFT;
					}
				}			
				
			}
			
			if (_posX >= k_SCREEN_WIDTH + 100 && _numRocket == 0 && !_isRocketGun)
			{
				_isRocketGun = true;
				_numRocket = CMath::RANDOM(3,5);
				_direcGun = CMath::RANDOM(0, 1) ? _TOP : _BOTTOM;
				_timeRocketGun = 0;
				if (_direcGun == _TOP)
				{
					_nextRocketGun = MIN_MOVE_Y;
				}
				else
				{
					_nextRocketGun = MAX_MOVE_Y;
				}
				_distanceRocketGun = (MAX_MOVE_Y - MIN_MOVE_Y) / (float)(_numRocket-1);
			}
		}
		break;
		
	
	case CHARATER_STATE_MOVE_RIGHT:
		_posX += _speedMove;
		if (_posX >= k_SCREEN_WIDTH + 100)
		{
			
			_posX = k_SCREEN_WIDTH +100;
			_state = CHARATER_STATE_STANDING;
			_timeDeplay = 0;
		}
		break;

	
	case CHARATER_STATE_MOVE_LEFT:		
		_posX -= _speedMove*3;
		if (_posX <= k_SCREEN_WIDTH/2)
		{
			_state = CHARATER_STATE_STANDING;
			if (_isRocketGun)
			{
				_isRocketGun = false;
			}
		}
		break;	

	case CHARATER_STATE_MOVE_TOP:
		_posY -= _speedMove*1;
		if (_posY <= _tagetMove)
		{
			_posY = _tagetMove;
			_state = CHARATER_STATE_STANDING;			
		}
		break;

	case CHARATER_STATE_MOVE_DOWN:
		_posY += _speedMove*1;
		if (_posY >= _tagetMove)
		{
			_posY = _tagetMove;
			_state = CHARATER_STATE_STANDING;			
		}
		break;


	case CHARATER_STATE_BOSS_BEFORE_RUN:
		_posX -= _speedMove * 2;
		if (_posX <= k_SCREEN_WIDTH / 2)
		{
			_state = CHARATER_STATE_BOSS_RUN;
			_numBome = 10;
		}
		break;

	case CHARATER_STATE_BOSS_RUN:
		_posX += _speedMove;		
		AddCoinBossDie();
		if (_posX >= k_SCREEN_WIDTH + 200)
		{
			_posX = k_SCREEN_WIDTH + 200;
			SetState(CHARATER_STATE_DIE);		
            BATTLE()._addBoss = 0;
		
		}
		break;

	case CHARATER_STATE_BOSS_DIE:
		if (_timeCoin >= _numCoin*2)
		{
			_opactiyBoss--;
		}
		
		_posX -= BATTLE_MAP()._speed_scroll_map;
		_posHX += 5;
		AddCoinBossDie();
		if (_posX <= -k_SCREEN_WIDTH/2)
		{
			_posX = -k_SCREEN_WIDTH/2 ;
			SetState(CHARATER_STATE_DIE);
			BATTLE()._addBoss = 0;
		}

		_countEx++;
		for (int i = 0; i < 4; i++)
		{
			if (_countEx == 5 + i*5)
			{				
				BATTLE().SetVibrate();
				_animEx[i].SetAnimSprite(GET_SPRITE(SPRITEID_EFFECT_EXPLOSION), SPRITEID_EFFECT_EXPLOSION);
				_animEx[i].SetAnim(0, false);
				GAME()->PlaySFX(SFX_BOMB);
				_animEx[i].SetPos(_posX + CMath::RANDOM(-100, 70), _posY + CMath::RANDOM(-70, 0));
			}
			_animEx[i]._x -= BATTLE_MAP()._speed_scroll_map;
		}
	
		break;

	case CHARATER_STATE_BOSS_NORMAL:
	{
		//printf("\n _stateMoveBoss = %d", _stateMoveBoss);
		if (_stateMoveBoss == 1)
		{
			float dd = CMath::DISTANCE(CHARATER().PosX(), CHARATER().PosY(), _posX, _attackY);
			float angle = CMath::ANGLE(CHARATER().PosX(), CHARATER().PosY(), _attackX, _attackY) - 180;
			if (dd > _distaneBoss)
			{
				_posX += _speedMove*MCOS(angle);
				_posY += _speedMove*MSIN(angle);
			}

			int ran = CMath::RANDOM(1, 100);
			if (ran == 2)
			{				
				_stateMoveBoss = 2;
				_tagetMove = _posY - CMath::RANDOM(0,100);
				if (_tagetMove < MIN_BOSS_Y)
				{
					_tagetMove = MIN_BOSS_Y;
				}
			}
			if (ran == 3)
			{
				_stateMoveBoss = 3;
				_tagetMove = _posY + CMath::RANDOM(0, 100);
				if (_tagetMove > MAX_BOSS_Y)
				{
					_tagetMove = MAX_BOSS_Y;
				}
			}
		}
		else if (_stateMoveBoss == 2)
		{
			_posY -= _speedMove;
			if (_posY <= _tagetMove)
			{
				_posY = _tagetMove;
				_stateMoveBoss = 1;
			}
		}
		else if (_stateMoveBoss == 3)
		{
			_posY += _speedMove;
			if (_posY >= _tagetMove)
			{
				_posY = _tagetMove;
				_stateMoveBoss = 1;
			}
		}
	

		_addRocket = !BATTLE().CheckRocketComplete();

		if (_lastHPRocket - _hp >= _perHPRocket && !_addgun && !_addgun1 && !_addRocket && !_fastBome)
		{
 			_animArmy.SetAnim(4, true);
			_lastHPRocket = _hp;
			_addRocket = true;
			int ran = CMath::RANDOM(0, BATTLE()._numRandRocket - 1);
			int type = CMath::RANDOM(0,5);// BATTLE()._randomTypeRocket[ran] - 1;
			int num = GET_SPRITE(SPRITEID_FROM_ROCKET)->GetModuleOfFrame(type);

			BYTE state = ROCKET_STATE_INIT_GUN;
			if (num == 1) 
			{ 
				state = ROCKET_STATE_INIT_FOLLOW;
			}
			for (int i = 0; i < num; i++)
			{
				int timeGun = GET_SPRITE(SPRITEID_FROM_ROCKET)->GetFModuleX(type, i) / 1000.0f * 60;
				int opacity = GET_SPRITE(SPRITEID_FROM_ROCKET)->GetFModuleOpacity(type, 0);
				int y = MIN_MOVE_Y + GET_SPRITE(SPRITEID_FROM_ROCKET)->GetFModuleY(type, i);
				BATTLE().AddRocket(1, k_SCREEN_WIDTH, y, state, timeGun);
			}
			
		}

		if (_lastHPBome - _hp >= _perHPBome && !_addRocket)
		{
			_lastHPBome = _hp;
			_numBome = _maxBome;
			_timeBome = 0;
			_fastBome = true;
		}

		if (_numBome > 0 && !_addRocket)
		{
			_timeBome++;
			if (_timeBome > 10)
			{
				if (!_addgun)
				{
					_timeBome = 0;
					_animArmy.SetAnim(5, false);
					_addgun = true;
					_numBome--;
				}
			}
		}
		else
		{
			_fastBome = false;

		}


		if (_addgun)
		{
			if (_animArmy.IsCurrentAnimStop())
			{
				_animArmy.SetAnim(4, true);
				_addgun = false;
				_addgun1 = false;
				float angle = CMath::RANDOM(45, 85);
				float dd = CMath::ABSOLUTE_VALUE(CHARATER().PosX() - _animArmy._x);
				if (dd < 200)
				{
					dd = 300;
				}
				BATTLE().AddObstacle(1, _animArmy._x, _animArmy._y - 70, OBSTACLE_BOME, OBSTACLE_STATE_INIT_FALL, 0	,dd - CMath::RANDOM(0, 200), angle);
				GAME()->PlaySFX(SFX_BOSS_THROWING_BOMB);
			}
		}

	}
		break;
	}
	
	int newtype = 0;
	float percent = _hp / (float)_maxHP * 100;
	/*for (int i = 0; i < TOTAL_AI_BOSS_DES_ROW; i++)
	{
		if (percent >= GET_DES().getAIBossDes(i, AI_BOSS_DES_PERCENT))
		{
			newtype = i;
			break;
		}
	}*/
	if (percent >= 70 )
	{
		newtype = 0;
	}
	else if (percent >= 30)
	{
		newtype = 1;
	}
	else
	{
		newtype = 2;
	}
	if (newtype > _typeDiff)
	{
		_typeDiff = newtype;
		SetValueBose();
	}


	_timeGun--;
	if (_timeGun <= 0 && _state != CHARATER_STATE_BOSS_DIE && !_addRocket)
	{			
		if (!_addgun1)
		{
			_animArmy.SetAnim(5, false);
			_addgun1 = true;
			_timeGun = _timeDeplayBome;
		}	
	}	

	if (_addgun1 && _state != CHARATER_STATE_BOSS_DIE && _state != CHARATER_STATE_BOSS_RUN)
	{
		if (_animArmy.IsCurrentAnimStop())
		{
			float dd = CMath::ABSOLUTE_VALUE(CHARATER().PosX() - _animArmy._x);
			//printf("\n ddddddddddd = %f",dd);
			if (dd >900.0f)
			{
				dd = 900;
			}			
			float angle = 90 - dd * 30 / 900.0f;			
			BATTLE().AddObstacle(1, _animArmy._x, _animArmy._y - 70, OBSTACLE_BOME, OBSTACLE_STATE_INIT_FALL,0, dd + CMath::RANDOM(-90, 50), angle);
			_animArmy.SetAnim(4, true);
			_addgun1 = false;
			GAME()->PlaySFX(SFX_BOSS_THROWING_BOMB);
		}
	}
	
	_animCharater.SetPos(_posX, _posY + BATTLE()._vibrateY);
	if (_state != CHARATER_STATE_BOSS_DIE)
	{
		_posHX = _posX;
		_posHY = _posY;
	}
	_animHorse1.SetPos(_posHX, _posHY + BATTLE()._vibrateY);

	int x = GET_SPRITE(_spriteBoss)->GetFModuleX(3, 4);
	int y = GET_SPRITE(_spriteBoss)->GetFModuleY(3, 4);
	_animArmy.SetPos(_posX + x, _posY + y + BATTLE()._vibrateY);

	percent = (1 - _hp / (float)_maxHP);
	_numFire = percent / (1 / 6.0f);
	if (_numFire > 6)
	{
		_numFire = 6;
	}
	for (int i = 0; i < _numFire; i++)
	{
		x = GET_SPRITE(_spriteBoss)->GetFModuleX(3, 5 + i);
		y = GET_SPRITE(_spriteBoss)->GetFModuleY(3, 5 + i);
		_animFire[i].SetPos(_posX + x, _posY + y + BATTLE()._vibrateY);
	}

	if (_animArmy.IsCurrentAnimStop())
	{
		_animArmy.SetAnim(4, true);
	}
}

void CArmy::ReplaceGold(int frame, int module)
{
	GET_SPRITE(SPRITEID_BOSS_02)->ReplaceModule(
		Button::GetModuleID(GET_SPRITE(SPRITEID_BOSS_02), 1, 20),
		GAME()->GetSpriteVerID(SPRITEID_BOSS_02),
		Button::GetModuleID(GET_SPRITE(SPRITEID_BOSS_02), frame, module)
		);
	
}
void CArmy::InitBoss_2(BYTE type)
{
	_spriteBoss = SPRITEID_BOSS_02;
	_maxHP = GET_DES().getCheckPointDes(BATTLE()._lvlCheckPiont, CHECK_POINT_DES_BOSS_HP);
	_hp = _maxHP;
	_animCharater.SetAnimSprite(GET_SPRITE(_spriteBoss), _spriteBoss);
	_animCharater.SetAnim(0, true);
	_animCharater.SetSubTime(4);

	_state = CHARATER_STATE_INIT_BOSS;
	
	BATTLE()._isTargetTrain = true;
	_timeRocketGun = 0;
    _addsoundTrain = false;
	_opacityHp = 0;
	_indexFameGold = 18;
	ReplaceGold(_indexFameGold,0);

	for (int i = 0; i < 3; i++)
	{
		_renderCavilian[i] = false;
		_animCavilian[i].SetAnimSprite(GET_SPRITE(_spriteBoss), _spriteBoss);
		_animCavilian[i].SetAnim(3 + 2 * i, true);
	}

	for (int i = 0; i < 4; i++)
	{
		_animFire[i].SetAnimSprite(GET_SPRITE(_spriteBoss), _spriteBoss);
		_animFire[i].SetAnim(2, true);
	}

	for (int i = 0; i < 5; i++)
	{
		_animLight[i].SetAnimSprite(GET_SPRITE(_spriteBoss), _spriteBoss);
		_animLight[i].SetAnim(1, false);
		_animLight[i].SetSubTime(4 + i);
		int x = GET_SPRITE(_spriteBoss)->GetFModuleX(1, 20);
		int y = GET_SPRITE(_spriteBoss)->GetFModuleY(1, 20);
		int w = GET_SPRITE(_spriteBoss)->GetFModuleWidth(1, 20);
		int h = GET_SPRITE(_spriteBoss)->GetFModuleHeight(1, 20);
		_lightX[i] = CMath::RANDOM(x, x + w);
		_lightY[i] = CMath::RANDOM(y, y + h);
	}
}
void CArmy::RenderBoss_2()
{
    float zoom = 1.0f;
    float offsetY = 0;
    if(CGame::_this->_device._version == SupportVersion::RESVER_960_640)
    {
        zoom = 1.20f;
        offsetY = -0.15*640/2;
    }
    if(CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
    {
        zoom = IPAD_ZOOM;
        offsetY = IPAD_OFFSET;
    }
	if (_state != CHARATER_STATE_STANDING)
	{
		GET_SPRITE(_spriteBoss)->DrawFrame(G(), 1, _posX, _posY);
	}
	else
	{
		_animCharater.DrawAnimObject(G());
		_animCharater.UpdateAnimObject();
	}

	if (_perHPBome <= 0)
	{
		for (int i = 0; i < 4; i++)
		{
			_animFire[i].DrawAnimObject(G());
			_animFire[i].UpdateAnimObject();
		}
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			_animLight[i].SetPos(_posX + _lightX[i], _posY + _lightY[i]);
			_animLight[i].DrawAnimObject(G());
			_animLight[i].UpdateAnimObject();
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (_renderCavilian[i])
		{
			_animCavilian[i].SetPos(_posX, _posY);
			_animCavilian[i].DrawAnimObject(G());
			_animCavilian[i].UpdateAnimObject();
		}
		
	}


	if (_posX < k_SCREEN_WIDTH )
	{
		G()->SetOpacity(_opacityHp);
		GET_SPRITE(_spriteBoss)->DrawFrame(G(), 16, 0, 0+offsetY);
		int clipX = GET_SPRITE(_spriteBoss)->GetFModuleX(17, 0);
		int clipY = GET_SPRITE(_spriteBoss)->GetFModuleY(17, 0)+offsetY;
		int clipH = GET_SPRITE(_spriteBoss)->GetFModuleHeight(17, 0);
		
		int clipW = (_hp / (float)_maxHP)*GET_SPRITE(_spriteBoss)->GetFModuleWidth(17, 0);

		G()->SetClip(clipX, clipY, clipW, clipH);
		GET_SPRITE(_spriteBoss)->DrawFrame(G(), 17, 0, 0+offsetY);
		G()->ClearClip();
		G()->SetOpacity(100);

	}
}
void CArmy::UpdateBoss_2()
{
	if (_posX < k_SCREEN_WIDTH)
	{
		
		BATTLE()._targetTrainX = GET_SPRITE(_spriteBoss)->GetFModuleCenterX(0, 0) + _posX;
		BATTLE()._targetTrainY = GET_SPRITE(_spriteBoss)->GetFModuleCenterY(0, 0) + _posY;
        if (!_addsoundTrain) {
            
            GAME()->PlaySFX(SFX_TRAIN_WARNING);
            _addsoundTrain = true;
        }
	}
	_perHPBome = _hp / (float)_maxHP * 100;
	switch (_state)
	{
	case CHARATER_STATE_INIT_BOSS:
		_posX -= BATTLE_MAP()._speed_scroll_map;
		if (_posX < 0)
		{
			_state = CHARATER_STATE_STANDING;		
			BATTLE()._isCalculateDistance = true;
			BATTLE()._addBoss = 0;
             GAME()->PlaySFX(SFX_TRAIN_START_STOP);
			 MAIN_GAME()->_timeTrainGoin = 60;
			 GAME()->PlaySFX(SFX_TRAIN_GOIN);
			 if (TUTORIAL()._step == 3)
			 {
				 MAIN_GAME()->SetState(k_GS_TUTORIAL);
			 }
		}
		break;
	case CHARATER_STATE_STANDING:
		
		MAIN_GAME()->_timeTrainGoin--;
		if (MAIN_GAME()->_timeTrainGoin <= 0)
		{
			MAIN_GAME()->_timeTrainGoin = 60;
			GAME()->PlaySFX(SFX_TRAIN_GOIN);
		}

		if (MAIN_GAME()->_timeTrainGoin % 20 == 0)
		{
			AddEffectTrainSmoke(960, 20,1,0.02);
		}

		_opacityHp += 3;
		if (_opacityHp > 100)
		{
			_opacityHp = 100;
		}
		if (BATTLE()._distanceRun >= BATTLE()._endTrain)
		{
			for (int i = 0; i < 3; i++)
			{
				_renderCavilian[i] = true;				
				_animCavilian[i].SetAnim(4 + 2 * i, true);
			}

             GAME()->PlaySFX(SFX_TRAIN_START_STOP);
             GAME()->PlaySFX(SFX_MISSION_COMPLETED);
			BATTLE()._isTargetTrain = false;
			_state = CHARATER_STATE_MOVE_LEFT;
			BATTLE()._offsetMoveY = 0;			
			BATTLE()._addBoss = 2;

			BATTLE()._lvlBosse = GET_DES().getCheckPointDes(BATTLE()._lvlCheckPiont, CHECK_POINT_DES_BOSS_TYPE) - 1;		
			BATTLE()._offsetMoveY = 0;		
			EFFECT().Init();	
			BATTLE()._distanceBoss = GET_DES().getCheckPointDes(BATTLE()._lvlCheckPiont, CHECK_POINT_DES_BOSS_SPAWN);			
			USER()._stateBoss = BOSS_STATE_NONE;	

			BATTLE_MAP()._endRailX = k_SCREEN_WIDTH;
		}
		else
		{
			float dd = CMath::DISTANCE(CHARATER().PosX(), CHARATER().PosY(), BATTLE()._targetTrainX, BATTLE()._targetTrainY);
			if (dd <= 75 )
			{				
				_timeRocketGun--;
				if (_timeRocketGun <= 0 && BATTLE().CheckRocketComplete())
				{
					BATTLE().AddRocket(1, k_SCREEN_WIDTH, 0, ROCKET_STATE_INIT_FOLLOW, 0);
					_timeRocketGun = 5 * 60;
				}
			}
		}
		break;

	case CHARATER_STATE_MOVE_LEFT:
		_posX -= BATTLE_MAP()._speed_scroll_map;
		if (_perHPBome <= 0)
		{
			_timeDeplay--;
			if (_timeDeplay <= 0)
			{
				MAIN_GAME()->SetState(k_GS_RESULT);
			}

		}
		else
		{
			_opacityHp -= 3;
			if (_opacityHp < 0)
			{
				_opacityHp = 0;
			}
			
			if (_posX < -k_SCREEN_WIDTH)
			{
				_state = CHARATER_STATE_DIE;
			}
		}
		
		break;
	}
	_animCharater.SetPos(_posX, _posY + BATTLE()._vibrateY);

	for (int i = 0; i < 4; i++)
	{	
		int x = GET_SPRITE(_spriteBoss)->GetFModuleX(0, 1 + i);
		int y = GET_SPRITE(_spriteBoss)->GetFModuleY(0, 1 + i);
		_animFire[i].SetPos(_posX + x, _posY + y + BATTLE()._vibrateY);
	}

	for (int i = 0; i < 5; i++)
	{
		if (_animLight[i].IsCurrentAnimStop())
		{
			_animLight[i].SetAnim(1, false);
			_animLight[i].SetSubTime(4 + i);

			int x = GET_SPRITE(_spriteBoss)->GetFModuleX(1, 20);
			int y = GET_SPRITE(_spriteBoss)->GetFModuleY(1, 20);
			int w = GET_SPRITE(_spriteBoss)->GetFModuleWidth(1, 20);
			int h = GET_SPRITE(_spriteBoss)->GetFModuleHeight(1, 20);
			_lightX[i] = CMath::RANDOM(x, x + w);
			_lightY[i] = CMath::RANDOM(y, y + h);
		}
	}

	
	//printf("\n percentHP = %f", _perHPBome);
	int frame = 18;
	if (_perHPBome > 80)
	{
		frame = 18;
	}
	else if (_perHPBome > 60)
	{
		frame = 19;
	}
	else if (_perHPBome > 40)
	{
		frame = 20;
	}
	else if (_perHPBome > 20)
	{
		frame = 21;
	}
	else if (_perHPBome > 0)
	{
		frame = 22;
	}
	else
	{
		frame = 1;
		ReplaceGold(1, 20);
	}
	if (frame > _indexFameGold)
	{
		_indexFameGold = frame;
		ReplaceGold(_indexFameGold, 0);
	}

	if (_perHPBome < 75)
	{
		_renderCavilian[0] = true;
	}
	if (_perHPBome < 50)
	{
		_renderCavilian[1] = true;
	}
	if (_perHPBome < 25)
	{
		_renderCavilian[2] = true;
	}
}

void CArmy::InitBoss_3(BYTE type)
{

	if (BATTLE()._lvlCheckPiont == 4)
	{
		_spriteBoss = SPRITEID_BOSS_03;
	}
	else
	{
		_spriteBoss = SPRITEID_BOSS_3_A;
	}


	_collisionW = GET_SPRITE(_spriteBoss)->GetFModuleWidth(1, 0);
	_collisionH = GET_SPRITE(_spriteBoss)->GetFModuleHeight(1, 0);

	_colMoveW = GET_SPRITE(_spriteBoss)->GetFModuleWidth(0, 0);
	_colMoveH = GET_SPRITE(_spriteBoss)->GetFModuleHeight(0, 0);

	_maxHP = GET_DES().getCheckPointDes(BATTLE()._lvlCheckPiont, CHECK_POINT_DES_BOSS_HP);
	_hp = _maxHP;
	_animIndex = 0;
	_animCharater.SetAnimSprite(GET_SPRITE(_spriteBoss), _spriteBoss);
	_animCharater.SetAnim(_animIndex, true);
	
	_state = CHARATER_STATE_INIT_BOSS;
	_scaleX = -1;

	_timeGun = 0;
	_timeGun2 = 0;
	
	_typeDiff = 0;
	GetRanTypeGun(_typeDiff);
	_speedGun = GET_DES().getBoss3Des(_typeDiff, BOSS3_DES_BULLET_SPEED);

	_typeCallArmy = 0;
	GetRanTypeGun(_typeCallArmy);
	_speedMove = 2;
	_opactiyBoss = 200;
	_addgun = false;
	_timeDeplay = 0;
}
void CArmy::RenderBoss_3()
{
    float zoom = 1.0f;
    float offsetY = 0;
    if(CGame::_this->_device._version == SupportVersion::RESVER_960_640)
    {
        zoom = 1.20f;
        offsetY = -0.15*640/2;
    }
    if(CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
    {
        zoom = IPAD_ZOOM;
        offsetY = IPAD_OFFSET;
    }
	if (_state == CHARATER_STATE_BOSS_DIE)
	{
		G()->SetOpacity(_opactiyBoss);
	}
	G()->SetScale(_scaleX, 1, _posX, _posY);

	if (_state == CHARATER_STATE_BOSS_DIE)
	{
		G()->SetOpacity(_opactiyBoss);
		if (_animCharater.IsCurrentAnimStop())
		{
			GET_SPRITE(_spriteBoss)->DrawFrame(G(), 64, _posX, _posY);
		}
	}
	else
	{
		GET_SPRITE(_spriteBoss)->DrawFrame(G(), 2, _posX, _posY);
	}
	BATTLE()._animManager.Add(&_animCharater, 100, _animCharater._x, _animCharater._y, _scaleX, 1, _animCharater._x, _animCharater._y);
	//_animCharater.DrawAnimObject(G());
	G()->ClearScale();
	_animCharater.UpdateAnimObject();
	
	GET_SPRITE(SPRITEID_BOSS_01)->DrawFrame(G(), 1, 0, 0+offsetY);

	int clipX = GET_SPRITE(SPRITEID_BOSS_01)->GetFModuleX(2, 0);
	int clipY = GET_SPRITE(SPRITEID_BOSS_01)->GetFModuleY(2, 0)+offsetY;
	int clipH = GET_SPRITE(SPRITEID_BOSS_01)->GetFModuleHeight(2, 0);

	int clipW = (_hp / (float)_maxHP)*GET_SPRITE(SPRITEID_BOSS_01)->GetFModuleWidth(2, 0);

	G()->SetClip(clipX, clipY, clipW, clipH);
	GET_SPRITE(SPRITEID_BOSS_01)->DrawFrame(G(), 2, 0, 0+offsetY);
	G()->ClearClip();

	G()->SetOpacity(100);
	

}
void CArmy::UpdateBoss_3()
{	
	GetTypeDiffBoss();
	GetTypeCallArmy();
	_angleGunX = -150;
	_angleGunY = -70;
	printf("\n state = %d, %d, %d", _state, _timeGun, _timeGun2);

	if (_timeGun > 0)
	{
		_timeGun--;
	}
	if (_timeGun2 > 0)
	{
		_timeGun2--;
	}
	switch (_state)
	{
	case CHARATER_STATE_INIT_BOSS:
		_posX -= BATTLE_MAP()._speed_scroll_map*2;	
		if (_posX <k_SCREEN_WIDTH/5*4)
		{
			_state = CHARATER_STATE_STANDING;
			_animIndex = 1;
			_animCharater.SetAnim(_animIndex, true);
			_speedMove = 2;
			_stateOld = CHARATER_STATE_INIT_BOSS;
		}
		break;
	case CHARATER_STATE_STANDING:
	{
		_targetX = k_SCREEN_WIDTH / 5 * 4 + CMath::RANDOM(-100, 100);
		int min = MIN_MOVE_Y;
		int max = MAX_MOVE_Y;
		_targetY = CMath::RANDOM(min + 50, max - 50);
		
		_typeGun = _ranTypeGun[CMath::RANDOM(0, _numRanTypeGun - 1)];
		if (_typeGun == 1 || _typeGun == 2 || _typeGun == 4)
		{
			_state = CHARATER_STATE_BOSS_RUN;			
			if (_stateOld != CHARATER_STATE_INIT_BOSS)
			{
				_timeGun2 = GET_DES().getBoss3Des(_typeDiff, BOSS3_DES_TIME_GUN) * 60;
			}
			
		}
		else
		{			
			_state = CHARATER_STATE_MOVE;
			if (_stateOld != CHARATER_STATE_INIT_BOSS)
			{
				_timeGun = GET_DES().getBoss3Des(_typeDiff, BOSS3_DES_TIME_GUN) * 60;
			}
			
		}
		if (_animCharater.IsCurrentAnimStop())
		{
			_animIndex = 0;
			_animCharater.SetAnim(_animIndex, true);
		}
		_stateOld = CHARATER_STATE_STANDING;
	}
		
		break;

	case CHARATER_STATE_MOVE:
		if (IsMoveToTarget(_posX, _posY, _targetX, _targetY, 1.5))
		{
			
		}
		if (_timeGun <= 0 && _timeGun2 <= 0)
		{
			_animIndex = 1;		
			_animCharater.SetAnim(_animIndex, false);
			_timeGun = GET_DES().getBoss3Des(_typeDiff, BOSS3_DES_TIME_GUN) * 60;

		}

		if (_animIndex == 1 && _animCharater.IsCurrentAnimStop())
		{		
			_animIndex = 2;
			int timeAnim = 2;
			float angle = CMath::ANGLE(_posX + _angleGunX, _posY + _angleGunY, CHARATER().PosAttackX(), CHARATER().PosAttackY());
			if (_typeGun == 0)
			{
				timeAnim = 3;
				AddEffectGunType1(_posX, _posY, _speedGun, 3, 15, angle, 25);
			}
			if (_typeGun == 3)
			{
				timeAnim = 5;
				AddEffectGunType1(_posX, _posY, _speedGun, 7, 15, angle, 20);
			}		
			_stateOld = CHARATER_STATE_MOVE;
			_state = CHARATER_STATE_STANDING;
			_animCharater.SetAnim(_animIndex, false);
			_animCharater.SetSubTime(timeAnim);
			GAME()->PlaySFX(SFX_GUNSHOT_BOSS);
		}
		
		if (_animCharater.IsCurrentAnimStop() && (_animIndex == 2 || _animIndex == 4))
		{
			_animIndex = 0;
			_animCharater.SetAnim(_animIndex, true);
		}
		break;

	case CHARATER_STATE_BOSS_RUN:
		if (IsMoveToTarget(_posX, _posY, _targetX, _targetY, 1.5))
		{
			if (_animIndex != 1 && _animIndex != 3)
			{
				_targetX = k_SCREEN_WIDTH / 5 * 4 + CMath::RANDOM(-100, 100);
				int min = MIN_MOVE_Y;
				int max = MAX_MOVE_Y;
				_targetY = CMath::RANDOM(min + 50, max - 50);
			}
			
		}

		
		if (_timeGun <= 0 && _timeGun2 <= 0)
		{
			_animIndex = 1;
			if (_typeGun == 4)
			{
				_animIndex = 3;
			}
			_animCharater.SetAnim(_animIndex, false);
			_timeGun2 = GET_DES().getBoss3Des(_typeDiff, BOSS3_DES_TIME_GUN) * 60;
		}
		if ((_animIndex == 1 || _animIndex == 3) && _animCharater.IsCurrentAnimStop())
		{
			float angle = 180;
			_animIndex = 2;
			if (_typeGun == 1)
			{
				AddEffectGunType1(_posX, _posY, _speedGun, 3, 0, angle, 25);
			}
			else if (_typeGun == 2)
			{
				AddEffectGunType2(_posX, _posY, _speedGun, 3, 180, 50);
			}
			else if (_typeGun == 4)
			{
				_animIndex = 4;
				_angleGunX = 20;
				_angleGunY = -270;

				AddEffectGunType1(_posX, _posY, _speedGun, 8, 0,0, 360 / 8.0);
			}			
			_stateOld = CHARATER_STATE_BOSS_RUN;
			_state = CHARATER_STATE_STANDING;			
			_animCharater.SetAnim(_animIndex, false);
			GAME()->PlaySFX(SFX_GUNSHOT_BOSS);
		}	
		if (_animCharater.IsCurrentAnimStop() && (_animIndex == 2 || _animIndex == 4))
		{
			_animIndex = 0;
			_animCharater.SetAnim(_animIndex, true);
		}
		break;
	case CHARATER_STATE_BOSS_DIE:
		BATTLE()._addBoss = 0;
		_opactiyBoss--;
		_posX -= BATTLE_MAP()._speed_scroll_map;
		if (_posX <= 0)
		{
			_stateOld = CHARATER_STATE_BOSS_DIE;
			_state = CHARATER_STATE_DIE;
		}
		AddCoinBossDie();		
		break;
	
	}
	
	if (_addgun)
	{
		if (_timeDeplay > 0)
		{
			_timeDeplay--;
			if (_timeDeplay <= 0)
			{
  				int typeE = _ranTypeEnemy[CMath::RANDOM(0, _numRanTypeEnemy - 1)] - 1;
				BATTLE().AddArmy(typeE, k_SCREEN_WIDTH, _posY - 50, CHARATER_STATE_ENEMY_FOLLOW_TOP, ENEMY_INIT_BY_BOSS);
				BATTLE().AddArmy(typeE, k_SCREEN_WIDTH, _posY + 50, CHARATER_STATE_ENEMY_FOLLOW_DOWN, ENEMY_INIT_BY_BOSS);
			}
		}
		else
		{
			if (BATTLE().GetArmyState(CHARATER_STATE_ENEMY_FOLLOW_TOP) == NULL && BATTLE().GetArmyState(CHARATER_STATE_ENEMY_FOLLOW_DOWN) == NULL && _timeDeplay <= 0)
			{
				_timeDeplay = 30;
			}
		}
	}

	_collisionX = GET_SPRITE(_spriteBoss)->GetFModuleX(1, 0) + _posX;
	_collisionY = GET_SPRITE(_spriteBoss)->GetFModuleY(1, 0) + _posY;

	_colMoveX = GET_SPRITE(_spriteBoss)->GetFModuleX(0, 0) + _posX;
	_colMoveY = GET_SPRITE(_spriteBoss)->GetFModuleY(0, 0) + _posY;

	_attackX = _posX + GET_SPRITE(_spriteBoss)->GetFModuleCenterX(1, 0);
	_attackY = _posY + GET_SPRITE(_spriteBoss)->GetFModuleCenterY(1, 0);

	_animCharater.SetPos(_posX, _posY + BATTLE()._vibrateY);
}
void CArmy::GetTypeDiffBoss()
{
	int type = 0;
	float percent = _hp / (float)_maxHP * 100;
	for (int i = 0; i < TOTAL_BOSS3_DES_ROW; i++)
	{
		if (percent <= GET_DES().getBoss3Des(i, BOSS3_DES_HP_WARNING))
		{
			type = i;			
		}
	}
	if (_typeDiff < type)
	{
		_typeDiff = type; 
		GetRanTypeGun(_typeDiff);
		_speedGun = GET_DES().getBoss3Des(_typeDiff, BOSS3_DES_BULLET_SPEED);
	}
	
}
void CArmy::GetTypeCallArmy()
{
	int type = 0;
	float percent = _hp / (float)_maxHP * 100;
	for (int i = 0; i < TOTAL_BOSS3_DES_ROW; i++)
	{
		if (percent <= GET_DES().getBoss3Des(i, BOSS3_DES_HP_WARNING_1))
		{
			type = i;
		}
	}
	if (_typeCallArmy < type)
	{		
		_typeCallArmy = type;
		GetRanTypeEnemy(_typeDiff);	

		_addgun = true;
		
		
	}
}
void CArmy::GetRanTypeGun(int level)
{
	char str[128];
	sprintf(str, "%s", GET_DES().getBoss3TypeDes(level, BOSS3_TYPE_DES_TYPE_BULLET));

	int lengh = strlen(str);
	_numRanTypeGun = 0;
	char string[5];
	int count = 0;
	for (int i = 0; i < lengh; i++)
	{
		if (str[i] != ',')
		{
			string[count] = str[i];
			count++;
		}
		else
		{
			string[count] = '\0';
			_ranTypeGun[_numRanTypeGun] = GET_DES().ConverNumber(string);
			_numRanTypeGun++;
			count = 0;
		}
		if (i == lengh - 1)
		{
			string[count] = '\0';
			_ranTypeGun[_numRanTypeGun] = GET_DES().ConverNumber(string);
			_numRanTypeGun++;
			count = 0;
		}
	}
}

void CArmy::GetRanTypeEnemy(int level)
{
	char str[128];
	sprintf(str, "%s", GET_DES().getBoss3TypeDes(level, BOSS3_TYPE_DES_TYPE_ENEMY));

	int lengh = strlen(str);
	_numRanTypeEnemy = 0;
	char string[5];
	int count = 0;
	for (int i = 0; i < lengh; i++)
	{
		if (str[i] != ',')
		{
			string[count] = str[i];
			count++;
		}
		else
		{
			string[count] = '\0';
			_ranTypeEnemy[_numRanTypeEnemy] = GET_DES().ConverNumber(string);
			_numRanTypeEnemy++;
			count = 0;
		}
		if (i == lengh - 1)
		{
			string[count] = '\0';
			_ranTypeEnemy[_numRanTypeEnemy] = GET_DES().ConverNumber(string);
			_numRanTypeEnemy++;
			count = 0;
		}
	}
}
void CArmy::Update()
{	
	UpdateAngleGun();
	_speedMove = _speedStart;
	if (_type == TYPE_ENEMY_BOSS_1)
	{		
		UpdateBoss_1();
	}
	else if (_type == TYPE_ENEMY_BOSS_2)
	{
		UpdateBoss_2();
	}
	else if (_type == TYPE_ENEMY_BOSS_3)
	{
		UpdateBoss_3();
	}
	else
	{
		if (BATTLE()._isTargetTrain)
		{
			_targetX = BATTLE()._targetTrainX;
			_targetY = BATTLE()._targetTrainY;
			
		}
		else
		{
			_targetX = CHARATER().PosX();
			_targetY = CHARATER().PosY();
		}
		
		if (_posY <= MIN_MOVE_Y + BATTLE()._offsetMoveY && _state != CHARATER_STATE_MOVE_TOP)
		{
			_posY = MIN_MOVE_Y + BATTLE()._offsetMoveY; 
			_angleMove = CMath::ANGLE(_posX, _posY, _targetX, _targetY) + 180;
		}

		
		if (CHARATER().GetTypePower() != POWER_MULE && CHARATER().GetTypePower() != POWER_MEDAL)
		{
			if (_state != CHARATER_STATE_MOVE_TOP)
			{
				CheckDirectionMove(_angleMove);
			}
			
		}

		
		if (_state != CHARATER_STATE_DIE)
		{
			if (_posX <= _targetX)
			{
				_scaleX = 1;
			}
			else
			{
				_scaleX = -1;
			}

			if (_state != CHARATER_STATE_MOVE_TOP)
			{
                float offsetspeed = 1;
                if (_posX < CHARATER().PosX())
                {
                    offsetspeed = 0.8;
                }
				if (_type == 3)
				{
					if (_timeGun > 0)
					{
						_timeGun--;
						if (_timeGun <= 0)
						{						
							AddEffectGun(_posX, _posY, _speedGun*offsetspeed);
							_timeGun = GET_DES().getEnemyDes(_type, ENEMY_DES_TIME_GUN);
						}
					}
				}
				else if (_type == 4)
				{
					if (_timeGun > 0)
					{
						_timeGun--;
						if (_timeGun <= 0)
						{
							float angle = CMath::ANGLE(_posX + _angleGunX, _posY + _angleGunY, CHARATER().PosAttackX(), CHARATER().PosAttackY());
							AddEffectGunType2(_posX, _posY, _speedGun*offsetspeed, 2, angle, 30);
							_timeGun = GET_DES().getEnemyDes(_type, ENEMY_DES_TIME_GUN);
						}
					}
				}
			}
			
			switch (_state)
			{
			case CHARATER_STATE_JUMP:
			{
				_t0 += 16;

				float ft = _t0 / 40.0f;
				float x = 20 * CMath::COS(_angle)*ft;
				float y = 20 * CMath::SIN(_angle)*ft - _g*ft*ft*0.5f;

				_posX += x;
				_posY -= y;

				if (_posY >= _y0)
				{

					SetState(CHARATER_STATE_MOVE);
				}
			}
			case CHARATER_STATE_ENEMY_FOLLOW_TOP:
				_stateOld = CHARATER_STATE_ENEMY_FOLLOW_TOP;
				_posX -= _speedMove;
				if (BATTLE().GetArmyBoss(TYPE_ENEMY_BOSS_3) != NULL)
				{
					_posY = BATTLE().GetArmyBoss(TYPE_ENEMY_BOSS_3)->PosY() - 70;
					if (_posX < BATTLE().GetArmyBoss(TYPE_ENEMY_BOSS_3)->PosX() - 50)
					{
						_posX = BATTLE().GetArmyBoss(TYPE_ENEMY_BOSS_3)->PosX() - 50;						
					}
				}
				else
				{
					SetState(CHARATER_STATE_MOVE);
				}
				break;
			case CHARATER_STATE_ENEMY_FOLLOW_DOWN:
				_stateOld = CHARATER_STATE_ENEMY_FOLLOW_DOWN;
				_posX -= _speedMove;
				if (BATTLE().GetArmyBoss(TYPE_ENEMY_BOSS_3) != NULL)
				{
					_posY = BATTLE().GetArmyBoss(TYPE_ENEMY_BOSS_3)->PosY() + 70;
					if (_posX < BATTLE().GetArmyBoss(TYPE_ENEMY_BOSS_3)->PosX()-50)
					{
						_posX = BATTLE().GetArmyBoss(TYPE_ENEMY_BOSS_3)->PosX() - 50;
					}
				}
				else
				{
					SetState(CHARATER_STATE_MOVE);
				}
				break;
			case CHARATER_STATE_MOVE:
				if (_posX > k_SCREEN_WIDTH)
				{
					_posX -= BATTLE_MAP()._speed_scroll_map;
				}
				else
				{
					if (_isMoveStraight)
 					{
						_posX -= _speedMove;
					}
					else
					{	
						float dd = CMath::DISTANCE(_posX, _posY, _targetX, _targetY);
						if (BATTLE()._isTargetTrain)
						{
							if (_posX > _targetX)
							{
								_posX -= BATTLE_MAP()._speed_scroll_map;
							}

							if (IsMoveToTarget(_posX, _posY, _targetX, _targetY, _speedMove))
							{
								
							}
							
							//printf("\n ddddddddd = %f",dd);
							if (dd <= 60)
							{
								SetState(CHARATER_STATE_MOVE_TOP);
								
							}
							
						}
						else
						{
							float accel = 1;
							_posX -= BATTLE_MAP()._speed_scroll_map;
							if (_posX <= _targetX)
							{
								accel = 1.5;
							}

							if (IsMoveToTarget(_posX, _posY, _targetX, _targetY, _speedMove*accel))
							{

							}
						}

						if (CHARATER().GetTypePower() != POWER_NONE)
						{
							if (dd < _distanceRunPower)
							{
								SetState(CHARATER_STATE_MOVE_RIGHT);
								_animCharater.SetAnim(5, true);
							}
						}                        
					}
				}

				break;
			case CHARATER_STATE_MOVE_TOP:
				_posY -= 1.5;
				
				if (BATTLE().GetArmyBoss(TYPE_ENEMY_BOSS_2) != NULL && BATTLE().GetArmyBoss(TYPE_ENEMY_BOSS_2)->GetState() != CHARATER_STATE_STANDING)
				{
					_posX -= BATTLE_MAP()._speed_scroll_map;
				}
				
				if (_posY <= TRAIN_Y - 80)
				{
					SetState(CHARATER_STATE_ENEMY_ATTACK_TRAIN);
				}
				break;
			case CHARATER_STATE_MOVE_RIGHT:
			{
				float speed = 0;
				if (CHARATER().GetTypePower() == POWER_MULE)
				{
					speed = GET_DES().getMuleDes(CHARATER().GetLVLMule(), MULE_DES_MINUS_ENEMY_SPEED);
					
				}
				else if (CHARATER().GetTypePower() == POWER_MEDAL)
				{
					speed = GET_DES().getCavalryDes(CHARATER().GetLVLCavalry(), CAVALRY_DES_MINUS_ENEMY_SPEED);				
				}
				else if (CHARATER().GetTypePower() == POWER_MACHETE)
				{
					speed = GET_DES().getMacheteDes(CHARATER().GetLVLMachete(), MACHETE_DES_MINUS_ENEMY_SPEED);
				}
				_posX += _speedMove - speed;
				_scaleX = 1;
				if (_posX > k_SCREEN_WIDTH + 50)
				{
					_state = CHARATER_STATE_DIE;
					
				}
				else
				{
					if (CHARATER().GetTypePower() == POWER_NONE)
					{
						SetState(CHARATER_STATE_MOVE);
						_animCharater.SetAnim(2, true);
					}
					
				}
			}
				
				break;

			case CHARATER_STATE_ENEMY_ATTACK_TRAIN:
			{
				int dam = GET_DES().getEnemyDes(_type, ENEMY_DES_DAME);

				_state = CHARATER_STATE_DIE;
				BATTLE().GetArmyBoss(TYPE_ENEMY_BOSS_2)->BeDame(dam);
				BATTLE().GetArmyBoss(TYPE_ENEMY_BOSS_2)->AddParticalWagon(_posX, _posY - 100, SPRITEID_EFFECT_GOLD_PARTICAL, 0, 2, 5);
			}
				break;
			case CHARATER_STATE_BE_ATTACK:
			{
				float accel = 1;
				_posX -= BATTLE_MAP()._speed_scroll_map;
				if (_posX < _targetX)
				{
					accel = 0.6;
				}

				float angle = CMath::ANGLE(CHARATER().PosX(), CHARATER().PosY(), _posX, _posY)+180;
				if (_posY - MIN_MOVE_Y > 50 && MAX_MOVE_Y - _posY > 50)
				{
					float vy = 5 * accel*CMath::SIN(angle + 180);
					float vx = 5 * accel*CMath::COS(angle + 180);

					_posX += vx;
					_posY += vy;
				}
				else
				{
					float vx = 5 * accel*CMath::COS(angle + 180);
					_posX += vx;
				}
				
				if (_stateColor == 0)
				{
					_color += 0x100000;
					if (_color >= 0xdd0000)
					{
						_color = 0xdd0000;
						_stateColor = 1;
					}
				}
				else if (_stateColor == 1)
				{
					_color -= 0x100000;
					if (_color <= 0x770000)
					{
						_color = 0x770000;
						_stateColor = 0;
					}
				}
				if (_timeDeplay > 0)
				{
					_timeDeplay--;
					if (_timeDeplay <= 0)
					{
						if (_stateOld == CHARATER_STATE_ENEMY_FOLLOW_TOP) 
						{
							_state = CHARATER_STATE_ENEMY_FOLLOW_TOP;
						}
						else if (_stateOld == CHARATER_STATE_ENEMY_FOLLOW_DOWN)
						{
							_state = CHARATER_STATE_ENEMY_FOLLOW_DOWN;
						}
						else
						{
							_state = CHARATER_STATE_MOVE;
						}
					}
				}
				break;
			}
			}

			
			_animCharater.SetPos(_posX, _posY + BATTLE()._vibrateY);
		}
		if (_state == CHARATER_STATE_DIE)
		{
			if (_scaleX < 0)
			{
				_scaleX += 0.1f;
				if (_scaleX >= 0)
				{
					_scaleX = 0;
				}
			}
			else if (_scaleX > 0)
			{
				_scaleX -= 0.1f;
				if (_scaleX <= 0)
				{
					_scaleX = 0;
				}
			}
		}
		
		_collisionX = GET_SPRITE(SPRITEID_ENEMY_01 + _type)->GetFModuleX(1, 0) + _posX;
		_collisionY = GET_SPRITE(SPRITEID_ENEMY_01 + _type)->GetFModuleY(1, 0) + _posY;

		_colMoveX = GET_SPRITE(SPRITEID_ENEMY_01 + _type)->GetFModuleX(0, 0) + _posX;
		_colMoveY = GET_SPRITE(SPRITEID_ENEMY_01 + _type)->GetFModuleY(0, 0) + _posY;

		_attackX = _posX + GET_SPRITE(SPRITEID_ENEMY_01 + _type)->GetFModuleCenterX(1, 0);
		_attackY = _posY + GET_SPRITE(SPRITEID_ENEMY_01 + _type)->GetFModuleCenterY(1, 0);
	}
	UpdateEffect();

}
void CArmy::Render()
{	
	if (_type == TYPE_ENEMY_BOSS_1)
	{
		RenderBoss_1();
	}
	else if (_type == TYPE_ENEMY_BOSS_2)
	{
		RenderBoss_2();
	}
	else if (_type == TYPE_ENEMY_BOSS_3)
	{
		RenderBoss_3();
	}
	else
	{
		if (_state == CHARATER_STATE_DIE)
		{
			G()->SetColorMask(0xff000000);
			G()->SetOpacity(CMath::ABSOLUTE_VALUE(_scaleX) * 100);
		}
		else if (_state == CHARATER_STATE_BE_ATTACK)
		{
			G()->SetColorMask(_color);
		}
		G()->SetScale(_scaleX, 1, _animCharater._x, _animCharater._y);
		GET_SPRITE(GET_DES().getShopCharaterDes(USER()._currentUseCharater, SHOP_CHARATER_SPRITE_ID))->DrawFrame(G(), 2, _animCharater._x, _animCharater._y);
		BATTLE()._animManager.Add(&_animCharater,100, _animCharater._x, _animCharater._y, _scaleX, 1, _animCharater._x, _animCharater._y);
		//_animCharater.DrawAnimObject(G());
		G()->ClearScale();
		_animCharater.UpdateAnimObject();

		G()->ClearColorMask();
		G()->SetOpacity(100);

		//G()->SetColor(0x88ff0000);
		//G()->FillRect(_colMoveX, _colMoveY, _colMoveW, _colMoveH);
	}	
	
}

void CArmy::RenderEffectTarget()
{
	//PDEBUG("\n _isTargetFocus:%d", _isTargetFocus);
	if (_isTargetFocus)
	{
		GET_SPRITE(SPRITEID_HUD_STICK)->DrawFrame(G(), 3, _animCharater._x, _animCharater._y - _collisionH / 2);
	}
}

void CArmy::RenderUnDie()
{
	if (_type == TYPE_ENEMY_BOSS_1)
	{
		//RenderBoss_1();
	}
	else if (_type == TYPE_ENEMY_BOSS_2)
	{
		//RenderBoss_2();
	}
	else if (_type == TYPE_ENEMY_BOSS_3)
	{
		//RenderBoss_3();
	}
	else
	{
		if (_state == CHARATER_STATE_ENEMY_FOLLOW_TOP || _state == CHARATER_STATE_ENEMY_FOLLOW_TOP)
		{
			return;
		}
		if (_state == CHARATER_STATE_DIE)
		{
			G()->SetColorMask(0xff000000);
			G()->SetOpacity(CMath::ABSOLUTE_VALUE(_scaleX) * 100);
		}
		else if (_state == CHARATER_STATE_BE_ATTACK)
		{
			G()->SetColorMask(_color);
		}
		G()->SetScale(_scaleX, 1, _animCharater._x, _animCharater._y);
		
		_animCharater.DrawAnimObject(G());
		G()->ClearScale();		
		G()->ClearColorMask();
		G()->SetOpacity(100);
	}
}
//effect

void CArmy::UpdateEffect()
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
			case EFFECT_TYPE_BOSS_GUN_1:
				UpdateEffectGunType1(i);
				break;
			case EFFECT_TYPE_BOSS_GUN_2:
				UpdateEffectGunType2(i);
				break;
			case EFFECT_TYPE_PARTICAL_WAGON:
				UpdateParticalWagon(i);
				break;
			case EFFECT_TYPE_BOSS_TRAIN_SMOKE:
				UpdateEffectTrainSmoke(i);
				break;
			}
		}
	}
}
void CArmy::RenderEffect()
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
			case EFFECT_TYPE_BOSS_TRAIN_SMOKE:
				RenderEffectTrainSmoke(i);
				break;
			
			}
		}
	}
}

void CArmy::AddEffectGun(float x, float y, float speed)
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		if (_eff_state[i] == EFFECT_STATE_ATTACK_NONE)
		{
			_eff_type[i] = EFFECT_TYPE_ATTACK_GUN;
			_eff_state[i] = 1;

			_varF1[i] = speed;

			_eff_anim[i].SetAnimSprite(GET_SPRITE(SPRITEID_EFFECT_BULLET), SPRITEID_EFFECT_BULLET);
			_eff_anim[i].SetAnim(1, true);

			_eff_anim2[i].SetAnimSprite(GET_SPRITE(SPRITEID_EFFECT_BULLET), SPRITEID_EFFECT_BULLET);
			_eff_anim2[i].SetAnim(2, false);
			_eff_anim2[i].SetSubTime(1);

			_varF2[i] = CHARATER().PosAttackX();
			_varF3[i] = CHARATER().PosAttackY();
			
			_eff_x[i] = x + _angleGunX;
			_eff_y[i] = y + _angleGunY;

			_eff_anim2[i].SetPos(_eff_x[i], _eff_y[i]);
			_varF4[i] = CMath::ANGLE(_eff_x[i], _eff_y[i], _varF2[i], _varF3[i]);

			break;
		}
	}
}

void CArmy::AddEffectGun(float x, float y, float speed, float angle)
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		if (_eff_state[i] == EFFECT_STATE_ATTACK_NONE)
		{
			_eff_type[i] = EFFECT_TYPE_ATTACK_GUN;
			_eff_state[i] = 1;

			_varF1[i] = speed;

			_eff_anim[i].SetAnimSprite(GET_SPRITE(SPRITEID_EFFECT_BULLET), SPRITEID_EFFECT_BULLET);
			_eff_anim[i].SetAnim(1, true);

			_eff_anim2[i].SetAnimSprite(GET_SPRITE(SPRITEID_EFFECT_BULLET), SPRITEID_EFFECT_BULLET);
			_eff_anim2[i].SetAnim(2, false);
			_eff_anim2[i].SetSubTime(1);


			_eff_x[i] = x + _angleGunX;
			_eff_y[i] = y + _angleGunY;

			_varF4[i] = angle;

			break;
		}
	}
}
void CArmy::UpdateEffectGun(int i)
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

		if (CHECK_POINT_IN_RECT(_eff_x[i], _eff_y[i], CHARATER().CollisionX(), CHARATER().CollisionY(), CHARATER().CollisionW(), CHARATER().CollisionH()))
		{			
			if (!CHARATER().IsUndead() && CHARATER().GetState() != CHARATER_STATE_JUMP && CHARATER().GetState() != CHARATER_STATE_DIE)
			{
				_eff_state[i] = EFFECT_STATE_ATTACK_NONE;
 				if (CHARATER().GetTypePower() == POWER_NONE)
				{
					CHARATER().BeDame(1);
					if (CHARATER().GetState() == CHARATER_STATE_DIE)
					{
						_renderTop = true;
						_eff_state[i] = 2;

					}
				}			
				
			}			
		}
	case 2:
		_eff_y[i] += _varF1[i]/2 * CMath::SIN(_varF4[i]);
		_eff_x[i] += _varF1[i]/2 * CMath::COS(_varF4[i]);

		break;


	}
}
void CArmy::RenderEffectGun(int i)
{
	_eff_anim[i].SetPos(_eff_x[i], _eff_y[i]);
	
	G()->SetRotate(_varF4[i], _eff_anim2[i]._x, _eff_anim2[i]._y);

	//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);

	_eff_anim2[i].DrawAnimObject(G());
	_eff_anim2[i].UpdateAnimObject();
	//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
	G()->ClearRotate();

	G()->SetScale(1.5, 1.5, _eff_x[i], _eff_y[i]);
	_eff_anim[i].DrawAnimObject(G());
	G()->ClearScale();
	_eff_anim[i].UpdateAnimObject();
}

void CArmy::AddEffectGunType1(float x, float y, float speed, int num_bulet, int time, float beginA, float offset_angle)
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		if (_eff_state[i] == EFFECT_STATE_ATTACK_NONE)
		{
			_eff_type[i] = EFFECT_TYPE_BOSS_GUN_1;
			_eff_state[i] = 1;
			_var1[i] = num_bulet;
			_var2[i] = time;
			_var3[i] = 0; // count

			_varF1[i] = speed;			
			_varF2[i] = beginA + (num_bulet - 1) / 2.0f * offset_angle; //angle begin
			_varF3[i] = offset_angle;
			_eff_x[i] = x;
			_eff_y[i] = y;
			break;
		}
	}
}
void CArmy::UpdateEffectGunType1(int i)
{	
	for (int j = 0; j < _var1[i]; j++)
	{
		if (_var3[i] == j*_var2[i])
		{			
			int angle = _varF2[i] - _varF3[i] * j;			
			AddEffectGun(_eff_x[i], _eff_y[i], _varF1[i], angle);
		}
	}
	_var3[i]++;
	if (_var3[i] >= _var1[i] * _var2[i])
	{
		_eff_state[i] = EFFECT_STATE_ATTACK_NONE;
	}
}

void CArmy::AddEffectGunType2(float x, float y, float speed, int num_bulet, float beginA, float offset_h)
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		if (_eff_state[i] == EFFECT_STATE_ATTACK_NONE)
		{
			_eff_type[i] = EFFECT_TYPE_BOSS_GUN_2;
			_eff_state[i] = 1;
			_var1[i] = num_bulet;
			_varF1[i] = speed;
			_varF2[i] = beginA;
			_varF3[i] = offset_h;
			_eff_x[i] = x;
			_eff_y[i] = y + -(num_bulet - 1) / 2.0f * offset_h; 
			break;
		}
	}
}
void CArmy::UpdateEffectGunType2(int i)
{
	for (int j = 0; j < _var1[i]; j++)
	{
		//if (_var3[i] == j*_var2[i])
		{
			int yy = _eff_y[i] + _varF3[i] * j;
			AddEffectGun(_eff_x[i], yy, _varF1[i], _varF2[i]);
		}
	}
	_eff_state[i] = EFFECT_STATE_ATTACK_NONE;
	
}

void CArmy::AddEffectTrainSmoke(float x, float y, float speedMove, float speedScale)
{
	for (int i = 0; i < MAX_EFFECT_ATTACK; i++)
	{
		if (_eff_state[i] == EFFECT_STATE_ATTACK_NONE)
		{
			_eff_type[i] = EFFECT_TYPE_BOSS_TRAIN_SMOKE;
			_eff_state[i] = 1;

			_eff_x[i] = x;
			_eff_y[i] = y;
			_varF3[i] = speedMove;
			_varF2[i] = speedScale;
			_varF1[i] = 0.3;
			_var1[i] = 50;

			break;
		}
	}
}
void CArmy::UpdateEffectTrainSmoke(int i)
{
	_eff_x[i] -= _varF3[i]*1.5;
	_eff_y[i] -= _varF3[i]/5.0f;
	_varF1[i] += _varF2[i];
	if (_varF1[i] > 2)
	{
		_varF1[i] = 2;
	}
	switch (_eff_state[i])
	{
	case 1:
		
		_var1[i] += 2;
		if (_var1[i] >= 100)
		{
			
			_eff_state[i] = 2;
		}
		
		break;
	case 2:	
		_var1[i] -= 1;
		if (_var1[i] <= 0)
		{
			_eff_state[i] = EFFECT_STATE_ATTACK_NONE;
		}
		break;
	}
}
void CArmy::RenderEffectTrainSmoke(int i)
{
	if (_eff_state[i] != EFFECT_STATE_ATTACK_NONE)
	{
		G()->SetOpacity(_var1[i]);
		G()->SetScale(_varF1[i], _varF1[i], _eff_x[i], _eff_y[i]);
		GET_SPRITE(SPRITEID_EFFECT_TRAIN_SMOKE)->DrawFrame(G(), 0, _eff_x[i], _eff_y[i]);
		G()->SetOpacity(100);
		G()->ClearScale();
	}
}