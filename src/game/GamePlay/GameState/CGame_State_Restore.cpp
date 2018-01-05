#include "pch.h"
#include "../../GameCore/CGame.h"

void CGame::Prepare_restore()
{
	
	if (_GameState != k_GS_RESTORE){
		_restore_state.LastState = _GameStateOld;
		_restore_state.CurrentState = _GameState;
		_restore_state.NewState = _GameStateNew;
		_restore_state.SubState = _GameSubState;
		_step_force_loading = 0;
		BackupAnimStock();
		for (int i = 0; i < TOTAL_SPRITEID; i++)
		{
			if (GET_SPRITE(i) != NULL)
			{
				_backup_sprite_restore[i] = 1;
				GET_SPRITE(i)->unloadTexture(G());
				//UnloadSpriteVer(i);
			}
			else{
				_backup_sprite_restore[i] = 0;
			}
		}
	}
	/*else{
		for (int i = 0; i < TOTAL_SPRITE_INGAME; i++)
		{
			if (GET_SPRITE(i) != NULL)
			{
				UnloadSpriteVer(i);
			}
		}
	}*/

}
void CGame::Update_state_restore()
{
	if (GAME()->_GameSubState == k_INIT)
	{
		//LoadSpriteCurrentVer(SPRITEID_GFX_HUD_WAITING);

		_restore_step = 0;
		GAME()->_GameSubState = k_LOOP;
		//GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_LOADING);
		//GAME()->LoadSpriteCurrentVer(SPRITEID_FONT_LOADING);
		GAME()->LoadSpriteCurrentVer(SPRITEID_BG_LOADING);
	}
	else if (GAME()->_GameSubState == k_LOOP)
	{
		//load sprite
		if (_restore_step < TOTAL_SPRITE_INGAME)
		{
			if (_backup_sprite_restore[_restore_step] == 1)
			{
				GAME()->LoadSpriteCurrentVer(_restore_step, GAME()->_sprite_antialias_loaded[_restore_step], GAME()->_sprite_optimized_loaded[_restore_step]);
			}
		}
		//change state to game
		else{
			GAME()->RestoreAnimStock();
			GAME()->_GameState = _restore_state.CurrentState;
			GAME()->_GameStateOld = _restore_state.LastState;
			GAME()->_GameStateNew = _restore_state.NewState;
			GAME()->_GameSubState = _restore_state.SubState;
			PDEBUG("\n _restore_state.CurrentState = %d", _restore_state.CurrentState);
			switch (_restore_state.CurrentState){
				//case k_GS_GAMEPLAY_CATCH_FRUIT:
				//_StateSlashFruit_buttons[0].Init(BUTTON_STATE_NORMAL,0,0,A_HVCENTER);
				//	init_button_agian = true;
				//	SetGameState(k_GS_GAMEPLAY_PAUSE, false);
				//	break;
			default:
				//SetGameState(k_GS_LOGO, false);
				break;
			}
			
		}
		_restore_step++;
	}
}
void CGame::Render_state_restore()
{
	if (GAME()->_GameSubState == k_LOOP)
	{
		GET_SPRITE(SPRITEID_BG_LOADING)->DrawFrame(G(), 0, 0, 0);
	}
}
	