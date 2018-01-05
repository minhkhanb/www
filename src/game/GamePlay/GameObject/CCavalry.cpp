#include "pch.h"
#include "../../GameCore/CGame.h"

void CCavalry::SetState(BYTE state)
{
	switch (state)
	{
	case BOSS_STATE_DIE:
		
		//AddParticalDie(_posX, _posY - 50, 40, 0);
		break;
	case BOSS_STATE_BE_ACCTACK:
		_timeDeplay = 50;
		
		break;
	}
	_state = state;
}
void CCavalry::Init(float x, float y, BYTE type)
{
	_posX = x + 100;
	_posY = y;

	_targetX = k_SCREEN_WIDTH + 100;
	_targetY = y;
	
	_speedMove = k_SCREEN_WIDTH/(float)(CHARATER().GetTimePower());

	_typePower = POWER_MULE;
	int indexPower = 0;
	if (_typePower == POWER_NONE)
	{
		indexPower = 0;
	}
	else if (_typePower == POWER_MACHETE)
	{
		indexPower = 1;
	}
	else
	{
		indexPower = 2;
	}

	_spriteID = SPRITEID_CHARATER_02;

	_animCharater.SetAnimSprite(GET_SPRITE(_spriteID), _spriteID);
	_animCharater.SetAnim(2 + indexPower * 5, true);

	_state = CHARATER_STATE_MOVE;

	_collisionW = GET_SPRITE(_spriteID)->GetFModuleWidth(5, 0);
	_collisionH = GET_SPRITE(_spriteID)->GetFModuleHeight(5, 0);

	_colMoveW = GET_SPRITE(_spriteID)->GetFModuleWidth(4, 0);
	_colMoveH = GET_SPRITE(_spriteID)->GetFModuleHeight(4, 0);

	InitEffect();
	
	_dame = 5;
	_timeGun = 30;
	_speedGun = 15;
	_scaleX = 1;
	_distanceGun = 1000;
	_armyBeAttack = NULL;
	
	_distanceMagnet = GET_DES().getMagnetDes(0, MAGNET_DES_DISTANCE_EFFECT);
}


void CCavalry::Update()
{
	UpdateAngleGun();
	if (IsMoveToTarget(_posX,_posY,_targetX,_targetY,_speedMove))
	{
		
	}
	if (_armyBeAttack != NULL)
	{
		_timeGun--;
		if (_timeGun < 0)
		{
			AddEffectGun(_posX, _posY,_speedGun);
			_timeGun = 30;
		}
	}
	_animCharater.SetPos(_posX, _posY + BATTLE()._vibrateY);
	_collisionX = GET_SPRITE(_spriteID)->GetFModuleX(5, 0) + _posX;
	_collisionY = GET_SPRITE(_spriteID)->GetFModuleY(5, 0) + _posY;

	_colMoveX = GET_SPRITE(_spriteID)->GetFModuleX(4, 0) + _posX;
	_colMoveY = GET_SPRITE(_spriteID)->GetFModuleX(4, 0) + _posY;

	_attackX = GET_SPRITE(_spriteID)->GetFModuleCenterX(5, 0) + _posX;
	_attackY = GET_SPRITE(_spriteID)->GetFModuleCenterY(5, 0) + _posY;
	UpdateEffect();

}
void CCavalry::Render()
{
	GET_SPRITE(_spriteID)->DrawFrame(G(), 3, _posX, _posY);
	_animCharater.DrawAnimObject(G());
	_animCharater.UpdateAnimObject();
	RenderEffect();
}
