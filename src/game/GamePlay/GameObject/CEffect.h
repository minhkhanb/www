#if !_EFFECT_MANAGER_H
#define _EFFECT_MANAGER_H 1

#define EFFECT_MANAGER_MAX_CACHE	(100)

#define EFFECT_PATICAL_MAX_CACHE	(150)

#define EFFECT_MANAGER_TYPE_ROCKET_SMOKE	0
#define EFFECT_MANAGER_TYPE_PATICAL_DUST	1
#define EFFECT_MANAGER_TYPE_PATICAL_RAIN	2
#define EFFECT_MANAGER_TYPE_TEXT_FLY		3

#define EFFECT_MANAGER_STATE_NONE       0



class CEffectManager
{

	//effect Attack 
	void RenderEffectText(int i);
	void UpdateEffectText(int i);

	void RenderEffectFirework(int i);
	void UpdateEffectFirework(int i);

	bool isMoveToTarget(float &angle, float &x1, float &y1, float x2, float y2, float speed);



public:
	CEffectManager(){};
	BYTE _state[EFFECT_MANAGER_MAX_CACHE];
	BYTE _type[EFFECT_MANAGER_MAX_CACHE];
	
	const char* _string[EFFECT_MANAGER_MAX_CACHE];

	int _var1[EFFECT_MANAGER_MAX_CACHE];
	int _var2[EFFECT_MANAGER_MAX_CACHE];
	int _var3[EFFECT_MANAGER_MAX_CACHE];
	int _var4[EFFECT_MANAGER_MAX_CACHE];

	int _var5[EFFECT_MANAGER_MAX_CACHE];
	int _var6[EFFECT_MANAGER_MAX_CACHE];
	int _var7[EFFECT_MANAGER_MAX_CACHE];
	int _var8[EFFECT_MANAGER_MAX_CACHE];

	float _varF1[EFFECT_MANAGER_MAX_CACHE];
	float _varF2[EFFECT_MANAGER_MAX_CACHE];	
	float _x[EFFECT_MANAGER_MAX_CACHE];
	float _y[EFFECT_MANAGER_MAX_CACHE];

	//patical
	bool CompleteStatePartical(int i, int num, BYTE state);

	BYTE _par_state[EFFECT_MANAGER_MAX_CACHE*EFFECT_PATICAL_MAX_CACHE];
	float _par_x[EFFECT_MANAGER_MAX_CACHE*EFFECT_PATICAL_MAX_CACHE];
	float _par_y[EFFECT_MANAGER_MAX_CACHE*EFFECT_PATICAL_MAX_CACHE];
	float _par_angle[EFFECT_MANAGER_MAX_CACHE*EFFECT_PATICAL_MAX_CACHE];	
	float _par_speed[EFFECT_MANAGER_MAX_CACHE*EFFECT_PATICAL_MAX_CACHE];	
	int _par_frame[EFFECT_MANAGER_MAX_CACHE*EFFECT_PATICAL_MAX_CACHE];
	int _par_opacity[EFFECT_MANAGER_MAX_CACHE*EFFECT_PATICAL_MAX_CACHE];

	void Init();


	//main
	void Update();
	void Render();

	bool CompleteEffect(BYTE type);
	bool CompleteEffect();
	void StopEffect(BYTE type);
	void SetStateEffect(BYTE type,BYTE state);
	void AddEffectSmoke(float x, float y, float speed);
	void RenderEffectSmoke(int i);
	void UpdateEffectSmoke(int i);

	void AddParticalDust(int sprite, int numFrame,int time, int direction, int beginAngle, int endAngle);
	void UpdateParticalDust(int i);
	void RenderParticalDust(int i);

	void AddParticalRain(int time);
	void UpdateParticalRain(int i);
	void RenderParticalRain(int i);

	void AddEffectTextFly(float x, float y, const char* string);
	void UpdateEffectTextFly(int i);
	void RenderEffectTextFly(int i);

};

#endif