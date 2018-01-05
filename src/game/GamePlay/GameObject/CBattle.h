#if !_CBATTLE_H
#define _CBATTLE_H 1

#define COIN_STATE_INIT             0
#define COIN_STATE_INIT_FALL        1

#define COIN_STATE_MOVE_CHARATER    2
#define COIN_STATE_COLLECT          3

struct Coin
{
	float _angle;
    CAnimObject _animCoin;
    float _x,_y;
    //int _timeAppear;
	int _direc;
	BYTE _state;
	float _v0;
	float _g;
	float _x0;
	float _y0;
	float _targety0;
	int _t0;
	int _count;
	int _type;
};


#define OBSTACLE_TNT				0
#define OBSTACLE_TRAP				1
#define OBSTACLE_BOME				2
#define OBSTACLE_CASTUS				3
#define OBSTACLE_CHESS				4
#define OBSTACLE_BARREL				7

#define OBSTACLE_STATE_INIT_FALL        5
#define OBSTACLE_STATE_INIT             0
#define OBSTACLE_STATE_ACTIVE			1
#define OBSTACLE_STATE_EXPLOSION		2
#define OBSTACLE_STATE_COLLECT			3

#define OBSTACLE_ACTIVE_BY_CHARACTER		0
#define OBSTACLE_ACTIVE_BY_BOMB				0
#define OBSTACLE_ACTIVE_BY_ROCKET			0



struct Obstacle
{
	CAnimObject _animObstacle;
	CAnimObject _animExplosion;
	int _activeBy;
	int _countFrame;
	BYTE _state,_type,_stateOld;

	bool _isCharater;
	float _x, _y;
	float _Cx, _Cy,_Cw,_Ch;	

	float _angle;
	float _v0;
	float _g;
	float _x0;
	float _y0;
	float _targety0;
	int _t0;
	int _count;

	BYTE _direcBome;
	
	int _opacity;
	int _numCoin;
	float _activeX;
	bool _renderTop;
};

#define ROCKET_STATE_INIT_FOLLOW		0
#define ROCKET_STATE_MOVE				1
#define ROCKET_STATE_EXPLOSION			2
#define ROCKET_STATE_INIT_GUN			3
struct Rocket
{
	CAnimObject _animAlert;
	float _x, _y;
	
	BYTE _state;
	int _countSmoke;
	int _timeGun;
	int _countFrame;
	bool _renderTop;
};

#define POWER_STATE_INIT			0
#define POWER_STATE_USE				1
struct Power
{
	CAnimObject _animPower;
	float _x, _y;
	float _cX, _cY,_cW,_cH;
	BYTE _state;
	BYTE _type;
};
struct DistanceInit
{
	BYTE _type;
	float _distacne;
};

#define BOSS_STATE_NONE					0
#define BOSS_STATE_ACTIVE_1				1
#define BOSS_STATE_RUN_1				2
#define BOSS_STATE_ACTIVE_2				3
#define BOSS_STATE_RUN_2				4
#define BOSS_STATE_ACTIVE_3				5
#define BOSS_STATE_RUN_3				6

class CArmy;
class CCavalry;
#include "../../GameCore/CAnimManager.h"
class CBattle
{
public:
	CBattle();
	int _mullScore;
	int _mullCoin;
	int _numLife;

	int _mull;
	CDrawableManager _animManager;
	CAnimObject _bossComming;
	int _timeBegin;
	int _timePlay;
	float _checkPointX, _checkPointY;
	
	float _tempDistance;
	float _offsetDistance;

	float _bossIncomingX;
	 BYTE _stateBossIncoming;
	int _countIncoming;
	int _numArmy;
    int _numCoin;
	int _numObstacle;

	BYTE _lvlCheckPiont;
	BYTE _lvlBosse;
	CArmy** _army;
    Coin** _coin;
	Obstacle** _obstacle;

	bool _isRunRocket;
	int _maxTimeRocket;
	INT32 _coinCollect;
    float _distanceRun;
	int _lvl_des;

	int _typePowerOld;
	int _arrayPower[3];

	int _totalRandom;
	float _distanceNext;
	//bool _addPower;
    float _distancePower;
	float _distanceBoss;
	float _distanceW;
	float _distanceRocketW;	
	float _distanceRocket;
	//enemy
	int _countArmy;
	int _numArmyInit;
	
	int _numRandArmy;
	BYTE _randomTypeArmy[10];

	int _numRandPowerArmy;
	BYTE _randomTypePowerArmy[10];

	//coin
	int _countCoin;
	int _numCoinInit;	

	//obstacle
	int _countObs;
	int _numObsInit;

	int _numRandObs;
	BYTE _randomTypeObs[100];

	int _numRandRocket;
	BYTE _randomTypeRocket[100];

	//Rocket
	Rocket** _rocket;
	int _numRocket;
	int _timeRocketGun;
	int _timeDeplayRocket;

	int _countRocket;
	int _numRocketInit; 

	void SetNextPower(BYTE lvl);
	//power
	Power** _power;
	int _numPower;
	bool _isUsePower;

	float _speedIncreaseMap;
	float _speedIncreaseMove;

	//cavalry
	CCavalry** _cavalry;
	int _numCavalry;
	void AddCavalry(int num);
	void UpdateCavalry();

	//
	float _distanceArmyNext;
	float _distanceArmyW;

	BYTE _addBoss;

	bool _addTutorial;
	void UpdateInitFromDesign();
	
	//CBattle(){};

	void AddCoin(int num, float x, float y, BYTE type = 0, float angle = 0, BYTE state = COIN_STATE_INIT, float targetX = 0, float targetY = 0);
	void AddObstacle(int num, float x, float y, BYTE type, BYTE state, float ActiveX = 0, float rangeMax = 0, float angle = 0);
	BYTE GetIndexObstacle(BYTE type);
	BYTE GetIndexAnimObstacle(BYTE type);
	void GetObstacleRandom(int level);
	void GetArmyRandom(int level);
	void GetRocketRandom(int level);
	void GetPowerArmyRandom(int lvlPower);

	void SetAnimCheckPoint(BYTE lvl);
	void Init();
	void Update();
	void Render();
	void AddArmy(BYTE type, float x, float y, BYTE State, BYTE initBy);
	void AddRocket(int num, float x, float y, BYTE state, int timeGun = 0);
	void AddPower(float x, float y, BYTE type);

	void SetDistanceInitArmy();
	void UpdateInitObstacleBoss();
	
	float _scoreRun;
	float _nextObstacle;

	bool checkRectOverlapRect(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
	bool CheckCollisionArmy();
    void CheckCollisionCoin();
	bool CheckCollisionObstacle();

	void UpdateCoin();
	void UpdateArmy();
	void UpdateObstacle();
	void UpdateRocket();
	void UpdatePower();
	void GetArmyAttack(CCharater *charater);

	//effect coin
	float _scaleCoin;
	BYTE _stateCoin;


	int _currentMix;
	int *_typMix;
	void SetMixInit();
	void UpdateIniteMix();
	int getTypePercent(int percent, BYTE lvl);

	bool CheckRocketComplete();

	BYTE _stateVibrate;
	float _vibrateY;
	void SetVibrate() { _stateVibrate = 1; };
	void UpdateVibrate();

	bool checkHasBoss();
	bool _isTargetTrain;
	float _targetTrainX;
	float _targetTrainY;
	float _endTrain;
	bool _isEndTrain;
	float _offsetMoveY; 
	CArmy* GetArmyBoss(BYTE type);	
	CArmy* GetArmyState(BYTE State);
	bool _isCalculateDistance;

	int NumRenderEnemy();
	int NumRenderOBS();
	int NumRenderCOIN();

	COLOR _colorDay;
	bool _isDay;
	bool _changeDay;
	int _opacityDay;
	float _currentDistance;
	float _maxDistance;
	bool _addText;
	void RenderTimeBar();

private:

};

#endif