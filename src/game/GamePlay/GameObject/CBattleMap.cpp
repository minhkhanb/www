#include "pch.h"
#include "../../GameCore/CGame.h"


void CBattleMap::SetSwapBG()
{
	_swapBG_x = k_SCREEN_WIDTH + 100;
	if (CGame::_this->_device._version == SupportVersion::RESVER_960_640)
	{
		_swapBG_x = k_SCREEN_WIDTH + 300;
	}
	if (CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
	{
		_swapBG_x = k_SCREEN_WIDTH + 400;
	}

	
	_stateSwap = 1;

	if (_usedBG_1)
	{
		
		SetSpriteBG_2(2);
		InitBattleMap_2();
		_currentBG = 2;
	}

	if (_usedBG_2)
	{
		
		SetSpriteBG_1(1);
		InitBattleMap_1();
		_currentBG = 1;
	}
}
void CBattleMap::RenderSwapBG(float x, float y)
{
	float zoom = 1.0f;
	float offsetY = 0;
	float yyyy = 0;
	if (CGame::_this->_device._version == SupportVersion::RESVER_960_640)
	{
		yyyy = 37;
		zoom = 1.20f;
		offsetY = -0.15 * 640 / 2;
	}
	if (CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
	{
		yyyy = 50;
		zoom = IPAD_ZOOM;
		offsetY = IPAD_OFFSET;
	}
	
	//render bg layer 1
	G()->SetScale(zoom, zoom, k_SCREEN_WIDTH, k_SCREEN_HEIGHT / 2);

	if (_stateSwap == 1)
	{
		GET_SPRITE(SPRITEID_BG_SWAP_LAYER_1)->DrawFrame(G(), 0, x + _swapBG_x, yyyy +  y + BATTLE()._vibrateY);
	}
	G()->ClearScale();
}
void CBattleMap::RenderSwapBG_1(float x, float y)
{
  	float zoom = 1.0f;
	float offxx = 0;
	float offsetY = 0;
	if (CGame::_this->_device._version == SupportVersion::RESVER_960_640)
	{
		offxx = -70;
		zoom = 1.20f;
		offsetY = -0.15 * 640 / 2;
	}
	if (CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
	{
		offxx = -150;
		zoom = IPAD_ZOOM;
		offsetY = IPAD_OFFSET;
	}

	//render bg layer 1
	G()->SetScale(zoom, zoom, k_SCREEN_WIDTH, k_SCREEN_HEIGHT / 2);

	if (_stateSwap == 1)
	{
		G()->SetClip(460 + offxx + _swapBG_x* zoom, 0, 780 * zoom, k_SCREEN_HEIGHT* zoom);
		GET_SPRITE(SPRITEID_BG_SWAP_LAYER_2)->DrawFrame(G(), 0, x + _swapBG_x, y + BATTLE()._vibrateY);
		G()->ClearClip();
		GET_SPRITE(SPRITEID_BG_SWAP_LAYER_1)->DrawFrame(G(), 1, x + _swapBG_x, BATTLE()._vibrateY);
		GET_SPRITE(SPRITEID_BG_SWAP_LAYER_1)->DrawFrame(G(), 2, x + _swapBG_x, BATTLE()._vibrateY);
	}
	G()->ClearScale();
}
void CBattleMap::UpdateSwapBG()
{
	if (_stateSwap == 1)
	{
		_swapBG_x -= _speed_scroll_map;
		if (_swapBG_x <= -k_DEVICE_WIDTH * 2)
		{
			_stateSwap = 0;
			if (_usedBG_1)
			{
				_usedBG_1 = false;
				_usedBG_2 = true;
			}
			else
			{
				if (_usedBG_2)
				{
					_usedBG_2 = false;
					_usedBG_1 = true;
				}
			}
		}

		if (_swapBG_x < -780 && BATTLE()._lvlBosse !=1 )
		{
			BATTLE()._isCalculateDistance = true;
			BATTLE()._addBoss = 0;
		}
	}
}

void CBattleMap::SetSpriteBG_1(BYTE type)
{
	switch (type)
	{
	case 1:
		_sprite_layer01_1 = SPRITEID_BG_01_PALYER_1;
		_sprite_layer01_2 = SPRITEID_BG_01_PALYER_2;
		_sprite_layer01_2a = SPRITEID_BG_01_PALYER_2A;
		_sprite_layer01_3 = SPRITEID_BG_01_PALYER_3;
		_sprite_layer01_4 = SPRITEID_BG_01_PALYER_4;
		break;
	case 2:
		_sprite_layer01_1 = SPRITEID_BG_02_PALYER_1;
		_sprite_layer01_2 = SPRITEID_BG_02_PALYER_2;
		_sprite_layer01_2a = SPRITEID_BG_01_PALYER_2A;
		_sprite_layer01_3 = SPRITEID_BG_02_PALYER_3;
		_sprite_layer01_4 = SPRITEID_BG_02_PALYER_4;
		break;
	}
}

void CBattleMap::SetSpriteBG_2(BYTE type)
{
	switch (type)
	{
	case 1:
		_sprite_layer02_1 = SPRITEID_BG_01_PALYER_1;
		_sprite_layer02_2 = SPRITEID_BG_01_PALYER_2;
		_sprite_layer02_2a = SPRITEID_BG_01_PALYER_2A;
		_sprite_layer02_3 = SPRITEID_BG_01_PALYER_3;
		_sprite_layer02_4 = SPRITEID_BG_01_PALYER_4;
		break;
	case 2:
		_sprite_layer02_1 = SPRITEID_BG_02_PALYER_1;
		_sprite_layer02_2 = SPRITEID_BG_02_PALYER_2;
		_sprite_layer02_2a = SPRITEID_BG_01_PALYER_2A;
		_sprite_layer02_3 = SPRITEID_BG_02_PALYER_3;
		_sprite_layer02_4 = SPRITEID_BG_02_PALYER_4;
		break;
	}
}
void CBattleMap::InitBattleMap_1()
{
	//init bg layer 1
	int _beginBG = 0;
	_bg01_layer_1_w = Button::GetFrameWidth(GET_SPRITE(_sprite_layer01_1), 0);
	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		_layer01_1_x[i] = i*_bg01_layer_1_w + _beginBG;
	}

	//init bg layer 2
	_bg01_layer_2_w = Button::GetFrameWidth(GET_SPRITE(_sprite_layer01_2), 0);
	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		_layer01_2_x[i] = i*_bg01_layer_2_w + _beginBG;
	}

	//init bg layer 2a
	_bg01_layer_2a_w = Button::GetFrameWidth(GET_SPRITE(_sprite_layer01_2a), 0);
	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		_layer01_2a_x[i] = i*_bg01_layer_2a_w + _beginBG;
	}

	//init bg layer 3
	_bg01_layer_3_w = Button::GetFrameWidth(GET_SPRITE(_sprite_layer01_3), 0) - (GAME()->_game_context.ScaleOut*2.0f);
	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		_layer01_3_x[i] = i*_bg01_layer_3_w + _beginBG;
	}

	_bg01_layer_4_w = Button::GetFrameWidth(GET_SPRITE(_sprite_layer01_4), 0) - 1;
	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		_layer01_4_x[i] = i*_bg01_layer_4_w + _beginBG;
	}


	_speed_start = GET_DES().getCharaterDes(0, CHARATER_DES_SPEED_SCROLL_MAP);

}
void CBattleMap::UpdateBattleMap_1()
{
	//increace speed
	_speed_scroll_map = _speed_start+BATTLE()._speedIncreaseMap;
    //printf("\n _speed_scroll_map = %f, dddd = %f",_speed_scroll_map,_speed_start);

	if (CHARATER().GetState() == CHARATER_STATE_DIE)
	{
		_speed_scroll_map = 0;
	}

	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		_layer01_1_x[i] -= _speed_scroll_map*0.05;
		if (_layer01_1_x[i] + _bg01_layer_1_w <= 0)
		{
			_layer01_1_x[i] += _bg01_layer_1_w * 2;
		}

		//if (BATTLE()._timeBegin <= 0)
		{
			_layer01_2_x[i] -= _speed_scroll_map*0.2;
			if (_layer01_2_x[i] + _bg01_layer_2_w <= 0)
			{
				_layer01_2_x[i] += _bg01_layer_2_w * 2;
			}
		}

		_layer01_2a_x[i] -= _speed_scroll_map*0.5;
		if (_layer01_2a_x[i] + _bg01_layer_2a_w <= 0)
		{
			_layer01_2a_x[i] += _bg01_layer_2a_w * 2;
		}

		_layer01_3_x[i] -= _speed_scroll_map;
		if (_layer01_3_x[i] + _bg01_layer_3_w <= 0)
		{
			_layer01_3_x[i] += _bg01_layer_3_w * 2;
		}

		_layer01_4_x[i] -= _speed_scroll_map;
		if (_layer01_4_x[i] + _bg01_layer_4_w <= 0)
		{
			_layer01_4_x[i] += _bg01_layer_4_w * 2;
		}
	}


}
void CBattleMap::RenderBattleMap_1(float x, float y)
{
    float zoom = 1.0f;
    float offsetY = 0;
    if(CGame::_this->_device._version == SupportVersion::RESVER_960_640)
    {
        zoom = 1.20f;
        offsetY = -0.15*640/2;
    }
    if(CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
    {
        zoom = IPAD_ZOOM;
        offsetY = IPAD_OFFSET;
    }
#if defined (ANDROID_OS)
	if (CGame::_this->_current_version == VERSION_640x1136)
	{
		float zoomx = GAME()->_device.ScreenSize.W / 1136;
		float zoomy = GAME()->_device.ScreenSize.H / 640;
		zoom = zoomx > zoomy ? zoomx : zoomy;
		if (zoom < 1.0f) zoom = 1.0f;
	}
#endif
	if (_stateSwap == 1 && _currentBG == 1)
	{
		G()->SetClip(k_SCREEN_WIDTH / 2 + _swapBG_x, -100, k_SCREEN_WIDTH * 10, k_SCREEN_HEIGHT*zoom);
	}
    //render bg layer 1
    G()->SetScale(zoom, zoom, k_SCREEN_WIDTH,k_SCREEN_HEIGHT/4);
    for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		if (x + _layer01_1_x[i] < k_SCREEN_WIDTH )
		{
			GET_SPRITE(_sprite_layer01_1)->DrawFrame(G(), 0, x + _layer01_1_x[i], y + BATTLE()._vibrateY);
		}
	}
    G()->ClearScale();
    
    //render bg layer 2
    for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		if (x + _layer01_2_x[i] < k_SCREEN_WIDTH )
		{
			GET_SPRITE(_sprite_layer01_2)->DrawFrame(G(), 0, x + _layer01_2_x[i], y + BATTLE()._vibrateY);
		}		
	}

	//render bg layer 2
	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		if (x + _layer01_2a_x[i] < k_SCREEN_WIDTH )
		{
			GET_SPRITE(_sprite_layer01_2a)->DrawFrame(G(), 0, x + _layer01_2a_x[i], y + BATTLE()._vibrateY);
		}		
	}
    
	//render bg layer 3
	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		if (x + _layer01_3_x[i] < k_SCREEN_WIDTH )
		{
			GET_SPRITE(_sprite_layer01_3)->DrawFrame(G(), 0, x + _layer01_3_x[i], y + BATTLE()._vibrateY);
		}		
	}
	G()->ClearClip();
}
void CBattleMap::RenderBGLayer4_1(float x, float y)
{
    float zoom = 1.0f;
    float offsetY = 0;
	float offsetX = 0;
    if(CGame::_this->_device._version == SupportVersion::RESVER_960_640)
    {
        zoom = 1.20f;
        offsetY = -0.15*640/2;
    }
    if(CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
    {
        zoom = IPAD_ZOOM;
        offsetY = IPAD_OFFSET;
		offsetX = -150;
    }
    G()->SetScale(zoom, zoom, k_SCREEN_WIDTH/2,k_SCREEN_HEIGHT/2);
	if (_stateSwap == 1 && _currentBG == 1)
	{
		G()->SetClip(k_SCREEN_WIDTH + _swapBG_x + offsetX, 0, k_SCREEN_WIDTH * 5 * zoom, k_SCREEN_HEIGHT*zoom);
	}
 
	if (_stateSwap == 1 && _currentBG == 2)
	{
		int www = _swapBG_x + k_SCREEN_WIDTH / 2;
		if (www < 0)
		{
			www = 1;
		}
		G()->SetClip(0, 0, www* zoom, k_SCREEN_HEIGHT * zoom);
	}

    for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		if (x + _layer01_4_x[i] < k_SCREEN_WIDTH )
		{
			GET_SPRITE(_sprite_layer01_4)->DrawFrame(G(), 0, x + _layer01_4_x[i], y + BATTLE()._vibrateY);
		}		
	}
	
	G()->ClearClip();
    G()->ClearScale();
} 

void CBattleMap::InitBattleMap_2()
{
	int _beginBG = 0;
	//init bg layer 1
	_bg02_layer_1_w = Button::GetFrameWidth(GET_SPRITE(_sprite_layer02_1), 0);
	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		_layer02_1_x[i] = i*_bg02_layer_1_w + _beginBG;
	}

	//init bg layer 2
	_bg02_layer_2_w = Button::GetFrameWidth(GET_SPRITE(_sprite_layer02_2), 0);
	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		_layer02_2_x[i] = i*_bg02_layer_2_w + _beginBG;
	}

	//init bg layer 2a
	_bg02_layer_2a_w = Button::GetFrameWidth(GET_SPRITE(_sprite_layer02_2a), 0);
	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		_layer02_2a_x[i] = i*_bg02_layer_2a_w + _beginBG;
	}

	//init bg layer 3
	_bg02_layer_3_w = Button::GetFrameWidth(GET_SPRITE(_sprite_layer02_3), 0) - (GAME()->_game_context.ScaleOut*2.0f);
	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		_layer02_3_x[i] = i*_bg02_layer_3_w + _beginBG;
	}

	_bg02_layer_4_w = Button::GetFrameWidth(GET_SPRITE(_sprite_layer02_4), 0) - 1;
	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		_layer02_4_x[i] = i*_bg02_layer_4_w + _beginBG;
	}


	_speed_start = GET_DES().getCharaterDes(0, CHARATER_DES_SPEED_SCROLL_MAP);
	
	
}
void CBattleMap::UpdateBattleMap_2()
{	
	//increace speed
	_speed_scroll_map = _speed_start+BATTLE()._speedIncreaseMap;
	

	if (CHARATER().GetState() == CHARATER_STATE_DIE)
	{
		_speed_scroll_map = 0;
	}

	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		_layer02_1_x[i] -= _speed_scroll_map*0.05;
		if (_layer02_1_x[i] + _bg02_layer_1_w <= 0)
		{
			_layer02_1_x[i] += _bg02_layer_1_w * 2;
		}

		//if (BATTLE()._timeBegin <= 0)
		{
			_layer02_2_x[i] -= _speed_scroll_map*0.2;
			if (_layer02_2_x[i] + _bg02_layer_2_w <= 0)
			{
				_layer02_2_x[i] += _bg02_layer_2_w * 2;
			}
		}

		_layer02_2a_x[i] -= _speed_scroll_map*0.5;
		if (_layer02_2a_x[i] + _bg02_layer_2a_w <= 0)
		{
			_layer02_2a_x[i] += _bg02_layer_2a_w * 2;
		}

		_layer02_3_x[i] -= _speed_scroll_map;
		if (_layer02_3_x[i] + _bg02_layer_3_w <= 0)
		{
			_layer02_3_x[i] += _bg02_layer_3_w * 2;
		}

		_layer02_4_x[i] -= _speed_scroll_map;
		if (_layer02_4_x[i] + _bg02_layer_4_w <= 0)
		{
			_layer02_4_x[i] += _bg02_layer_4_w * 2;
		}
	}


}
void CBattleMap::RenderBattleMap_2(float x, float y)
{
	//render bg layer 1
    float zoom = 1.0f;
    float offsetY = 0;
    if(CGame::_this->_device._version == SupportVersion::RESVER_960_640)
    {
        zoom = 1.20f;
        offsetY = -0.15*640/2;
    }
    if(CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
    {
        zoom = IPAD_ZOOM;
        offsetY = IPAD_OFFSET;
    }
#if defined (ANDROID_OS)
	if (CGame::_this->_current_version == VERSION_640x1136)
	{
		float zoomx = GAME()->_device.ScreenSize.W / 1136;
		float zoomy = GAME()->_device.ScreenSize.H / 640;
		zoom = zoomx > zoomy ? zoomx : zoomy;
		if (zoom < 1.0f) zoom = 1.0f;
	}
#endif
	if (_stateSwap == 1 && _currentBG == 2)
	{
		G()->SetClip(k_SCREEN_WIDTH / 2 + _swapBG_x, -100, k_SCREEN_WIDTH * 10 * zoom, k_SCREEN_HEIGHT*zoom);
	}
	
    G()->SetScale(zoom,zoom,k_SCREEN_WIDTH/2, k_SCREEN_HEIGHT/4);
	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		if (x + _layer02_1_x[i] < k_SCREEN_WIDTH)
		{
			GET_SPRITE(_sprite_layer02_1)->DrawFrame(G(), 0, x + _layer02_1_x[i], y + BATTLE()._vibrateY);
		}
	}
    G()->ClearScale();
	//render bg layer 2
	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		if (x + _layer02_2_x[i] < k_SCREEN_WIDTH)
		{
			GET_SPRITE(_sprite_layer02_2)->DrawFrame(G(), 0, x + _layer02_2_x[i], y + BATTLE()._vibrateY);
		}
	}

	//render bg layer 2
	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		if (x + _layer02_2a_x[i] < k_SCREEN_WIDTH)
		{
			GET_SPRITE(_sprite_layer02_2a)->DrawFrame(G(), 0, x + _layer02_2a_x[i], y + BATTLE()._vibrateY);
		}
	}

	//render bg layer 3
	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		if (x + _layer02_3_x[i] < k_SCREEN_WIDTH)
		{
			GET_SPRITE(_sprite_layer02_3)->DrawFrame(G(), 0, x + _layer02_3_x[i], y + BATTLE()._vibrateY);
		}
	}
	G()->ClearClip();
}
void CBattleMap::RenderBGLayer4_2(float x, float y)
{
    float zoom = 1.0f;
    float offsetY = 0;
	float offsetX = 0;
    if(CGame::_this->_device._version == SupportVersion::RESVER_960_640)
    {
        zoom = 1.20f;
        offsetY = -0.15*640/2;
    }
    if(CGame::_this->_device._version == SupportVersion::RESVER_1024_768)
    {
        zoom = IPAD_ZOOM;
        offsetY = IPAD_OFFSET;
		offsetX = -150;
    }
    G()->SetScale(zoom, zoom, k_SCREEN_WIDTH/2,k_SCREEN_HEIGHT/2);
	if (_stateSwap == 1 && _currentBG == 2)
	{
		G()->SetClip(k_SCREEN_WIDTH + _swapBG_x + offsetX, 0, k_SCREEN_WIDTH * 10 * zoom, k_SCREEN_HEIGHT*zoom);
	}
	if (_stateSwap == 1 && _currentBG == 2)
	{
		int www = _swapBG_x + k_SCREEN_WIDTH / 2;
		if (www < 0)
		{
			www = 1;
		}
		G()->SetClip(0, 0, www* zoom, k_SCREEN_HEIGHT* zoom);
	}

	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		if (x + _layer02_4_x[i] < k_SCREEN_WIDTH)
		{
			GET_SPRITE(_sprite_layer02_4)->DrawFrame(G(), 0, x + _layer02_4_x[i], y + BATTLE()._vibrateY);
		}
	}

	G()->ClearClip();
    G()->ClearScale();
}

void CBattleMap::InitRail()
{
	_isEndRail = false;
	//init bg layer 1
	if (_stateSwap == 1 && _currentBG == 2)
	{
		G()->SetClip(k_SCREEN_WIDTH / 2 + _swapBG_x, 0, k_SCREEN_WIDTH * 10, k_SCREEN_HEIGHT);
	}
	_rail_w = Button::GetFrameWidth(GET_SPRITE(SPRITEID_BOSS_02), 13);
	for (int i = 0; i < NUM_FRAME_MAP; i++)
	{
		_rail_x[i] = i*_rail_w + _beginRail;
		_rail_frame[i] = 13 + i;
	}
	BATTLE()._endTrain = GET_DES().getCheckPointDes(BATTLE()._lvlCheckPiont + 1, CHECK_POINT_DES_DISTANCE);
//	_endRail = false;
	_endRailX = -200;
	BATTLE()._offsetMoveY = 50;
}
void CBattleMap::UpdateRail()
{	
	if ( BATTLE().GetArmyBoss(TYPE_ENEMY_BOSS_2) != NULL)
	{
		for (int i = 0; i < NUM_FRAME_MAP; i++)
		{
			_rail_x[i] -= _speed_scroll_map;
			if (_rail_x[i] + _rail_w <= 0 )
			{
				_rail_x[i] += _rail_w * 2;
				_rail_frame[i] = 14;				
			}
		}
	}
	_endRailX -= _speed_scroll_map;
}
void CBattleMap::RenderRail(float x, float y)
{
	if (BATTLE().GetArmyBoss(TYPE_ENEMY_BOSS_2) != NULL)
	{
		int ww = _endRailX + 70;
		if (ww <0)
		{
			ww = 1;
		}
		if (BATTLE()._distanceRun >= BATTLE()._endTrain)
		{
			G()->SetClip(0, 0, ww, k_SCREEN_HEIGHT);
		}
		

		for (int i = 0; i < NUM_FRAME_MAP; i++)
		{
			if (x + _rail_x[i] < k_SCREEN_WIDTH)
			{
				GET_SPRITE(SPRITEID_BOSS_02)->DrawFrame(G(), _rail_frame[i], x + _rail_x[i], y + BATTLE()._vibrateY);
			}
		}
		G()->ClearClip();
	}
	RenderEndRail(x, y);
}
void CBattleMap::RenderEndRail(float x, float y)
{
	if (x + _endRailX < k_SCREEN_WIDTH && _endRailX >= -200)
	{		
		GET_SPRITE(SPRITEID_BOSS_02)->DrawFrame(G(), 15, x + _endRailX, y + BATTLE()._vibrateY);		
	}
}