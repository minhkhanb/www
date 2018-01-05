#include "pch.h"
#include "../../GameCore/CGame.h"

void CAchievement::LoadInfoQuest() {
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	num_quest_done = 0;
	int num = 4;
	int end[4] = { 4, 5, 6, 7 };
	bool remove = false;

	char rewardQuest[128];
	
	for (int i = 0; i < MAX_QUEST; i++)
	{
		srand(time(NULL));
		switch (i) {

		case 0:
			game->_quest_info[i]._id = CMath::RANDOM(0,1);
			_maxQuest[i] = GET_DES().getDailyQuest_number(game->_quest_info[i]._id, DAILY_NUMBER_VALUE);
			sprintf(_descQuest, "%s", GET_DES().getDailyQuest_text(game->_quest_info[i]._id, DAILY_NUMBER_VALUE));
			break;
		case 1:
			game->_quest_info[i]._id = CMath::RANDOM(2,3);
			_maxQuest[i] = GET_DES().getDailyQuest_number(game->_quest_info[i]._id, DAILY_NUMBER_VALUE);
			sprintf(_descQuest, "%s", GET_DES().getDailyQuest_text(game->_quest_info[i]._id, DAILY_NUMBER_VALUE));
			description[i] = game->_quest_info[i]._id;
			break;

		}
		if (i >= 2)
		{
			int rand = CMath::RANDOM(0, num - 1);
			game->_quest_info[i]._id = end[rand];
			
			_maxQuest[i] = GET_DES().getDailyQuest_number(game->_quest_info[i]._id, DAILY_NUMBER_VALUE);
			sprintf(_descQuest, "%s", GET_DES().getDailyQuest_text(game->_quest_info[i]._id, DAILY_NUMBER_VALUE));
			description[i] = game->_quest_info[i]._id;

			for (int j = rand; j < num; j++)
			{
				end[j] = end[j + 1];
			}
			num--;
		}

		sprintf(game->_quest_info[i]._name, "%s", GET_DES().getDailyQuest_text(game->_quest_info[i]._id, 0));
		sprintf(game->_quest_info[i]._title, "%s", GET_DES().getDailyQuest_text(game->_quest_info[i]._id, 1));
		sprintf(game->_quest_info[i]._desc, "%s", game->_quest_info[i]._title);
		sprintf(game->_quest_info[i]._progress, "Progress: %d", _percentQuest[i]);
		game->_quest_info[i]._reward_quest = GET_DES().getDailyQuest_number(game->_quest_info[i]._id, 1);

		

		//PDEBUG("\n _descQuest:%s ### _maxQuest:%d ### randQuest:%d \n\n", _descQuest, _maxQuest[i], randQuest);
		_currentQuest[i] = 0;

	}
}

void CAchievement::LoadInfoQuestFromSave()
{
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	for (int i = 0; i < MAX_QUEST; i++)
	{
		_maxQuest[i] = GET_DES().getDailyQuest_number(game->_quest_info[i]._id, DAILY_NUMBER_VALUE);
		description[i] = game->_quest_info[i]._id;
		sprintf(_descQuest, "%s", GET_DES().getDailyQuest_text(game->_quest_info[i]._id, DAILY_NUMBER_VALUE));
		sprintf(game->_quest_info[i]._name, "%s", GET_DES().getDailyQuest_text(game->_quest_info[i]._id, 0));
		sprintf(game->_quest_info[i]._title, "%s", GET_DES().getDailyQuest_text(game->_quest_info[i]._id, 1));
		sprintf(game->_quest_info[i]._desc, "%s", game->_quest_info[i]._title);
		_percentQuest[i] = GetPercentQuest(i);
		sprintf(game->_quest_info[i]._progress, "Progress: %d", _percentQuest[i]);
		game->_quest_info[i]._reward_quest = GET_DES().getDailyQuest_number(game->_quest_info[i]._id, 1);
		_currentQuest[i] = 0;

	}
}
void CAchievement::ResetInfoQuest() {
	_dailyQuestCurrent = 0;
	LoadInfoQuest();
	_numDailyBombActive = 0;
	_totalDailyCoinCollect = 0;
	_numDailyRocketByBarrel = 0;
	_numDailyEnemyJumpBarrel = 0;
	_numDailyUsePowerUp = 0;
	for (int i = 0; i < MAX_QUEST; i++)
	{
		if (_isQuestComplete[i] != QUEST_NONE)
		{
			_isQuestComplete[i] = QUEST_NONE;
		}
	}
}

void CAchievement::UpdateInfoQuest() {
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	//PDEBUG("\n\n\nUpdate quest\t");
	for (int i = 0; i < MAX_QUEST; i++)
	{
		if (_isQuestComplete[i] == QUEST_END)
		{
			game->_quest_info[i].isComplete = true;
		}
	}
}
void CAchievement::LoadInfoAchievement()
{
	for (int i = 0; i < TOTAL_ACHIEVEMENT_DES_ROW; i++)
	{
		_maxValue[i] = GET_DES().getAchievementDes(i, ACHIEVEMENT_DES_VALUE);
		_currentValue[i] = 0;
		if (_isComplete[i] == ACHIEVEMENT_COMPLETE)
		{
#if defined MAC_OS
			if (GAME()->_pine_social->GetState() == PINESOCIAL_STATE_COMPLETED)
			{
				PineGameCenterController::UpdateAchievement(GET_DES().getAchievementName(i, ACHIEVEMENT_NAME_ACHIEVEMENT_ID), _percent[i], true);
				_isComplete[i] = ACHIEVEMENT_END_SUBMIT;
			}
#elif defined (_WINDOWS_PHONE8) || defined(_WIN8) || defined(ANDROID_OS)
			{
				//GAME()->_social.UpdateAchievementsFB(&GAME()->_svc_update_achievement_fb, "699158856865846");
				_isComplete[i] = ACHIEVEMENT_END_SUBMIT;
			}

#endif
		}
	}
}

int CAchievement::GetPercent(int index)
{
	int res = 0;
	res = ((float)_currentValue[index] / (float)_maxValue[index]) * 100;
	if (res >= 100)
	{
		res = 100;
	}
	return res;
}

int CAchievement::GetPercentQuest(int index) {
	int res = 0;
	res = ((float)_currentQuest[index] / (float)_maxQuest[index]) * 100;
	if (res >= 100)
	{
		res = 100;
	}
	return res;
}

void CAchievement::SetCompleteAchivement(int index)
{
	if (_isComplete[index] == ACHIEVEMENT_NONE)
	{
		_currentValue[index] = _maxValue[index];
		int newPercent = 0;
		newPercent = GetPercent(index);
		if (newPercent > _percent[index])
		{
			_percent[index] = newPercent;

#if defined MAC_OS
			if (GAME()->_pine_social->GetState() == PINESOCIAL_STATE_COMPLETED)
			{
				PineGameCenterController::UpdateAchievement(GET_DES().getAchievementName(index, ACHIEVEMENT_NAME_ACHIEVEMENT_ID), _percent[index], true);
				if (CheckComplete(index))
				{
					_isComplete[index] = ACHIEVEMENT_END_SUBMIT;
				}
			}
			else
#endif
			{
				if (CheckComplete(index))
				{
					_isComplete[index] = ACHIEVEMENT_COMPLETE;
				}
			}
		}
	}

}

void CAchievement::DailyQuestNext()
{
	_dailyQuestCurrent++;
//	if (_dailyQuestCurrent == 2 && !USER()._hasRate)
//	{
//#if MAC_OS
//		//PineSocial::ShowPopUpRating(APPIRATER_MESSAGE_TITLE, APPIRATER_MESSAGE);
//#elif ANDROID_OS
//		sprintf(GAME()->_async_task_manager._message_box_title, "%s", APPIRATER_MESSAGE_TITLE);
//		sprintf(GAME()->_async_task_manager._message_box_message, "%s", APPIRATER_MESSAGE);
//		GAME()->_async_task_manager._message_box_confirm_type = CAsyncTasks::enumCloudSaveConfirmType::CONFIRM_TYPE_RATE;
//		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_SHOW_MESSAGE_BOX);
//
//#endif
//	}
	_numEnemyDie = 0;
	_numDailyRocketByBarrel = 0;
	_totalDailyCoinCollect = 0;
	_numDailyBombActive = 0;
	_numCoinColect = 0;
	_numDailyEnemyJumpBarrel = 0;
	_numDailyUsePowerUp = 0;
	if (_dailyQuestCurrent >= MAX_QUEST)
	{
		_dailyQuestCurrent = MAX_QUEST;
		return;
	}
}

void CAchievement::CheckQuestComplete() {
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
 	for (int i = 0; i < MAX_QUEST; i++)
	{
		if (i < _dailyQuestCurrent)
		{
			_isQuestComplete[i] = QUEST_END;
			game->_quest_info[i]._readyCollect = true;
			game->_quest_info[i].isComplete = true;
			continue;
		}
		else
		{
			
			if (i == _dailyQuestCurrent)
			{
				if (_isQuestComplete[i] != QUEST_END)
				{
					_currentQuest[i] = 0;
					int newPercent = 0;

					switch (game->_quest_info[i]._id)
					{
					case 0:
					case 4:
						_currentQuest[i] = _numEnemyDie;
						break;
					case 1:
						_currentQuest[i] = _numDailyUsePowerUp;
						break;
					case 2:
					case 6:
						_currentQuest[i] = _totalDailyCoinCollect;
						break;
					case 3:
					case 7:
						_currentQuest[i] = _numDailyBombActive;
						break;
					case 5:
						_currentQuest[i] = _numCoinColect;
						break;
					}
					
					newPercent = GetPercentQuest(i);
					//PDEBUG("\n _currentQuest[%d]:%d -- newPercent:%d -- _percentQuest[i]:%d", i, _currentQuest[i], newPercent, _percentQuest[i]);
					if (newPercent >= _percentQuest[i])
					{
						_percentQuest[i] = newPercent;
						{
							if (_percentQuest[i] >= 100)
							{
								_isQuestComplete[i] = QUEST_COMPLETE;
								game->_quest_info[i]._readyCollect = true;
								num_quest_done++;
							}
						}
					}
					sprintf(game->_quest_info[i]._progress, "Progress: %d", _percentQuest[i]);
				}
				else
				{
					_currentQuest[i] = 0;
				}
			}
			else
			{
				_percentQuest[i] = 0;
				_isQuestComplete[i] = QUEST_NONE;
				_currentQuest[i] = 0;
				game->_quest_info[i]._readyCollect = false;
				game->_quest_info[i].isComplete = false;
				sprintf(game->_quest_info[i]._progress, "Progress: %d", _percentQuest[i]);
			}
		}
		
		//PDEBUG("\n sumited _maxQuest %s: %d", GET_DES().getDailyQuest_text(i, 0), _maxQuest[i]);
	}
}

void CAchievement::CheckAchievementComplete()
{
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	for (int i = 0; i < TOTAL_ACHIEVEMENT_DES_ROW; i++)
	{
		if (_isComplete[i] == ACHIEVEMENT_NONE)
		{
			_currentValue[i] = 0;
			int newPercent = 0;

			switch (i)
			{
			case 0:
				_currentValue[i] = _numEnemyDie;
				break;

			case 1:
				_currentValue[i] = _totalEnemyDie;
				break;

			case 2:
			case 3:
				_currentValue[i] = _numCoinColect;
				break;

			case 4:
			case 5:
				_currentValue[i] = _totalCoinColect;
				break;

			case 6:
				_currentValue[i] = _numPowerUse;
				break;

			case 7:
				_currentValue[i] = _numEnemyJumpBarrelDie;
				break;

			case 8:_currentValue[i] = _totalEnemyJumpBarrelDie;
				break;

			case 9:
				_currentValue[i] = _numRocketOpenBarrel;
				break;

			case 10:
				_currentValue[i] = _totalRocketOpenBarrel;
				break;

			case 11:
				_currentValue[i] = _numBombOpenBarrel;
				break;

			case 12:
				_currentValue[i] = _totalBombOpenBarrel;
				break;

			case 13:
			case 14:
				_currentValue[i] = _numBombActive;
				break;

			case 15:
			case 16:
				_currentValue[i] = _totalBombActive;
				break;

			case 17:
				_currentValue[i] = _totalEnemyDieByRocket;
				break;

			case 18:
				_currentValue[i] = _totalEnemyDieByBomb;
				break;

			case 19:
				break;

			case 20:
				break;
			}

			newPercent = GetPercent(i);
			if (newPercent > _percent[i])
			{
				_percent[i] = newPercent;

#if defined MAC_OS
				if (GAME()->_pine_social->GetState() == PINESOCIAL_STATE_COMPLETED)
				{
					PineGameCenterController::UpdateAchievement(GET_DES().getAchievementName(i, ACHIEVEMENT_NAME_ACHIEVEMENT_ID), _percent[i], true);
					if (CheckComplete(i))
					{
						_isComplete[i] = ACHIEVEMENT_END_SUBMIT;
					}
				}
				else
#endif
				{
					if (CheckComplete(i))
					{
						_isComplete[i] = ACHIEVEMENT_COMPLETE;
						PDEBUG("\n ---------------> Submit achievement: %d -- value:%d", i, _currentValue[i]);
						num_quest_done++;
						game->SubmitAchievement(i);

					}
				}
			}
		}
		//PDEBUG("\n achivemtn %d => %d", i, _percent[i]);
	}

	//printf("\n =======================\n\n");



}
bool CAchievement::CheckComplete(int index)
{
	if (_percent[index] >= 100)
	{
		return true;
}
	return false;
}