#if !CACHIEVEMENT_H
#define CACHIEVEMENT_H 1

#define MAX_ACHIEVEMENT_VER_0		50
#define MAX_QUEST					(5)

#define ACHIEVEMENT_NONE			0
#define ACHIEVEMENT_COMPLETE		1
#define ACHIEVEMENT_END_SUBMIT		2

#define QUEST_NONE					0
#define QUEST_COMPLETE				1
#define QUEST_END					2

class CAchievement
{
public:
	
	//single
	int _numEnemyDie;
	int _numCoinColect;
	int _numEnemyJumpBarrelDie;
	int _numRocketOpenBarrel;
	int _numBombOpenBarrel;
	int _numBombActive;
	int _numPowerUse;
	//total
	int _totalEnemyDie;
	INT32 _totalCoinColect;
	int _totalEnemyJumpBarrelDie;
	int _totalRocketOpenBarrel;
	int _totalBombOpenBarrel;
	int _totalBombActive;
	int _totalEnemyDieByRocket;
	int _totalEnemyDieByBomb;

	//daily
	int _numDailyBombActive;
	int _totalDailyCoinCollect;
	int _numDailyRocketByBarrel;
	int _numDailyEnemyJumpBarrel;
	int _numDailyUsePowerUp;
	int _dailyQuestCurrent;

	BYTE _isComplete[MAX_ACHIEVEMENT_VER_0];
	int _percent[MAX_ACHIEVEMENT_VER_0];
	int _currentValue[MAX_ACHIEVEMENT_VER_0];
	int _maxValue[MAX_ACHIEVEMENT_VER_0];

	BYTE _isQuestComplete[MAX_QUEST];
	int _percentQuest[MAX_QUEST];
	int _currentQuest[MAX_QUEST];
	int _maxQuest[MAX_QUEST];
	int description[MAX_QUEST];
	char _descQuest[128];
	
	

	int num_quest_done;

	int GetPercent(int index);
	void LoadInfoAchievement();
	void CheckAchievementComplete();
	bool CheckComplete(int index);

	void LoadInfoQuest();
	void LoadInfoQuestFromSave();
	void UpdateInfoQuest();
	void ResetInfoQuest();
	void CheckQuestComplete();
	int GetPercentQuest(int index);
	void DailyQuestNext();

	void SetCompleteAchivement(int index);
};

#endif
