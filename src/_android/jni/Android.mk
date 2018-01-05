LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := PineEngine
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../game/Lib/PineEngine/_android/libPineEngine.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := FreeType
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../game/Lib/FreeType/_android/libFreeType.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm

LOCAL_MODULE := topebox_framework

LOCAL_C_INCLUDES += $(LOCAL_PATH) \
					$(STLPORT_BASE)/stlport \
					$(LOCAL_PATH)/../../game/Lib/PineEngine/includes \
					$(LOCAL_PATH)/../../game/Lib/FreeType/header \
					$(LOCAL_PATH)/../../game/GameCore

LOCAL_CFLAGS := -DANDROID_NDK \
				-DANDROID_OS \
				-std=c++11 \
				-w \
				-DOPENSSL_STATIC\
				-D_DEBUG \
				-DNDK_DEBUG=1
				
LOCAL_CPPFLAGS  += -std=c++11 -DANDROID_NDK -DANDROID_OS -D_DEBUG

LOCAL_CPP_FEATURES += exceptions

LOCAL_SRC_FILES := \
	core/jni_OsFunction.cpp \
	core/jni_game.cpp \
	core/jni_environment.cpp \
	core/jni_log.cpp \
	core/jni_service_manager.cpp \
	../../game/GameCore/CAnimManager.cpp \
	../../game/GameCore/CAnimObject.cpp \
	../../game/GameCore/CGame_Core.cpp \
	../../game/GameCore/CTouch.cpp \
	../../game/GameCore/Device.cpp \
    ../../game/GameCore/GameContext.cpp \
    ../../game/GameCore/PineGame.cpp \
	../../game/GamePlay/GameObject/AccountManager.cpp \
	../../game/GamePlay/GameObject/CAchievement.cpp \
	../../game/GamePlay/GameObject/CArmy.cpp \
	../../game/GamePlay/GameObject/CBattle.cpp \
	../../game/GamePlay/GameObject/CBattleMap.cpp \
	../../game/GamePlay/GameObject/CCavalry.cpp \
	../../game/GamePlay/GameObject/CCharater.cpp \
	../../game/GamePlay/GameObject/CDrawableManager.cpp \
	../../game/GamePlay/GameObject/CEffect.cpp \
	../../game/GamePlay/GameObject/CInstane.cpp \
	../../game/GamePlay/GameObject/CScrollBar.cpp \
	../../game/GamePlay/GameObject/CTutorial.cpp \
	../../game/GamePlay/GameObject/CUser.cpp \
	../../game/GamePlay/GameObject/Design_Util.cpp \
	../../game/GamePlay/GameObject/GDS_Exporter.cpp \
	../../game/GamePlay/GameObject/Topebox_PromoGame.cpp \
	../../game/GamePlay/GameState/CGame_State_Confirm.cpp \
	../../game/GamePlay/GameState/CGame_State_IGM.cpp \
	../../game/GamePlay/GameState/CGame_State_Main_Menu.cpp \
	../../game/GamePlay/GameState/CGame_State_Play_Game.cpp \
	../../game/GamePlay/GameState/CGame_State_Result.cpp \
	../../game/GamePlay/GameState/CGame_State_Tutorial.cpp \
	../../game/GamePlay/Game_ExternFunctions.cpp \
	../../game/GamePlay/Android_Connector.cpp \
	../../game/Lib/png/png.cpp \
	../../game/Lib/png/lodepng.cpp \
	../../game/Lib/Math/PineMatrix4.cpp \
	../../game/Lib/Math/PinePolygon.cpp \
	../../game/Lib/Math/PineVector3.cpp \
	../../game/Lib/Math/SMatrix4x4.cpp \
	../../game/Lib/Math/CMath.cpp \
	../../game/Lib/OpenGLES/GLESLib1.cpp \
	../../game/Lib/OpenGLES/Graphic.cpp \
	../../game/Lib/Sound/OpenSLES/Sound.cpp \
	../../game/Lib/Sound/OpenSLES/Music.cpp \
	../../game/Lib/Sound/OpenSLES/SoundOpenSLES.cpp \
	../../game/Lib/Sound/OpenSLES/WaveFormat.cpp \
	../../game/Lib/DataInputStream/DataInputStream.cpp \
	../../game/Lib/DataType/BufferStream.cpp \
	../../game/Lib/DataType/ServiceStream.cpp \
	../../game/Lib/Sound/SoundEngine.cpp \
	../../game/Lib/ArrayList/ArrayList.cpp \
	../../game/Lib/Sprite/CSprite.cpp \
	../../game/Lib/String/StringUtils.cpp \
	../../game/Lib/PineGameCenterController.cpp \
	../../game/Lib/PineHighScore.cpp \
	../../game/Lib/PineLeaderBoard.cpp \
	../../game/Lib/PineSocial.cpp \
	../../game/Lib/PineImage.cpp \
	../../game/Lib/PineAdBox.cpp \
	../../game/Lib/PineADVERT.cpp \
	../../game/Lib/PineAdmobAsync.cpp \
	../../game/Lib/PineTapdaqAsync.cpp \
	../../game/MainGame/WildWildWest.cpp \
	../../game/GamePlay/CGame_State.cpp \
	../../game/GamePlay/GameState/CGame_State_Logo.cpp \
	../../game/GamePlay/GameObject/MenuSystem.cpp \
	../../game/GamePlay/GameObject/SpriteList.cpp \
	../../game/GamePlay/GameObject/CAsyncTasks.cpp \
	
	
LOCAL_SHARED_LIBRARIES := PineEngine FreeType
	
LOCAL_LDLIBS := -latomic -ldl -llog -lstdc++ -lc -lGLESv1_CM -lm -lOpenSLES -pthread -landroid
#-lGLESv1_CM  -lGLESv2
include $(BUILD_SHARED_LIBRARY)
