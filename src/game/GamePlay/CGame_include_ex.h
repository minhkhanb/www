/**
*	will be included in CGame.h
*	for include specific *.h
*/
//#include "some_classes.h"
#include "../GamePlay/GameObject/AccountManager.h"
#include "../Lib/PineSocialAsync.h"

#include "../GamePlay/GameObject/MenuSystem.h"
#include "../GamePlay/GameObject/SpriteList.h"
#include "../GamePlay/GameObject/CDrawableManager.h"
#include "../GamePlay/GameObject/CTutorial.h"
#include "../GamePlay/GameObject/CInstane.h"
#include "../GamePlay/GameObject/CCharater.h"
#include "../GamePlay/GameObject/CArmy.h"
#include "../GamePlay/GameObject/CCavalry.h"
#include "../GamePlay/GameObject/CBattleMap.h"
#include "../GamePlay/GameObject/CBattle.h"
#include "../GamePlay/GameObject/CObstacle.h"
#include "../GamePlay/GameObject/GDS_Exporter.h"
#include "../GamePlay/GameObject/Design_Util.h"
#include "../GamePlay/GameObject/CEffect.h"
#include "../GamePlay/GameObject/CAchievement.h"

#include "../Lib/DataType/BufferStream.h"
#include "../Lib/Math/SMatrix4x4.h"
#include "../Lib/PineAdBox.h"
#include "../Lib/Sprite/PineSprite.h"
#include "../Lib/Graphic/PineCanvas.h"
#include "../Lib/Graphic/PineCanvasResource.h"
#include "../Lib/Graphic/PineTexture.h"
#include "../GamePlay/GameObject/CUser.h"
#include "../GameCore/CAnimManager.h"



#include "../GameCore/PineGame.h"
#include "../MainGame/WildWildWest.h"
#include "../GamePlay/GameObject/Topebox_PromoGame.h"

#if defined(_WINDOWS_PHONE8)
#include "../GamePlay/GameObject/AdmobManager.h"
#elif defined (ANDROID_OS)
#include "../Lib/PineTapdaqAsync.h"
#include "../GamePlay/GameObject/CAsyncTasks.h"
#endif


