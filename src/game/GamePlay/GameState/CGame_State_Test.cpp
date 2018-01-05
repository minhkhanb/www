#include "pch.h"
#include "../../GameCore/CGame.h"

void WildWildWestGame::UpdateTest()
{

}
void WildWildWestGame::RenderTest()
{
	GET_SPRITE(SPRITEID_BG_LOADING)->DrawFrame(G(), 0, 0, 0);
}