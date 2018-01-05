#include "pch.h"
#include "MenuSystem.h"
#include "../../GameCore/CGame.h"

Button* Button::_bt_actived = NULL;
int Button::_num_button = 0;
Button** Button::_button_namager = NULL;
void Button::InitButtonManager()
{
	_num_button =0;
	_button_namager = new Button*[10000];
}
void Button::AddButton(Button* button)
{
	for(int i = 0; i < _num_button; i++)
	{
		if(button == _button_namager[i])
		{
			return;
		}
	}
	_button_namager[_num_button] = button;
	_num_button++;
}
void Button::DrawFrameWithScale(CSprite* sprite,int frame_id,float scalex,float scaley,int xx,int yy)
{
    float posX = Button::GetFrameX(sprite, frame_id)+xx;
    float posY = Button::GetFrameY(sprite, frame_id)+yy;
    float w = Button::GetFrameWidth(sprite,frame_id);
    float h = Button::GetFrameHeight(sprite,frame_id);
    sprite->DrawFrame(G(),frame_id,posX*scalex-posX+(w*scalex-w)/2,posY*scaley-posY+(h*scalex-h)/2);
}
void Button::SetPosScale(float scalex, float scaley)
{
    _pos_scalex = scalex;
    _pos_scaley = scaley;
}
Button::Button()
{
	_anim = new CAnimObject();
	_anim_overplay = new CAnimObject();
	_posX = 0;
	_posY = 0;
	_anchor = A_NORMAL;
	_extWidth = 0;
	_extHeight = 0;
	_state = BUTTON_STATE_NORMAL;
}

void Button::Init(int state, float x, float y, int anchor, BYTE effectNormal, int subType, int sound, float delateScale)
{
	_deltaS = delateScale;
	_bt_actived = NULL;
	_effectNormal = effectNormal;

	_effectNormalState = 0;

	//effectzoom default var
	_effectNormalScale = 1.0f;
	_effectNormalTargetScale = 1.0f;

	//effectMoveHorization default var
	_effectMoveHorization = 0;
	_effectMoveHorizationTarget = 0;

	_offsetX = 0;
	_offsetY = 0;

	_scaleX = 1 + _deltaS;
	_scaleY = 1 + _deltaS;

	if (_effectNormal & BUTTON_EFFECT_NORMAL_ZOOM)
	{
		_effectNormalTargetScale = 0.05f;
		_effectNormalState = 1;
	}

	if (_effectNormal & BUTTON_EFFECT_NORMAL_MOVE_HORIZATION)
	{
		_effectNormalState = 1;
		if (subType == 0)
		{
			_effectMoveHorizationTarget = -4;
		}
		else if (subType == 1)
		{
			_effectMoveHorizationTarget = 4;
		}
		
	}

	if (_effectNormal & BUTTON_EFFECT_TOUCH_ZOOM)
	{
		_effectNormalState = subType;
		_scaleX = 1 + _deltaS;
		_scaleY = 1 + _deltaS;
		_offsetX = 0;
		_offsetY = 0;
	}

	_posX = x + _offsetX;
	_posY = y + _offsetY;
	_anchor = anchor;
	_state = state;

	_sound = sound;
	_pos_scalex = 1;
	_pos_scaley = 1;
    
    _touchID = -1;
    _touchState = -1;
	_touch_id = -1;
}
/*
void Button::SetAnim(CSprite *sprite,int lockID, int normalID, int selectID, int activeID)
{
	_anim->SetAnimSprite(sprite);
	_anim_overplay->SetAnimSprite(sprite);
	_lockID = lockID;
	_normalID = normalID;
	_selectID = selectID;
	_activeID = activeID;
	_anim_overplay->SetAnim(activeID);
	SetState(_state);
}
*/
/*
void Button::SetAnim(CSprite *sprite,int lockID)
{
	SetAnim(sprite,lockID,lockID+1,lockID+2,lockID+3);
}
*/
void Button::SetAnim(int spriteID,int lockID, int normalID, int selectID, int activeID)
{

	_anim->SetAnimSprite(GET_SPRITE(spriteID),spriteID);
	_anim_overplay->SetAnimSprite(GET_SPRITE(spriteID),spriteID);
	_lockID = lockID;
	_normalID = normalID;
	_selectID = selectID;
	_activeID = activeID;
	_anim_overplay->SetAnim(_selectID);
	SetState(_state);
}
void Button::SetAnim(int spriteID,int lockID)
{
	SetAnim(spriteID,lockID,lockID+1,lockID+2,lockID+3);
}
void Button::SetExt(int width, int height)
{
	_extWidth = width;
	_extHeight = height;
}
float Button::GetWidth()
{
	if(!_anim->IsCurrentAnimStop())
	{
		//return _anim->GetCurrentAnimWidth();
		CSprite* _target_sprite =_anim->_anim_sprite;
		INT32 frame_index = _anim-> _current_aframes;
		INT32 frame_id = _target_sprite->_aframe_id[frame_index];
		return GetFrameWidth(_target_sprite,frame_id);
	}
	return 0;
}
float Button::GetHeight()
{
	if(!_anim->IsCurrentAnimStop())
	{
		CSprite* _target_sprite =_anim->_anim_sprite;
		INT32 frame_index = _anim-> _current_aframes;
		INT32 frame_id = _target_sprite->_aframe_id[frame_index];
		return GetFrameHeight(_target_sprite,frame_id);
	}
	return 0;
}
void Button::SetPos(float x, float y)
{
	_posX = x;
	_posY = y;
}
void Button::SetState(int state)
{
	_state = state;
	switch(state)
	{
	case BUTTON_STATE_LOCK:
		_anim->SetAnim(_lockID,false);
		_touch_id = -1;
		break;
	case BUTTON_STATE_NORMAL:
		
		_bt_actived = NULL;
		_anim->SetAnim(_normalID,false);
       
		break;
	case BUTTON_STATE_SELECT:
		if (_effectNormal & BUTTON_EFFECT_TOUCH_ZOOM)
		{
			_effectNormalState = 1;
			_scaleX = 1.0 + _deltaS;
			_scaleY = 1.0 + _deltaS;
		}
		_bt_actived = this;
		_anim->SetAnim(_selectID,false);
		break;
	case BUTTON_STATE_ACTIVE:		
		if(CGame::_this->IsEnableSound() && _sound != -1)
		{
			PLAY_SFX(_sound);			
			//printf("\nPlay sound");
		}
		_anim->SetAnim(_activeID,false);
		//_bt_actived = NULL;
		break;
	}
    
    
}
bool Button::NeedAction()
{
	if(_state == BUTTON_STATE_ACTIVE && _anim->IsCurrentAnimStop())
	{		
		//if (_effectNormalState == 0)
		{
			//_anim->SetAnim(_activeID,false);			
			_anim->SetToFrameLast();
			_bt_actived = NULL;
			return true;
		}
		/*else
		{
			_anim->SetAnim(_normalID, false);

		}*/
		
	}
	return false;
}
void Button::Render(Graphic* g, int offx, int offy, bool overplay)
{
	if (_state == BUTTON_STATE_HIDE)
	{
		return;
	}
	if(overplay)	{

		_anim_overplay->SetPos(_posX+GetFixX()+offx,_posY + GetFixY()+offy);
		_anim_overplay->DrawAnimObject(g,true);
		_anim_overplay->UpdateAnimObject();
	}
	else
	if(!_anim->IsCurrentAnimStop())
	{
		//_anim->SetPos((_posX+offx)*_pos_scalex,(_posY+offy)*_pos_scaley);
		_anim_overplay->SetPos(_posX+GetFixX()+offx,_posY + GetFixY()+offy);
        _anim->SetPos(_posX+GetFixX()+offx,_posY + GetFixY()+offy);

		_centerX = TZ_GetX(_anim, _lockID) + TZ_GetX() + TZ_GetWidth()/2;
		_centerY = TZ_GetY(_anim, _lockID) + TZ_GetY() + TZ_GetHeight()/2;

		if (_effectNormal & BUTTON_EFFECT_NORMAL_MOVE_HORIZATION)
		{			
			_anim->SetPos(_anim->_x + _effectMoveHorization, _anim->_y);
			_centerX += _effectMoveHorization;
		}

		if (_effectNormal & BUTTON_EFFECT_NORMAL_ZOOM)
		{
			G()->SetScale(_effectNormalScale, 2.0f - _effectNormalScale, _centerX, _centerY);
		}


		if (_effectNormal & BUTTON_EFFECT_TOUCH_ZOOM)
		{
			_centerX = TZ_GetX(_anim, _lockID) + TZ_GetX() + TZ_GetWidth() / 2 + _offsetX;
			_centerY = TZ_GetY(_anim, _lockID) + TZ_GetY() + TZ_GetHeight() / 2 + _offsetY;
			_anim->SetPos(_posX + GetFixX() + offx + _offsetX, _posY + GetFixY() + offy + _offsetY);

			G()->SetScale(_scaleX, _scaleY, _centerX, _centerY);

			
			
		}
		
		switch(_state)
		{
			case BUTTON_STATE_LOCK:
				_anim->DrawAnimObject(g);
				break;
			case BUTTON_STATE_NORMAL:
				_anim->DrawAnimObject(g);
				break;
			case BUTTON_STATE_SELECT:
				_anim->DrawAnimObject(g);
				break;
			case BUTTON_STATE_ACTIVE:
				_anim->DrawAnimObject(g);
				break;
		}

		if (_effectNormal & BUTTON_EFFECT_NORMAL_MOVE_HORIZATION || _effectNormal & BUTTON_EFFECT_TOUCH_ZOOM)
		{
			G()->ClearScale();
		}

		
#if DEBUG_BUTTON
		float x = TZ_GetX();
		float y = TZ_GetY();
		float w = TZ_GetWidth();
		float h = TZ_GetHeight();
		G()->SetColor(0x44ff0000);
		G()->FillRect(x,y,w,h,true);
#endif
	}
}
void Button::SetEffect(BYTE effectNormal, BYTE effectNormalState)
{
	_effectNormal = effectNormal;
	_effectNormalState = effectNormalState;
	if (effectNormalState == 0)
	{
		_scaleX = 1 + _deltaS;
		_scaleY = 1 + _deltaS;
		_offsetX = 0;
		_offsetY = 0;
	}
}
void Button::Update()
{
	if (_state == BUTTON_STATE_HIDE)
	{
		return;
	}
	_anim->UpdateAnimObject();
	if(_anim->IsCurrentAnimStop())
	{
		//printf("state = %d",_state);
		switch(_state)
		{
			case BUTTON_STATE_LOCK:
				_anim->SetAnim(_lockID,false);
			break;
			case BUTTON_STATE_NORMAL:
				_anim->SetAnim(_normalID,false);
			break;
			case BUTTON_STATE_SELECT:
				_anim->SetAnim(_selectID,false);
				_anim->SetToFrameLast();
			break;
			case BUTTON_STATE_ACTIVE:
				//_anim->SetAnim(_activeID, false);
			break;
		}
	}
	
	//update effect normal
	if (_effectNormal & BUTTON_EFFECT_NORMAL_ZOOM)
	{
		if (_effectNormalScale < 1.0f + _effectNormalTargetScale)
		{
			_effectNormalScale += 0.004f;
			if (_effectNormalScale >= 1.0f + _effectNormalTargetScale)
			{
				_effectNormalScale = 1.0f + _effectNormalTargetScale;
				_effectNormalTargetScale = -0.05f;
			}
		}
		else
		{
			_effectNormalScale -= 0.004f;
			if (_effectNormalScale <= 1.0f + _effectNormalTargetScale)
			{
				_effectNormalScale = 1.0f + _effectNormalTargetScale;
				_effectNormalTargetScale = 0.05f;				
			}
		}
	}
	
	if (_effectNormal & BUTTON_EFFECT_NORMAL_MOVE_HORIZATION)
	{
		if (_effectMoveHorization < _effectMoveHorizationTarget)
		{
			_effectMoveHorization += 0.25f;
			if (_effectMoveHorization >= _effectMoveHorizationTarget)
			{
				_effectMoveHorization = _effectMoveHorizationTarget;
				_effectMoveHorizationTarget = -4;
			}
		}
		else
		{
			_effectMoveHorization -= 0.25f;
			if (_effectMoveHorization <= _effectMoveHorizationTarget)
			{
				_effectMoveHorization = _effectMoveHorizationTarget;
				_effectMoveHorizationTarget = 4;
			}
		}
	}
	
	if (_effectNormal & BUTTON_EFFECT_TOUCH_ZOOM)
	{
		switch (_effectNormalState)
		{
		case 1:
			_scaleX += 0.06f / 2.0f;
			_scaleY -= 0.08f / 2.0f;
			_offsetY += 3 / 2.0f;
			if (_offsetY >= 15 / 2.0f)
			{
				_effectNormalState = 2;
			}
			
			break;
		case 2:
			_scaleX -= 0.04f / 2.0f;
			_scaleY += 0.05f / 2.0f;
			_offsetY -= 3 / 2.0f;
			if (_offsetY <= -10 / 2.0f)
			{
				_effectNormalState = 3;
			}
			break;
		case 3:
			_scaleX += 0.02f / 2.0f;
			_scaleY -= 0.02f / 2.0f;
			_offsetY += 3 / 2.0f;
			if (_offsetY >= 8 / 2.0f)
			{
				_effectNormalState = 4;
			}
			break;
		case 4:
			_scaleX -= 0.015f / 2.0f;
			_scaleY += 0.01f / 2.0f;
			_offsetY -= 3 / 2.0f;
			if (_offsetY <= -5 / 2.0f)
			{
				_effectNormalState = 5;
			}
			break;
		case 5:
			_scaleX += 0.01f / 2.0f;
			_scaleY -= 0.01f / 2.0f;
			_offsetY += 3 / 2.0f;
			if (_offsetY >= 0)
			{
				_effectNormalState = 6;
			}
			break;
		case 6:
			_scaleX = 1 + _deltaS;
			_scaleY = 1 + _deltaS;
			_offsetX = 0;
			_offsetY = 0;
			_effectNormalState = 0;
			break;
		}
	}

	CSprite* _target_sprite =_anim->_anim_sprite;
    INT32 frame_index = _anim-> _current_aframes;
    INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	int modulex = _target_sprite->GetFModuleX(frame_id,0) + Button::GetCurrentAnimFrameAnimX(_anim);
	int moduley = _target_sprite->GetFModuleY(frame_id,0) + Button::GetCurrentAnimFrameAnimY(_anim);
	int modulew = _target_sprite->GetFModuleWidth(frame_id,0);
	int moduleh = _target_sprite->GetFModuleHeight(frame_id,0);
   	
	_fixedX = modulex + _posX;//luonghuynh change
    _fixedY = moduley + _posY;//luonghuynh change
    _fixedW = modulew;
    _fixedH = moduleh;
}

bool Button::isMove(float &x1, float &y1, float x2, float y2, float speed)
{
	bool ret = false;
	float distance = CMath::DistancePOW2(x2, y2, x1, y1);
	if (distance < (speed * speed))
	{
		x1 = x2;
		y1 = y2;
		ret = true;
	}
	else
	{
		float angle_move = CMath::ANGLE(x2, y2, x1, y1) - 180;
		float vy = speed*MSIN(angle_move);
		float vx = speed*MCOS(angle_move);

		float del_x = (speed / 2.5);
		float del_y = (speed / 2.5);

		x1 += vx + del_x;
		y1 += vy + del_y;
	}

	return ret;
}
bool Button::CurrentAnimStop()
{
	return _anim->IsCurrentAnimStop();
}
float Button::GetFixX()
{
    if(!_anim->IsCurrentAnimStop())
	{
		
        CSprite* _target_sprite =_anim->_anim_sprite;
        INT32 frame_index = _anim-> _current_aframes;
        INT32 frame_id = _target_sprite->_aframe_id[frame_index];
        float w = (GetFrameWidth(_target_sprite,frame_id)*_pos_scalex - GetFrameWidth(_target_sprite,frame_id))/2;
        return (GetFrameX(_target_sprite,frame_id)*_pos_scalex - GetFrameX(_target_sprite,frame_id))+w;
		
	}
    return 0;
}
float Button::GetFixY()
{
    if(!_anim->IsCurrentAnimStop())
	{
		
        CSprite* _target_sprite =_anim->_anim_sprite;
        INT32 frame_index = _anim-> _current_aframes;
        INT32 frame_id = _target_sprite->_aframe_id[frame_index];
        float h = (GetFrameHeight(_target_sprite,frame_id)*_pos_scalex - GetFrameHeight(_target_sprite,frame_id))/2;
        return (GetFrameY(_target_sprite,frame_id)*_pos_scaley -  GetFrameY(_target_sprite,frame_id))+h;
		
	}
    return 0;
}
int Button::TZ_GetX()
{
	if(!_anim->IsCurrentAnimStop())
	{
		switch(_anchor)
		{
		case A_NORMAL:
			return (_posX - _extWidth/2)*_pos_scalex;
		case A_HVCENTER:
			{
				CSprite* _target_sprite =_anim->_anim_sprite;
				INT32 frame_index = _anim-> _current_aframes;
				INT32 frame_id = _target_sprite->_aframe_id[frame_index];
				//printf("\nframe_id = %d",frame_id);
                float w = (GetFrameWidth(_target_sprite,frame_id)*_pos_scalex - GetFrameWidth(_target_sprite,frame_id))/2;
				return (_posX - ((_extWidth)/2) + GetFrameX(_target_sprite,frame_id))*_pos_scalex+w;
			}
		}
	}
	return 0;
}

//luonghuynh90 add
int Button::TZ_GetX(CAnimObject *anim, int iframe)
{	
	return anim->_anim_sprite->_aframe_x[iframe];
}

int Button::TZ_GetY()
{
	if(!_anim->IsCurrentAnimStop())
	{
		switch(_anchor)
		{
		case A_NORMAL:
			return (_posY - _extHeight/2)*_pos_scaley;
		case A_HVCENTER:
			{
				CSprite* _target_sprite =_anim->_anim_sprite;
				INT32 frame_index = _anim-> _current_aframes;
				INT32 frame_id = _target_sprite->_aframe_id[frame_index];
                float h = (GetFrameHeight(_target_sprite,frame_id)*_pos_scalex - GetFrameHeight(_target_sprite,frame_id))/2;
				return (_posY - ((_extHeight)/2) + GetFrameY(_target_sprite,frame_id))*_pos_scaley+h;
			}
		}
	}
	return 0;
}

//luonghuynh90 add
int Button::TZ_GetY(CAnimObject *anim,int iframe)
{
	return anim->_anim_sprite->_aframe_y[iframe];
}

int Button::TZ_GetWidth()
{
	if(!_anim->IsCurrentAnimStop())
	{	
		return (_extWidth+GetWidth());	
	}
	return 0;
}
int Button::TZ_GetHeight()
{
	if(!_anim->IsCurrentAnimStop())
	{	
		return (_extHeight+GetHeight());	
	}
	return 0;
}

void Button::ResetPlaySound()
{
	//_bt_actived = NULL;
}
Button* Button::GetButtonActive()
{
	return _bt_actived;
}
int Button::GetFrameX(CSprite* spr, int frameID)
{
	int min = spr->GetFModuleX(frameID,0);
	//printf("\t spr->GetFModuleNum(%d) = %d",frameID,spr->GetFModuleNum(frameID));
	for(int i = 1; i <  spr->GetFModuleNum(frameID); i++)
	{
		if(min > spr->GetFModuleX(frameID,i))
		{
			min = spr->GetFModuleX(frameID,i);
		}
	}
	return min;
}
int Button::GetFrameY(CSprite* spr, int frameID)
{
	int min =  spr->GetFModuleY(frameID,0);
	for(int i = 0; i <  spr->GetFModuleNum(frameID); i++)
	{
		if(min > spr->GetFModuleY(frameID,i))
		{
			min = spr->GetFModuleY(frameID,i);
		}
	}
	return min;
}

int Button::GetFrameWidth(CSprite* sprite, int frame_id)
{
	int fmodule_num = sprite->GetFModuleNum(frame_id);
	float top =	sprite->GetFModuleY(frame_id,0);
	float left = sprite->GetFModuleX(frame_id,0);
	float bottom =sprite->GetFModuleY(frame_id,0)+sprite->GetFModuleHeight(frame_id,0);
	float right= sprite->GetFModuleX(frame_id,0)+sprite->GetFModuleWidth(frame_id,0);
	for (INT32 j = 0; j < fmodule_num; j++)
	{
		INT32 mid =sprite->_fmodule_id[j];
		float moduleW=sprite->GetFModuleWidth(frame_id,j);
		float moduleH= sprite->GetFModuleHeight(frame_id,j);
		float mx = sprite->GetFModuleX(frame_id,j);
		float my = sprite->GetFModuleY(frame_id,j);
		if(mx<left)left=mx;
		if(my<top)top=my;
		if((mx+moduleW)>right)right = mx+moduleW;
		if((my+moduleH)>bottom)bottom = my+moduleH;
	}
	return (right-left);
}
int Button::GetFrameHeight(CSprite* sprite, int frame_id)
{
	int fmodule_num = sprite->GetFModuleNum(frame_id);
	float top =	sprite->GetFModuleY(frame_id,0);
	float left = sprite->GetFModuleX(frame_id,0);
	float bottom =sprite->GetFModuleY(frame_id,0)+sprite->GetFModuleHeight(frame_id,0);
	float right= sprite->GetFModuleX(frame_id,0)+sprite->GetFModuleWidth(frame_id,0);
	for (INT32 j = 0; j < fmodule_num; j++)
	{
		INT32 mid =sprite->_fmodule_id[j];
		float moduleW=sprite->GetFModuleWidth(frame_id,j);
		float moduleH= sprite->GetFModuleHeight(frame_id,j);
		float mx = sprite->GetFModuleX(frame_id,j);
		float my = sprite->GetFModuleY(frame_id,j);
		if(mx<left)left=mx;
		if(my<top)top=my;
		if((mx+moduleW)>right)right = mx+moduleW;
		if((my+moduleH)>bottom)bottom = my+moduleH;
	}
	return (bottom-top);
}
int  Button::GetCurrentAnimMarkerX(CAnimObject *anim,int module)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim-> _current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	return _target_sprite->GetFModuleX(frame_id,module) + _target_sprite->GetFModuleWidth(frame_id,module)/2;
}
int  Button::GetCurrentAnimMarkerY(CAnimObject *anim,int module)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim-> _current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	return _target_sprite->GetFModuleY(frame_id,module) + _target_sprite->GetFModuleHeight(frame_id,module)/2;
}
 int Button::GetModuleWidth(CSprite* _target_sprite,int frame_id,int module)
{
	return _target_sprite->GetFModuleWidth(frame_id,module);
}
 int Button::GetModuleHeight(CSprite* _target_sprite,int frame_id,int module)
{
	return _target_sprite->GetFModuleHeight(frame_id,module);
}
 int Button::GetModuleMarkerX(CSprite* _target_sprite,int frame_id,int module)
{
	return _target_sprite->GetFModuleX(frame_id,module) + _target_sprite->GetFModuleWidth(frame_id,module)/2;
}
 int Button::GetModuleMarkerY(CSprite* _target_sprite,int frame_id,int module)
{
	return _target_sprite->GetFModuleY(frame_id,module) + _target_sprite->GetFModuleHeight(frame_id,module)/2;
}
 int Button::GetCurrentAnimFModuleAngle(CAnimObject *anim,int module)
{
	/*
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim-> _current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	return _target_sprite->GetFModuleAngle(frame_id,module);
	*/
	return 0;
}
 int Button::GetModuleOpacity(CSprite *sprite,int frame_id, int moduleOfFrame)
 {

	//return sprite->GetFModuleOpacity(frame_id,moduleOfFrame);
	 return 100;
 }
int Button::GetModuleNum(CSprite* sprite,int frame_id)
{
	return sprite->GetModuleOfFrame(frame_id);
}
int Button::GetCurrentAnimFModuleX(CAnimObject *anim,int module)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim-> _current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	return _target_sprite->GetFModuleX(frame_id,module);
}
int Button::GetCurrentAnimFModuleY(CAnimObject *anim,int module)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim-> _current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	return _target_sprite->GetFModuleY(frame_id,module);
}

int Button::GetCurrentAnimFModuleWidth(CAnimObject *anim,int module)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim-> _current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	return _target_sprite->GetFModuleWidth(frame_id,module);
}
int Button::GetCurrentAnimFModuleHeight(CAnimObject *anim,int module)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim-> _current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	return _target_sprite->GetFModuleHeight(frame_id,module);
}

int Button::GetCurrentAnimFrameX(CAnimObject *anim,int frameID)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim-> _current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	return GetFrameX(_target_sprite,frame_id);
}
int Button::GetCurrentAnimFrameY(CAnimObject *anim,int frameID)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim-> _current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	return GetFrameY(_target_sprite,frame_id);
}
int Button::GetCurrentAnimFrameWidth(CAnimObject *anim,int frameID)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim-> _current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	return GetFrameWidth(_target_sprite,frame_id);
}
int Button::GetCurrentAnimFrameHeight(CAnimObject *anim,int frameID)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim-> _current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	return GetFrameHeight(_target_sprite,frame_id);
}

int Button::GetModuleID(CSprite *sprite,int frame_id, int moduleOfFrame)
{
	INT32 fmodule_min = 0;
	INT32 fmodule_max = 0;
	
	if (frame_id < (sprite->_num_frames - 1))
	{
		fmodule_min = sprite->_frame_offset[frame_id];
		fmodule_max = sprite->_frame_offset[frame_id + 1] - 1;
	}
	else if (frame_id == (sprite->_num_frames - 1))
	{
		fmodule_min = sprite->_frame_offset[frame_id];
		fmodule_max = sprite->_num_fmodules - 1;
	}
	return sprite->_fmodule_id[fmodule_min+moduleOfFrame];
}

int Button::GetCurrentAnimFrameAnimX(CAnimObject *anim)
{
	return anim->_anim_sprite->_aframe_x[anim->_current_aframes];
}

int Button::GetCurrentAnimFrameAnimY(CAnimObject *anim)
{
	return anim->_anim_sprite->_aframe_y[anim->_current_aframes];
}

/////
void Button::SetState(int state, int touchID)
{
    _touchID = touchID;
    SetState(state);
}
void Button::SetState(int state, int touchID,int touchState)
{
    _touchState = touchState;
    SetState(state, touchID);
}

bool Button::CheckState(int state)
{
    return _state == state;
}
bool Button::CheckState(int state, int touchID)
{
    return CheckState(state) && _touchID == touchID;
}
bool Button::CheckState(int state, int touchID, int touchState)
{
    return CheckState(state, touchID) && _touchState == touchState;
}
