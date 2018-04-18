LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
 $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
$(LOCAL_PATH)/../../../Classes/MainGame.cpp \
$(LOCAL_PATH)/../../../Classes/Button.cpp \
$(LOCAL_PATH)/../../../Classes/ButtonBase.cpp \
$(LOCAL_PATH)/../../../Classes/Enemy.cpp \
$(LOCAL_PATH)/../../../Classes/HUDLayer.cpp \
$(LOCAL_PATH)/../../../Classes/Joystick.cpp \
$(LOCAL_PATH)/../../../Classes/JoystickBase.cpp \
$(LOCAL_PATH)/../../../Classes/MainScene.cpp \
$(LOCAL_PATH)/../../../Classes/Player.cpp \
$(LOCAL_PATH)/../../../Classes/Skill.cpp \
$(LOCAL_PATH)/../../../Classes/StatPlayer.cpp


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
