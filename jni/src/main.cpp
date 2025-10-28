#include <main.h>
#include <check.h>
#include <gles3>
#include <complex.h>
#include <imgui_internal.h>
#include <dirent.h>
#include <stb_image.h>
#include "namepath.h"
#include "VecTool.h"
#include "DrawTools.h"
#include "Aim.h"
#include <sys>
#include <thread>
#include <fstream>
//#include "MyDriver.h"
#include <sys>
#include <iostream>
#include <string>
#include <ctime>
#include <unistd.h>
#include <netdb.h> 
#define CornerSize 6 // 角的大小
#define CornerThickness 2 // 角线的粗细
#include "hook.h"
#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <vector>
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
#include <stb_image.h>
#include <cstdlib>
#include "base64.h"

// ====== СТРУКТУРА ДЛЯ СЕГМЕНТОВ ПРИЦЕЛИВАНИЯ ======
// Структура для представления сегмента между двумя костями
struct TargetSegment {
    int startBoneId;      // ID начальной кости сегмента
    int endBoneId;        // ID конечной кости сегмента  
    const char* segmentName;  // Описательное имя сегмента
    
    TargetSegment() : startBoneId(0), endBoneId(0), segmentName("") {}
    TargetSegment(int start, int end, const char* name) 
        : startBoneId(start), endBoneId(end), segmentName(name) {}
};

// ====== ГЛОБАЛЬНЫЕ МАССИВЫ СЕГМЕНТОВ ДЛЯ ОСНОВНЫХ КОНЕЧНОСТЕЙ ======
// Сегменты для правой руки (плечо, предплечье, кисть)
TargetSegment rightArmSegments[] = {
    TargetSegment(12, 13, "Right Upper Arm"),     // Плечо правое
    TargetSegment(13, 14, "Right Forearm"),       // Предплечье правое  
    TargetSegment(14, 92, "Right Hand")           // Кисть правая
};

// Сегменты для левой руки (плечо, предплечье, кисть)
TargetSegment leftArmSegments[] = {
    TargetSegment(33, 34, "Left Upper Arm"),      // Плечо левое
    TargetSegment(34, 35, "Left Forearm"),        // Предплечье левое
    TargetSegment(35, 113, "Left Hand")           // Кисть левая
};

// Сегменты для правой ноги (бедро, голень, стопа)
TargetSegment rightLegSegments[] = {
    TargetSegment(52, 53, "Right Thigh"),         // Бедро правое
    TargetSegment(53, 54, "Right Shin"),          // Голень правая
    TargetSegment(54, 55, "Right Foot")           // Стопа правая
};

// Сегменты для левой ноги (бедро, голень, стопа)  
TargetSegment leftLegSegments[] = {
    TargetSegment(56, 57, "Left Thigh"),          // Бедро левое
    TargetSegment(57, 58, "Left Shin"),           // Голень левая
    TargetSegment(58, 59, "Left Foot")            // Стопа левая
};

// Сегменты для торса (позвоночник, грудь)
TargetSegment torsoSegments[] = {
    TargetSegment(5, 6, "Upper Spine"),           // Верхний отдел позвоночника
    TargetSegment(6, 7, "Mid Spine"),             // Средний отдел позвоночника
    TargetSegment(7, 2, "Lower Spine")            // Нижний отдел позвоночника
};

// ====== ПЕРЕМЕННЫЕ ДЛЯ УПРАВЛЕНИЯ СЕГМЕНТАМИ ======
int currentSelectedSegmentIndex = 0;              // Индекс выбранного сегмента в текущем массиве
float limbSegmentInterpolationAlpha = 0.5f;       // Коэффициент интерполяции вдоль сегмента (0.0 = начало, 1.0 = конец)
int currentLimbGroupIndex = 0;                    // Текущая группа конечностей (0=правая рука, 1=левая рука, и т.д.)

// Указатель на текущий массив сегментов и его размер
TargetSegment* activeSegmentArray = rightArmSegments;
int activeSegmentArraySize = 3;

const char* limbGroupNames[] = {
    "Right Arm", "Left Arm", "Right Leg", "Left Leg", "Torso"
};

// Функция для получения интерполированной позиции цели на основе сегмента и alpha
Vec3 GetInterpolatedTargetPosition(int enemyIndex, TargetSegment segment, float alpha) {
    Vec3 startBonePos = GetBone(enemyIndex, segment.startBoneId);
    Vec3 endBonePos = GetBone(enemyIndex, segment.endBoneId);
    
    // Линейная интерполяция между начальной и конечной костью
    Vec3 interpolatedPos;
    interpolatedPos.x = startBonePos.x + (endBonePos.x - startBonePos.x) * alpha;
    interpolatedPos.y = startBonePos.y + (endBonePos.y - startBonePos.y) * alpha;
    interpolatedPos.z = startBonePos.z + (endBonePos.z - startBonePos.z) * alpha;
    
    return interpolatedPos;
}

// ====== ФУНКЦИЯ ОБНОВЛЕНИЯ АКТИВНОГО МАССИВА СЕГМЕНТОВ ======
// Функция для обновления указателя на текущий массив сегментов в зависимости от выбранной группы
void UpdateActiveSegmentArray() {
    switch (currentLimbGroupIndex) {
        case 0:  // Right Arm
            activeSegmentArray = rightArmSegments;
            activeSegmentArraySize = 3;
            break;
        case 1:  // Left Arm
            activeSegmentArray = leftArmSegments;
            activeSegmentArraySize = 3;
            break;
        case 2:  // Right Leg
            activeSegmentArray = rightLegSegments;
            activeSegmentArraySize = 3;
            break;
        case 3:  // Left Leg
            activeSegmentArray = leftLegSegments;
            activeSegmentArraySize = 3;
            break;
        case 4:  // Torso
            activeSegmentArray = torsoSegments;
            activeSegmentArraySize = 3;
            break;
    }
    // Сброс индекса сегмента, если он выходит за границы нового массива
    if (currentSelectedSegmentIndex >= activeSegmentArraySize) {
        currentSelectedSegmentIndex = 0;
    }
}

// ====== IMGUI МЕНЮ ДЛЯ ВЫБОРА СЕГМЕНТА И ALPHA ======
// Функция для отрисовки ImGui меню выбора сегментов (добавьте в вашу главную ImGui функцию)
void RenderSegmentSelectionMenu() {
    if (ImGui::CollapsingHeader("Advanced Aim Settings")) {
        ImGui::Indent();
        
        // Выбор группы конечностей
        if (ImGui::Combo("Limb Group", &currentLimbGroupIndex, limbGroupNames, 5)) {
            UpdateActiveSegmentArray();
        }
        
        ImGui::Separator();
        
        // Отображение доступных сегментов в текущей группе
        ImGui::Text("Select Target Segment:");
        for (int i = 0; i < activeSegmentArraySize; i++) {
            bool isSelected = (i == currentSelectedSegmentIndex);
            if (ImGui::Selectable(activeSegmentArray[i].segmentName, isSelected)) {
                currentSelectedSegmentIndex = i;
            }
        }
        
        ImGui::Separator();
        
        // Слайдер для настройки коэффициента интерполяции
        ImGui::Text("Aim Point Position:");
        ImGui::SliderFloat("Alpha", &limbSegmentInterpolationAlpha, 0.0f, 1.0f, "%.2f");
        ImGui::Text("0.0 = Start Bone | 1.0 = End Bone");
        
        // Отображение текущих настроек
        ImGui::Separator();
        ImGui::Text("Current Settings:");
        ImGui::BulletText("Group: %s", limbGroupNames[currentLimbGroupIndex]);
        ImGui::BulletText("Segment: %s", activeSegmentArray[currentSelectedSegmentIndex].segmentName);
        ImGui::BulletText("Alpha: %.2f", limbSegmentInterpolationAlpha);
        
        ImGui::Unindent();
    }
}

// ====== ИНТЕГРАЦИЯ В ЛОГИКУ АВТОНАВОДКИ ======
// ИНСТРУКЦИЯ: В вашей функции автонаводки замените вычисление targetPos на следующий код:
// 
// Пример интеграции:
// Vec3 targetPos = GetInterpolatedTargetPosition(
//     enemyIndex, 
//     activeSegmentArray[currentSelectedSegmentIndex], 
//     limbSegmentInterpolationAlpha
// );
// 
// Затем используйте targetPos для вычисления AimObj как обычно.
// Это позволит системе прицеливаться в интерполированную точку между двумя костями сегмента.

// ПРИМЕЧАНИЕ: Не забудьте вызвать RenderSegmentSelectionMenu() в вашей главной ImGui функции отрисовки меню
