#if !_CARMY_H
#define _CARMY_H 1

#define TYPE_ENEMY_BOSS_1				10
#define TYPE_ENEMY_BOSS_2				11
#define TYPE_ENEMY_BOSS_3				12

#define MIN_BOSS_Y						MIN_MOVE_Y + 150
#define MAX_BOSS_Y						MAX_MOVE_Y

#define EFFECT_TYPE_BOSS_GUN_1			20
#define EFFECT_TYPE_BOSS_GUN_2			21
#define EFFECT_TYPE_BOSS_TRAIN_SMOKE	22

#define ENEMY_DIE_BY_GUN				0				
#define ENEMY_DIE_BY_MACHETE			1
#define ENEMY_DIE_BY_MULE				2
#define ENEMY_DIE_BY_CAVLRY				3
#define ENEMY_DIE_BY_ROCKET				4
#define ENEMY_DIE_BY_BOMB				5


#define ENEMY_INIT_BY_NORMAL				0
#define ENEMY_INIT_BY_BARREL				1
#define ENEMY_INIT_BY_BOSS					2
#define ENEMY_INIT_BY_OBSTACLE				3

class CArmy : public CCharater
{
	int _dieBy;
	int _hp;
	int _maxHP;
	float _angleBullet;
	bool _isMoveStraight;
	CAnimObject _animHorse1;
	//CAnimObject _animHorse2;
	CAnimObject _animArmy;
	CAnimObject _animFire[6];
	bool _addRocket;
	int _numFire;
	//int _numBome;
	bool _addgun;
    bool _addgun1;
	float _nextRocketGun;
	float _timeRocketGun;
	float _distanceRocketGun;
	bool _isRocketGun;
	int _numRocket;
	int _direcGun;
	float _tagetMove;
	
	BYTE _stateOld;

	int _perHPRocket;
	int _lastHPRocket;

	int _perHPBome;
	int _lastHPBome;
	int _numBome;
	int _timeBome;

	float _distaneBoss;
	int _maxBome;
	int _timeDeplayBome;

    int _counttttt;
	BYTE _typeDiff;
	BYTE _typeCallArmy;
	int _opactiyBoss;
	int _animIndex;
	bool _fastBome;
	float _posHX, _posHY;

	
	int _indexFameGold;

	bool _renderCavilian[3];
	CAnimObject _animCavilian[3];
	CAnimObject _animLight[6];
	float _lightX[6], _lightY[6];

	int _initBy;

   public:
	   bool _renderTop;
	CArmy(){};
    ~CArmy(){};
	void ReplaceGold(int frame, int module);
	void SetValueBose();
	BYTE GetType(){ return _type; }
	void SetType(BYTE type){ _type = type; }
	void SetMoveStraight(bool value){ _isMoveStraight = value; }
	void Init(float x, float y, BYTE type, BYTE initBy);
	void Update();
	void Render();
	void RenderUnDie();
	void RenderEffectTarget();
	void SetState(BYTE state);

	void SetAngleBullet(float value){ _angleBullet = value; }
	void BeDame(int dame);
    int GetHp(){return _hp;}
	void getArmyRandom(int level);

	void SetDieBy(int val){ _dieBy = val; }
	int GetDieBy(){ return _dieBy; }
	int GetInitBy(){ return _initBy; }
	//bose
	void InitBoss_1(BYTE type);
	void RenderBoss_1();
	void UpdateBoss_1();

	//bose
	void InitBoss_2(BYTE type);
	void RenderBoss_2();
	void UpdateBoss_2();

	//bose
	void InitBoss_3(BYTE type);
	void RenderBoss_3();
	void UpdateBoss_3();
	int _typeGun;

	void GetTypeDiffBoss();
	void GetRanTypeGun(int lvl);
	int _numRanTypeGun;
	int _ranTypeGun[10];

	void GetTypeCallArmy();
	void GetRanTypeEnemy(int lvl);
	int _numRanTypeEnemy;
	int _ranTypeEnemy[10];

	//effect

	void UpdateEffect();
	void RenderEffect();

	void AddEffectGun(float x, float y, float speed, float angle);
	void AddEffectGun(float x, float y, float speed);
	void RenderEffectGun(int i);
	void UpdateEffectGun(int i);

	void AddEffectGunType1(float x, float y, float speed, int num_bulet, int time, float beginA, float offset_angle);
	void UpdateEffectGunType1(int i);

	void AddEffectGunType2(float x, float y, float speed, int num_bulet, float beginA, float offset_h);
	void UpdateEffectGunType2(int i);

	void AddEffectTrainSmoke(float x, float y, float speedMove, float speedScale);
	void UpdateEffectTrainSmoke(int i);
	void RenderEffectTrainSmoke(int i);


	int _spriteBoss;
	int _numCoin;
	int _timeCoin;
	int _opacityHp;
	int _hpEnd;
	void AddCoinBossDie();
	COLOR _color;
	CAnimObject _animEx[4];
	int _countEx;
	int _stateMoveBoss;
    bool _addsoundTrain;
	float _distanceRunPower;
private:

};

#endif
