#include "GUI.h"

const int flatWindow = {ImGuiWindowFlags_NoScrollbar | 
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoTitleBar | 
				ImGuiWindowFlags_NoBringToFrontOnFocus};
				
const int childWindow = {ImGuiWindowFlags_NoScrollbar | 
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoTitleBar | 
				ImGuiWindowFlags_NoBringToFrontOnFocus};

const ImVec4 FUNC_WIDGET_COL = ImVec4(64.0f/255, 61.0f/255, 61.0f/255,1);
const ImVec4 LOCAL_VARS_WIDGET_COL = ImVec4(67.0f/255, 66.0f/255, 67.0f/255, 1);
const ImVec4 TEXT_UNDERLINE_COL = ImVec4(136.0f/255, 136.0f/255, 136.0f/255, 80.0f/255);

ImGuiStyle style;
ImGuiIO io;

ImFont *Arial = nullptr;
ImFont *NotoMath = nullptr;
ImFont *LatexMath = nullptr;

namespace GUI
{
    void InitalizeUI(GLFWwindow *window) 
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();	

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();

        style = ImGui::GetStyle();
        io = ImGui::GetIO(); (void)io;

        Arial = io.Fonts->AddFontFromFileTTF("/Library/Fonts/Arial Bold.ttf");
        NotoMath = io.Fonts->AddFontFromFileTTF("/Library/Fonts/NotoSansMath-Regular.ttf", 25);
        LatexMath = io.Fonts->AddFontFromFileTTF("/Library/Fonts/cmu.serif-bolditalic.ttf");

        style.FontScaleMain = 1;

        style.WindowRounding = 8.0f;
        style.WindowPadding = ImVec2(3.0f, 3.0f);
        style.ChildRounding = 8.0f;
        style.ChildBorderSize = 1;
        style.FrameRounding = 8.0f;
        // style.FramePadding = ImVec2(10.0f, 3.0f);
        style.WindowMinSize.x = 320;

        style.Alpha = 1;
        style.Colors[ImGuiCol_WindowBg] = ImVec4(48.0f/255,44.0f/255,44.0f/255,1);
        style.Colors[ImGuiCol_TitleBgActive] = style.Colors[ImGuiCol_WindowBg];
        style.Colors[ImGuiCol_TitleBg] = style.Colors[ImGuiCol_WindowBg];
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0, 0, 0, 0);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(1, 0, 0, 0.1f);
        style.Colors[ImGuiCol_Button] = ImVec4(0, 0, 0, 0.0f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0, 0, 0, 0.2f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0, 0, 0, 0.5f);
        style.Colors[ImGuiCol_Text] = ImVec4(200.0f/255, 200.0f/255, 200.0/255, 1);
        style.Colors[ImGuiCol_Border] = ImVec4(93.0f/255, 93.0f/255, 93.0f/255, 1);
    }

    void ShutdownUI()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void DrawUI(Plot &plot)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::GetStyle() = style;
        // ImGui::GetIO() = io;

        {
            style.WindowMinSize.y = plot.functions.size() * (120 + 9) + 40;
            ImGui::Begin("main window", NULL, flatWindow);

            ImVec2 size = ImGui::GetWindowSize();
            
            ImGui::Text(" Plots");
            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + size.x - 90);
            if (ImGui::Button("+", ImVec2(25, 20)))
            {
                /*
                    requires an equation at init time or the function seg faults when
                    the later changed. If an equation is added at init then any later 
                    changes including invalid or null strings work as expected.
                */
                plot.functions.emplace_back("", "sin(x0)+cos(x1)-x2");
            }
            
            for (int i = 0; i < plot.functions.size(); i++) 
            {
                DrawFunctionWidget(size.x, plot.functions[i], i);
                ImGui::Dummy(ImVec2(0.0f, 3.0f)); 
            }

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void DrawFunctionWidget(float windowWidth, Function &function, int index)
    {
        ImDrawList *drawList = ImGui::GetWindowDrawList();

        float localVarMin = 0;
        float localVarMax = 0;

        //MAGIC NUMBERS
        //arbitrary choosen for purely visual reasons
        //most constants are further nudged by some small number of pixels
        float boxHeight = 120;
        float nameBoxWidth = 175;
        float localVarsWidth = windowWidth * 0.2f;
        float mainContWidth = windowWidth * 0.8f;
        float localVarsThird = (localVarsWidth - 45)/2;
        
        ImVec2 screenPos = ImVec2(
            ImGui::GetCursorPos().x + ImGui::GetWindowPos().x,
            ImGui::GetCursorPos().y + ImGui::GetWindowPos().y
        );
        AddDropShadow(screenPos, ImVec2(windowWidth - 6, boxHeight), drawList);

        std::string text = "Plots" + std::to_string(index);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, FUNC_WIDGET_COL);
        ImGui::BeginChild(
            text.c_str(), 
            ImVec2(windowWidth - 6, boxHeight), 
            true, 
            childWindow | ImGuiWindowFlags_NoScrollWithMouse
        );
        ImGui::PopStyleColor();
        {
            ImGui::SetNextItemWidth(nameBoxWidth);
            // ImGui::SetCursorPos(ImVec2(pos.x + 3, pos.x));
            ImGui::SetWindowFontScale(0.9);

            text = "##nameInputText" + std::to_string(index);
            ImGui::InputText(text.c_str(), &function.name);
            ImGui::SetWindowFontScale(1);
            
            ImGui::SameLine();

            ImGui::SetCursorPosX(mainContWidth - 70);
            ImGui::PushFont(NotoMath);
            // ImGui::Text("ℝ%d→ℝ%d", function.inDim, function.outDim);
            ImGui::Text("ℝ%d→ℝ%d", 3, 0);
            ImGui::PopFont();

            ImGui::SameLine();

            text = "localVariables" + std::to_string(index);
            ImGui::PushStyleColor(ImGuiCol_ChildBg, LOCAL_VARS_WIDGET_COL);
            ImGui::SetCursorPos(ImVec2(mainContWidth - 5, ImGui::GetCursorPosY() - 3));
            ImGui::BeginChild(
                text.c_str(),
                ImVec2(localVarsWidth, boxHeight), 
                true,
                childWindow
            );
            ImGui::PopStyleColor(1);
            {
                ImGui::PushFont(LatexMath);

                ImGui::SetNextItemWidth(localVarsThird);
                text = "##localVarMin" + std::to_string(index);
                ImGui::InputFloat(text.c_str(), &localVarMin, 0, 0, "%.0f");

                ImGui::SameLine();

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 7);
                ImGui::Text("<x<");

                ImGui::SameLine();

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 7);
                ImGui::SetNextItemWidth(localVarsThird);
                text = "##localVarMax" + std::to_string(index);
                ImGui::InputFloat(text.c_str(), &localVarMax, 0, 0,"%.0f");

                ImGui::PopFont();
            }
            ImGui::EndChild();

            ImGui::SetCursorPos(ImVec2(3, 60));
            text = "##exprTextInput" + std::to_string(index);
            ImGui::InputText(text.c_str(), &function.expr);

            if (ImGui::IsItemDeactivatedAfterEdit())
            {
                function.UpdateFunction();
            }

            drawList->AddRectFilled(
                ImVec2(ImGui::GetWindowPos().x + 10, ImGui::GetWindowPos().y + 25),
                ImVec2(ImGui::GetWindowPos().x + 150, ImGui::GetWindowPos().y + 26),
                ImGui::ColorConvertFloat4ToU32(TEXT_UNDERLINE_COL), 
                8.0f
            );
        }
        ImGui::EndChild();
    }

    void AddDropShadow(ImVec2 pos, ImVec2 size, ImDrawList *drawList)
    {
        drawList->AddRectFilled(
            ImVec2(pos.x, pos.y + 10),
            ImVec2(pos.x + size.x, pos.y + size.y + 5),
            IM_COL32(0,0,0,80), 8
        );
    }
}
