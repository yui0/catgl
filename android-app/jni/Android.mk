LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#LOCAL_CFLAGS += -fPIE
#LOCAL_LDFLAGS += -fPIE -pie

LOCAL_MODULE := CatNative
LOCAL_CFLAGS := -std=c++11
LOCAL_SRC_FILES := @@SOURCE@@
LOCAL_C_INCLUDES := @@INCLUDES@@
LOCAL_CXXFLAGS := -DNDEBUG -DCK_ANDROID
LOCAL_LDLIBS := -L@@LIBS@@ -llog -landroid -lEGL -lGLESv1_CM -lGLESv2 -lOpenSLES
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
