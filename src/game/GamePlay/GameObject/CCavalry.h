#if !_CCAVALRY_H
#define _CBOSS_H 1

#define BOSS_STATE_INIT					0
#define BOSS_STATE_MOVE					1
#define BOSS_STATE_STAND				2
#define BOSS_STATE_BE_ACCTACK			3
#define BOSS_STATE_DIE					4

class CCavalry : public CCharater
{
public:
	CCavalry(){};	
	void Init(float x, float y, BYTE type);
	void Update();
	void Render();
	void SetState(BYTE state);

	
private:

};

#endif