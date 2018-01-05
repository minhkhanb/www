

#include "../GameCore/PineGame.h"

#ifndef WILD_WILD_WEST_GAME
#define WILD_WILD_WEST_GAME

#include <stdio.h>
class WildWildWestGame :public PineGame
{
public:
	void UpdateAndRender();
	void Init();

	//////////////////////IMPLEMENT GAMEPLAY
	void UpdateLogo();
	void RenderLogo();

	bool _autoLoadingStep;
	int _loading_step;

	//object
	CCharater _charater;
	CBattle _battle;
	CBattleMap _battleMap;
	CObstacle _obstacle;
	Design_Util _design_util;
	CEffectManager _effect;
	CAchievement _achievement;
	CUser _user;

#define PLAYGAME_BUTTON_NUMS			1
#define PLAYGAME_BUTTON_PAUSE			0

	Button _buttonPlayGame[PLAYGAME_BUTTON_NUMS];

	//state play game
	void UpdateStatePlayGame();
	void RenderStatePlayGame();
	bool TouchButtonPlayGame();

	//state result
	void UpdateStateResult();
	void RenderStateResult();
	bool TouchButtonStateResult();

	bool _isWaiting, _notShowPineAdds, _not_showads_interrupt;

	CAnimObject _animGameOver;

	void UpdateScrollCoin();
	INT32 _coin_scroll;
	int _deltaBone;
	bool _increase;
	bool _descrease;

	bool _isMusicEnable;
	bool _isSoundEnable;
	bool _isNotiEnable;

	int _purchase_iap_id;


	//state main menu
#define MAINMENU_BUTTON_NUMS				17
#define MAINMENU_BUTTON_SETING				0
#define MAINMENU_BUTTON_RANKING				1
#define MAINMENU_BUTTON_MOREGAME			2
#define MAINMENU_BUTTON_PLAY				3
#define MAINMENU_BUTTON_SHOP				4
#define MAINMENU_BUTTON_IAP					5
#define MAINMENU_BUTTON_SHOP_YES			6
#define MAINMENU_BUTTON_SHOP_NO				7
#define MAINMENU_BUTTON_SETTING_MUSIC		8
#define MAINMENU_BUTTON_SETTING_SOUND		9
#define MAINMENU_BUTTON_SETTING_NOTIFY		10
#define MAINMENU_BUTTON_SETTING_ABOUT		11
#define MAINMENU_BUTTON_SETTING_RESET		12
#define MAINMENU_BUTTON_ACHIEVEMENT			13
#define MAINMENU_BUTTON_GGPLAY				14
#define QUEST_BUTTON_GIFT					15
#define	QUEST_BUTTON_TASK					16

//ggplay menu
#define GGPLAY_BUTTON_NUM					4

#define GGPLAY_BUTTON_LEADERBOARD			0
#define GGPLAY_BUTTON_ACHIEVEMENT			1
#define GGPLAY_BUTTON_SYNC					2
#define GGPLAY_BUTTON_CLOSE					3

	Button _buttonGGPlayMenu[GGPLAY_BUTTON_NUM];

	Button _buttonMainMenu[MAINMENU_BUTTON_NUMS];
	CAnimObject _animBird;
	CAnimObject _animGrass1;
	CAnimObject _animGrass2;
	CAnimObject _animTree;
	CAnimObject _animCharater;
	float _birdX;
	CAnimObject _animCoin;
	//state play game
	void UpdateStateMainMenu();
	void RenderStateMainMenu();
	bool TouchButtonMainMenu();
    bool TouchButtonMainMenu(int touch_x, int touch_y, int touch_state, int touch_id);
    bool ProcessingButton();
	void MainMenuInit();

	bool ProcessingButtonGGPlay();
	bool ProcessingButtonQuest();
	bool TouchButtonGGPlay(int touch_x, int touch_y, int touch_state, int touch_id);

	float _scale_leaderboard, _scale_achievement, _scale_sync;

	//ggplay menu
	void MenuGGPlayActive();
	void MenuGGPlayInit();
	void MenuGGPlayRender();
	void MenuGGPlayUpdate();
	//quest
#define NUM_BUTTON_QUEST			2
#define BUTTON_QUEST_CLAIM			0
#define BUTTON_QUEST_X2REWARD		1
	
	CScrollBar _scroll_quest;

	float _quest_offsetDrag;
	float _quest_touch_drag_y;
	float _quest_scroll_y;

	struct CQuest
	{
		float _x, _y;
		int _id;
		bool _isDoubleReward;
		bool isComplete;
		bool _readyCollect;
		int reward_coin;
		int _index;
		char _name[128];
		char _title[128];
		char _desc[128];
		char _progress[128];
		int _reward_quest;
		char rewardQuest[128];
		void Init();
		void Render(float x, float y, int opac);
		void Update();
		bool UpdateTouch();
		CAnimObject _animCoin;
		Button _quest_button[NUM_BUTTON_QUEST];
	};
	//Button _quest_notify[NUM_BUTTON_QUEST];
	
	CQuest _quest_info[MAX_QUEST];
	int _quest_info_num;
	int index_quest;

	void NotifyRender(float x, float  y, int opac);
	
#define QUEST_ITEM_WIDTH		700
#define QUEST_ITEM_HEIGHT		232

	void QuestActive();
	void QuestInit();
	void QuestRender(float x, float y, int opac);
	void QuestUpdate();
	int NumQuestCompleted();
	int _numQuestCompleted;

	bool _quest_popup_isclose, _quest_popup_active;


	bool _is_ggplay_menu_active, _is_ggplay_menu_close;

	bool isShowQuest, isCloseQuest;
    
	void RenderStringVsWidth(int SpriteFont, float x, float y, INT32 value, float maxWidth, const char* ch = "");
	void RenderMoney(float x, float y, float maxW, INT32 value, float scale, bool updateAnim = true);

	//setting & about
	CAnimObject _animSetting;
	void InitSetting();
	void CloseSetting();
	bool _closeAbout;
	bool _scrollLogo;
	float _aboutScrollY;
	int _opacityAbout;
	int _opacityMainMenu;
	void RenderSetting();
	void RenderAbout();


	void RenderLocation();
	void UpdateChooseLocation();
    void UpdateChooseLocation(int touch_x, int touch_y, int touch_state, int touch_id);
	float _moveButtonPlay;
	bool _isRenderSetting;
	bool _isRenderAbout;

	bool _isRenderShop;
	bool _isMoveButon;
	int _opacityShop;
	int _ChosseIndex;
	CAnimObject _animShop;
	CScrollBar _scroll_shop;

	float _offsetDrag;
	float _touch_drag_y;
	float _scroll_y;


	int _numShopIap;
	int _numShopPower;
	int _numShopCharacter;

#define ITEM_CAN_BUY			0
#define ITEM_CAN_MAX			1
#define ITEM_USE				2
#define ITEM_UNLOCK				3




#define TYPE_SHOP_SPEED				37
#define TYPE_SHOP_FAST_TRIGGER		36
#define TYPE_SHOP_REVOLVER			35
#define TYPE_SHOP_MAGNET			38
#define TYPE_SHOP_MACHETE			39
#define TYPE_SHOP_MULE				40
#define TYPE_SHOP_CAVALRY			41
#define TYPE_SHOP_LIVE				34


#define TYPE_SHOP_IAP_PACK_1		42
#define TYPE_SHOP_IAP_PACK_2		43
#define TYPE_SHOP_IAP_PACK_3		44
#define TYPE_SHOP_IAP_PACK_4		45
#define TYPE_SHOP_IAP_REMOVE		46
#define TYPE_SHOP_IAP_RESTORE		47


#define TYPE_SHOP_CHARATER_0		100
#define TYPE_SHOP_CHARATER_1		101
#define TYPE_SHOP_CHARATER_2		102
#define TYPE_SHOP_CHARATER_3		103

	struct ItemShop
	{
		BYTE _type;
		BYTE _stateScale;
		float _scale;
		INT32 _money;
		char _itemName[128];
		char _itemDescrip[128];
		char _itemDescrip_2[128];

		BYTE _state;
		float _y;
		BYTE _currnetLvl;

		CAnimObject _animNotify;
		bool _isLock;
		bool _isAcctive;

		bool _unlock;
		bool _isuse;
		int _spriteID;

		CAnimObject _animCharater;
	};
	ItemShop _itemShop[10];

#define NOTIFY_TYPE_NONE		0
#define NOTIFY_TYPE_NEW			1
#define NOTIFY_TYPE_OLD			2

	BYTE _isNotifyRevolver;
	BYTE _isNotifyFastTrigger;
	BYTE _isNotifySpeed;
	BYTE _isNotifyMagnet;
	BYTE _isNotifyMachete;
	BYTE _isNotifyMule;
	BYTE _isNotifyCavalry;
	BYTE _isNotifyShop;

	int _effect_sta[4], _effect_opa[4];
	float _effect_accel[4], _effect_y[4];
	int _beginEffect;

	bool _touchDrag;
	bool _quest_touchDrag;
	BYTE _typeShop;
	float _broadWidth;
	int _maxButtonShop;

	bool _optimizeDate;

#define SHOP_TYPE_CHARATER		    0
#define SHOP_TYPE_POWER			    1
#define SHOP_TYPE_IAP				2

	void IniteShop();
	void SwitchTypeShop(BYTE type);

	void UpdateScrollShop();
    void UpdateScrollShop(int touch_x, int touch_y, int touch_state, int touch_id);
	void UpdateScrollQuest(int touch_x, int touch_y, int touch_state, int touch_id);
    
	void RenderShop(float x, float y);
	bool TouchButtonShop();
	void ProccessTouch(BYTE index);

	//state igm
#define IGM_BUTTON_NUMS				2
#define IGM_BUTTON_RESUM			0
#define IGM_BUTTON_END				1

	Button _buttonIGM[IGM_BUTTON_NUMS];

	//state play game
	void UpdateStateIGM();
	void RenderStateIGM();
	bool TouchButtonIGM();
	CAnimManager _gameplay_list_render;

	int _scrollScore;
	float _scoreX, _scoreY;
	float _scaleScore;
	int _stateScore;
	int _opacityNew;
	bool _playWitchEndlest;
	bool _isGameOver;
	bool _newScore;
	void UpdateEffectScore();

	void RenderResult();
	void CheckNewNotify();

	float _rota1, _rota2;

	void UpdateTest();
	void RenderTest();
	bool _isRain;
	bool _isTouchMove;
	int _timeGunOver;
	int _timeKnife;
	int _timeTrainGoin;
	//int _opacityRain;

	//tutorial

#define TUTORIAL_BUTTON_NUMS		1
#define TUTORIAL_BUTTON_CONTINUE	0


	Button _buttonTutorial[TUTORIAL_BUTTON_NUMS];

	void UpdateStateTutorial();
	void RenderStateTutorial();
	bool TouchButtonTutorial();
	CTutorial _tutorial;



	int _online_town;
	int _online_howfar;
	int _online_total_coin;
	int _check_syn_save_complete;
	void CheckAndSynSave();

#define SYN_SAVE_NONE (0)
#define SYN_SAVE_PROCESSING (1)

	int  _syn_save_state;
	int  _syn_save_step;
	int  _syn_time_out;
	bool  _can_submit;
	bool  UpdateSynSave();
	void SubmitAllScore();
	void SubmitTotalCoin();
	void SubmitHighScore();
	void SubmitTown();

#define MAX_LOCATION 6
	//state confirm
#define CONFIRM_BUTTON_NUMS			 2
#define CONFIRM_BUTTON_YES			0
#define CONFIRM_BUTTON_NO			1

	Button _buttonConfirm[CONFIRM_BUTTON_NUMS];

	void UpdateStateConfirm();
	void RenderStateConfirm();
	bool TouchButtonConfirm();
	char _stringConfirm[1024];
	BYTE _typeConfirm;

#define CONFIRM_RESET_DATE		0
#define CONFIRM_NOTIFY			1

#define TIME_CHARBOST		4

	int _countShowAdsMods, _countShowAdsUnity, _countShowPineAds;
	int _stateShowPineAds;
	bool _notShowPineAds;
	int _counttoPlay;

	CAnimObject _iconLoading;
	int _oldLocation;

#define IPAD_ZOOM (1.33f)
#define IPAD_OFFSET (-(786-640)*768.0f/640/2)


	void PurchaseIAP(const char* strID, enumIAPResponseStatus status);
	bool _isProssesIap;




	//port
	PineSocialAsync _social;
	AccountManager _account_mng;
	PineServiceHandle _svc_inapp_load, _svc_acc_login, _svc_acc_load, _svc_check_server_able, _svc_check_acc,
		_svc_create_new_acc, _svc_sync_save_to_server, _svc_sync_save_from_server,
		_svc_post_score;
	char _waiting_feed[1024];
	bool _is_waiting_feed;
	PineServiceHandle _svc_login_fb;
	void FeedFacebook(const char* feed);

	bool _loaded_store;
	char _inapp_price[TOTAL_IAP_NAME_DES_ROW][256];

    int  _touchID;
    void SetTouchProcessing(int touchID);
    void ClearTouchProcessing();
    bool CheckTouchProcessing(int touchID);
    
    int _typeTouchShop;
	int _typeTouchQuest;

	void GooglePlayShowLeaderBoard();
	void GooglePlaySynDriver();
	void GooglePlayShowAchievement();

	void GooglePlaySubmitAchievement(int index);
	void SubmitAchievement(int index);
	void UpdateStatusAchievement();
	void SetAchievementSubmitCompleted(int index);
	int GetPercent(int num, int max);

	bool isLogged;
	void Logs_QuestCompleted(char *name, int number_quest);
	void Logs_GameStorePWLevel(char *name,int level);
	void Logs_GameStoreCharacter(char *charname);
	void Logs_GamePlaying_Restart(int restart_count, bool isFirstPlay);
	void Logs_LocationPass(int location);
	void Logs_DailyQuest_Double(bool status);


#define DAILY_COMPLETE			0
#define DAILY_QUEST_DOUBLE		1
#define ADS_SHOW				2
#define GAME_STORE				3
#define PLAYING_RESTART			4

	int num_playing_restart;
	int num_continue;

	int playing_current_date_local;

	int _count_frame_show_ads;
	int _time_delay_show_ads;
	int _count_frame_delay_show_ads;
	bool _show_ads_interrupt;
#define TIME_DELAY_SHOW_ADS					(2.5f*60)
	bool _has_watched_ads = false;
	bool _on_show_video_ads = false;
	bool _on_video_ads_reward = false;
	bool _is_waiting_for_reward = false;
	int _count_frame_waiting;
	bool _has_new_location;
	//Lam Update
	void RatingRender(float x, float y);
	void RatingUpdate();
	void RatingInit();
	void RatingActive();
	bool RatingUpdateTouch();
	float _rating_opac, _rating_scale;
	bool _rating_active, _rating_is_close;
	int _rating_state,_rating_num_star,_rating_touch_id;
#define RATING_NUM_BUTTON			2
	Button _rating_button[RATING_NUM_BUTTON];
	enum enumRatingButton {
		RATING_BUTTON_LATER,
		RATING_BUTTON_RATE,
	};
};

#endif

#define DISABLE_COIN		1
#define NUM_COIN			0
#if ANDROID_OS
	#define GGPLAY_IMPLEMENT	1
#else
	#define GGPLAY_IMPLEMENT	1
#endif 

#if defined (ANDROID_OS)
#define	ADD_GOOGLE_ANALYTICS		1
#endif


