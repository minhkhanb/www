#if !_CBATTLE_MAP_H
#define _CBATTLE_MAP_H 1

#define NUM_FRAME_MAP		2

struct BGPlayer
{

};

class CBattleMap
{
    float _bg01_layer_1_w;
	int _sprite_layer01_1;
	float _layer01_1_x[NUM_FRAME_MAP];
	

    float _bg01_layer_2_w;
	int _sprite_layer01_2;
	float _layer01_2_x[NUM_FRAME_MAP];

	float _bg01_layer_2a_w;
	int _sprite_layer01_2a;
	float _layer01_2a_x[NUM_FRAME_MAP];
    
    float _bg01_layer_3_w;
	int _sprite_layer01_3;
	float _layer01_3_x[NUM_FRAME_MAP];
    
    float _bg01_layer_4_w;
	int _sprite_layer01_4;
	float _layer01_4_x[NUM_FRAME_MAP];
    
	////////////////////
	float _bg02_layer_1_w;
	int _sprite_layer02_1;
	float _layer02_1_x[NUM_FRAME_MAP];

	float _bg02_layer_2_w;
	int _sprite_layer02_2;
	float _layer02_2_x[NUM_FRAME_MAP];

	float _bg02_layer_2a_w;
	int _sprite_layer02_2a;
	float _layer02_2a_x[NUM_FRAME_MAP];

	float _bg02_layer_3_w;
	int _sprite_layer02_3;
	float _layer02_3_x[NUM_FRAME_MAP];

	float _bg02_layer_4_w;
	int _sprite_layer02_4;
	float _layer02_4_x[NUM_FRAME_MAP];

	float _rail_w;	
	float _rail_x[NUM_FRAME_MAP];
	float _rail_frame[NUM_FRAME_MAP];
	

public:
	CBattleMap(){};
   // PineImage _bgLayer3;
    float _speed_scroll_map;
	float _speed_start;

	bool _usedBG_1;	
	void InitBattleMap_1();
	void UpdateBattleMap_1();
	void RenderBattleMap_1(float x, float y);
    void RenderBGLayer4_1(float x, float y);

	bool _usedBG_2;
	void InitBattleMap_2();
	void UpdateBattleMap_2();
	void RenderBattleMap_2(float x, float y);
	void RenderBGLayer4_2(float x, float y);

	float _swapBG_x;
	BYTE _stateSwap;
	void SetSwapBG();
	void RenderSwapBG(float x, float y);
	void RenderSwapBG_1(float x, float y);
	void UpdateSwapBG();
	void SetSpriteBG_1(BYTE type);
	void SetSpriteBG_2(BYTE type);

	float _beginRail;
	BYTE _currentBG;

	void InitRail();
	void UpdateRail();
	void RenderRail(float x, float y);
	bool _isEndRail;
	float _endRailX;
	void RenderEndRail(float x, float y);

private:

};

#endif