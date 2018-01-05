#include "pch.h"
#include "../../GameCore/CGame.h"

void CTutorial::SetTutorial(BYTE state)
{
	switch (state)
	{
	case 0:
		BATTLE()._mull = 10;
		GAME()->StopAllSFX();
		sprintf(_string, "%s", GET_DES().getString(18));
		break;
	case 1:
		sprintf(_string, "%s", GET_DES().getString(19));
		break;
	case 2:
		sprintf(_string, "%s", GET_DES().getString(20));
		break;
	case 3:
		sprintf(_string, "%s", GET_DES().getString(21));
		break;

	case 10:
		sprintf(_string, "%s", GET_DES().getString(22));
		break;

	}
	_state = state;
}

void CTutorial::RenderTutorial(float x, float y)
{
	int line_spac = GET_SPRITE(SPRITEID_FONT_MONEY)->_line_spacing;
	GET_SPRITE(SPRITEID_FONT_MONEY)->SetLineSpacing(10);
	switch (_state)
	{
	case 0:
		GET_SPRITE(SPRITEID_HUD_TUTORIAL)->DrawFrame(G(), 3, x, y + 10);
		
		G()->SetScale(0.8, 0.8, k_SCREEN_WIDTH / 2, 130);
		GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, 130, _HCENTER | _VCENTER, "1/3");
		G()->SetScale(0.7, 0.7, k_SCREEN_WIDTH / 2, 440);
		GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiPageFormat(G(), k_SCREEN_WIDTH / 2, 460, 800, _HCENTER | _BOTTOM, _string);
		G()->ClearScale();
		break;
	case 1:
		GET_SPRITE(SPRITEID_HUD_TUTORIAL)->DrawFrame(G(), 4, x, y);
		GET_SPRITE(SPRITEID_HUD_STICK)->DrawFrame(G(), 0, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2 - 50);
		GET_SPRITE(SPRITEID_HUD_STICK)->DrawFrame(G(), 2, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2 - 50);
		G()->SetScale(0.8, 0.8, k_SCREEN_WIDTH / 2, 130);
		GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, 130, _HCENTER | _VCENTER, "2/3");
		G()->SetScale(0.7, 0.7, k_SCREEN_WIDTH / 2, 440);
		GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiPageFormat(G(), k_SCREEN_WIDTH / 2, 460, 800, _HCENTER | _BOTTOM, _string);
		G()->ClearScale();
		break;
	case 2:
		GET_SPRITE(SPRITEID_HUD_TUTORIAL)->DrawFrame(G(), 5, x, y);
		G()->SetScale(0.8, 0.8, k_SCREEN_WIDTH / 2, 130);
		GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2, 130, _HCENTER | _VCENTER, "3/3");
		G()->SetScale(0.7, 0.7, k_SCREEN_WIDTH / 2, 440);
		GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiPageFormat(G(), k_SCREEN_WIDTH / 2, 460, 800, _HCENTER | _BOTTOM, _string);
		G()->ClearScale();
		break;
	case 3:
		GET_SPRITE(SPRITEID_HUD_TUTORIAL)->DrawFrame(G(), 6, x, y);
		G()->SetScale(0.7, 0.7, k_SCREEN_WIDTH / 2, 440);
		GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiPageFormat(G(), k_SCREEN_WIDTH / 2, 460,800, _HCENTER | _BOTTOM, _string);
		G()->ClearScale();
		break;
	case 10:
		GET_SPRITE(SPRITEID_HUD_TUTORIAL)->DrawFrame(G(), 7, x, y);


		G()->SetScale(0.7, 0.7, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT/2 + 50);
		GET_SPRITE(SPRITEID_FONT_MONEY)->DrawAnsiPageFormat(G(), k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT/2 + 50, 700, _HCENTER | _VCENTER, _string);
		G()->ClearScale();

		break;
	}
	GET_SPRITE(SPRITEID_FONT_MONEY)->SetLineSpacing(line_spac);
}

void CTutorial::UpdateTutorial()
{

}