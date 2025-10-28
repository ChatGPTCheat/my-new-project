LOCAL_PATH := $(call my-dir)

# Build Android_touch as static library
include $(CLEAR_VARS)
LOCAL_MODULE := android_touch_static
LOCAL_CPPFLAGS += -w -s -Wno-error=format-security -fvisibility=hidden -std=c++17
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -fpermissive -Wall -fexceptions
LOCAL_CPPFLAGS += -Ofast
LOCAL_CPPFLAGS += -O3 -fvisibility=hidden -fvisibility-inlines-hidden
LOCAL_CPPFLAGS += -fpermissive -Wall -fexceptions -fstack-protector
LOCAL_CPPFLAGS += -finline-functions
LOCAL_CPPFLAGS += -funroll-loops
LOCAL_CPPFLAGS += -ftree-vectorize
LOCAL_CPPFLAGS += -g0
LOCAL_CPPFLAGS += -fno-exceptions
LOCAL_CPPFLAGS += -fno-rtti
LOCAL_CPPFLAGS += -fomit-frame-pointer
LOCAL_CPPFLAGS += -ffast-math
LOCAL_CPPFLAGS += -fstrict-aliasing
LOCAL_CFLAGS += -Wno-c++11-narrowing

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/Android_touch
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/Tools

# Only Android_touch source files
TOUCH_FILE_LIST := $(wildcard $(LOCAL_PATH)/src/Android_touch/*.c*)
LOCAL_SRC_FILES := $(TOUCH_FILE_LIST:$(LOCAL_PATH)/%=%)

# Export headers for main executable
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include/Android_touch

include $(BUILD_STATIC_LIBRARY)

# Build ImGui as static library
include $(CLEAR_VARS)
LOCAL_MODULE := imgui_static
LOCAL_CPPFLAGS += -w -s -Wno-error=format-security -fvisibility=hidden -std=c++17
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -fpermissive -Wall -fexceptions
LOCAL_CPPFLAGS += -Ofast
LOCAL_CPPFLAGS += -O3 -fvisibility=hidden -fvisibility-inlines-hidden
LOCAL_CPPFLAGS += -fpermissive -Wall -fexceptions -fstack-protector
LOCAL_CPPFLAGS += -finline-functions
LOCAL_CPPFLAGS += -funroll-loops
LOCAL_CPPFLAGS += -ftree-vectorize
LOCAL_CPPFLAGS += -g0
LOCAL_CPPFLAGS += -fno-exceptions
LOCAL_CPPFLAGS += -fno-rtti
LOCAL_CPPFLAGS += -fomit-frame-pointer
LOCAL_CPPFLAGS += -ffast-math
LOCAL_CPPFLAGS += -fstrict-aliasing
LOCAL_CFLAGS += -Wno-c++11-narrowing

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/ImGui
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/Tools

# Only ImGui source files
IMGUI_FILE_LIST := $(wildcard $(LOCAL_PATH)/src/ImGui/*.c*)
LOCAL_SRC_FILES := $(IMGUI_FILE_LIST:$(LOCAL_PATH)/%=%)

# Export headers for main executable
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include/ImGui

# Link with system libraries that ImGui needs
LOCAL_LDLIBS := -lEGL -lGLESv2 -lGLESv3

include $(BUILD_STATIC_LIBRARY)

# Build main executable
include $(CLEAR_VARS)
LOCAL_MODULE := Zalypa.sh
LOCAL_CPPFLAGS += -w -s -Wno-error=format-security -fvisibility=hidden -Werror -std=c++17
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -fpermissive -Wall -fexceptions
LOCAL_CPPFLAGS += -Ofast
LOCAL_CPPFLAGS += -O3 -fvisibility=hidden -fvisibility-inlines-hidden
LOCAL_CPPFLAGS += -w -s -Wno-error=format-security -Werror
LOCAL_CPPFLAGS += -fpermissive -Wall -fexceptions -fstack-protector
LOCAL_CPPFLAGS += -finline-functions
LOCAL_CPPFLAGS += -funroll-loops
LOCAL_CPPFLAGS += -ftree-vectorize
LOCAL_CPPFLAGS += -g0
LOCAL_CPPFLAGS += -fno-exceptions
LOCAL_CPPFLAGS += -fno-rtti
#LOCAL_CPPFLAGS += -march=core2
LOCAL_CPPFLAGS += -fomit-frame-pointer
LOCAL_CPPFLAGS += -ffast-math
#LOCAL_CPPFLAGS += -flto
#LOCAL_CPPFLAGS += -march=native
#LOCAL_CPPFLAGS += -mtune=native
#LOCAL_CPPFLAGS += -mfpmath=sse
LOCAL_CPPFLAGS += -fomit-frame-pointer
LOCAL_CPPFLAGS += -fstrict-aliasing
LOCAL_CFLAGS += -Wno-c++11-narrowing

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/Aim
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/Android_draw
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/Android_check
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/Tools
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/native_surface

# All source files EXCEPT Android_touch and ImGui (now in static libs)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/Aim/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/Android_draw/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/Android_read/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/Android_check/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/Tools/*.c*)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

# Link with both static libraries
LOCAL_STATIC_LIBRARIES := android_touch_static imgui_static

LOCAL_LDLIBS := -lEGL -lGLESv2 -lGLESv3 -landroid -llog
include $(BUILD_EXECUTABLE)