#include "pch.h"
#include "../GameCore/CGame.h"
#if defined(_WINDOWS)
extern "C" void ShowIADFullScreen(){}
extern "C" void charboostShowAtMenu(const char* title){}
extern "C" void charboostMoreGame(){}
extern "C" void OS_AdBoxDownloadFile(){}
extern "C" void OS_GameCenterAuthentication(){}
extern "C" void OS_GameCenterLoadScore(){}
extern "C" void OS_GameCenterSubmitScore(INT_TIME value, const char* leaderboard){}
extern "C" void OS_ShowLeaderboard(const char*  leaderboard){}

extern "C" void OS_GameCenterLoadFriend(){}
extern "C" void OS_GameCenterSubmitAchievement(int percent, const char* leaderboard, bool noti){}
extern "C" void OS_SystemMessageAsk(const char* feed, int code){}
extern "C" bool isChartboostAvailable(){ return true; }
extern "C" void OS_ResetAllAchievement(){}
extern "C" void ShowAdmobFullScreen(){}
extern "C" bool charboostHasShowVideoReward(const char* title) { return false; }
extern "C" void charboostShowVideoReward(const char* title) {}
extern "C" bool HasAdmobToShow() { return false; }
extern "C" void OpenAppStore(INT64 appleID) {}
#endif
#if defined(_WINDOWS_PHONE8) || defined (_WIN8)
extern "C" void ShowIADFullScreen(){}
extern "C" void charboostShowAtMenu(const char* title){}
extern "C" void charboostMoreGame(){}
extern "C" void OS_AdBoxDownloadFile(){}
extern "C" void OS_GameCenterAuthentication(){}
extern "C" void OS_GameCenterLoadScore(){}
extern "C" void OS_GameCenterSubmitScore(INT_TIME value, const char* leaderboard){}
extern "C" void OS_ShowLeaderboard(const char*  leaderboard){}

extern "C" void OS_GameCenterLoadFriend(){}
extern "C" void OS_GameCenterSubmitAchievement(int percent, const char* leaderboard, bool noti){}
extern "C" void OS_SystemMessageAsk(const char* feed, int code){}
extern "C" bool isChartboostAvailable(){ return true; }
#endif

