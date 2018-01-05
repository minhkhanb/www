#include "pch.h"
#include "../../GameCore/CGame.h"

CBattle::CBattle()
{
	_army = NULL;
	_obstacle = NULL;
	_power = NULL;
	_coin = NULL;
	_rocket = NULL;
}
void CBattle::Init()
{
	_mullCoin = 1;
	_mullScore = 1;
	_numLife = 0;

	if (USER()._currentUseCharater == 1)
	{
		_mullCoin = 2;
	}
	else if (USER()._currentUseCharater == 2)
	{
		_numLife = 1;
	}
	else if (USER()._currentUseCharater == 3)
	{
		_mullScore = 2;
	}


	USER()._score = 0;
	_scoreRun = 0;
#ifdef MAC_OS
	PineAdBox::SetSkipAdvert();
#endif
	MAIN_GAME()->_countShowAdsMods--;
    
#if ADD_ADVERT
    if(GAME()->_advert.IsAvailable())
    {
        MAIN_GAME()->_countShowPineAds++;
    }
#endif

	_addTutorial = false;
	BATTLE_MAP()._stateSwap = 0;
	_isCalculateDistance = true;
	_mull = 1;
	_stateVibrate = 0;
	_vibrateY = 0;
	USER()._timePlay++;
	CHARATER().Init(300,k_SCREEN_HEIGHT/2 + 50,0,0);

	_numArmy = 0;    
    _numCoin = 0;
	_numObstacle = 0;
	_numRocket = 0;
	_numPower = 0;

	MAIN_GAME()->_oldLocation = USER()._lvlLocation;
    _distanceRun = GET_DES().getCheckPointDes(USER()._currentLocation,CHECK_POINT_DES_DISTANCE);
	_lvlCheckPiont = USER()._currentLocation;
    _coinCollect = 0;	
	_lvl_des = -1;

	_stateCoin = 0;
	_scaleCoin = 1;
    
    if (USER()._currentLocation < TOTAL_CHECK_POINT_DES_ROW - 1)
    {
		MAIN_GAME()->_playWitchEndlest = false;
    }

	if (MAIN_GAME()->_playWitchEndlest)
	{
		_lvlBosse = CMath::RANDOM(0, 1) ? 0: 2;		
		_distanceBoss = _distanceRun + 500;
	}
	else
	{
		_lvlBosse = GET_DES().getCheckPointDes(_lvlCheckPiont, CHECK_POINT_DES_BOSS_TYPE) - 1;
		if (_lvlBosse < 0)
		{
			_lvlBosse = 0;
		}		
		_distanceBoss = GET_DES().getCheckPointDes(_lvlCheckPiont, CHECK_POINT_DES_BOSS_SPAWN);
		USER()._stateBoss = BOSS_STATE_NONE;
	}
	
	

	BATTLE()._offsetMoveY = 0;

	SetAnimCheckPoint(USER()._currentLocation);

	_addBoss = 0;
	EFFECT().Init();


	USER()._stateBoss = BOSS_STATE_NONE;		
	

	_distanceNext = _distanceRun;
	_distanceRocket = 0;
	_distancePower = 0 ;
	
	_isUsePower = false;
	_isTargetTrain = false;
	_isEndTrain = false;
	if (_lvlBosse == 1)
	{
		BATTLE_MAP()._usedBG_1 = false;
		BATTLE_MAP()._usedBG_2 = true;
		BATTLE_MAP().SetSpriteBG_2(2);
		BATTLE_MAP().InitBattleMap_2();

		_isCalculateDistance = false;
		_addBoss = 2;
	}
	else
	{		
		BATTLE_MAP()._usedBG_1 = true;
		BATTLE_MAP()._usedBG_2 = false;
		BATTLE_MAP().SetSpriteBG_1(1);
		BATTLE_MAP().InitBattleMap_1();
	}

	

	_timeBegin = 3 * 60;
	CHARATER().SetTypePower(POWER_MULE);
	_speedIncreaseMap = 10;
	_speedIncreaseMove = 1;
	_bossComming.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_INGAME), SPRITEID_HUD_INGAME);
	_bossComming.SetAnim(13, true);
	_bossComming.SetPos(0, 0);
	_isDay = CMath::RANDOM(0, 1);
	_changeDay = false;

	
	int lvl = USER()._currentLocation + 1;
	if (lvl >= TOTAL_CHECK_POINT_DES_ROW - 1)
	{
		lvl = TOTAL_CHECK_POINT_DES_ROW - 1;
	}
	int nextDistance = GET_DES().getCheckPointDes(lvl, CHECK_POINT_DES_DISTANCE);
	_currentDistance = 0;
	_maxDistance = nextDistance - _distanceRun;

	if (_isDay)
	{
		MAIN_GAME()->_isRain = false;
		EFFECT().AddParticalDust(SPRITEID_EFFECT_DUST, 4, 5, CHARATER_MOVE_DOWN_LEFT, 345, 360);	
		_opacityDay = 0;
	}
	else
	{
		EFFECT().AddParticalRain(1);		
		MAIN_GAME()->_isRain = true;
		_opacityDay = 100;
	}
	//EFFECT().AddParticalDust(SPRITEID_EFFECT_DUST, 4,5, CHARATER_MOVE_DOWN_LEFT, 345, 360);
	

	
	_nextObstacle = 0;

	_timeDeplayRocket = 0;
	_timeRocketGun = 0;
	_distanceArmyNext = 0;
	_timePlay = 0;
	_tempDistance = 0;
	_offsetDistance = _distanceRun / 60.0f;
    if (_offsetDistance < 1)
    {
        _offsetDistance = 1;
    }

	MAIN_GAME()->_newScore = false;

	
	_isRunRocket = false;
	CHARATER().SetRenderStick(false) ;
	_typePowerOld = -1;
	
	for (int i = 0; i < 3; i++)
	{
		_arrayPower[i] = -1;
	}
	if (USER()._isUnlockMachete)
	{
		_arrayPower[0] = 0;
	}
	if (USER()._isUnlockMule)
	{
		_arrayPower[1] = 1;
	}
	if (USER()._isUnlockCavalry)
	{
		_arrayPower[2] = 2;
	}
	_totalRandom = -1;
	_addText = false;
	
	//achievement
	ACHIEVEMENT()._numEnemyDie = 0;
	ACHIEVEMENT()._numCoinColect = 0;
	ACHIEVEMENT()._numEnemyJumpBarrelDie = 0;
	ACHIEVEMENT()._numRocketOpenBarrel = 0;
	ACHIEVEMENT()._numBombOpenBarrel = 0;
	ACHIEVEMENT()._numBombActive = 0;
	ACHIEVEMENT()._numPowerUse = 0;

	
	
}
void CBattle::UpdateCoin()
{
	//update coin
	for (int i = 0; i < _numCoin; i++)
	{
		if (_coin[i] != NULL)
		{
			if (_coin[i]->_state == COIN_STATE_INIT)
			{
				_coin[i]->_animCoin.SetPos(_coin[i]->_x, _coin[i]->_y + _vibrateY);
			}
			else
			{
				_coin[i]->_animCoin.SetPos(_coin[i]->_x, _coin[i]->_y);
			}
			
			if (_coin[i]->_state != COIN_STATE_COLLECT && _coin[i]->_state != COIN_STATE_MOVE_CHARATER)
			{
				if (_coin[i]->_y < MIN_MOVE_Y + _offsetMoveY + 40)
				{
					_coin[i]->_y = MIN_MOVE_Y + _offsetMoveY + 40;
				}

				if (_coin[i]->_y > MAX_MOVE_Y)
				{
					_coin[i]->_y = MAX_MOVE_Y;
				}
			}
			

			if (_coin[i]->_state == COIN_STATE_INIT_FALL)
			{

				_coin[i]->_t0 += 16;// DT();

				float ft = _coin[i]->_t0 / 20.0f;
				float x = _coin[i]->_v0 * CMath::COS(_coin[i]->_angle)*ft;
				float y = _coin[i]->_v0 * CMath::SIN(_coin[i]->_angle*1.5)*ft - _coin[i]->_g*ft*ft*0.5f;

				
				if (_coin[i]->_direc == _LEFT)
				{
					_coin[i]->_x = _coin[i]->_x0 - x;
				}
				else
				{
					_coin[i]->_x = _coin[i]->_x0 + x;
				}				
				_coin[i]->_y = _coin[i]->_y0 - y;

				if (_coin[i]->_y > _coin[i]->_targety0)
				{
					_coin[i]->_count++;
					_coin[i]->_g = 10;
					_coin[i]->_t0 = 0;
					_coin[i]->_v0 *= 0.8;
					_coin[i]->_y0 = _coin[i]->_y;
					_coin[i]->_x0 = _coin[i]->_x;

				}
				if (_coin[i]->_count > 1)
				{
					_coin[i]->_state = COIN_STATE_INIT;				
				}

			}
			else if (_coin[i]->_state == COIN_STATE_INIT )
			{
				_coin[i]->_x -= BATTLE_MAP()._speed_scroll_map;
			}
			else if (_coin[i]->_state == COIN_STATE_MOVE_CHARATER)
			{
				if (CHARATER().IsMoveToTarget(_coin[i]->_x, _coin[i]->_y, CHARATER().PosX(), CHARATER().PosY()-30, 15))
				{
					_coin[i]->_state = COIN_STATE_COLLECT;
				}
			}
			else if (_coin[i]->_state == COIN_STATE_COLLECT)
			{
				if (_coin[i]->_type == 1)
				{
					if (CHARATER().IsMoveToTarget(_coin[i]->_x, _coin[i]->_y, 68, 90, 25))
					{
						SAFE_DEL(_coin[i]);
						_coin[i] = NULL;
                        //delete[] _coin[i];
                        
						CHARATER().AddLive();
					}
				}
				else
				{
					if (CHARATER().IsMoveToTarget(_coin[i]->_x, _coin[i]->_y, COIN_CENTER_X, COIN_CENTER_Y, 25))
					{
						SAFE_DEL(_coin[i]);
						_coin[i] = NULL;
                        //delete[] _coin[i];
                        
						_stateCoin = 1;
						_coinCollect++;
						USER().AddCoin(1*_mullCoin);
					}
				}				
			}
		}
	}

	CheckCollisionCoin();
}
void CBattle::UpdateArmy()
{
	//printf("\n _distanceBoss = %d, _lvlBosse = %d, _addBoss = %d, _stateBoss = %d", (int)_distanceBoss, _lvlBosse, _addBoss, USER()._stateBoss);
	if (_distanceRun >= _distanceBoss && _addBoss == 0 /*&& !_isUsePower*/ && !checkHasBoss() && _lvlBosse != 1 &&
		(USER()._stateBoss == BOSS_STATE_NONE || USER()._stateBoss == BOSS_STATE_RUN_1 || USER()._stateBoss == BOSS_STATE_RUN_2))
    {
		_bossIncomingX = -k_SCREEN_WIDTH;
		_isCalculateDistance = false;
		_stateBossIncoming = 0;


		if (USER()._stateBoss == BOSS_STATE_NONE)
		{
			USER()._stateBoss = BOSS_STATE_ACTIVE_1;            
		}
		else if (USER()._stateBoss == BOSS_STATE_RUN_1)
		{
			USER()._stateBoss = BOSS_STATE_ACTIVE_2;			
		}
		else if (USER()._stateBoss == BOSS_STATE_RUN_2)
		{
			USER()._stateBoss = BOSS_STATE_ACTIVE_3;
		}

		int nextLocation = _lvlCheckPiont + 1;
		
		if (nextLocation >= TOTAL_CHECK_POINT_DES_ROW - 1)
		{		
			nextLocation = TOTAL_CHECK_POINT_DES_ROW - 1;
			_distanceBoss = _distanceRun +  500;
		}
		else
		{
			_distanceBoss = GET_DES().getCheckPointDes(nextLocation, CHECK_POINT_DES_BOSS_SPAWN);
		}

		_currentDistance = _distanceRun;
		_nextObstacle = _distanceNext;		
		_addBoss = 1;
	}

	if (_addBoss == 1)
	{
		switch (_stateBossIncoming)
		{
		case 0: 
			_bossIncomingX+= 50;
			if (_bossIncomingX>=0)
			{
				_bossIncomingX = 0;
				_stateBossIncoming = 1;
				_countIncoming = 0;

				_bossComming.SetAnim(12,true);
				_bossComming.SetPos(0, 0);
			}
			break;
		case 1:

			_countIncoming++;
			if (_countIncoming % 8 == 0)
			{
				GAME()->PlaySFX(SFX_WARNING);
			}
			if (_countIncoming > 150)
			{
				_stateBossIncoming = 2;	
				GAME()->StopCurrentMusic();
				GAME()->LoadMusic(BGM_BOSS_FIGHT);
				GAME()->PlayCurrentMusic(BGM_BOSS_FIGHT);
				//_lvlBosse = 0;
				if (_lvlBosse == 0)
				{
					AddArmy(TYPE_ENEMY_BOSS_1, k_SCREEN_WIDTH + 200, k_SCREEN_HEIGHT / 2 + 100, CHARATER_STATE_INIT_BOSS, ENEMY_INIT_BY_NORMAL);
				}
				else if (_lvlBosse == 2)
				{
					AddArmy(TYPE_ENEMY_BOSS_3, k_SCREEN_WIDTH + 200, k_SCREEN_HEIGHT / 2 + 100, CHARATER_STATE_INIT_BOSS, ENEMY_INIT_BY_NORMAL);
				}
			}
			break;
		case 2:
			_bossIncomingX += 50;
			if (_bossIncomingX >= k_SCREEN_WIDTH)
			{
				_bossIncomingX = k_SCREEN_WIDTH;
				_stateBossIncoming = 3;				
				_addBoss = 2;
			}
			break;
		}		
	}

	//update enemy
	for (int i = 0; i < _numArmy; i++)
	{
		if (_army[i] != NULL)
		{
			_army[i]->Update();
		}
	}

	for (int j = 0; j < _numArmy; j++)
	{
		if (_army[j] != NULL)
		{
			int min = -50;
			if (_army[j]->GetType() >= TYPE_ENEMY_BOSS_1)
			{
				min = -k_SCREEN_WIDTH;
			}			
			if (_army[j]->PosX() < min)
			{
				
				SAFE_DEL(_army[j]);
               // delete[] _army[j] ;
				_army[j] = NULL;
                
				if (CHARATER().GetAttackArmy() != NULL)
				{
					CHARATER().SetNullAttack();
				}
				if (CHARATER().GetAttackArmy1() != NULL)
				{
					CHARATER().SetNullAttack1();
				}
				if (CHARATER().GetAttackArmy2() != NULL)
				{
					CHARATER().SetNullAttack2();
				}

				continue;
			}			
			
			if (_army[j]->GetState() == CHARATER_STATE_DIE)
			{
				if (_army[j]->CompleteEffect())
				{					
					if (CHARATER().GetAttackArmy() == BATTLE()._army[j])
					{
						CHARATER().SetNullAttack();
					}
					if (CHARATER().GetAttackArmy1() == BATTLE()._army[j])
					{
						CHARATER().SetNullAttack1();
					}
					if (CHARATER().GetAttackArmy2() == BATTLE()._army[j])
					{
						CHARATER().SetNullAttack2();
					}
					SAFE_DEL(_army[j]);
                   // delete[] _army[j];
					_army[j] = NULL;
				}
			}			
		}
	}

	if (CHARATER().GetTypePower() != POWER_MACHETE)
	{
		if (CHARATER().GetState() != CHARATER_STATE_DIE)
		{
			GetArmyAttack(&MAIN_GAME()->_charater);
		}		
	}
	if (CHARATER().GetTypePower() != POWER_NONE)
	{
		int dieBy = ENEMY_DIE_BY_MACHETE;
		if (CHARATER().GetTypePower() == POWER_MACHETE)
		{
			dieBy = ENEMY_DIE_BY_MACHETE;
		}
		else if (CHARATER().GetTypePower() == POWER_MULE)
		{
			dieBy = ENEMY_DIE_BY_MULE;
		}
		else if (CHARATER().GetTypePower() == POWER_MEDAL)
		{
			dieBy = ENEMY_DIE_BY_CAVLRY;
		}

		for (int i = 0; i < _numArmy; i++)
		{
			if (_army[i] != NULL  && _army[i]->GetType() < TYPE_ENEMY_BOSS_1)
			{
				if (CHECK_POINT_IN_CIRCLE(_army[i]->PosX(),_army[i]->PosY(),CHARATER().PosX(),CHARATER().PosY(),150))
				{
					if (_army[i]->GetState() != CHARATER_STATE_DIE && _army[i]->GetState() != CHARATER_STATE_BE_ATTACK)
					{
						PDEBUG("\n die by Power");
						//BATTLE().AddCoin(1, _army[i]->PosX(), _army[i]->PosY() - 40, 0, 75 + CMath::RANDOM(-5, 3), COIN_STATE_INIT_FALL, _army[i]->PosX() + CMath::RANDOM(30, 70), _army[i]->PosY() + CMath::RANDOM(-30, 0));
						_army[i]->SetState(CHARATER_STATE_DIE);
						_army[i]->SetDieBy(dieBy);

						//set Achivement
						ACHIEVEMENT()._numEnemyDie++;
						ACHIEVEMENT()._totalEnemyDie++;
						if (_army[i]->GetInitBy() == ENEMY_INIT_BY_BARREL)
						{
							ACHIEVEMENT()._numEnemyJumpBarrelDie++;
							ACHIEVEMENT()._totalEnemyJumpBarrelDie++;
							ACHIEVEMENT()._numDailyEnemyJumpBarrel++;
						}


					}					
				}
			}
		}
	}
}

void CBattle::UpdateObstacle()
{
	//update obstacle
	for (int i = 0; i < _numObstacle; i++)
	{
		if (_obstacle[i] != NULL)
		{


			_obstacle[i]->_x -= BATTLE_MAP()._speed_scroll_map;
			_obstacle[i]->_activeX -= BATTLE_MAP()._speed_scroll_map;
			if (_obstacle[i]->_state != OBSTACLE_STATE_INIT_FALL)
			{
				_obstacle[i]->_animObstacle.SetPos(_obstacle[i]->_x, _obstacle[i]->_y + _vibrateY);
			}
			else
			{
				_obstacle[i]->_animObstacle.SetPos(_obstacle[i]->_x, _obstacle[i]->_y);
			}
			
			_obstacle[i]->_animExplosion.SetPos(_obstacle[i]->_x + 30, _obstacle[i]->_y - 20 );
            
			//if (_obstacle[i]->_type == OBSTACLE_BARREL)
			{
				//G()->SetColor(0xaa0000ff);
				//G()->FillRect(_obstacle[i]->_x + _obstacle[i]->_Cx, _obstacle[i]->_y + _obstacle[i]->_Cy - 20, _obstacle[i]->_Cw, _obstacle[i]->_Ch +20,true);
			}
			//PDEBUG("\n_obstacle[%d]->_state:%d", i,_obstacle[i]->_state);
			switch (_obstacle[i]->_state)
			{
			case OBSTACLE_STATE_INIT_FALL:
			{		
		
				_obstacle[i]->_t0 += 16;// DT();

				float ft = _obstacle[i]->_t0 / 20.0f;
				float x = _obstacle[i]->_v0 * CMath::COS(_obstacle[i]->_angle)*ft;
				float y = _obstacle[i]->_v0 * CMath::SIN(_obstacle[i]->_angle)*ft - _obstacle[i]->_g*ft*ft*0.5f;

				
				if (_obstacle[i]->_direcBome == _LEFT)
				{
					_obstacle[i]->_x = _obstacle[i]->_x0 - x;
				}
				else
				{
					_obstacle[i]->_x = _obstacle[i]->_x0 + x;
				}				
				_obstacle[i]->_y = _obstacle[i]->_y0 - y;
				
				if (_obstacle[i]->_y > _obstacle[i]->_targety0 && _obstacle[i]->_t0 > 50)
				{
					_obstacle[i]->_count++;
					_obstacle[i]->_g = 25;
					_obstacle[i]->_t0 = 0;
					_obstacle[i]->_v0 *= 0.5;
					_obstacle[i]->_y0 = _obstacle[i]->_y;
					_obstacle[i]->_x0 = _obstacle[i]->_x;
						
				}
				if (_obstacle[i]->_count > 1)
				{
					_obstacle[i]->_state = OBSTACLE_STATE_ACTIVE;
					_obstacle[i]->_animObstacle.SetAnim(GetIndexAnimObstacle(_obstacle[i]->_type) * 3 + 1, true);
					_obstacle[i]->_countFrame = 0;

					if (_obstacle[i]->_type == OBSTACLE_CHESS)
					{
						_obstacle[i]->_animObstacle.SetAnim(1,false);
					}
				}
				//printf("\n rangeMax = %f", (2 * _obstacle[i]->_v0*_obstacle[i]->_v0*CMath::SIN(_obstacle[i]->_angle)*CMath::COS(_obstacle[i]->_angle)) / _obstacle[i]->_g);
			}
				
				break;
			case OBSTACLE_STATE_INIT:
			{
				bool collision = false;
				if (checkRectOverlapRect(CHARATER().ColMoveX(), CHARATER().ColMoveY(), CHARATER().ColMoveW(), CHARATER().ColMoveH(),
					_obstacle[i]->_Cx + _obstacle[i]->_x, _obstacle[i]->_Cy + _obstacle[i]->_y, _obstacle[i]->_Cw, _obstacle[i]->_Ch))
				{
					collision = true;
					_obstacle[i]->_isCharater = true;
					_obstacle[i]->_activeBy = OBSTACLE_ACTIVE_BY_CHARACTER;
				}

				if (CHARATER().GetTypePower() == POWER_MACHETE && CHECK_POINT_IN_CIRCLE(_obstacle[i]->_x, _obstacle[i]->_y, CHARATER().PosX(), CHARATER().PosY(), 150))
				{
					collision = true;
					AddCoin(1, _obstacle[i]->_x, _obstacle[i]->_y, 0, CMath::RANDOM(0, 360));
					if (_obstacle[i]->_type != OBSTACLE_BARREL)
					{
						_obstacle[i]->_animObstacle.SetAnim(GetIndexAnimObstacle(_obstacle[i]->_type) * 3 + 2, true);
						_obstacle[i]->_countFrame = 0;
						_obstacle[i]->_state = OBSTACLE_STATE_EXPLOSION;
						_obstacle[i]->_activeBy = OBSTACLE_ACTIVE_BY_CHARACTER;
						break;
					}
					
				}


				//rocket
				for (int j = 0; j < _numRocket; j++)
				{
					if (_rocket[j] != NULL && _rocket[j]->_state == ROCKET_STATE_MOVE)
					{
						if (_rocket[j]->_x < k_SCREEN_WIDTH)
						{
							if (checkRectOverlapRect(_rocket[j]->_x, _rocket[j]->_y - 10, 10, 20,
								_obstacle[i]->_Cx + _obstacle[i]->_x, _obstacle[i]->_Cy + _obstacle[i]->_y - 20, _obstacle[i]->_Cw, _obstacle[i]->_Ch + 20))
							{                              
								collision = true;
								_obstacle[i]->_activeBy = OBSTACLE_ACTIVE_BY_ROCKET;
								break;
							}
						}
					}
				}


				for (int j = 0; j < _numArmy; j++)
				{
					if (_army[j] != NULL && _army[j]->GetType() == TYPE_ENEMY_BOSS_1)
					{
						if (_army[j]->GetState() != CHARATER_STATE_DIE && _army[j]->GetState() != CHARATER_STATE_BE_ATTACK)
						{

							if (checkRectOverlapRect(_army[j]->CollisionX(), _army[j]->CollisionY(), _army[j]->CollisionW(), _army[j]->CollisionH(),
								_obstacle[i]->_Cx + _obstacle[i]->_x, _obstacle[i]->_Cy + _obstacle[i]->_y - 30, _obstacle[i]->_Cw, _obstacle[i]->_Ch + 30))
							{
								collision = true;
								_obstacle[i]->_activeBy = OBSTACLE_ACTIVE_BY_CHARACTER;
								break;
							}
						}

					}
				}
			
				for (int j = 0; j < _numCavalry; j++)
				{					
					if (_cavalry[j] != NULL)
					{
						if (checkRectOverlapRect(_cavalry[j]->CollisionX(), _cavalry[j]->CollisionY(), _cavalry[j]->CollisionW(), _cavalry[j]->CollisionH(),
							_obstacle[i]->_Cx + _obstacle[i]->_x, _obstacle[i]->_Cy + _obstacle[i]->_y, _obstacle[i]->_Cw, _obstacle[i]->_Ch))
						{
							collision = true;
							_obstacle[i]->_isCharater = true;
							_obstacle[i]->_activeBy = OBSTACLE_ACTIVE_BY_CHARACTER;
						}
					}					
				}


				if (collision)
				{
					_obstacle[i]->_state = OBSTACLE_STATE_ACTIVE;
					_obstacle[i]->_animObstacle.SetAnim(GetIndexAnimObstacle(_obstacle[i]->_type) * 3 + 1, true);
					_obstacle[i]->_countFrame = 0;

					if (_obstacle[i]->_type == OBSTACLE_BOME)
					{
						//set Achivement
						ACHIEVEMENT()._numBombActive++;
						ACHIEVEMENT()._totalBombActive++;
						ACHIEVEMENT()._numDailyBombActive++;
					}
					else if (_obstacle[i]->_type == OBSTACLE_TNT)
					{
                        /*for (int k = 0; k < 2; k++)
                        {
							PDEBUG("\n TNT Active");
                             AddCoin(1, _obstacle[i]->_x, _obstacle[i]->_y, 0,CMath::RANDOM(0, 360));
                        }*/
						//AddCoin(1, _obstacle[i]->_x, _obstacle[i]->_y, 0, CMath::RANDOM(0, 360));
						SetVibrate();
						_obstacle[i]->_animExplosion.SetAnimSprite(GET_SPRITE(SPRITEID_EFFECT_EXPLOSION), SPRITEID_EFFECT_EXPLOSION);
						_obstacle[i]->_animExplosion.SetAnim(0, false);
						GAME()->PlaySFX(SFX_BOMB);
					}
					else if (_obstacle[i]->_type == OBSTACLE_BARREL)
					{						
						_obstacle[i]->_animObstacle.SetAnim(GetIndexAnimObstacle(_obstacle[i]->_type) * 3 + 1, false);	
						_obstacle[i]->_countFrame = 0;

						if(_obstacle[i]->_activeBy == OBSTACLE_ACTIVE_BY_ROCKET)
						{
							//set Achivement
							ACHIEVEMENT()._numRocketOpenBarrel++;
							ACHIEVEMENT()._totalRocketOpenBarrel++;
							ACHIEVEMENT()._numDailyRocketByBarrel++;
						}
                        
                        if (_obstacle[i]->_isCharater)
                        {
                            if (CHARATER().GetTypePower() == POWER_NONE)
                            {
                                if (!CHARATER().IsUndead() && CHARATER().GetState() != CHARATER_STATE_JUMP  && CHARATER().GetState() != CHARATER_STATE_DIE)
                                {
                                    CHARATER().BeDame(1);
                                    if (CHARATER().GetState() == CHARATER_STATE_DIE)
                                    {
                                        _obstacle[i]->_renderTop = true;
                                    }
                                }
                            }                            
                        }
						
					}
					AddCoin(1, _obstacle[i]->_x, _obstacle[i]->_y, 0, CMath::RANDOM(0, 360));
				}

				if (_obstacle[i]->_type == OBSTACLE_BARREL)
				{
					if (CHARATER().PosX() >= _obstacle[i]->_activeX)
					{
						_obstacle[i]->_state = OBSTACLE_STATE_ACTIVE;
						_obstacle[i]->_animObstacle.SetAnim(GetIndexAnimObstacle(_obstacle[i]->_type) * 3 + 1, false);
						_obstacle[i]->_countFrame = 0;
					}
				}
			}
				break;
			case OBSTACLE_STATE_COLLECT:
				_obstacle[i]->_countFrame++;
				if (_obstacle[i]->_countFrame > 5)
				{
					PDEBUG("\n Obstacle destroy");
					AddCoin(1, _obstacle[i]->_x + CMath::RANDOM(-5, 5), _obstacle[i]->_y - 30 + CMath::RANDOM(-5, 5), 0, 0, COIN_STATE_COLLECT);
					_obstacle[i]->_numCoin--;
					_obstacle[i]->_countFrame = 0;
				}
				if (_obstacle[i]->_numCoin<= 0)
				{
					SAFE_DEL(_obstacle[i]);
					//delete[] _obstacle[i];
					_obstacle[i] = NULL;
				}
				break;
			case OBSTACLE_STATE_ACTIVE:	
				if (_obstacle[i]->_type == OBSTACLE_CHESS)
				{
					if (_obstacle[i]->_animObstacle.IsCurrentAnimStop())
					{
						_obstacle[i]->_animObstacle.SetAnim(2, true);
					}

					if (checkRectOverlapRect(CHARATER().ColMoveX(), CHARATER().ColMoveY(), CHARATER().ColMoveW(), CHARATER().ColMoveH(),
						_obstacle[i]->_Cx + _obstacle[i]->_x, _obstacle[i]->_Cy + _obstacle[i]->_y, _obstacle[i]->_Cw, _obstacle[i]->_Ch))
					{
						_obstacle[i]->_state = OBSTACLE_STATE_COLLECT;
						_obstacle[i]->_numCoin = CMath::RANDOM(7, 10);
						_obstacle[i]->_countFrame = 0;
					}
				}
				else if (_obstacle[i]->_type == OBSTACLE_BOME)
				{
					_obstacle[i]->_countFrame++;
					if (_obstacle[i]->_countFrame == 40)
					{
						 int x = GET_SPRITE(SPRITEID_OBSTACLE)->GetFModuleX(7,0);
                         int y = GET_SPRITE(SPRITEID_OBSTACLE)->GetFModuleY(7,0);
                         int w = GET_SPRITE(SPRITEID_OBSTACLE)->GetFModuleWidth(7,0)*2;
                         int h = GET_SPRITE(SPRITEID_OBSTACLE)->GetFModuleHeight(7,0);

						 for (int j = 0; j < _numObstacle; j++)
						 {
							 if (_obstacle[j] != NULL && _obstacle[j]->_state == OBSTACLE_STATE_INIT && i != j)
							 {
								 if (checkRectOverlapRect(_obstacle[i]->_x + x, _obstacle[i]->_y + y, w, h, 
									 _obstacle[j]->_Cx + _obstacle[j]->_x, _obstacle[j]->_Cy + _obstacle[j]->_y - 30, _obstacle[j]->_Cw, _obstacle[j]->_Ch + 30))
								 {
									 _obstacle[i]->_activeBy = OBSTACLE_ACTIVE_BY_BOMB;
									 _obstacle[j]->_state = OBSTACLE_STATE_ACTIVE;
									 if (_obstacle[j]->_type == OBSTACLE_BARREL)
									 {
										 _obstacle[j]->_animObstacle.SetAnim(GetIndexAnimObstacle(_obstacle[j]->_type) * 3 + 1, false);
										 //set Achivement
										 ACHIEVEMENT()._numBombOpenBarrel++;
										 ACHIEVEMENT()._totalBombOpenBarrel++;
									 }
									 else if (_obstacle[j]->_type == OBSTACLE_BOME)
									 {
										 //set Achivement
										 ACHIEVEMENT()._numBombActive++;
										 ACHIEVEMENT()._totalBombActive++;
										 ACHIEVEMENT()._numDailyBombActive++;
									 }
									 else
									 {
										 _obstacle[j]->_animObstacle.SetAnim(GetIndexAnimObstacle(_obstacle[j]->_type) * 3 + 1, true);
									 }
									 
									 _obstacle[j]->_countFrame = 0;
								 }								
							 }
						 }
						
						if (CHECK_POINT_IN_RECT(CHARATER().PosX(), CHARATER().PosY(), _obstacle[i]->_x + x, _obstacle[i]->_y + y, w,h))
						{
							if (CHARATER().GetTypePower() == POWER_NONE)
							{
								if (!CHARATER().IsUndead() && CHARATER().GetState() != CHARATER_STATE_JUMP && CHARATER().GetState() != CHARATER_STATE_DIE)
								{
									CHARATER().BeDame(1);
									if (CHARATER().GetState() == CHARATER_STATE_DIE)
									{
										_obstacle[i]->_renderTop = true;
									}
								}
							}
						}

						for (int j = 0; j < _numArmy; j++)
						{
							if (_army[j] != NULL && _army[j]->GetType() < TYPE_ENEMY_BOSS_1)
							{
								if (CHECK_POINT_IN_RECT(_army[j]->PosX(), _army[j]->PosY(), _obstacle[i]->_x + x, _obstacle[i]->_y + y, w, h))
								{
									if (_army[j]->GetState() != CHARATER_STATE_DIE && _army[j]->GetState() != CHARATER_STATE_BE_ATTACK)
									{
										printf("\n die by BOMB");
										_army[j]->SetState(CHARATER_STATE_DIE);
										_army[j]->SetDieBy(ENEMY_DIE_BY_BOMB);

										//set Achivement
										ACHIEVEMENT()._numEnemyDie++;
										ACHIEVEMENT()._totalEnemyDie++;
										ACHIEVEMENT()._totalEnemyDieByBomb++;

										if (_army[j]->GetInitBy() == ENEMY_INIT_BY_BARREL)
										{
											ACHIEVEMENT()._numEnemyJumpBarrelDie++;
											ACHIEVEMENT()._totalEnemyJumpBarrelDie++;
											ACHIEVEMENT()._numDailyEnemyJumpBarrel++;
										}

										
									}
								}
							}
						}
						PDEBUG("\n Bomb active");
                         AddCoin(1, _obstacle[i]->_x, _obstacle[i]->_y, 0,CMath::RANDOM(0, 360));
						_obstacle[i]->_animExplosion.SetAnimSprite(GET_SPRITE(SPRITEID_EFFECT_EXPLOSION), SPRITEID_EFFECT_EXPLOSION);
						_obstacle[i]->_animExplosion.SetAnim(0, false);
						_obstacle[i]->_state = OBSTACLE_STATE_EXPLOSION;
						_obstacle[i]->_animObstacle.SetAnim(GetIndexAnimObstacle(_obstacle[i]->_type) * 3 + 2, true);
						SetVibrate();
						GAME()->PlaySFX(SFX_BOMB);
					}
				}
				else if (_obstacle[i]->_type == OBSTACLE_BARREL)
				{
					_obstacle[i]->_countFrame++;
					if (_obstacle[i]->_countFrame == 10)
					{
						GAME()->PlaySFX(SFX_BOMB);
						AddArmy(getTypePercent(CMath::RANDOM(1, 100), _lvl_des) - 1, _obstacle[i]->_x, _obstacle[i]->_y + 50, CHARATER_STATE_JUMP, ENEMY_INIT_BY_BARREL);
					}
					if (_obstacle[i]->_animObstacle.IsCurrentAnimStop())
					{
						_obstacle[i]->_animObstacle.SetAnim(GetIndexAnimObstacle(_obstacle[i]->_type) * 3 + 2, true);
						_obstacle[i]->_state = OBSTACLE_STATE_EXPLOSION;
					}

				}
				else /*if (_obstacle[i]->_type != OBSTACLE_BOME)*/
				{
					_obstacle[i]->_countFrame++;
					if (_obstacle[i]->_countFrame == 5)
					{
						if (_obstacle[i]->_isCharater)
						{
							if (CHARATER().GetTypePower() == POWER_NONE)
							{
								if (!CHARATER().IsUndead() && CHARATER().GetState() != CHARATER_STATE_JUMP  && CHARATER().GetState() != CHARATER_STATE_DIE)
								{
									CHARATER().BeDame(1);
									if (CHARATER().GetState() == CHARATER_STATE_DIE)
									{
										_obstacle[i]->_renderTop = true;
									}
								}
							}

						}
						_obstacle[i]->_state = OBSTACLE_STATE_EXPLOSION;
						_obstacle[i]->_animObstacle.SetAnim(GetIndexAnimObstacle(_obstacle[i]->_type) * 3 + 2, true);
					}
				}
				break;
			case OBSTACLE_STATE_EXPLOSION:
				_obstacle[i]->_opacity -= 1;
				if (_obstacle[i]->_opacity <= 0)
				{
					_obstacle[i]->_opacity = 0;
				}
				//PDEBUG("\n explosion ---- CHARATER().GetTypePower():%d", CHARATER().GetTypePower());
				break;
			}

			if (_obstacle[i] != NULL && _obstacle[i]->_x <= -k_SCREEN_WIDTH / 2)
			{
				SAFE_DEL(_obstacle[i]);
				_obstacle[i] = NULL;
				//delete[] _obstacle[i];
			}
			if (_obstacle[i]->_stateOld != _obstacle[i]->_state)
			{
				_obstacle[i]->_stateOld = _obstacle[i]->_state;
			}
		}
	}
}
void CBattle::UpdateRocket()
{
	
	int nextR = 0;
	if (USER()._timePlay == 0)
	{
		nextR = 100;
	}
	//int
	if (_distanceRun >= nextR  && _timeBegin <= 0 && _distanceRun >= _distanceRocket && !_changeDay)
	{
		if (_addBoss == 0 && _countRocket < _numRocketInit)
		{
			_timeDeplayRocket  --;
			if (_timeDeplayRocket <= 0 && !_isRunRocket)
			{
				_isRunRocket = true;
				int ran = CMath::RANDOM(0, _numRandRocket - 1);
				int type =  _randomTypeRocket[ran];
				int num = GET_SPRITE(SPRITEID_FROM_ROCKET)->GetModuleOfFrame(type);
                int opacity = GET_SPRITE(SPRITEID_FROM_ROCKET)->GetFModuleOpacity(type, 0);
                int offsetY = MIN_MOVE_Y;
                if (opacity != 100)
                {
                    int H = Button::GetFrameHeight(GET_SPRITE(SPRITEID_FROM_ROCKET), type);
                    offsetY = CMath::RANDOM(MIN_MOVE_Y, MAX_MOVE_Y);
                    if(offsetY + H > MAX_MOVE_Y)
                    {
                        offsetY =MAX_MOVE_Y-H;
                    }
                }
                
				BYTE state = ROCKET_STATE_INIT_GUN;
				if (num == 1)
				{
					state = ROCKET_STATE_INIT_FOLLOW;
				}
				_maxTimeRocket = 0;
				for (int  i = 0; i < num; i++)
				{
					int timeGun = GET_SPRITE(SPRITEID_FROM_ROCKET)->GetFModuleX(type,i)/1000.0f * 60;
					if (_maxTimeRocket < timeGun)
					{
						_maxTimeRocket = timeGun;
					}
					int y = offsetY + GET_SPRITE(SPRITEID_FROM_ROCKET)->GetFModuleY(type, i);
					AddRocket(1, k_SCREEN_WIDTH, y, state, timeGun);
				}
				_countRocket++;
				_distanceRocket = _distanceRun + _distanceRocketW;
			}		
			_maxTimeRocket--;
			if (_isRunRocket && _maxTimeRocket <= 0)
			{
				if (CheckRocketComplete())
				{
					_isRunRocket = false;
					_timeDeplayRocket =  GET_DES().getLevelDes(_lvl_des, LVL_DES_TIME_MIN_RELEASE_ROCKET) * 60;
				}
			}
		}		
	}

	if (CHARATER().PosX() >= k_SCREEN_WIDTH - 500)
	{
		if (_distanceRun >= nextR && _timeBegin <= 0 && !_changeDay)
		{
			if (_addBoss == 0)
			{
				_timeDeplayRocket--;
				if (_timeDeplayRocket <= 0 && !_isRunRocket)
				{
					AddRocket(1, k_SCREEN_WIDTH, 0, ROCKET_STATE_INIT_FOLLOW, 0);
					_isRunRocket = true;				
				}

				if (_isRunRocket)
				{
					if (CheckRocketComplete())
					{
						_isRunRocket = false;
						_timeDeplayRocket = GET_DES().getLevelDes(_lvl_des, LVL_DES_TIME_MIN_RELEASE_ROCKET) * 60;
					}
				}
			}
		}
	}
	//printf("\n _numRocket = %d", _numRocket);

	//update rocket
	for (int i = 0; i < _numRocket; i++)
	{
		if (_rocket[i] != NULL)
		{
			//G()->SetColor(0xff0000);
			//G()->FillRect( _rocket[i]->_x, _rocket[i]->_y, 8, 8);
			_rocket[i]->_timeGun --;
			if (_rocket[i]->_timeGun <= 0)
			{
				//printf("\n rrrrrrrrrrr = %d", _rocket[i]->_countFrame);
				_rocket[i]->_countSmoke++;
				if (_rocket[i]->_state == ROCKET_STATE_INIT_FOLLOW)
				{
					if (_rocket[i]->_countFrame<35)
					{
						if (_rocket[i]->_countFrame % 10 == 0)
						{
							GAME()->PlaySFX(SFX_WARNING);
						}
					}
					else if (_rocket[i]->_countFrame< 70)
					{
						if (_rocket[i]->_countFrame % 6 == 0)
						{
							GAME()->PlaySFX(SFX_WARNING);
						}
					}
					else 
					{
						if (_rocket[i]->_countFrame % 2 == 0)
						{
							GAME()->PlaySFX(SFX_WARNING);
						}
					}
					_rocket[i]->_countFrame++;


					if (_rocket[i]->_countSmoke < 60)
					{
						_rocket[i]->_y = CHARATER().PosY() - 40;
						_rocket[i]->_animAlert.SetPos(k_SCREEN_WIDTH - ALERT_ROCKET_X, _rocket[i]->_y);
					}

					if (_rocket[i]->_animAlert.IsCurrentAnimStop())
					{
						_rocket[i]->_state = ROCKET_STATE_MOVE;
						_rocket[i]->_countSmoke = 0;
						GAME()->PlaySFX(SFX_MISSILES);
					}
				}
				else if (_rocket[i]->_state == ROCKET_STATE_INIT_GUN)
				{
					if (_rocket[i]->_countFrame < 35)
					{
						if (_rocket[i]->_countFrame % 10 == 0)
						{
							GAME()->PlaySFX(SFX_WARNING);
						}
					}
					else if (_rocket[i]->_countFrame < 70)
					{
						if (_rocket[i]->_countFrame % 6 == 0)
						{
							GAME()->PlaySFX(SFX_WARNING);
						}
					}
					else
					{
						if (_rocket[i]->_countFrame % 3 == 0)
						{
							GAME()->PlaySFX(SFX_WARNING);
						}
					}
					_rocket[i]->_countFrame++;
					_rocket[i]->_animAlert.SetPos(k_SCREEN_WIDTH - ALERT_ROCKET_X, _rocket[i]->_y);
					if (_rocket[i]->_animAlert.IsCurrentAnimStop())
					{
						_rocket[i]->_state = ROCKET_STATE_MOVE;
						_rocket[i]->_countSmoke = 0;
						GAME()->PlaySFX(SFX_MISSILES);
					}
				}
				else if (_rocket[i]->_state == ROCKET_STATE_MOVE)
				{
					if (CHARATER().PosX() >= k_SCREEN_WIDTH / 2)
					{
						_rocket[i]->_x -= 60;
					}
					else
					{
						_rocket[i]->_x -= 40;
					}

					if (CHECK_POINT_IN_RECT(_rocket[i]->_x, _rocket[i]->_y, CHARATER().CollisionX(), CHARATER().CollisionY(), CHARATER().CollisionW(), CHARATER().CollisionH()))
					{
						if (!CHARATER().IsUndead() && CHARATER().GetState() != CHARATER_STATE_JUMP && CHARATER().GetState() != CHARATER_STATE_DIE)
						{
							GAME()->PlaySFX(SFX_BOMB);
							_rocket[i]->_animAlert.SetAnimSprite(GET_SPRITE(SPRITEID_EFFECT_EXPLOSION), SPRITEID_EFFECT_EXPLOSION);
							_rocket[i]->_animAlert.SetAnim(0, false);
							_rocket[i]->_animAlert.SetPos(_rocket[i]->_x, _rocket[i]->_y);
							SetVibrate();
							_rocket[i]->_state = ROCKET_STATE_EXPLOSION;

							if (CHARATER().GetTypePower() == POWER_NONE)
							{
								CHARATER().BeDame(1);		
								if (CHARATER().GetState() == CHARATER_STATE_DIE)
								{
									_rocket[i]->_renderTop = true;
									_rocket[i]->_animAlert.SetSubTime(12);
								}
							}
						}
					}

					for (int j = 0; j < _numArmy; j++)
					{
						if (_army[j] != NULL)
						{
							if (CHECK_POINT_IN_RECT(_rocket[i]->_x, _rocket[i]->_y, _army[j]->CollisionX(), _army[j]->CollisionY(), _army[j]->CollisionW(), _army[j]->CollisionH()))
							{
								if (_army[j]->GetState() != CHARATER_STATE_DIE && _army[j]->GetState() != CHARATER_STATE_BE_ATTACK)
								{
									if (_army[j]->GetType() < TYPE_ENEMY_BOSS_1)
									{
										printf("\n die by Rocket");
										_army[j]->SetState(CHARATER_STATE_DIE);
										_army[j]->SetDieBy(ENEMY_DIE_BY_ROCKET);

										//set Achivement
										ACHIEVEMENT()._numEnemyDie++;
										ACHIEVEMENT()._totalEnemyDie++;
										ACHIEVEMENT()._totalEnemyDieByRocket++;

										if (_army[j]->GetInitBy() == ENEMY_INIT_BY_BARREL)
										{
											ACHIEVEMENT()._numEnemyJumpBarrelDie++;
											ACHIEVEMENT()._totalEnemyJumpBarrelDie++;
											ACHIEVEMENT()._numDailyEnemyJumpBarrel++;
										}
										
									}
								}
							}
						}
					}
				}
				else if (_rocket[i]->_state == ROCKET_STATE_EXPLOSION)
				{
					_rocket[i]->_x = CHARATER().PosX();
					_rocket[i]->_animAlert.SetPos(_rocket[i]->_x, _rocket[i]->_y);
                    if(_rocket[i]->_animAlert.IsCurrentAnimStop())
                    {
                        SAFE_DEL(_rocket[i]);
                        _rocket[i] = NULL;
                        //delete[] _rocket[i];
                    }
				}

				if (_rocket[i] != NULL && _rocket[i]->_x <= -100)
				{
					SAFE_DEL(_rocket[i]);
					_rocket[i] = NULL;
					//delete[] _rocket[i];
				}
				
			}
		}
	}
}
void CBattle::UpdatePower()
{
	
	for (int i = 0; i < _numPower; i++)
	{
		if (_power[i] != NULL)
		{
			if (_power[i]->_type == POWER_MACHETE)
			{
				_power[i]->_cX = GET_SPRITE(SPRITEID_HUD_MACHETE)->GetFModuleX(0, 0) + _power[i]->_x;
				_power[i]->_cY = GET_SPRITE(SPRITEID_HUD_MACHETE)->GetFModuleY(0, 0) + _power[i]->_y;
			}
			else if (_power[i]->_type == POWER_MULE)
			{
				_power[i]->_cX = GET_SPRITE(SPRITEID_HUD_MULE)->GetFModuleX(0, 0) + _power[i]->_x;
				_power[i]->_cY = GET_SPRITE(SPRITEID_HUD_MULE)->GetFModuleY(0, 0) + _power[i]->_y;
			}
			else if (_power[i]->_type == POWER_MEDAL)
			{
				_power[i]->_cX = GET_SPRITE(SPRITEID_HUD_MEDAL)->GetFModuleX(0, 0) + _power[i]->_x;
				_power[i]->_cY = GET_SPRITE(SPRITEID_HUD_MEDAL)->GetFModuleY(0, 0) + _power[i]->_y;
			}
		
			_power[i]->_x -= BATTLE_MAP()._speed_scroll_map;
			_power[i]->_animPower.SetPos(_power[i]->_x, _power[i]->_y);
			if (checkRectOverlapRect(CHARATER().ColMoveX(), CHARATER().ColMoveY(), CHARATER().ColMoveW(), CHARATER().ColMoveH(), _power[i]->_cX, _power[i]->_cY, _power[i]->_cW, _power[i]->_cH))
			{
				/*if (_addtext)
				{
					if (_power[i]->_type == power_mule)
					{
						effect().addeffecttextfly(k_screen_width / 2, k_screen_height / 2, "donkey shrek is unlocked!");
					}
					else if (_power[i]->_type == power_medal)
					{
						effect().addeffecttextfly(k_screen_width / 2, k_screen_height / 2, "cavalry is unlocked!");
					}
				}*/

				CHARATER().SetTypePower(_power[i]->_type);	
				//set Achivement
				ACHIEVEMENT()._numPowerUse++;
				ACHIEVEMENT()._numDailyUsePowerUp++;
				if (_power[i]->_type== POWER_MEDAL)
				{
					AddCavalry(5);
				}
				SAFE_DEL(_power[i]);
				_power[i] = NULL;
				//delete[] _power[i];
			}
		}		
	}
}

bool CBattle::CheckRocketComplete()
{
	bool res = true;
	for (int i = 0; i < _numRocket; i++)
	{
		if (_rocket[i] != NULL)
		{ 
			if (_rocket[i]->_state != ROCKET_STATE_EXPLOSION)
			{				
				res = false;
			}
		}
	}
	return res;
}

void CBattle::Update()
{		
	//ACHIEVEMENT().CheckAchievementComplete();
    //printf("\n _speedIncreaseMap = %f",_speedIncreaseMap);

	if (_addTutorial)
	{
		MAIN_GAME()->SetState(k_GS_TUTORIAL);
		_addTutorial = false;
	}
	_timePlay ++;
	if (_timeBegin > 0)
	{
		_timeBegin --;
		if (_timeBegin <= 0)
		{
			CHARATER().SetTypePower(POWER_NONE);
			if (_lvlBosse == 1)
			{				
				BATTLE_MAP()._beginRail = _checkPointX + 200;
				BATTLE_MAP().InitRail();
				AddArmy(TYPE_ENEMY_BOSS_2, BATTLE_MAP()._beginRail + 100, TRAIN_Y, CHARATER_STATE_INIT_BOSS, ENEMY_INIT_BY_NORMAL);
			}
			
		}
		if (_timeBegin <= 1*60)
		{
			_speedIncreaseMap -= 0.4;
			if (_speedIncreaseMap <= 0)
			{
				_speedIncreaseMap = 0;
			}
		}
		
	}
	else
	{
		_checkPointX -= BATTLE_MAP()._speed_scroll_map;
	}
    
	
	BATTLE_MAP().UpdateBattleMap_1();	
	BATTLE_MAP().UpdateBattleMap_2();	
	BATTLE_MAP().UpdateRail();
	BATTLE_MAP().UpdateSwapBG();
	
	
    if (_timeBegin <= 0) 
	{
		if (_isCalculateDistance)
		{
			_distanceRun += BATTLE_MAP()._speed_scroll_map / DELTA_DISTANCE;
			_currentDistance += BATTLE_MAP()._speed_scroll_map / DELTA_DISTANCE;
		}
		if (MAIN_GAME()->_playWitchEndlest)
		{
			_scoreRun += BATTLE_MAP()._speed_scroll_map / DELTA_DISTANCE;
		}
		USER()._score = _scoreRun*_mullScore;
		//printf("\nUSER()._score=%d ", USER()._score);
    }	
	
 	if (GetArmyBoss(TYPE_ENEMY_BOSS_2) != NULL)
	{
		
		if (_distanceRun >= _endTrain - 100 && _lvlBosse == 1)
		{
			_lvlBosse++;
			_isEndTrain = true;
		}
	}
	
	if (_changeDay)
	{
		if (_checkPointX < k_SCREEN_WIDTH)
		{
			if (_isDay)
			{
				EFFECT().SetStateEffect(EFFECT_MANAGER_TYPE_PATICAL_RAIN, 2);
				EFFECT().AddParticalDust(SPRITEID_EFFECT_DUST, 4, 5, CHARATER_MOVE_DOWN_LEFT, 345, 360);

			}
			else
			{
				EFFECT().SetStateEffect(EFFECT_MANAGER_TYPE_PATICAL_DUST, 2);
				EFFECT().AddParticalRain(1);
			}
			_changeDay = false;
		}		
	}

	if (_isDay)
	{
		if (_currentDistance <_maxDistance / 2)
		{
			_opacityDay = 0;
			_colorDay = 0x000000ff;
		}
		else
		{
			int old = _opacityDay;
			_opacityDay = (_currentDistance - _maxDistance / 2) * 100 / (_maxDistance / 2);
			if (_opacityDay > old)
			{
				if (_colorDay < 0x700000ff)
				{
					_colorDay += 0x01000000;
					if (_colorDay >= 0x700000ff)
					{
						_colorDay = 0x700000ff;
					}
				}
			}			
		}		
	}
	else
	{
		if (_currentDistance <_maxDistance / 2)
		{
			_opacityDay = 100;
			_colorDay = 0x700000ff;
		}
		else
		{
			int old = _opacityDay;
			_opacityDay = 100 - (_currentDistance - _maxDistance / 2) * 100 / (_maxDistance / 2);
			if (old < _opacityDay)
			{
				if (_colorDay > 0x000000ff)
				{
					_colorDay -= 0x01000000;
					if (_colorDay <= 0x000000ff)
					{
						_colorDay = 0x000000ff;
					}
				}
			}
			
		}
		
	}
	//printf("\n _opacityRain = %d", _opacityRain);
	// init from design
	SetDistanceInitArmy();
	//printf("\n e = %d, o = %d, c = %d", NumRenderEnemy(), NumRenderOBS(), NumRenderCOIN());
	if (_timeBegin <= 0)
	{
		int lvl = GET_DES().getLevlCheckPointDes(_distanceRun - 5);
		if (lvl > _lvlCheckPiont)
		{
			_lvlCheckPiont = lvl;
			SetAnimCheckPoint(lvl);
			if (_lvlCheckPiont >= TOTAL_CHECK_POINT_DES_ROW - 1)
			{
				MAIN_GAME()->_playWitchEndlest = true;
				TUTORIAL()._step = 10;
				MAIN_GAME()->SetState(k_GS_TUTORIAL);
			}
			else
			{
				if (USER()._stateBoss == BOSS_STATE_NONE)
				{
					BATTLE()._isCalculateDistance = false;
					BATTLE_MAP().SetSwapBG();

					
					_checkPointX = k_SCREEN_WIDTH + 1950;
					if (CGame::_this->_device._version == SupportVersion::RESVER_960_640)
					{
						_checkPointX = k_SCREEN_WIDTH + 1950 + 200;
					}
					if (CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
					{
						_checkPointX = k_SCREEN_WIDTH + 1950 + 300;
					}
					

					if (_lvlBosse == 1)
					{
						BATTLE_MAP()._beginRail = _checkPointX + 200;
						BATTLE_MAP().InitRail();
						AddArmy(TYPE_ENEMY_BOSS_2, BATTLE_MAP()._beginRail + 100, TRAIN_Y, CHARATER_STATE_INIT_BOSS, ENEMY_INIT_BY_NORMAL);
					}

					_lvlBosse = GET_DES().getCheckPointDes(_lvlCheckPiont, CHECK_POINT_DES_BOSS_TYPE) - 1;

					_addBoss = 0;

					int bossState = GET_DES().getCheckPointDes(_lvlCheckPiont, CHECK_POINT_DES_BOSS_STATE);
					_distanceBoss = GET_DES().getCheckPointDes(_lvlCheckPiont, CHECK_POINT_DES_BOSS_SPAWN);
					USER()._stateBoss = BOSS_STATE_NONE;

				}
			}
			

			_changeDay = true;

			if (_isDay)
			{				//_opacityRain = 0;
				MAIN_GAME()->_isRain = true;
			}
			_isDay = !_isDay;
			int lvlnew = lvl+1;
			if (lvlnew >= TOTAL_CHECK_POINT_DES_ROW - 1)
			{
				lvlnew = TOTAL_CHECK_POINT_DES_ROW - 1;
			}
			int nextDistance = GET_DES().getCheckPointDes(lvlnew, CHECK_POINT_DES_DISTANCE);
			_currentDistance = 0;
			_maxDistance = nextDistance - _distanceRun;

		}
		if (lvl > USER()._lvlLocation)
		{
			USER()._lvlLocation = lvl;
			USER()._currentLocation = lvl;
			USER().DataSave();
		}
	}
	UpdateIniteMix();
	
	switch (_stateCoin)
	{
	case 0:
		_scaleCoin = 1;
		break;
	case 1:
		_scaleCoin += 0.075;
		if (_scaleCoin>= 1.2f)
		{
			_scaleCoin = 1.2f;
			_stateCoin = 2;
		}		
		break;
	case 2:
		_scaleCoin -= 0.075;
		if (_scaleCoin <= 1.0f)
		{
			_scaleCoin = 1.0f;
			_stateCoin = 0;
		}
		break;
	}

	


	UpdateCavalry();
	UpdateArmy();
	UpdateObstacle();
	UpdateCoin();
	UpdateRocket();
	UpdatePower();
	
	EFFECT().Update();
	
	
	CHARATER().Update();	

    for (int i = 0; i < _numArmy; i++)
    {
        if (_army[i] != NULL )
        {
            if (checkRectOverlapRect(CHARATER().ColMoveX(), CHARATER().ColMoveY(), CHARATER().ColMoveW(), CHARATER().ColMoveH(),
                                        _army[i]->ColMoveX(), _army[i]->ColMoveY(), _army[i]->ColMoveW(), _army[i]->ColMoveH()))
            {             
				if (!CHARATER().IsUndead() && CHARATER().GetState() != CHARATER_STATE_JUMP && CHARATER().GetState() != CHARATER_STATE_DIE)
				{
					if (_army[i]->GetType() == TYPE_ENEMY_BOSS_1 || _army[i]->GetType() == TYPE_ENEMY_BOSS_3)
					{
						if (_army[i]->GetState() != CHARATER_STATE_BOSS_DIE)
						{
							CHARATER().BeDame(1);
							if (CHARATER().GetState() == CHARATER_STATE_DIE)
							{
								_army[i]->_renderTop = true;
							}
						}						
					}					
					else if(CHARATER().GetTypePower() == POWER_NONE)
					{
						CHARATER().BeDame(1);
						if (CHARATER().GetState() == CHARATER_STATE_DIE)
						{
							_army[i]->_renderTop = true;
						}
					}
				}					
            }
        }
    }

	
  
	
    //delete
    for (int i = 0; i < _numArmy; i++)
    {
        if (_army[i] != NULL)
        {
            if(_army[i]->PosX() <= - k_SCREEN_WIDTH)
            {
               
				SAFE_DEL(_army[i]);
				//delete[] _army[i];
				_army[i] = NULL;
            }
        }        
    }
    
    for (int i = 0; i < _numObstacle; i++)
    {
        if (_obstacle[i] != NULL)
        {
			if(_obstacle[i]->_x <= - 200)
			{
				SAFE_DEL(_obstacle[i]);
				//delete[] _obstacle[i];
				_obstacle[i] = NULL;
			} 
        }
    }
    
    for (int i = 0; i < _numCoin; i++)
    {
        if (_coin[i] != NULL)
        {
			if(_coin[i]->_x <= - 200)
			{
				SAFE_DEL(_coin[i]);
				//delete[] _coin[i];
				_coin[i] = NULL;
			}
        }
    }

	UpdateVibrate();
}
void CBattle::Render()
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
	_animManager.Init();
	//render map
	if (BATTLE_MAP()._usedBG_1)
	{
		BATTLE_MAP().RenderBattleMap_1(0, 0);
		BATTLE_MAP().RenderRail(0, TRAIN_Y);
		if (BATTLE_MAP()._stateSwap == 1)
		{
			BATTLE_MAP().RenderBattleMap_2(0, 0);
			BATTLE_MAP().RenderRail(0, TRAIN_Y);
			BATTLE_MAP().RenderSwapBG(0, 0);
		}
		
	}
	
	if (BATTLE_MAP()._usedBG_2)
	{
		BATTLE_MAP().RenderBattleMap_2(0, 0);
		BATTLE_MAP().RenderRail(0, TRAIN_Y);
		if (BATTLE_MAP()._stateSwap == 1)
		{
			BATTLE_MAP().RenderBattleMap_1(0, 0);
			BATTLE_MAP().RenderSwapBG(0, 0);
		}
		
	}
	//printf("\n _opacityRain = %d",GAME()->_opacityRain);
	//if (GAME()->_isRain)
	{
		G()->SetOpacity(_opacityDay);
		G()->SetColor(_colorDay);
		//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LOW_BRIGHTNESS);
		G()->FillFullScreen(true);
		//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
	}
	G()->SetOpacity(100);
	if (_timeBegin <= 0)
	{
		if (_checkPointX >= -k_SCREEN_WIDTH / 2 && _checkPointX <= k_SCREEN_WIDTH + k_SCREEN_WIDTH / 2)
		{			
			GET_SPRITE(SPRITEID_HUD_CHECK_POINT)->DrawFrame(G(), _lvlCheckPiont, _checkPointX, _checkPointY + _vibrateY);
		}
	}

	//render obstacle
	for (int i = 0; i < _numObstacle; i++)
	{
		if (_obstacle[i] != NULL)
		{
			if (_obstacle[i]->_state != OBSTACLE_STATE_INIT_FALL)
			{
				
				_animManager.Add(&_obstacle[i]->_animObstacle, _obstacle[i]->_opacity, _obstacle[i]->_animObstacle._x, _obstacle[i]->_animObstacle._y);
				//_obstacle[i]->_animObstacle.DrawAnimObject(G());
				_obstacle[i]->_animObstacle.UpdateAnimObject();
			
			}
			
		}
	}    
	
	//render Power
	for (int i = 0; i < _numPower; i++)
	{
		if (_power[i] != NULL)
		{				
			if (_power[i]->_type == POWER_MACHETE )
			{
				GET_SPRITE(SPRITEID_HUD_MACHETE)->DrawFrame(G(), 1, _power[i]->_x, _power[i]->_y);
				
			}
			else if (_power[i]->_type == POWER_MULE )
			{
				GET_SPRITE(SPRITEID_HUD_MULE)->DrawFrame(G(), 1, _power[i]->_x, _power[i]->_y);
			}
			else if (CHARATER().GetTypePower() == POWER_MEDAL)
			{
				GET_SPRITE(SPRITEID_HUD_MEDAL)->DrawFrame(G(), 1, _power[i]->_x, _power[i]->_y);
			}
			_animManager.Add(&_power[i]->_animPower,100, _power[i]->_animPower._x, _power[i]->_animPower._y);
			//_power[i]->_animPower.DrawAnimObject(G());
			_power[i]->_animPower.UpdateAnimObject();
			
		}
	}

	//render coin
	for (int i = 0; i < _numCoin; i++)
	{
		if (_coin[i] != NULL)
		{
			if (_coin[i]->_state != COIN_STATE_COLLECT)
			{
				if (_coin[i]->_state == COIN_STATE_INIT)
				{
					if (_coin[i]->_type == 1)
					{
						GET_SPRITE(SPRITEID_HUD_APPLE)->DrawFrame(G(), 0, _coin[i]->_x, _coin[i]->_y);
					}
					else
					{
						GET_SPRITE(SPRITEID_HUD_COIN)->DrawFrame(G(), 0, _coin[i]->_x, _coin[i]->_y);
					}
					
				}
				_animManager.Add(&_coin[i]->_animCoin,100, _coin[i]->_animCoin._x, _coin[i]->_animCoin._y);
				//_coin[i]->_animCoin.DrawAnimObject(G());
				_coin[i]->_animCoin.UpdateAnimObject();
			}
			
		}
	}


	if (GetArmyBoss(TYPE_ENEMY_BOSS_2) != NULL)
	{
		GetArmyBoss(TYPE_ENEMY_BOSS_2)->Render();
	}
	

	for (int i = 0; i < _numObstacle; i++)
	{
		if (_obstacle[i] != NULL)
		{
			if (_obstacle[i]->_state == OBSTACLE_STATE_INIT_FALL)
			{
				
				_animManager.Add(&_obstacle[i]->_animObstacle, _obstacle[i]->_opacity, _obstacle[i]->_animObstacle._x, _obstacle[i]->_animObstacle._y);
				//_obstacle[i]->_animObstacle.DrawAnimObject(G());
				_obstacle[i]->_animObstacle.UpdateAnimObject();
				
			}

		}
	}
	

	
	//render charter
	
	
    //enemy
	

	for (int i = 0; i < _numArmy; i++)
	{
		if (_army[i] != NULL && _army[i]->GetType() != TYPE_ENEMY_BOSS_2)
		{
			_army[i]->Render();
		}
	}
    


    CHARATER().Render();
	_animManager.Render(0, 0);


	for (int i = 0; i < _numArmy; i++)
	{
		if (_army[i] != NULL)
		{
			_army[i]->RenderUnDie();
			_army[i]->RenderEffect();
			_army[i]->RenderEffectTarget();
		}
	}

	if (GetArmyBoss(TYPE_ENEMY_BOSS_3) != NULL)
	{
		//GetArmyBoss(TYPE_ENEMY_BOSS_3)->Render();
	}

	for (int i = 0; i < _numCavalry; i++)
	{
		if (_cavalry[i] != NULL)
		{
			_cavalry[i]->Render();
		}
	}



	CHARATER().RenderEffect();
	CHARATER().RenderUnDie();
    //render rocket
	for (int i = 0; i < _numRocket; i++)
	{
		if (_rocket[i] != NULL && _rocket[i]->_timeGun <= 0)
		{
			
			_rocket[i]->_animAlert.DrawAnimObject(G());
			_rocket[i]->_animAlert.UpdateAnimObject();
			
			if (_rocket[i]->_state == ROCKET_STATE_MOVE)
			{
				float y = _rocket[i]->_y + 10 * sin(_rocket[i]->_x  * 0.0087453293);
				if (_rocket[i]->_countSmoke % 2 == 0)
				{
					EFFECT().AddEffectSmoke(_rocket[i]->_x + 140, y, 0.02);
				}				
				GET_SPRITE(SPRITEID_OBSTACLE_ROCKET)->DrawFrame(G(), 0, _rocket[i]->_x, y);
			}
		}
	}
    
	//render obstacle
	for (int i = 0; i < _numObstacle; i++)
	{
		if (_obstacle[i] != NULL)
		{
			if (_obstacle[i]->_state != OBSTACLE_STATE_INIT_FALL)
			{

				_obstacle[i]->_animExplosion.DrawAnimObject(G());
				_obstacle[i]->_animExplosion.UpdateAnimObject();
				//G()->SetColor(0x440000ff);
				//G()->FillRect(_obstacle[i]->_x + _obstacle[i]->_Cx, _obstacle[i]->_y + _obstacle[i]->_Cy, _obstacle[i]->_Cw, _obstacle[i]->_Ch, true);
			}
		}
	}
	
	if (BATTLE()._timeBegin >0 && BATTLE()._timeBegin <= 1.4 * 60)
	{
		_bossComming.DrawAnimObject(G());
		_bossComming.UpdateAnimObject();
	}

	if (_addBoss == 1)
	{
		GET_SPRITE(SPRITEID_HUD_INGAME)->DrawFrame(G(), 10, _bossIncomingX, 0);
		if (_stateBossIncoming == 1)
		{
			_bossComming.DrawAnimObject(G());
			_bossComming.UpdateAnimObject();
		}
	}

	
	if (MAIN_GAME()->_isRain)
	{
		//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LOW_BRIGHTNESS);
	}
	if (BATTLE_MAP()._usedBG_1)
	{	
		BATTLE_MAP().RenderBGLayer4_1(0, 30);
		if (BATTLE_MAP()._stateSwap == 1)
		{
			BATTLE_MAP().RenderBGLayer4_2(0, 30);
			//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
			BATTLE_MAP().RenderSwapBG_1(0, 0);
		}
	}

	if (BATTLE_MAP()._usedBG_2)
	{	
		BATTLE_MAP().RenderBGLayer4_2(0, 30);
		if (BATTLE_MAP()._stateSwap == 1)
		{
			BATTLE_MAP().RenderBGLayer4_1(0, 30);
			//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
			BATTLE_MAP().RenderSwapBG_1(0, 0);
		}
	}
	//G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);


	//render coin

	for (int i = 0; i < _numCoin; i++)
	{
		if (_coin[i] != NULL && _coin[i]->_type == 1)
		{			
			GET_SPRITE(SPRITEID_HUD_APPLE)->DrawFrame(G(), 0, _coin[i]->_x, _coin[i]->_y);
			_animManager.Add(&_coin[i]->_animCoin,100, _coin[i]->_animCoin._x, _coin[i]->_animCoin._y);
		}
	}



	EFFECT().Render();

	//hud interface

	if (BATTLE()._timeBegin <= 0)
	{
		_tempDistance += _offsetDistance;
		if (_tempDistance >= USER()._score)
		{
			_tempDistance = USER()._score;
		}
		if(_tempDistance < 0)
		{
			_tempDistance =0;
		}

		if (MAIN_GAME()->_playWitchEndlest)
		{
			GET_SPRITE(SPRITEID_HUD_INGAME)->DrawFrame(G(), 0, 0, 0+offsetY);
			MAIN_GAME()->RenderStringVsWidth(SPRITEID_FONT_MONEY, ALIG_DISTANCE_X, ALIG_DISTANCE_Y + offsetY, (int)_tempDistance, 130);
		}
		GET_SPRITE(SPRITEID_HUD_INGAME)->DrawFrame(G(), 1, 0, 0+offsetY);

		MAIN_GAME()->RenderMoney(ALIG_MONEY_X, ALIG_MONEY_Y + offsetY, 90, USER()._coin, _scaleCoin);

		//render coin collect

		for (int i = 0; i < _numCoin; i++)
		{

			if (_coin[i] != NULL)
			{
				if (_coin[i]->_state == COIN_STATE_COLLECT)
				{
					_coin[i]->_animCoin.DrawAnimObject(G());
					_coin[i]->_animCoin.UpdateAnimObject();
				}
			}
		}

	}

	
	CHARATER().RenderLive();
	//if (CHARATER().GetStick())
	{
		CHARATER().RenderStick();
	}
	CHARATER().RenderStickShoot();
	if (CHARATER().GetState() == CHARATER_STATE_DIE && !CHARATER().CompleteEffect(EFFECT_TYPE_HAT_FALL,4))
	{
		G()->SetColor(0xdd000000);
		G()->FillFullScreen(true);
		CHARATER().GetAnimCharater().DrawAnimObject(G());
		CHARATER().RenderEffect();
		//render rocket
		for (int i = 0; i < _numRocket; i++)
		{
			if (_rocket[i] != NULL &&  _rocket[i]->_renderTop)
			{
				_rocket[i]->_animAlert.DrawAnimObject(G());			
				float y = _rocket[i]->_y + 10 * sin(_rocket[i]->_x  * 0.0087453293);					
				
			}
		}

		for (int i = 0; i < _numObstacle; i++)
		{
			if (_obstacle[i] != NULL && _obstacle[i]->_renderTop)
			{
				//if (_obstacle[i]->_state != OBSTACLE_STATE_INIT_FALL)
				{
					_obstacle[i]->_animExplosion.DrawAnimObject(G());				
					_obstacle[i]->_animObstacle.DrawAnimObject(G());
					
				}
			}
		}

		for (int i = 0; i < _numArmy; i++)
		{
			if (_army[i] != NULL && _army[i]->_renderTop)
			{			
				_army[i]->Render();	
				_army[i]->RenderEffect();
				_army[i]->RenderEffectTarget();
			}
		}

		if (CHARATER().GetAnimCharater().IsCurrentAnimStop())
		{
			GET_SPRITE(GET_DES().getShopCharaterDes(USER()._currentUseCharater, SHOP_CHARATER_SPRITE_ID))->DrawFrame(G(), 117, CHARATER().GetAnimCharater()._x, CHARATER().GetAnimCharater()._y + offsetY);
		}
	}
}
BYTE CBattle::GetIndexObstacle(BYTE type)
{
	BYTE res = 0;
	switch (type)
	{
	case OBSTACLE_TNT:
		res = 0;
		break;
	case OBSTACLE_TRAP:
		res = 2;
		break;
	case OBSTACLE_CASTUS:
		res = 4;
		break;
	case OBSTACLE_BOME:
		res = 6;
		break;
	case OBSTACLE_BARREL:
		res = 10;
		break;
	}
	return res;
}
BYTE CBattle::GetIndexAnimObstacle(BYTE type)
{
	BYTE res = 0;
	switch (type)
	{
	case OBSTACLE_TNT:
		res = 0;
		break;
	case OBSTACLE_TRAP:
		res = 1;
		break;
	case OBSTACLE_CASTUS:
		res = 3;
		break;
	case OBSTACLE_BOME:
		res = 2;
		break;
	case OBSTACLE_BARREL:
		res = 4;
		break;
	}
	return res;
}
void CBattle::AddObstacle(int num, float x, float y, BYTE type, BYTE state, float ActiveX, float rangeMax, float angle)
{
	
	for (int j = 0; j < num; j++)
	{
		bool expandObs = true;
		for (int i = 0; i < _numObstacle; i++)
		{
			if (_obstacle[i] == NULL)
			{
				expandObs = false;
				_obstacle[i] = new Obstacle();	

				if (type == OBSTACLE_BOME || type == OBSTACLE_CHESS)
				{
					_obstacle[i]->_g = 10;
					_obstacle[i]->_x0 = x;
					_obstacle[i]->_y0 = y + CMath::RANDOM(0, 50);
					

					_obstacle[i]->_targety0 = CHARATER().PosY()- 50 + CMath::RANDOM(-20,50);
					_obstacle[i]->_angle = angle;					
					_obstacle[i]->_v0 = CMath::SQRT((rangeMax*_obstacle[i]->_g) / (2 * MSIN(angle)*MCOS(angle)));
					
					
					_obstacle[i]->_t0 = 0;
					_obstacle[i]->_count = 0;

					if (x > CHARATER().PosX())
					{
						_obstacle[i]->_direcBome = _LEFT;
					}
					else
					{
						_obstacle[i]->_direcBome = _RIGHT;
					}

					if (_obstacle[i]->_targety0 <= MIN_MOVE_Y)
					{
						_obstacle[i]->_targety0 = MIN_MOVE_Y;
					}
					if (_obstacle[i]->_targety0 >= MAX_MOVE_Y-50)
					{
						_obstacle[i]->_targety0 = MAX_MOVE_Y-50;
					}
				}
				

				//float dd = CMath::DISTANCE(CHARATER().PosX(), CHARATER().PosY(), x, y);
				//_obstacle[i]->_v0 = CMath::SQRT((dd / _obstacle[i]->_g) / (2 * MSIN(_obstacle[i]->_angle)*MCOS(_obstacle[i]->_angle)));
				_obstacle[i]->_renderTop = false;
				_obstacle[i]->_x = x;
				
				_obstacle[i]->_isCharater = false;
				_obstacle[i]->_animObstacle.SetAnimSprite(GET_SPRITE(SPRITEID_OBSTACLE), SPRITEID_OBSTACLE);
				_obstacle[i]->_animObstacle.SetAnim(GetIndexAnimObstacle(type) * 3, true);
				_obstacle[i]->_activeX = ActiveX;
				

				_obstacle[i]->_opacity = 100;
				_obstacle[i]->_Cx = GET_SPRITE(SPRITEID_OBSTACLE)->GetFModuleX(GetIndexObstacle(type), 1);
				_obstacle[i]->_Cy = GET_SPRITE(SPRITEID_OBSTACLE)->GetFModuleY(GetIndexObstacle(type), 1);
				_obstacle[i]->_Cw = GET_SPRITE(SPRITEID_OBSTACLE)->GetFModuleWidth(GetIndexObstacle(type), 1);
				_obstacle[i]->_Ch = GET_SPRITE(SPRITEID_OBSTACLE)->GetFModuleHeight(GetIndexObstacle(type), 1);

				_obstacle[i]->_y = y + CMath::ABSOLUTE_VALUE(GET_SPRITE(SPRITEID_OBSTACLE)->GetFModuleY(GetIndexObstacle(type), 0));
				if (type == OBSTACLE_CHESS)
				{
					_obstacle[i]->_animObstacle.SetAnimSprite(GET_SPRITE(SPRITEID_TRESURE_CHESS), SPRITEID_TRESURE_CHESS);
					_obstacle[i]->_animObstacle.SetAnim(0, true);

					_obstacle[i]->_Cx = GET_SPRITE(SPRITEID_TRESURE_CHESS)->GetFModuleX(0, 0);
					_obstacle[i]->_Cy = GET_SPRITE(SPRITEID_TRESURE_CHESS)->GetFModuleY(0, 0);
					_obstacle[i]->_Cw = GET_SPRITE(SPRITEID_TRESURE_CHESS)->GetFModuleWidth(0, 0);
					_obstacle[i]->_Ch = GET_SPRITE(SPRITEID_TRESURE_CHESS)->GetFModuleHeight(0, 0);
				}

				_obstacle[i]->_state = state;
				_obstacle[i]->_type = type;
				_obstacle[i]->_stateOld = _obstacle[i]->_state;
				break;
			}
		}

		if (expandObs)
		{
			Obstacle** temp = new Obstacle*[1 + _numObstacle];
			for (int i = 0; i<1 + _numObstacle; i++)
			{
				if (i < _numObstacle)
				{
					temp[i] = _obstacle[i];
				}
				else
				{
					temp[i] = NULL;
				}
			}
			if (_obstacle != NULL)
			{
				delete[] _obstacle;
				
			}
			_obstacle = temp;

			_obstacle[_numObstacle] = new Obstacle();

			if (type == OBSTACLE_BOME || type == OBSTACLE_CHESS)
			{
				_obstacle[_numObstacle]->_g = 10;
				_obstacle[_numObstacle]->_x0 = x;
				_obstacle[_numObstacle]->_y0 = y + CMath::RANDOM(0, 50);
				if (_obstacle[_numObstacle]->_y0 <= MIN_MOVE_Y)
				{
					_obstacle[_numObstacle]->_y0 = MIN_MOVE_Y;
				}
				_obstacle[_numObstacle]->_targety0 = CHARATER().PosY() - 50 + CMath::RANDOM(-20, 50);
				_obstacle[_numObstacle]->_angle = angle;
				_obstacle[_numObstacle]->_v0 = CMath::SQRT((rangeMax*_obstacle[_numObstacle]->_g) / (2 * MSIN(angle)*MCOS(angle)));

				_obstacle[_numObstacle]->_t0 = 0;
				_obstacle[_numObstacle]->_count = 0;

				

				if (x > CHARATER().PosX())
				{
					_obstacle[_numObstacle]->_direcBome = _LEFT;
				}
				else
				{
					_obstacle[_numObstacle]->_direcBome = _RIGHT;
				}

				if (_obstacle[_numObstacle]->_targety0 <= MIN_MOVE_Y)
				{
					_obstacle[_numObstacle]->_targety0 = MIN_MOVE_Y;
				}
				if (_obstacle[_numObstacle]->_targety0 >= MAX_MOVE_Y - 50)
				{
					_obstacle[_numObstacle]->_targety0 = MAX_MOVE_Y - 50;
				}
			}
			_obstacle[_numObstacle]->_renderTop = false;
			_obstacle[_numObstacle]->_x = x;
			
			_obstacle[_numObstacle]->_isCharater = false;

			_obstacle[_numObstacle]->_animObstacle.SetAnimSprite(GET_SPRITE(SPRITEID_OBSTACLE), SPRITEID_OBSTACLE);
			_obstacle[_numObstacle]->_animObstacle.SetAnim(GetIndexAnimObstacle(type) * 3, true);
			_obstacle[_numObstacle]->_activeX = ActiveX;
			
			_obstacle[_numObstacle]->_opacity = 100;

			_obstacle[_numObstacle]->_Cx = GET_SPRITE(SPRITEID_OBSTACLE)->GetFModuleX(GetIndexObstacle(type), 1);
			_obstacle[_numObstacle]->_Cy = GET_SPRITE(SPRITEID_OBSTACLE)->GetFModuleY(GetIndexObstacle(type), 1);
			_obstacle[_numObstacle]->_Cw = GET_SPRITE(SPRITEID_OBSTACLE)->GetFModuleWidth(GetIndexObstacle(type), 1);
			_obstacle[_numObstacle]->_Ch = GET_SPRITE(SPRITEID_OBSTACLE)->GetFModuleHeight(GetIndexObstacle(type), 1);

			_obstacle[_numObstacle]->_y = y + CMath::ABSOLUTE_VALUE(GET_SPRITE(SPRITEID_OBSTACLE)->GetFModuleY(GetIndexObstacle(type), 0));
			if (type == OBSTACLE_CHESS)
			{
				_obstacle[_numObstacle]->_animObstacle.SetAnimSprite(GET_SPRITE(SPRITEID_TRESURE_CHESS), SPRITEID_TRESURE_CHESS);
				_obstacle[_numObstacle]->_animObstacle.SetAnim(0, true);

				_obstacle[_numObstacle]->_Cx = GET_SPRITE(SPRITEID_TRESURE_CHESS)->GetFModuleX(0, 0);
				_obstacle[_numObstacle]->_Cy = GET_SPRITE(SPRITEID_TRESURE_CHESS)->GetFModuleY(0, 0);
				_obstacle[_numObstacle]->_Cw = GET_SPRITE(SPRITEID_TRESURE_CHESS)->GetFModuleWidth(0, 0);
				_obstacle[_numObstacle]->_Ch = GET_SPRITE(SPRITEID_TRESURE_CHESS)->GetFModuleHeight(0, 0);
			}

			_obstacle[_numObstacle]->_state = state;

			_obstacle[_numObstacle]->_type = type;
			_numObstacle += 1;
		}
	}

}
void CBattle::GetObstacleRandom(int level)
{	
	char str[512];
	sprintf(str, "%s", GET_DES().getLevelObs(level, LVL_OBS_OBSTACLE_TYPE));

	int lengh = strlen(str);
	_numRandObs = 0;
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
			_randomTypeObs[_numRandObs] = GET_DES().ConverNumber(string);
			_numRandObs++;
			count = 0;
		}
		if (i == lengh - 1)
		{
			string[count] = '\0';
			_randomTypeObs[_numRandObs] = GET_DES().ConverNumber(string);
			_numRandObs++;
			count = 0;
		}
	}
	printf("\n ");
	for (int i = 0; i < _numRandObs; i++)
	{
		printf(" %d", _randomTypeObs[i]);
	}
}
void CBattle::GetArmyRandom(int level)
{
	char str[128];
	sprintf(str, "%s", GET_DES().getLevelObs(level, LVL_OBS_ENEMY_TYPE));

	int lengh = strlen(str);
	_numRandArmy = 0;
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
			_randomTypeArmy[_numRandArmy] = GET_DES().ConverNumber(string);
			_numRandArmy++;
			count = 0;
		}
		if (i == lengh - 1)
		{
			string[count] = '\0';
			_randomTypeArmy[_numRandArmy] = GET_DES().ConverNumber(string);
			_numRandArmy++;
			count = 0;
		}
	}
	
	
}
void CBattle::GetRocketRandom(int level)
{
	char str[128];
	sprintf(str, "%s", GET_DES().getLevelObs(level, LVL_OBS_ROCKET_TYPE));

	int lengh = strlen(str);
	_numRandRocket = 0;
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
			_randomTypeRocket[_numRandRocket] = GET_DES().ConverNumber(string);
			_numRandRocket++;
			count = 0;
		}
		if (i == lengh - 1)
		{
			string[count] = '\0';
			_randomTypeRocket[_numRandRocket] = GET_DES().ConverNumber(string);
			_numRandRocket++;
			count = 0;
		}
	}
}
void CBattle::GetPowerArmyRandom(int lvlPower)
{
	char str[20];
	sprintf(str, "%s", GET_DES().getMacheteTypeDes(lvlPower));

	int lengh = strlen(str);
	_numRandPowerArmy = 0;
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
			_randomTypePowerArmy[_numRandPowerArmy] = GET_DES().ConverNumber(string);
			_numRandPowerArmy++;
			count = 0;
		}
		if (i == lengh - 1)
		{
			string[count] = '\0';
			_randomTypePowerArmy[_numRandPowerArmy] = GET_DES().ConverNumber(string);
			_numRandPowerArmy++;
			count = 0;
		}
	}
}
void CBattle::AddCoin(int num, float x, float y, BYTE type, float angle, BYTE state, float targetX, float targetY)
{	

    for (int j = 0; j < num; j ++)
    {
        bool expandCoin = true;
        for (int i = 0; i < _numCoin; i++)
        {
            if (_coin[i] == NULL)
            {
                expandCoin = false;
                _coin[i] = new Coin();
				_coin[i]->_state = state;

				if (state == COIN_STATE_INIT_FALL)
				{
					_coin[i]->_g = 5;
					_coin[i]->_x0 = x;
					_coin[i]->_y0 = y;

					if (_coin[i]->_y0 <= MIN_MOVE_Y)
					{
						_coin[i]->_y0 = MIN_MOVE_Y;
					}

					if (_coin[i]->_y0 >= MAX_MOVE_Y)
					{
						_coin[i]->_y0 = MAX_MOVE_Y;
					}
					int maxW = CMath::ABSOLUTE_VALUE(x - targetX);
					_coin[i]->_targety0 = targetY;
					_coin[i]->_angle = angle;
					_coin[i]->_v0 = CMath::SQRT((maxW*_coin[i]->_g) / (2 * MSIN(angle)*MCOS(angle)));
					_coin[i]->_t0 = 0;
					_coin[i]->_count = 0;
					_coin[i]->_direc = CMath::RANDOM(0,1)? _LEFT:_RIGHT;
						
				}				

				if (type == 0)
				{
					_coin[i]->_animCoin.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_COIN), SPRITEID_HUD_COIN);
				}
				else
				{
					_coin[i]->_direc = _RIGHT;
					_coin[i]->_animCoin.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_APPLE), SPRITEID_HUD_APPLE);
				}
                
				_coin[i]->_type = type;
                _coin[i]->_animCoin.SetAnim(0,true);
				_coin[i]->_x =  x;
				_coin[i]->_y =  y;
                
                break;
            }
        }
        
        if (expandCoin)
        {
            Coin** temp = new Coin*[1 + _numCoin];
            for (int i = 0; i<1 + _numCoin; i++)
            {
                if (i < _numCoin)
                {
                    temp[i] = _coin[i];
                }
                else
                {
                    temp[i] = NULL;
                }
            }
			if (_coin != NULL)
			{
				delete[] _coin;
			}
            _coin = temp;
            
            _coin[_numCoin] = new Coin();
			_coin[_numCoin]->_state = state;

			if (state == COIN_STATE_INIT_FALL)
			{
				_coin[_numCoin]->_g = 5;
				_coin[_numCoin]->_x0 = x;
				_coin[_numCoin]->_y0 = y;

				if (_coin[_numCoin]->_y0 <= MIN_MOVE_Y)
				{
					_coin[_numCoin]->_y0 = MIN_MOVE_Y;
				}

				if (_coin[_numCoin]->_y0 >= MAX_MOVE_Y)
				{
					_coin[_numCoin]->_y0 = MAX_MOVE_Y;
				}

				
				int maxW = CMath::ABSOLUTE_VALUE(x - targetX);
				_coin[_numCoin]->_targety0 = targetY;
				_coin[_numCoin]->_angle = angle;
				_coin[_numCoin]->_v0 = CMath::SQRT((maxW*_coin[_numCoin]->_g) / (2 * MSIN(angle)*MCOS(angle)));
				_coin[_numCoin]->_t0 = 0;
				_coin[_numCoin]->_count = 0;
				_coin[_numCoin]->_direc = CMath::RANDOM(0, 1) ? _LEFT : _RIGHT;

			}


		

			if (type == 0)
			{
				_coin[_numCoin]->_animCoin.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_COIN), SPRITEID_HUD_COIN);
			}
			else
			{
				_coin[_numCoin]->_direc =  _RIGHT;
				_coin[_numCoin]->_animCoin.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_APPLE), SPRITEID_HUD_APPLE);
			}
			_coin[_numCoin]->_type = type;
            _coin[_numCoin]->_animCoin.SetAnim(0,true);
			_coin[_numCoin]->_x =  x;
			_coin[_numCoin]->_y =  y;
			
//			_coin[_numCoin]->_timeAppear = armyDie ? 5 * 60 : 60 * 60;
            _numCoin += 1;
        }
    }
}
void CBattle::AddPower(float x, float y, BYTE type)
{
	bool expandPower = true;
	for (int i = 0; i < _numPower; i++)
	{
		if (_power[i] == NULL)
		{
			expandPower = false;
			_power[i] = new Power();
			_power[i]->_state = POWER_STATE_INIT;				
			_power[i]->_type = type;
			switch (type)
			{
			case POWER_MACHETE:
				_power[i]->_animPower.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_MACHETE), SPRITEID_HUD_MACHETE);
				_power[i]->_animPower.SetAnim(0, true);
				_power[i]->_cW = GET_SPRITE(SPRITEID_HUD_MACHETE)->GetFModuleWidth(0, 0);
				_power[i]->_cH = GET_SPRITE(SPRITEID_HUD_MACHETE)->GetFModuleWidth(0, 0);
				break;
			case POWER_MULE:			
				_power[i]->_cW = GET_SPRITE(SPRITEID_HUD_MULE)->GetFModuleWidth(0, 0);
				_power[i]->_cH = GET_SPRITE(SPRITEID_HUD_MULE)->GetFModuleWidth(0, 0);
				_power[i]->_animPower.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_MULE), SPRITEID_HUD_MULE);
				_power[i]->_animPower.SetAnim(0, true);
				break;
			case POWER_MEDAL:
				_power[i]->_cW = GET_SPRITE(SPRITEID_HUD_MEDAL)->GetFModuleWidth(0, 0);
				_power[i]->_cH = GET_SPRITE(SPRITEID_HUD_MEDAL)->GetFModuleWidth(0, 0);
				_power[i]->_animPower.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_MEDAL), SPRITEID_HUD_MEDAL);
				_power[i]->_animPower.SetAnim(0, true);
				break;
			}			
			_power[i]->_x = x;
			_power[i]->_y = y;			
			break;
		}
	}

	if (expandPower)
	{
		Power** temp = new Power*[1 + _numPower];
		for (int i = 0; i < 1 + _numPower; i++)
		{
			if (i < _numPower)
			{
				temp[i] = _power[i];
			}
			else
			{
				temp[i] = NULL;
			}
		}
		if (_power != NULL)
		{
			delete[] _power;
		}
		_power = temp;

		_power[_numPower] = new Power();
			
		_power[_numPower]->_state = POWER_STATE_INIT;
		_power[_numPower]->_type = type;
		switch (type)
		{
		case POWER_MACHETE:
			_power[_numPower]->_animPower.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_MACHETE), SPRITEID_HUD_MACHETE);
			_power[_numPower]->_animPower.SetAnim(0, true);
			_power[_numPower]->_cW = GET_SPRITE(SPRITEID_HUD_MACHETE)->GetFModuleWidth(0, 0);
			_power[_numPower]->_cH = GET_SPRITE(SPRITEID_HUD_MACHETE)->GetFModuleWidth(0, 0);
			break;
		case POWER_MULE:
			_power[_numPower]->_cW = GET_SPRITE(SPRITEID_HUD_MULE)->GetFModuleWidth(0, 0);
			_power[_numPower]->_cH = GET_SPRITE(SPRITEID_HUD_MULE)->GetFModuleWidth(0, 0);
			_power[_numPower]->_animPower.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_MULE), SPRITEID_HUD_MULE);
			_power[_numPower]->_animPower.SetAnim(0, true);
			break;
		case POWER_MEDAL:
			_power[_numPower]->_cW = GET_SPRITE(SPRITEID_HUD_MEDAL)->GetFModuleWidth(0, 0);
			_power[_numPower]->_cH = GET_SPRITE(SPRITEID_HUD_MEDAL)->GetFModuleWidth(0, 0);
			_power[_numPower]->_animPower.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_MEDAL), SPRITEID_HUD_MEDAL);
			_power[_numPower]->_animPower.SetAnim(0, true);
			break;
		}
		_power[_numPower]->_x = x;
		_power[_numPower]->_y = y;
		_numPower += 1;
	}
	
}
void CBattle::AddArmy(BYTE type, float x, float y, BYTE state, BYTE initBy)
{
    {
        bool expandArmy = true;
        for (int i = 0; i < _numArmy; i++)
        {
            if (_army[i] == NULL)
            {
                expandArmy = false;
                _army[i] = new CArmy();
				_army[i]->Init(x, y, type, initBy);
				_army[i]->SetState(state);
                break;
            }
        }
        
        if (expandArmy)
        {
            CArmy** temp = new CArmy*[1 + _numArmy];
            for (int i = 0; i<1 + _numArmy; i++)
            {
                if (i < _numArmy)
                {
                    temp[i] = _army[i];
                }
                else
                {
                    temp[i] = NULL;
                }
            }
			if (_army != NULL)
			{
				delete[] _army;
			}
            _army = temp;
            _army[_numArmy] = new CArmy();
			_army[_numArmy]->Init(x, y, type, initBy);
			_army[_numArmy]->SetState(state);
            _numArmy += 1;
        }
    }
}
void CBattle::AddRocket(int num, float x, float y, BYTE state, int timeGun)
{
	//printf("\n add rocket");
	for (int j = 0; j < num; j++)
	{
		bool expandRocket = true;
		for (int i = 0; i < _numRocket; i++)
		{
			if (_rocket[i] == NULL)
			{
				expandRocket = false;
				_rocket[i] = new Rocket();
				_rocket[i]->_state = state;
				_rocket[i]->_timeGun = timeGun;
				_rocket[i]->_countFrame = 0;
				_rocket[i]->_x = x;
				_rocket[i]->_y = y;
				_rocket[i]->_renderTop = false;

				_rocket[i]->_animAlert.SetAnimSprite(GET_SPRITE(SPRITEID_OBSTACLE_ROCKET), SPRITEID_OBSTACLE_ROCKET);
				_rocket[i]->_animAlert.SetAnim(0, false);
				_rocket[i]->_animAlert.SetPos(k_SCREEN_WIDTH - ALERT_ROCKET_X, _rocket[i]->_y);
				
				_rocket[i]->_countSmoke = 0;
				break;
			}
		}

		if (expandRocket)
		{
			Rocket** temp = new Rocket*[1 + _numRocket];
			for (int i = 0; i<1 + _numRocket; i++)
			{
				if (i < _numRocket)
				{
					temp[i] = _rocket[i];
				}
				else
				{
					temp[i] = NULL;
				}
			}
			if (_rocket != NULL)
			{
				delete[] _rocket;
			}
			_rocket = temp;
			_rocket[_numRocket] = new Rocket();
			_rocket[_numRocket]->_state = state;
			_rocket[_numRocket]->_timeGun = timeGun;
			_rocket[_numRocket]->_countFrame = 0;
			_rocket[_numRocket]->_x = x;
			_rocket[_numRocket]->_y = y;
			_rocket[_numRocket]->_renderTop = false;

			_rocket[_numRocket]->_animAlert.SetAnimSprite(GET_SPRITE(SPRITEID_OBSTACLE_ROCKET), SPRITEID_OBSTACLE_ROCKET);
			_rocket[_numRocket]->_animAlert.SetAnim(0, false);
			_rocket[_numRocket]->_animAlert.SetPos(k_SCREEN_WIDTH - ALERT_ROCKET_X, _rocket[_numRocket]->_y);
			
			_rocket[_numRocket]->_countSmoke = 0;
			_numRocket += 1;
		}
	}
}
bool CBattle::checkRectOverlapRect(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
	if (x1 < x2 + w2 && x1 + w1 > x2 &&
		y1 < y2 + h2 && y1 + h1 > y2)
	{
		return true;
	}
	return false;
};
bool CBattle::CheckCollisionArmy()
{
	bool res = false;
	
	return res;
}
bool CBattle::CheckCollisionObstacle()
{
	bool res = false;
	for (int i = 0; i < _numObstacle; i++)
	{
		if (_obstacle[i] != NULL)
		{
			if (checkRectOverlapRect(CHARATER().ColMoveX(), CHARATER().ColMoveY(), CHARATER().ColMoveW(), CHARATER().ColMoveH(),
				_obstacle[i]->_Cx + _obstacle[i]->_x, _obstacle[i]->_Cy + _obstacle[i]->_y, _obstacle[i]->_Cw, _obstacle[i]->_Ch))
			{
				res = true;
				break;
			}
		}
	}
	return res;
}

void CBattle::CheckCollisionCoin()
{
	for (int i = 0; i < _numCoin; i++)
	{
		if (_coin[i] != NULL)
		{
            if (_coin[i]->_state == COIN_STATE_INIT)
            {
				if(CHARATER().CheckCollisonCoin(_coin[i]->_animCoin._x + 25, _coin[i]->_animCoin._y + 25, 50))
                {
                    _coin[i]->_state = COIN_STATE_MOVE_CHARATER;
					//set Achivement
					ACHIEVEMENT()._numCoinColect++;
					ACHIEVEMENT()._totalCoinColect++;
					ACHIEVEMENT()._totalDailyCoinCollect++;
					GAME()->PlaySFX(SFX_COIN);
                }
            }            
        }
    }
}

void CBattle::GetArmyAttack(CCharater *charater)
{
	//return;
	if (CHARATER().GetState() == CHARATER_STATE_DIE || !CHARATER()._autoShot)
	{
		if (CHARATER()._forceShot)
		{
			//CHARATER()._forceShot = false;
		}
		else
		{
			return;
		}
		
	}
	//if (CHARATER().GetAttackArmy() == NULL)
	{
		bool isAttack = false;
		float* distance = new float[_numArmy];
		for (int i = 0; i < _numArmy; i++)
		{
			distance[i] = k_SCREEN_WIDTH*k_SCREEN_HEIGHT;
		}
		for (int i = 0; i < _numArmy; i++)
		{
			if (_army[i] != NULL)
			{
				if (CHECK_POINT_IN_CIRCLE(_army[i]->PosX(), _army[i]->PosY(), charater->PosX(), charater->PosY(), charater->GetDistanceGun()))
				{					
					if (CHECK_POINT_IN_RECT(_army[i]->PosX(), _army[i]->PosY(), 0, 0, k_SCREEN_WIDTH, k_SCREEN_HEIGHT))
					{
						if (_army[i]->GetState() != CHARATER_STATE_DIE && _army[i]->GetState() != CHARATER_STATE_BOSS_DIE )
						{
							if (_army[i]->GetType() == TYPE_ENEMY_BOSS_1)
							{
								if (_army[i]->GetState() != CHARATER_STATE_INIT_BOSS)
								{
									isAttack = true;
									distance[i] = CMath::DISTANCE(charater->PosX(), charater->PosY(), _army[i]->PosX(), _army[i]->PosY());
								}
							}
							if (_army[i]->GetType() == TYPE_ENEMY_BOSS_3)
							{
								if (_army[i]->GetState() != CHARATER_STATE_INIT_BOSS && _army[i]->GetState() != CHARATER_STATE_BOSS_DIE)
								{
									isAttack = true;
									distance[i] = CMath::DISTANCE(charater->PosX(), charater->PosY(), _army[i]->PosX(), _army[i]->PosY());
								}
							}
							else if (_army[i]->GetType() != TYPE_ENEMY_BOSS_2)
							{
								isAttack = true;
								distance[i] = CMath::DISTANCE(charater->PosX(), charater->PosY(), _army[i]->PosX(), _army[i]->PosY());
								if (_army[i]->GetState() == CHARATER_STATE_MOVE_TOP)
								{
									distance[i] = CMath::DISTANCE(charater->PosX(), charater->PosY(), _army[i]->PosX(), _army[i]->PosY()) - k_SCREEN_WIDTH*k_SCREEN_HEIGHT;
								}
							}							
						}
					} 
				}
			}			
		}

		if (isAttack)
		{
			int index = 0;
			int min = distance[index];
			
			for (int i = 0; i < _numArmy; i++)
			{			
				if (min > distance[i])
				{
					min = distance[i];
					index = i;
				}
			}		
			
			for (int i = index; i < _numArmy-1; i++)
			{
				distance[i] = distance[i + 1];
			}

			int index2 = 0;
			int min2 = distance[index2];			
			for (int i = 0; i < _numArmy-1; i++)
			{	
				if (min2 > distance[i])
				{
					min2 = distance[i];
					index2 = i;
				}
			}
			
			//printf("\n attack 1 = %d, attack 2 = %d", index, index2);
			//charater->SetAttackArmy(_army[index]);
			charater->SetAttackArmy1(_army[index]);
			_army[index]->_isTargetFocus = true;
	
			if (min2 != k_SCREEN_WIDTH*k_SCREEN_HEIGHT)
			{
				charater->SetAttackArmy2(_army[index2]);
			}
			else
			{
				//if (_army[index]->GetHp()  >= CHARATER().GetDame()*2)
				{
					charater->SetAttackArmy2(_army[index]);
					//printf("\nattack 2 = %d", index2);
				}
				//else
				{
					//charater->SetNullAttack2();
					//printf("\n NULLLLLLLLLLLLLLLLLLL");
				}
			}
			
		}
		else
		{
			charater->SetNullAttack();
			charater->SetNullAttack1();
			charater->SetNullAttack2();
		}		
	}
}
void CBattle::SetDistanceInitArmy()
{
 	int levelNew = GET_DES().getLevlFromDes(_distanceRun);
    bool newValue = false;
	if (levelNew >= TOTAL_LVL_DES_ROW)
	{
		levelNew = TOTAL_LVL_DES_ROW - 1;
		if (_currentMix >= _totalRandom && _totalRandom >0)
		{
			SetMixInit();
			newValue = true;
		}
	}
	if (_lvl_des < levelNew)
	{
		_timeDeplayRocket = GET_DES().getLevelDes(_lvl_des, LVL_DES_TIME_MIN_RELEASE_ROCKET) * 60;
		_lvl_des = levelNew;
        newValue = true;
	}
	
	
    if (newValue)
    {
        
		GetObstacleRandom(_lvl_des);
		GetArmyRandom(_lvl_des);		
		GetRocketRandom(_lvl_des);
        
		//set init rocket
		_countRocket = 0;
		int min = GET_DES().getLevelDes(_lvl_des, LVL_DES_MIN_ROCKET);
		int max = GET_DES().getLevelDes(_lvl_des, LVL_DES_MAX_ROCKET);
		_numRocketInit = CMath::RANDOM(min, max);
        
		float distanceWidth = (GET_DES().getLevelDes(_lvl_des, LVL_DES_DISTANCE) - _distanceRun);
		if (_lvl_des == TOTAL_LVL_DES_ROW - 1)
		{
			distanceWidth = GET_DES().getLevelDes(_lvl_des, LVL_DES_DISTANCE) - GET_DES().getLevelDes(_lvl_des-1, LVL_DES_DISTANCE);
		}
		float distance = distanceWidth / (float)(_numRocketInit + 1);
		_distanceRocketW = distance < 2 ? 2 : distance;
		_distanceRocket = _distanceRun + _distanceRocketW;

        //set init eneny
		_countArmy = 0;
		min = GET_DES().getLevelDes(_lvl_des, LVL_DES_MIN_ENEMY);
		max = GET_DES().getLevelDes(_lvl_des, LVL_DES_MAX_ENEMY);
		_numArmyInit = CMath::RANDOM(min, max);
        
		//total ran
		
		distance = distanceWidth / (float)(_numArmyInit);
		_distanceArmyW = distance<2 ? 2 : distance;
		_distanceNext += distance;

		_distanceArmyNext += distance;
		
		//set init coin
		_countCoin = 0;
		min = GET_DES().getLevelDes(_lvl_des, LVL_DES_MIN_COIN);
 		max = GET_DES().getLevelDes(_lvl_des, LVL_DES_MAX_COIN);
		_numCoinInit =  CMath::RANDOM(min, max);
		
		//set init obstacle
		_countObs = 0;
		min = GET_DES().getLevelDes(_lvl_des, LVL_DES_MIN_OBS);
		max = GET_DES().getLevelDes(_lvl_des, LVL_DES_MAX_OBS);
		_numObsInit = CMath::RANDOM(min, max);		
		//total ran
		_totalRandom = _numArmyInit + _numCoinInit + _numObsInit;
		distance = distanceWidth  / (float)(_totalRandom);
		_distanceW = distance<2?2:distance;
		_distanceNext += distance;
	
		
		SetNextPower(_lvl_des);
		_typePowerOld = _distancePower;
		SetMixInit();		
    }
}

void CBattle::SetNextPower(BYTE lvl)
{
	if (lvl >= TOTAL_LVL_DES_ROW - 1)
	{
		lvl = TOTAL_LVL_DES_ROW - 1;
	}
 	int min = 0, max = 0;
	if (lvl == 0)
	{
		min = 0;
		max = GET_DES().getLevelDes(0, LVL_DES_DISTANCE);
	}
	else if (lvl == TOTAL_LVL_DES_ROW - 1)
	{
		min = GET_DES().getLevelDes(TOTAL_LVL_DES_ROW - 2, LVL_DES_DISTANCE);
		max = GET_DES().getLevelDes(TOTAL_LVL_DES_ROW - 1, LVL_DES_DISTANCE);
	}
	else
	{
		min = GET_DES().getLevelDes(lvl - 1, LVL_DES_DISTANCE);
		max = GET_DES().getLevelDes(lvl, LVL_DES_DISTANCE);
	}

	int www = CMath::RANDOM(min, max);
	int newPower = www;

	if (lvl >= TOTAL_LVL_DES_ROW - 1)
	{
		www = max - min;
		newPower = _distanceRun + CMath::RANDOM(0, www);
	}	
	if (newPower - _typePowerOld  < 150 && lvl > 0)
	{
		_distancePower = _typePowerOld + 150;
	}
	else
	{
		_distancePower = newPower;
	}
	printf("\n = _distancePower = %f",_distancePower);
}

void CBattle::SetMixInit()
{
	
	_currentMix = 0;
	int _currentMix = 0;
	int temp = _numCoinInit + _numObsInit;
	if (temp == 0)
	{
		temp = 1;
	}
	int divE = _numArmyInit / (temp);
	int modE = _numArmyInit % (temp);

	int divO = 1;
	int modO = 0;
	if (_numCoinInit > 0)
	{
		int temp = _numCoinInit;
		if (temp == 0)
		{
			temp = 1;
		}
		divO = _numObsInit / _numCoinInit;
		modO = _numObsInit % _numCoinInit;
	}
	if (divO > 3)
	{
		divO = 3;
	}
	int countE = 0;
	int countO = 0;
	int countC = 0;

	int curE = 0;
	int curO = 0;
	int curC = 0;

	int maxE = divE + (modE > 0 ? 1 : 0);
	modE--;

	int maxO = divO + (modO > 0 ? 1 : 0);
	modO--;

	char typeAll = 'O';
	char typeOC = 'O';
	int count = 0;
	_typMix = new int[_totalRandom];
	while (count < _totalRandom)
	{
		//enemy
		if (typeAll == 'E')
		{
			if (countE < _numArmyInit)
			{
				if (curE < maxE)
				{
					_typMix[count] = 0;
					count++;
					curE++;
					countE++;
				}
				else
				{
					curE = 0;
					maxE = divE + (modE > 0 ? 1 : 0);
					modE--;

					if (curO < maxO)
					{
						typeAll = 'O';
					}
					else
					{
						typeAll = 'C';
						curO = 0;
					}
				}
			}
			else if (countO < _numObsInit)
			{
				if (curO < maxO)
				{
					typeAll = 'O';
				}
				else
				{
					typeAll = 'C';
					curO = 0;
				}
			}
			else
			{
				typeAll = 'C';
			}
		}
		//obstacle
		else if (typeAll == 'O' )
		{
			if (countO < _numObsInit)
			{
				if (curO < maxO)
				{
					_typMix[count] = 1;
					count++;
					curO++;
					countO++;
					typeAll = 'E';
				}
				else
				{
					curO = 0;
					maxO = divO + (modO > 0 ? 1 : 0);
					modO--;

					if (countC < _numCoinInit)
					{
						typeAll = 'C';
					}
					else
					{
						typeAll = 'E';
					}

				}
			}
			else if (countC < _numCoinInit)
			{
				typeAll = 'C';
			}
			else
			{
				typeAll = 'E';
			}
		}
		
		//coin
		else if (typeAll == 'C')
		{
			if (countC < _numCoinInit)
			{
				_typMix[count] = 2;
				count++;
				curC++;
				countC++;
				typeAll = 'E';
			}
			else if (countO <_numObsInit)
			{
				typeAll = 'O';
			}
			else 
			{
				typeAll = 'E';
			}
					
		}
		
	}
	
	//printf("\n total = %d, enemy = %d, obs = %d, coin = %d \n",_totalRandom, _numArmyInit, _numObsInit, _numCoinInit);
	//for (int i = 0; i < _totalRandom; i++)
	{
		//printf(" %d",_typMix[i]);
	}
	//printf("\n ===================\n");
}
void CBattle::UpdateIniteMix()
{
	//GAME()->RenderStringVsWidth(SPRITEID_FONT_MONEY, k_SCREEN_WIDTH/2, ALIG_DISTANCE_Y, (int)_tempDistance, 130);

	
	if (_timeBegin > 0)
	{
		return;
	}
	if (_distanceRun >= GET_DES().getMacheteDes(0, MACHETE_DES_DISTANCE_UNLOCK) && !USER()._isUnlockMachete)
	{
		_arrayPower[0] = 0;
		USER()._isUnlockMachete = true;
		USER().DataSave();
	}
	if (_distanceRun >= GET_DES().getMuleDes(0, MULE_DES_DISTANCE_UNLOCK) && !USER()._isUnlockMule)
	{
		_arrayPower[0] = 1;
		_arrayPower[1] = 0;		
		USER()._isUnlockMule = true;
		_addText = true;
		USER().DataSave();
		_mull = 10;
	}
	if (_distanceRun >= GET_DES().getCavalryDes(0, CAVALRY_DES_DISTANCE_UNLOCK) && !USER()._isUnlockCavalry)
	{
		_arrayPower[0] = 2;
		_arrayPower[1] = 0;
		_arrayPower[2] = 1;
		_mull = 10;	
		USER()._isUnlockCavalry = true;
		_addText = true;
		USER().DataSave();
	}
	//printf("\n _lvl_des = %d", _lvl_des);

	if (_distanceRun >= _distanceArmyNext && _addBoss == 0)
	{
		int x = k_SCREEN_WIDTH + 50;
		int y = CMath::RANDOM(MIN_MOVE_Y, MAX_MOVE_Y);

		int ran = CMath::RANDOM(0, _numRandArmy - 1);
		int type = _randomTypeArmy[ran];

		type = getTypePercent(CMath::RANDOM(1, 100), _lvl_des);
		AddArmy(type - 1, x, y, CHARATER_STATE_MOVE, ENEMY_INIT_BY_NORMAL);

		_distanceArmyNext = _distanceRun + _distanceArmyW + CMath::RANDOM(-2,2);
		
	}

	if (_distanceRun >= _distanceNext)
	{
		//printf("\n \t truoc = %f", _distanceNext);
		int type = 10;
		if (_currentMix >= _totalRandom)
		{
			type = 10;
		}
		else
		{
			//printf("\n_typMix[%d] = %d ", _currentMix, _typMix[_currentMix]);
			type = _typMix[_currentMix];
		}		

		if (_distanceRun >= _distancePower && (USER()._isUnlockMachete || USER()._isUnlockMule || USER()._isUnlockCavalry))
		{
			if (_distancePower + 100 > _distanceBoss && _lvlBosse != 1)
			{
				_distancePower = _distanceBoss + 50;
			}
			else
			{
				type = 3;
			}
		}
		int WW = _distanceW < 5 ? _distanceW : CMath::RANDOM(5, _distanceW);
		if (type == 0)
		{	
			_distanceNext = _distanceRun + WW;
			_currentMix++;
		}
		else if (type == 1)
		{
			int ran = CMath::RANDOM(0, _numRandObs - 1);
			int frame = _randomTypeObs[ran];
			//Kframe = 1;
			int numModul = GET_SPRITE(SPRITEID_FROM_OBSTACLE)->GetModuleOfFrame(frame);
			int beginY = MIN_MOVE_Y;// CMath::RANDOM(MIN_MOVE_Y, MAX_MOVE_Y);

			int H = Button::GetFrameHeight(GET_SPRITE(SPRITEID_FROM_OBSTACLE), frame);
			int W = Button::GetFrameWidth(GET_SPRITE(SPRITEID_FROM_OBSTACLE), frame);

			if (beginY + H >= MAX_MOVE_Y)
			{
				beginY = MAX_MOVE_Y - H;
			}

			//printf("\n current obs = %d",_numObstacle);
			for (int j = 0; j < numModul; j++)
			{
 				int x = GET_SPRITE(SPRITEID_FROM_OBSTACLE)->GetFModuleX(frame, j);
				x += k_SCREEN_WIDTH;
				int y = GET_SPRITE(SPRITEID_FROM_OBSTACLE)->GetFModuleY(frame, j);
				y += MIN_MOVE_Y + _offsetMoveY;

				BYTE type = GET_SPRITE(SPRITEID_FROM_OBSTACLE)->GetFModuleOpacity(frame, j) % 90;	
				
			
				//printf("\n\t obstype = %d", type);
				if (type != 10)
				{
					if (type == 4)//coin
					{
						AddCoin(1, x, y, 0);
					}
					else if (type == 6)//enymy
					{
						int ran = CMath::RANDOM(0, _numRandArmy - 1);
						int type = _randomTypeArmy[ran];
						AddArmy(type - 1, x, y, CHARATER_STATE_MOVE, ENEMY_INIT_BY_NORMAL);
					}
					else
					{
						float activeX = 0;
						if (type == 7)
						{
							activeX = k_SCREEN_WIDTH + GET_SPRITE(SPRITEID_FROM_OBSTACLE)->GetFModuleCenterX(frame, j + 1);
						}						
						AddObstacle(1, x, y , type, OBSTACLE_STATE_INIT, activeX);
												
					}
				}
			}
			
			_distanceNext = _distanceRun + WW + W / DELTA_DISTANCE;
			_currentMix++;
		}
		else if (type == 2)
		{
			//printf("\n coin coin coin coin coin");			
			int frame =  CMath::RANDOM(0, 12);
			int numModul = GET_SPRITE(SPRITEID_FROM_COIN)->GetModuleOfFrame(frame);
			int beginY = CMath::RANDOM(MIN_MOVE_Y, MAX_MOVE_Y);

			int H = Button::GetFrameHeight(GET_SPRITE(SPRITEID_FROM_COIN), frame);
			int W = Button::GetFrameWidth(GET_SPRITE(SPRITEID_FROM_COIN), frame);

			if (beginY + H >= MAX_MOVE_Y)
			{
				beginY = MAX_MOVE_Y - H;
			}
			for (int j = 0; j < numModul; j++)
			{
				int x = GET_SPRITE(SPRITEID_FROM_COIN)->GetFModuleX(frame, j) + k_SCREEN_WIDTH;
				int y = GET_SPRITE(SPRITEID_FROM_COIN)->GetFModuleY(frame, j) + beginY;

				AddCoin(1, x, y, 0);
			}			
			_distanceNext = _distanceRun + WW + W / DELTA_DISTANCE;
			_currentMix++;
		}
		else if ((type == 3 && !_isUsePower) )
		{
			
			bool loop = true;
			int state = 0;
			int percent = 0;
			int typePower = -1;
		
			int count = 0;
			for (int i = 0; i < 3; i++)
			{
				if (_arrayPower[i] != -1)
				{
					count++;
				}
			}
			while (loop)
			{				
				switch (state)
				{
				case 0:
					percent = CMath::RANDOM(0, 100);
					switch (_arrayPower[state])
					{
					case 0:
						if (percent < GET_DES().getMacheteDes(CHARATER().GetLVLMachete(), MACHETE_DES_SPAWN_RATIO)*_mull)
						{
							int temp = _arrayPower[state];
							for (int i = state; i < count - 1; i++)
							{								
								_arrayPower[i] = _arrayPower[i + 1];
							}
							_arrayPower[count - 1] = temp;
							typePower = POWER_MACHETE;
							loop = false;
						}
						else
						{
							state = 1;
						}
						break;
					case 1:
						if (percent < GET_DES().getMuleDes(CHARATER().GetLVLMule(), MULE_DES_SPAWN_RATIO)*_mull)
						{
							int temp = _arrayPower[state];
							for (int i = state; i < count - 1; i++)
							{
								_arrayPower[i] = _arrayPower[i + 1];
							}
							_arrayPower[count - 1] = temp;
							typePower = POWER_MULE;
							loop = false;
						}
						else
						{
							state = 1;
						}
						break;
					case 2:
						if (percent < GET_DES().getCavalryDes(CHARATER().GetLVLCavalry(), CAVALRY_DES_SPAWN_RATIO)*_mull)
						{
							int temp = _arrayPower[state];
							for (int i = state; i < count - 1; i++)
							{
								_arrayPower[i] = _arrayPower[i + 1];
							}
							_arrayPower[count - 1] = temp;
							typePower = POWER_MEDAL;
							loop = false;
						}
						else
						{
							state = 1;
						}
						break;
					default:
						loop = false;
						_mull = 2;
						break;
					}
					break;
				case 1:
					percent = CMath::RANDOM(0, 100);
					switch (_arrayPower[state])
					{
					case 0:
						if (percent < GET_DES().getMacheteDes(CHARATER().GetLVLMachete(), MACHETE_DES_SPAWN_RATIO)*_mull)
						{
							if (_arrayPower[2] != -1)
							{
								int temp = _arrayPower[state];
								_arrayPower[state] = _arrayPower[2];
								_arrayPower[2] = temp;
							}
							typePower = POWER_MACHETE;
							loop = false;
						}
						else
						{
							state = 2;
						}
						break;
					case 1:
						if (percent < GET_DES().getMuleDes(CHARATER().GetLVLMule(), MULE_DES_SPAWN_RATIO)*_mull)
						{
							if (_arrayPower[2] != -1)
							{
								int temp = _arrayPower[state];
								_arrayPower[state] = _arrayPower[2];
								_arrayPower[2] = temp;
							}
							typePower = POWER_MULE;
							loop = false;
						}
						else
						{
							state = 2;
						}
						break;
					case 2:
						if (percent < GET_DES().getCavalryDes(CHARATER().GetLVLCavalry(), CAVALRY_DES_SPAWN_RATIO)*_mull)
						{
							if (_arrayPower[2] != -1)
							{
								int temp = _arrayPower[state];
								_arrayPower[state] = _arrayPower[2];
								_arrayPower[2] = temp;
							}
							typePower = POWER_MEDAL;
							loop = false;
						}
						else
						{
							state = 2;
						}
						break;
					default:
					loop = false;
					_mull = 2;
						break;
					}
					break;
				case 2:
					percent = CMath::RANDOM(0, 100);
					switch (_arrayPower[state])
					{
					case 0:
						if (percent < GET_DES().getMacheteDes(CHARATER().GetLVLMachete(), MACHETE_DES_SPAWN_RATIO)*_mull)
						{							
							typePower = POWER_MACHETE;						
						}
						loop = false;
						break;
					case 1:
						if (percent < GET_DES().getMuleDes(CHARATER().GetLVLMule(), MULE_DES_SPAWN_RATIO)*_mull)
						{							
							typePower = POWER_MULE;
							loop = false;
						}
						loop = false;
						break;
					case 2:
						if (percent < GET_DES().getCavalryDes(CHARATER().GetLVLCavalry(), CAVALRY_DES_SPAWN_RATIO)*_mull)
						{						
							typePower = POWER_MEDAL;
							loop = false;
						}
						loop = false;
						break;
					default:
					{
						int temp = _arrayPower[0];
						for (int i = 0; i < count - 1; i++)
						{
							_arrayPower[i] = _arrayPower[i + 1];
						}
						_arrayPower[count - 1] = temp;
						loop = false;
						_mull = 2;
					}

						break;
					}
					break;
				}
			}
			////////////
			//typePower = POWER_MEDAL;
			if (typePower > 0)
			{	
				_mull = 1;
				int lvl = 0;
				lvl = CHARATER().GetLVLMachete();
				GetPowerArmyRandom(lvl);

				int x = k_SCREEN_WIDTH;
				int y = CMath::RANDOM(MIN_MOVE_Y + 50, MAX_MOVE_Y - 50);
				AddPower(x, y, typePower);

				if (typePower == POWER_MACHETE)
				{
					int num = GET_DES().getMacheteDes(lvl, MACHETE_DES_SPAM_ENEMY);
					int W = k_SCREEN_WIDTH / (float)num;
					for (int j = 0; j < num; j++)
					{
						int ran = CMath::RANDOM(0, _numRandPowerArmy - 1);
						int type = _randomTypePowerArmy[ran];
						AddArmy(type - 1, x + (W + CMath::RANDOM(-5, 5))*j, CMath::RANDOM(MIN_MOVE_Y, MAX_MOVE_Y), CHARATER_STATE_MOVE, ENEMY_INIT_BY_NORMAL);
					}
				}				
				_distanceNext = _distanceRun + WW;
			}			
			if (_lvl_des < TOTAL_LVL_DES_ROW - 1)
			{
				SetNextPower(_lvl_des + 1);
			}
			else
			{
				_distancePower = _distanceRun + 1000;
			}
			
		}
		///printf("\n \t sau  = %f \n =============\n", _distanceNext);
	}
}
int CBattle::getTypePercent(int percent, BYTE lvl)
{
	int res = 1;
	for (int  i = 0; i < 5; i++)
	{
		int maxPer = 0;
		int minPer = 0;
		for (int j = 0; j <= i; j++)
		{
			maxPer += GET_DES().getLevelDes(lvl, LVL_DES_ENEMY_1_RATIO + j);
		}
		for (int j = 0; j < i; j++)
		{
			minPer += GET_DES().getLevelDes(lvl, LVL_DES_ENEMY_1_RATIO + j);
		}
		if (percent > minPer &&percent <= maxPer && GET_DES().getLevelDes(lvl, LVL_DES_ENEMY_1_RATIO + i) != 0)
		{
			res = i+1;
		}
	}
	return res;
}


void CBattle::AddCavalry(int num)
{
	_numCavalry = num;
	_cavalry = new CCavalry*[_numCavalry];
	for (int i = 0; i < 3; i++)
	{
		_cavalry[i] = new CCavalry;
		_cavalry[i]->Init(0, MIN_MOVE_Y + (MAX_MOVE_Y - MIN_MOVE_Y) / (float)3*(i + 1), 0);
	}

	
	_cavalry[3] = new CCavalry;
	_cavalry[3]->Init(200, MIN_MOVE_Y + 170,0); 

	_cavalry[4] = new CCavalry;
	_cavalry[4]->Init(200, MAX_MOVE_Y - 70, 0);
	
}
void CBattle::UpdateCavalry()
{
	for (int i = 0; i < _numCavalry; i++)
	{
		if (_cavalry[i] != NULL)
		{
			GetArmyAttack((CCharater*)_cavalry[i]);
			_cavalry[i]->Update();

			for (int j = 0; j < _numArmy; j++)
			{
				if (_army[j] != NULL)
				{
					if (checkRectOverlapRect(_cavalry[i]->ColMoveX(), _cavalry[i]->ColMoveY(), _cavalry[i]->ColMoveW(), _cavalry[i]->ColMoveH(),
						_army[j]->ColMoveX(), _army[j]->ColMoveY(), _army[j]->ColMoveW(), _army[j]->ColMoveH()))
					{
						if (_army[j]->GetState() != CHARATER_STATE_DIE && _army[j]->GetState() != CHARATER_STATE_BE_ATTACK)
						{
							if (_army[j]->GetType() < TYPE_ENEMY_BOSS_1)
							{
								_army[j]->SetState(CHARATER_STATE_DIE);
							}
						}
					}
				}
			}

			for (int j = 0; j < _numCoin; j++)
			{
				if (_coin[j] != NULL)
				{
					if (_coin[j]->_state == COIN_STATE_INIT)
					{
						if (_cavalry[i]->CheckCollisonCoin(_coin[j]->_animCoin._x + 25, _coin[j]->_animCoin._y + 25, 50))
						{
							_coin[j]->_state = COIN_STATE_COLLECT;
						}
					}
				}
			}
		}
	}
}

void CBattle::SetAnimCheckPoint(BYTE lvl)
{
	_checkPointX = k_SCREEN_WIDTH + 50;
	_checkPointY = MIN_MOVE_Y;
	_distanceNext += 5;


}
void CBattle::UpdateVibrate()
{
	switch (_stateVibrate)
	{
	case 1:
		_vibrateY -= 1*1.3;
		if (_vibrateY<= -5)
		{
			_stateVibrate = 2;
		}
		break;
	case 2:
		_vibrateY += 1 * 1.3;
		if (_vibrateY >= 3)
		{
			_stateVibrate = 3;
		}
		break;
	case 3:
		_vibrateY -= 1 * 1.3;
		if (_vibrateY <= -3)
		{
			_stateVibrate = 4;
		}
		break;
	case 4:
		_vibrateY += 1 * 1.3;
		if (_vibrateY >= 2)
		{
			_stateVibrate = 5;			
		}
		break;
	case 5:
		_vibrateY -= 1 * 1.3;
		if (_vibrateY <= 0)
		{
			_stateVibrate = 0;
			_vibrateY = 0;
		}
		break;
	default:
		break;
	}
}

bool CBattle::checkHasBoss()
{
	for (int i = 0; i < _numArmy; i++)
	{
		if (_army[i] != NULL)
		{
			if (_army[i] ->GetType() >= TYPE_ENEMY_BOSS_1)
			{
				return true;
			}
		}
	}
	return false;
}

CArmy* CBattle::GetArmyBoss(BYTE type)
{
	for (int i = 0; i < _numArmy; i++)
	{
		if (_army[i] != NULL)
		{
			if (_army[i]->GetType() == type)
			{
				return _army[i];
			}
		}
	}
	return NULL;
}
CArmy* CBattle::GetArmyState(BYTE State)
{
	for (int i = 0; i < _numArmy; i++)
	{
		if (_army[i] != NULL)
		{
			if (_army[i]->GetState() == State)
			{
				return _army[i];
			}
		}
	}
	return NULL;
}

int CBattle::NumRenderEnemy()
{
	int res = 0;
	for (int i = 0; i < _numArmy; i++)
	{
		if (_army[i] != NULL)
		{
			res++;
		}
	}
	return res;
}
int CBattle::NumRenderOBS()
{
	int res = 0;
	for (int i = 0; i < _numObstacle; i++)
	{
		if (_obstacle[i] != NULL)
		{
			res++;
		}
	}
	return res;
}
int CBattle::NumRenderCOIN()
{
	int res = 0;
	for (int i = 0; i < _numCoin; i++)
	{
		if (_coin[i] != NULL)
		{
			res++;
		}
	}
	return res;
}
