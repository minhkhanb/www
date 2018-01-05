#include "../../Lib/Math/CMath.h"
#include "../../GameCore/CAnimObject.h"
#include "../../Lib/Sprite/CSprite.h"
#include "stdio.h"
#if !_MENU_SYSTEM_H
#define _MENU_SYSTEM_H 1
#define A_NORMAL	0
#define A_HVCENTER	1

#define BUTTON_STATE_LOCK	0
#define BUTTON_STATE_NORMAL 1
#define BUTTON_STATE_SELECT 2
#define BUTTON_STATE_ACTIVE 3
#define BUTTON_STATE_HIDE	4
#define DEBUG_BUTTON 0
 
#define BUTTON_EFFECT_NORMAL_NONE					0
#define BUTTON_EFFECT_NORMAL_ZOOM					1//(1<<0)
#define BUTTON_EFFECT_NORMAL_MOVE_HORIZATION		2//(1<<1)
#define BUTTON_EFFECT_TOUCH_ZOOM					3


class Button
{
	float _effectNormalScale, _effectNormalTargetScale;
	float _effectMoveHorization, _effectMoveHorizationTarget;
	BYTE _effectNormal;
	BYTE _effectNormalState;


	float _maxScale, _minScale,_deltaS;
public:
	float _offsetX, _offsetY;

	float _scaleX, _scaleY;
	int _centerX, _centerY;

	void SetEffect(BYTE effectNormal, BYTE effectNormalState); 
	static Button** _button_namager;
	static int _num_button;
	static void InitButtonManager();
	static void AddButton(Button* button);
	CAnimObject* _anim;
	CAnimObject* _anim_overplay;
	static Button* _bt_actived;
    float _pos_scalex;
    float _pos_scaley;
    float GetFixX();
    float GetFixY();
	float _posX;
	float _posY;
	int _anchor;
	int _extWidth;
	int _extHeight;
	int _state;
	
	int _lockID;
	int _normalID;
	int _selectID;
	int _activeID;
	int _sound;
	Button();
	int GetLockID(){return _lockID;};
    void SetPosScale(float scalex, float scaley);
    static void DrawFrameWithScale(CSprite* sprite,int frame_id,float scalex,float scaley,int posX = 0, int posY = 0);
	void Init(int state,float x, float y,int anchor, BYTE effectNormal = BUTTON_EFFECT_NORMAL_NONE, int subType = 0, int sound = -1, float delateScale = 0);
	void SetExt(int width, int height);
	float GetWidth();
	float GetHeight();
	void SetPos(float x, float y);
	void SetState(int state);
	bool NeedAction();
	void Render(Graphic* g,int offx=0,int offy=0,bool overplay=false);
	void Update();

    int _fixedX;
    int _fixedY;
    int _fixedW;
    int _fixedH;
	int _touch_id;

	int TZ_GetX();
	int TZ_GetX(CAnimObject *anim,int iframe);
	int TZ_GetY();
	int TZ_GetY(CAnimObject *anim,int iframe);
	int TZ_GetWidth();
	int TZ_GetHeight();
	int GetCenterX(){return TZ_GetX()+TZ_GetWidth()/2;};
	int GetCenterY(){return TZ_GetY() + TZ_GetHeight()/2;};
	void SetAnim(int spriteID,int lockID, int normalID, int selectID, int activeID);
	void SetAnim(int spriteID,int lockID);
	//void SetAnim(CSprite *sprite,int lockID, int normalID, int selectID, int activeID);
	//void SetAnim(CSprite *sprite,int lockID);

	bool isMove(float &x1, float &y1, float x2, float y2, float speed); 
	bool CurrentAnimStop();
	static void ResetPlaySound();
	static Button* GetButtonActive();

    //////
    int _touchID,_touchState;
    void SetState(int state, int touchID);
    void SetState(int state, int touchID, int touchState);
    bool CheckState(int state);
    bool CheckState(int state, int touchID);
    bool CheckState(int state, int touchID, int touchState);
	//Utils
	
	static int GetFrameX(CSprite* spr, int frameID);
	static int GetFrameY(CSprite* spr, int frameID);
	static int GetFrameWidth(CSprite* spr, int frameID);
	static int GetFrameHeight(CSprite* spr, int frameID);
	static int GetCurrentAnimFrameX(CAnimObject *anim,int frameID);
	static int GetCurrentAnimFrameY(CAnimObject *anim,int frameID);
	static int GetCurrentAnimFrameWidth(CAnimObject *anim,int frameID);
	static int GetCurrentAnimFrameHeight(CAnimObject *anim,int frameID);

	static int GetCurrentAnimFModuleAngle(CAnimObject *anim,int module);
	static int GetCurrentAnimFModuleX(CAnimObject *anim,int module);
	static int GetCurrentAnimFModuleY(CAnimObject *anim,int module);
	static int GetCurrentAnimFModuleWidth(CAnimObject *anim,int module);
	static int GetCurrentAnimFModuleHeight(CAnimObject *anim,int module);
	static int GetCurrentAnimMarkerX(CAnimObject *anim,int module);
	static int GetCurrentAnimMarkerY(CAnimObject *anim,int module);

	static int GetModuleMarkerX(CSprite* _target_sprite,int frame_id,int module);
	static int GetModuleMarkerY(CSprite* _target_sprite,int frame_id,int module);
	static int GetModuleWidth(CSprite* _target_sprite,int frame_id,int module);
	static int GetModuleHeight(CSprite* _target_sprite,int frame_id,int module);
	static int GetModuleID(CSprite *sprite,int frame_id, int moduleOfFrame);
	static int GetModuleOpacity(CSprite *sprite,int frame_id, int moduleOfFrame);
	static int GetModuleNum(CSprite* sprite,int frame_id);

	static int GetCurrentAnimFrameAnimX(CAnimObject *anim);
	static int GetCurrentAnimFrameAnimY(CAnimObject *anim);
};
#endif
