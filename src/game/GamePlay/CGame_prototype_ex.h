/**
*	will be included in CGame.h
*	specific vars and methods for gameplay
*/

public:
// game state
	
	
private:
	CAnimObject** _game_obj;
public:




//logo Menu

int _loading_opacity;
int _flag_opacity;
int _menu_opacity;

//Save System.
void GameDataLoad();
void GameDataSave();
void GameDataDefault();
void GameDataToBuffer();
void GameLoadDataFromBuffer();

#define GAME_FILE	"pinerecord.dat"

#define GAME_VERSION				100

#define GAME_DATA_VERSION			(0)

#define GAME_DATA_LENGTH				(GAME_DATA_VERSION + 4)



char _buffer_save[GAME_DATA_LENGTH];
int _game_version;
int _rate;

//PineHighScore _pineHighScore;

 long _sys_begin_second;
 long _sys_millisecond;
 long _game_millisecond;
 long _sys_second;

void UnloadGameBackground();
bool LoadGameForceground();
bool _needresume_when_playing;



//restore
struct RestoreState{
	int LastState;
	int CurrentState;
	int NewState;
	int SubState;
};

RestoreState _restore_state;
int _backup_sprite_restore[TOTAL_SPRITE_INGAME];
void Prepare_restore();
void Update_state_restore();
void Render_state_restore();
int _restore_step;
bool _on_pause;



PineGame *_main_game;
PineGame  *_feature_game;
PineGame    *_current_game;

//Port
PinePoint _point_o, _point_right_bottom;
float _screen_width;
float _screen_height;
float _port_scale_x;
float _port_scale_y;

#if defined(_WINDOWS_PHONE8)
AdmobManager _admob_manager;
#elif defined (ANDROID_OS)
PineTapdaqAsync _tapdaq_manager;
// unity ads
//PineUnityAdsAsync _unityads_manager;
INT_TIME _time_start_wait_for_reinit_ads;
bool _on_video_ads_reward;
bool _is_waiting_for_reward;
#define TIME_WAIT_FOR_REINIT_ADS 60*1000
#endif
bool InitAds();
void ShowAds();
void ShowVideoAds();
void ShowBannerAds();
void HideBannerAds();
void UpdateAds();
void PrepareAndShowInterstitial();
void PrepareAndShowVideoAds();

bool _on_show_video_ads;
int _admob_play_count;
int _time_count_for_show_ads_banner;
bool _has_show_ads_banner;
bool _isAdsProcessing;


bool _optimizeTexture;
//handlebackey
#if !defined (MAC_OS)
bool			_isBackkeyPress;
int				_waitForProcessBackkeyCount;
#endif
#if ANDROID_OS
CAsyncTasks _async_task_manager;
BoardScoreItem		_gameBoardScoreItem;
#endif

bool is_reset_quest;

//NDT Topebox ads
TopeBox_PromoGame _topebox_game;
float _topebox_game_scrollEff;

char gameName[128];
char type[128];
char linkStore[128];
char linkIcon[256];
int revison;
bool closeTopeboxAds;
bool disableTopeboxAds;
bool configDownloaded;

int time_get_remote_config;