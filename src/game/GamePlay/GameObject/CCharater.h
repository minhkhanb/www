#if !_CCHARATER_H
#define _CCHARATER_H 1


#define CHARATER_MOVE_TOP                       0
#define CHARATER_MOVE_TOP_RIGHT                 1
#define CHARATER_MOVE_RIGHT                     2
#define CHARATER_MOVE_DOWN_RIGHT                3

#define CHARATER_MOVE_DOWN                      4
#define CHARATER_MOVE_DOWN_LEFT                 5
#define CHARATER_MOVE_LEFT                      6
#define CHARATER_MOVE_TOP_LEFT                  7

#define CHARATER_STATE_NULL						0
#define CHARATER_STATE_MOVE						1
#define CHARATER_STATE_BE_ATTACK				2
#define CHARATER_STATE_DIE						3

//bosse
#define CHARATER_STATE_INIT_BOSS				4
#define CHARATER_STATE_STANDING					5
#define CHARATER_STATE_MOVE_RIGHT				6
#define CHARATER_STATE_MOVE_LEFT				7
#define CHARATER_STATE_MOVE_TOP					8
#define CHARATER_STATE_MOVE_DOWN				9
#define CHARATER_STATE_BOSS_RUN					10
#define CHARATER_STATE_BOSS_DIE					11
#define CHARATER_STATE_BOSS_BEFORE_RUN			12
#define CHARATER_STATE_ENEMY_ATTACK_TRAIN		13

#define CHARATER_STATE_ENEMY_FOLLOW_TOP			14
#define CHARATER_STATE_ENEMY_FOLLOW_DOWN		15





#define CHARATER_STATE_BOSS_NORMAL				50

#define CHARATER_STATE_JUMP						51

#define POWER_NONE								0
#define POWER_MACHETE							1
#define POWER_MULE								2
#define POWER_MEDAL								3

#define MAX_EFFECT_ATTACK						30
#define MAX_EFFECT_PARTICAL						50

#define EFFECT_STATE_ATTACK_NONE				0
#define EFFECT_TYPE_ATTACK_GUN					1
#define EFFECT_TYPE_PARTICAL_DIE				2
#define EFFECT_TYPE_ATTACK_GUN_2				3
#define EFFECT_TYPE_HAT_FALL					4
#define EFFECT_TYPE_HAT_BULET					5
#define EFFECT_TYPE_PARTICAL_WAGON				6
#define EFFECT_TYPE_TARGET_FOCUS				7

class CArmy;
class CCharater : public CInstance
{
	
protected:
	BYTE _type;
	BYTE _state;
	BYTE _stateOld;
	float _scaleX;

	int _spriteID;
	float _posX, _posY;
	float _targetX, _targetY;
	float _attackX, _attackY;

	float _angleGunX, _angleGunY;
	
	float _speedMove;
	CAnimObject _animCharater;
	
	
	int _collisionX, _collisionY, _collisionW, _collisionH;
	int _colMoveX, _colMoveY, _colMoveW, _colMoveH;

    float _angleMove;
    int _directionMove;
	bool _isRenderStick;
    
	int _timeDeplay;	
	
	BYTE _stateColor;

	float _speedGun;
	int _timeGun;
	int _timeGun2;
	int _timeGunSpeed;
	int _timeGun2Speed;
	BYTE _typePower;
	float _distanceGun;
	CArmy* _armyBeAttack;

	CArmy* _armyAttack1;
	CArmy* _armyAttack2;

	float _dame;
	float _speedStart;
	float _distanceMagnet;

	float _angle;
	float _v0;
	float _g;
	float _y0;
	int _t0;
	
private:
	CAnimObject _animHorse;
	int _isRenderHorse;
	float _posHorseX, _posHorseY;
    float _touch_drag_x,_touch_drag_y;
    //BYTE _stateMove;
    
	bool _isUndead;
	int _timePower;
	//from design

	BYTE _lvlAlive;
	BYTE _lvlRevolver;
	BYTE _lvlFastTrigger;
	BYTE _lvlSpeed;
	BYTE _lvlMagnet;
	BYTE _lvlMachete;
	BYTE _lvlMule;
	BYTE _lvlCavalry;

	//BYTE _lvl;
	
	bool _isRenderCharater;
	BYTE _live;
	//stick
	float _stickX, _stickY;
	float _stickCenterX, _stickCenterY;
    
    float _stickRenderX, _stickRenderY;
	float _stickCenterRenderX, _stickCenterRenderY;
   
	int _timeSoundHore;
	int _touchID;
	int _touchIDShot;
	float _stickShootRenderX, _stickShootRenderY;
	float _stickShootScale;
	//
	
	
protected:
	//effecet
	BYTE _eff_state[MAX_EFFECT_ATTACK];
	BYTE _eff_type[MAX_EFFECT_ATTACK];

	CArmy* _eff_army[MAX_EFFECT_ATTACK];
	CAnimObject _eff_anim[MAX_EFFECT_ATTACK];
	CAnimObject _eff_anim2[MAX_EFFECT_ATTACK];
	float _eff_x[MAX_EFFECT_ATTACK];
	float _eff_y[MAX_EFFECT_ATTACK];
	float _eff_speed[MAX_EFFECT_ATTACK];
	

	int _var1[MAX_EFFECT_ATTACK];
	int _var2[MAX_EFFECT_ATTACK];
	int _var3[MAX_EFFECT_ATTACK];
	int _var4[MAX_EFFECT_ATTACK];

	float _varF1[MAX_EFFECT_ATTACK];
	float _varF2[MAX_EFFECT_ATTACK];	
	float _varF3[MAX_EFFECT_ATTACK];
	float _varF4[MAX_EFFECT_ATTACK];
	float _varF5[MAX_EFFECT_ATTACK];

	//partical
	bool CompleteStatePartical(int i, int num, BYTE state);

	BYTE _par_state[MAX_EFFECT_ATTACK*MAX_EFFECT_PARTICAL];
	
	
	float _par_x[MAX_EFFECT_ATTACK*MAX_EFFECT_PARTICAL];
	float _par_y[MAX_EFFECT_ATTACK*MAX_EFFECT_PARTICAL];

	float _par_angle[MAX_EFFECT_ATTACK*MAX_EFFECT_PARTICAL];
	int	_par_opacity[MAX_EFFECT_ATTACK*MAX_EFFECT_PARTICAL];

	int _par_v1[MAX_EFFECT_ATTACK*MAX_EFFECT_PARTICAL];
	int _par_frame[MAX_EFFECT_ATTACK*MAX_EFFECT_PARTICAL];


	float _par_count[MAX_EFFECT_ATTACK*MAX_EFFECT_PARTICAL];
	float _par_v0[MAX_EFFECT_ATTACK*MAX_EFFECT_PARTICAL];
	float _par_g0[MAX_EFFECT_ATTACK*MAX_EFFECT_PARTICAL];
	float _par_x0[MAX_EFFECT_ATTACK*MAX_EFFECT_PARTICAL];
	float _par_y0[MAX_EFFECT_ATTACK*MAX_EFFECT_PARTICAL];
	float _par_target_y0[MAX_EFFECT_ATTACK*MAX_EFFECT_PARTICAL];
	int _par_t0[MAX_EFFECT_ATTACK*MAX_EFFECT_PARTICAL];

public:
	CCharater(){};
		
	virtual void Init(float x, float y, BYTE type, BYTE initBy);
	virtual void Update();
	virtual void Render();
	
	virtual void RenderUnDie();

	virtual void SetState(BYTE state);
	virtual void BeDame(int dame);
	void SetAnimChater(int index, bool loop){ _animCharater.SetAnim(index, loop); };
	void AddLive();
	BYTE GetLive(){ return _live; };

	int GetDame(){ return _dame; }
	void SetNullAttack(){ _armyBeAttack = NULL; }
	void SetAttackArmy(CArmy *army){ _armyBeAttack = army; }
	CArmy* GetAttackArmy(){ return _armyBeAttack; }

	void SetTimeGun2(int time){ _timeGun2 = time; }
	void SetNullAttack1(){ _armyAttack1 = NULL; }
	void SetAttackArmy1(CArmy *army);
	CArmy* GetAttackArmy1(){ return _armyAttack1; }

	void SetNullAttack2(){ _armyAttack2 = NULL; }
	void SetAttackArmy2(CArmy *army);
	CArmy* GetAttackArmy2(){ return _armyAttack2; }

	void UpdateTouchMove();
	bool IsMoveToTarget(float &x, float &y, float targetX, float targetY, float speed);
	
	int GetTimePower(){ return _timePower; }
	BYTE GetDirection() { return _directionMove; }
	float GetScale() { return _scaleX; }
	CAnimObject GetAnimCharater(){ return _animCharater; }

	void SetRenderStick(bool value){ _isRenderStick = value; }
	bool GetStick(){ return _isRenderStick; }
	BYTE GetState() { return _state; }
	BYTE IsUndead() { return _isUndead; }
	float PosX(){ return _posX; }
	float PosY(){ return _posY; }

	float PosAttackX(){ return _attackX; }
	float PosAttackY(){ return _attackY; }

    BYTE GetType(){return _type;}
	float GetDistanceGun(){ return _distanceGun; }
    void CheckDirectionMove(float angle);
	void UpdateAngleGun();

	void SetTypePower(BYTE type);
	BYTE GetTypePower(){ return _typePower; }
	void SetAnimPower(BYTE type);

	int CollisionX(){ return _collisionX; }
	int CollisionY(){ return _collisionY; }
	int CollisionW(){ return _collisionW; }
	int CollisionH(){ return _collisionH; }

	int ColMoveX(){ return _colMoveX; }
	int ColMoveY(){ return _colMoveY; }
	int ColMoveW(){ return _colMoveW; }
	int ColMoveH(){ return _colMoveH; }

	void SetPosStick();
	void RenderStick();
	void RenderLive();

	void SetPosStickShoot();
	void RenderStickShoot();

	void SetLVLAlive(BYTE lvl) { _lvlAlive = lvl; }
	BYTE GetLVLAlive() { return _lvlAlive; }

	void SetLVLRevolver(BYTE lvl) { _lvlRevolver = lvl; }
	BYTE GetLVLRevolver() { return _lvlRevolver; }

	void SetLVLFastTrigger(BYTE lvl) { _lvlFastTrigger = lvl; }
	BYTE GetLVLFastTrigger() { return _lvlFastTrigger; }

	void SetLVLSpeed(BYTE lvl) { _lvlSpeed = lvl; }
	BYTE GetLVLSpeed() { return _lvlSpeed; }

	void SetLVLMagnet(BYTE lvl) { _lvlMagnet = lvl; }
	BYTE GetLVLMagnet() { return _lvlMagnet; }

	void SetLVLMachete(BYTE lvl) { _lvlMachete = lvl; }
	BYTE GetLVLMachete() { return _lvlMachete; }

	void SetLVLMule(BYTE lvl) { _lvlMule = lvl; }
	BYTE GetLVLMule() { return _lvlMule; }

	void SetLVLCavalry(BYTE lvl) { _lvlCavalry = lvl; }
	BYTE GetLVLCavalry() { return _lvlCavalry; }

	bool CheckCollisonCoin(float coinX, float coinY, float coinRad);
	//effect	

	virtual void UpdateEffect();
	virtual void RenderEffect();

	void InitEffect();
	bool CompleteEffect(BYTE type,BYTE state);
	bool CompleteEffect(BYTE type);
	bool CompleteEffect();
	void StopEffect(BYTE type);
	void SetStateEffect(BYTE type, BYTE state);
	void StopAllEffect();

	virtual void AddEffectGun(float x, float y ,float speed);
	virtual void RenderEffectGun(int i);
	virtual void UpdateEffectGun(int i);

	void AddParticalDie(float x, float y, int numPar, float angle);
	void RenderParticalDie(int i);
	void UpdateParticalDie(int i);

	void AddParticalWagon(float x, float y, int sprite, int beginFarme, int numFrame, int numPar);
	void RenderParticalWagon(int i);
	void UpdateParticalWagon(int i);

	void AddEffectGun2(float x, float y, CArmy* enemy, float speed);
	void AddEffectGun2(float x, float y, float tarx, float tary, float speed);
	void RenderEffectGun2(int i);
	void UpdateEffectGun2(int i);

	void AddEffectHatFall(float x, float y, int typeHat, float speed);
	void UpdateEffectHatFall(int i);
	void RenderEffectHatFall(int i);

	void AddEffectBulet(float x, float y, float amgle);
	void RenderEffectBulet(int i);
	void UpdateEffectBulet(int i);
	bool _autoShot;
	bool _forceShot;
	void RenderEffectTarget();
	bool _isTargetFocus;
};

#endif