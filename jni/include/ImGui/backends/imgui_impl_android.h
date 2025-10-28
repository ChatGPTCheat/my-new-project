#pragma once

struct ANativeWindow;
struct AInputEvent;

IMGUI_IMPL_API bool     ImGui_ImplAndroid_Init(ANativeWindow* window = NULL);
IMGUI_IMPL_API int32_t  ImGui_ImplAndroid_HandleInputEvent(AInputEvent* input_event);
IMGUI_IMPL_API int32_t  ImGui_ImplAndroid_HandleInputEvent(AInputEvent *input_event, ImVec2 screen_scale);
IMGUI_IMPL_API void     ImGui_ImplAndroid_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplAndroid_NewFrame(int32_t window_width,int32_t window_height);
