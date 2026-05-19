#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_stdlib.h>

#include "Function.h"
#include "Plot.h"

namespace GUI
{
    void InitalizeUI(GLFWwindow *window);
    void ShutdownUI();

    void DrawUI(Plot &plot);
    void DrawFunctionWidget(float windowWidth, Function &function, int index);
    void AddDropShadow(ImVec2 pos, ImVec2 size, ImDrawList *drawList);

    std::string SuperScriptDigit(int digit);
}