#include "shou.hpp"
#include <sched.h>
#include <unistd.h>
#include <time.h>
#include <thread>
#include <random>
#include <linux/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <fstream>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <unordered_map>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <thread>
#include <array>
#include <android/native_window_jni.h>
#include "imgui.h"
#include "imgui_image.h"
uintptr_t arraygg= 0x0;
uintptr_t Ue4gg = 0x0;
uintptr_t 数组 = 0;

bool 初始化 = false;

int 测试 = 0;

uintptr_t 模块地址 = 0x0;

const ImColor TITLE_BG_COLOR(192, 192, 192);
const ImColor RESIZE_GRIP_COLOR(192, 192, 192);
const ImColor WINDOW_BG_COLOR(0.85f, 0.85f, 0.85f, 0.95f);
const ImColor BORDER_COLOR(192, 192, 192);
const ImColor FRAME_BG_COLOR(192, 192, 192);
const ImColor SCROLLBAR_BG_COLOR(192, 192, 192);
const ImColor SCROLLBAR_GRAB_COLOR(138, 43, 226);
const ImColor CHECKMARK_COLOR(255, 0, 0); // 将颜色修改为红色
const ImColor SLIDER_GRAB_COLOR(192, 192, 192);
const ImColor SEPARATOR_COLOR(192, 192, 192);
const ImColor BUTTON_COLOR(192, 192, 192);
const ImColor HEADER_COLOR(192, 192, 192);
const ImColor TEXT_SELECTED_BG_COLOR(0, 255, 255);

void 冰冷() {
  ImGuiStyle * style = &ImGui::GetStyle();
        style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);//文本颜色
        style->Colors[ImGuiCol_WindowBg] = ImColor(255, 255, 255, 255);//窗口背景颜色
        style->Colors[ImGuiCol_PopupBg] = ImVec4(96, 155, 223,255);//弹出窗口背景颜色
        style->Colors[ImGuiCol_Border] = ImColor(80, 102, 155,255); // 整体边框颜色
        //style->Colors[ImGuiCol_BorderShadow] = ImColor(80, 102, 155,255);//边框阴影颜色(内部控件边框颜色)
        style->Colors[ImGuiCol_FrameBg] = ImColor(255, 255, 255, 255);//框架背景颜色(按钮没动的颜色)
        style->Colors[ImGuiCol_FrameBgActive] = ImColor(255, 255, 255, 255);//激活的框架背景颜色
        style->Colors[ImGuiCol_FrameBgHovered] = ImColor(255, 255, 255, 255);//鼠标悬停的框架背景颜色
        style->Colors[ImGuiCol_ScrollbarBg] = ImColor(255, 255, 255, 255);//滚动区域背景颜色
        style->Colors[ImGuiCol_ScrollbarGrab] = ImColor(96, 155, 223,255);//滚动条抓取颜色
        style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(80, 102, 155,255);//鼠标悬停时的滚动条抓取颜色
        style->Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(80, 102, 155,255);//激活时的滚动条抓取颜色
        style->Colors[ImGuiCol_CheckMark] = ImColor(80, 102, 155,255);//选中标记颜色
        style->Colors[ImGuiCol_SliderGrab] = ImColor(96, 155, 223,255);//滑块抓取颜色
        style->Colors[ImGuiCol_SliderGrabActive] = ImColor(80, 102, 155,255);//激活时的滑块抓取颜色
        style->Colors[ImGuiCol_Separator] = ImColor(80, 102, 155,255);//分隔线颜色
        style->Colors[ImGuiCol_SeparatorActive] = ImColor(80, 102, 155,255);//激活时的分隔线颜色//深灰色
        style->Colors[ImGuiCol_SeparatorHovered] = ImColor(80, 102, 155,255);//鼠标悬停时的分隔线颜色
        style->Colors[ImGuiCol_Button] = ImColor(96, 155, 223,255);//正常按钮颜色//酷安绿
        style->Colors[ImGuiCol_ButtonActive] = ImColor(80, 102, 155,255);//激活的按钮颜色//微信绿
        style->Colors[ImGuiCol_ButtonHovered] = ImColor(80, 102, 155,255);//鼠标悬停时的按钮颜色
        style->Colors[ImGuiCol_HeaderActive] = ImColor(80, 102, 155,255);//激活的标题颜色
        style->Colors[ImGuiCol_HeaderHovered] = ImColor(80, 102, 155,255);//鼠标悬停时的标题颜色
		style->Colors[ImGuiCol_TextSelectedBg] = ImColor(87, 59, 128,255);//选中文本背景颜色
		style->Colors[ImGuiCol_Header] = ImColor(96, 155, 223,255);//设置下展标题颜色
  
}

void 巴巴托斯()
{
    
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // 修改颜色主题为浅色粉色
    colors[ImGuiCol_Text] = ImVec4(0.85f, 0.33f, 0.73f, 1.00f); // 文本颜色改为浅色粉色
    colors[ImGuiCol_TextDisabled] = ImVec4(0.85f, 0.33f, 0.73f, 0.50f); // 禁用文本颜色改为浅色粉色
    colors[ImGuiCol_WindowBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f); // 窗口背景颜色改为浅灰色
    colors[ImGuiCol_ChildBg] = ImVec4(0.95f, 0.95f, 0.95f, 0.00f); // 子窗口背景颜色改为透明
    colors[ImGuiCol_Border] = ImVec4(0.85f, 0.33f, 0.73f, 0.65f); // 边框颜色改为浅色粉色
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f); // 边框阴影颜色改为透明
    colors[ImGuiCol_FrameBg] = ImVec4(0.95f, 0.95f, 0.95f, 0.18f); // 框架背景颜色改为浅色灰色
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.95f, 0.95f, 0.95f, 0.27f); // 鼠标悬停时的框架背景颜色改为浅色灰色
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.85f, 0.33f, 0.73f, 0.66f); // 激活状态时的框架背景颜色改为浅色粉色
    colors[ImGuiCol_TitleBg] = ImVec4(0.85f, 0.33f, 0.73f, 0.73f); // 标题栏背景颜色改为浅色粉色
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.85f, 0.33f, 0.73f, 0.54f); // 折叠时标题栏背景颜色改为浅色粉色
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.85f, 0.33f, 0.73f, 0.27f); // 激活状态时标题栏背景颜色改为浅色粉色
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.95f, 0.95f, 0.95f, 0.20f); // 菜单栏背景颜色改为浅色灰色
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.85f, 0.33f, 0.73f, 0.71f); // 滚动条背景颜色改为浅色粉色
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.95f, 0.95f, 0.95f, 0.44f); // 滚动条抓取按钮颜色改为浅色灰色
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.95f, 0.95f, 0.95f, 0.74f); // 鼠标悬停时的滚动条抓取按钮颜色改为浅色灰色
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.85f, 0.33f, 0.73f, 1.00f); // 激活状态时的滚动条抓取按钮颜色改为浅色粉色
    colors[ImGuiCol_CheckMark] = ImVec4(0.85f, 0.33f, 0.73f, 0.68f); // 复选标记颜色改为浅色粉色
    colors[ImGuiCol_SliderGrab] = ImVec4(0.95f, 0.95f, 0.95f, 0.36f); // 滑块抓取按钮颜色改为浅色灰色
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.85f, 0.33f, 0.73f, 0.76f); // 激活状态时的滑块抓取按钮颜色改为浅色粉色
    colors[ImGuiCol_Button] = ImVec4(0.95f, 0.95f, 0.95f, 0.46f); // 按钮背景颜色改为浅色灰色
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.95f, 0.95f, 0.95f, 0.43f); // 鼠标悬停时的按钮背景颜色改为浅色灰色
    colors[ImGuiCol_ButtonActive] = ImVec4(0.85f, 0.33f, 0.73f, 0.62f); // 激活状态时的按钮背景颜色改为浅色粉色
    colors[ImGuiCol_Header] = ImVec4(0.85f, 0.33f, 0.73f, 0.33f); // 标题颜色改为浅色粉色    

    colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.00f, 1.00f, 0.42f); // 鼠标悬停时的标题颜色改为粉色
    colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 0.00f, 1.00f, 0.54f); // 激活状态时的标题颜色改为粉色
    colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 0.00f, 1.00f, 0.54f); // 调整大小手柄颜色改为粉色
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.00f, 1.00f, 0.74f); // 鼠标悬停时的调整大小手柄颜色改为粉色
    colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.00f, 1.00f, 1.00f); // 激活状态时的调整大小手柄颜色改为粉色
    colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 1.00f, 1.00f); // 绘制线条颜色改为粉色
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 1.00f, 1.00f); // 鼠标悬停时的绘制线条颜色改为粉色
    colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 1.00f, 1.00f); // 绘制直方图颜色改为粉色
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 1.00f, 1.00f); // 鼠标悬停时的绘制直方图颜色改为粉色
    colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.00f, 1.00f, 0.22f); // 选中文本背景颜色改为粉色

// 在 ImGui 渲染循环中调用 SetImGuiPinkTheme() 函数即可将整个 ImGui 的颜色主题修改为粉色

}


void 酷安(){
		ImGuiStyle * style = &ImGui::GetStyle();
       style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);//文本颜色
    style->Colors[ImGuiCol_WindowBg] = ImColor(255, 255, 255, 255);//窗口背景颜色
    style->Colors[ImGuiCol_PopupBg] = ImVec4(118, 238, 198,255);//弹出窗口背景颜色
    style->Colors[ImGuiCol_Border] = ImColor(127, 255, 212,255); // 整体边框颜色
    //style->Colors[ImGuiCol_BorderShadow] = ImColor(118, 238, 198,255);//边框阴影颜色(内部控件边框颜色)
    style->Colors[ImGuiCol_FrameBg] = ImColor(255, 255, 255, 255);//框架背景颜色(按钮没动的颜色)
     style->Colors[ImGuiCol_FrameBgActive] = ImColor(255, 255, 255, 255);//激活的框架背景颜色
     style->Colors[ImGuiCol_FrameBgHovered] = ImColor(255, 255, 255, 255);//鼠标悬停的框架背景颜色
     style->Colors[ImGuiCol_ScrollbarBg] = ImColor(255, 255, 255, 255);//滚动区域背景颜色
     style->Colors[ImGuiCol_ScrollbarGrab] = ImColor(118, 238, 198,255);//滚动条抓取颜色
     style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(118, 238, 198,255);//鼠标悬停时的滚动条抓取颜色
     style->Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(118, 238, 198,255);//激活时的滚动条抓取颜色
     style->Colors[ImGuiCol_CheckMark] = ImColor(118, 238, 198,255);//选中标记颜色
     style->Colors[ImGuiCol_SliderGrab] = ImColor(16, 255, 225,255);//滑块抓取颜色
     style->Colors[ImGuiCol_SliderGrabActive] = ImColor(118, 238, 198,255);//激活时的滑块抓取颜色
     style->Colors[ImGuiCol_Separator] = ImColor(118, 238, 198,255);//分隔线颜色
     style->Colors[ImGuiCol_SeparatorActive] = ImColor(118, 238, 198,255);//激活时的分隔线颜色//深灰色
     style->Colors[ImGuiCol_SeparatorHovered] = ImColor(118, 238, 198,255);//鼠标悬停时的分隔线颜色
     style->Colors[ImGuiCol_Button] = ImColor(118, 238, 198,255);//正常按钮颜色//酷安绿
     style->Colors[ImGuiCol_ButtonActive] = ImColor(118, 238, 198,255);//激活的按钮颜色//微信绿
     style->Colors[ImGuiCol_ButtonHovered] = ImColor(118, 238, 198,255);//鼠标悬停时的按钮颜色
     style->Colors[ImGuiCol_HeaderActive] = ImColor(118, 238, 198,255);//激活的标题颜色
     style->Colors[ImGuiCol_HeaderHovered] = ImColor(118, 238, 198,255);//鼠标悬停时的标题颜色
	 style->Colors[ImGuiCol_TextSelectedBg] = ImColor(87, 59, 128,255);//选中文本背景颜色
	 style->Colors[ImGuiCol_Header] = ImColor(118, 238, 198,255);//设置下展标题颜色
}