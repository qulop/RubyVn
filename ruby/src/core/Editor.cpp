#include "Editor.hpp"
#include "Application.hpp"

#include <types/Logger.hpp>
#include <utility/Assert.hpp>
#include <events/EventManager.hpp>

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>


namespace Ruby {
    Editor::Editor() :
        Layer("RubyEngine Editor")
    {}

    void Editor::OnAttach() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsClassic();

        auto& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        io.Fonts->AddFontFromFileTTF("resources/fonts/Ubuntu/Ubuntu-Bold.ttf", 15.0f);

        auto window = Application::GetInstance().GetWindow();
        RUBY_ASSERT(window->GetVendor() == VENDOR_GLFW, "Unknown window vendor!");

        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->GetNativeWindowPtr(), true);
        ImGui_ImplOpenGL3_Init("#version 330 core");

        addEventListener(RUBY_ANY_EVENT, &Editor::OnEvent, this);
    }

    void Editor::OnDetach() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Editor::OnEvent(IEvent* event) {

    }

    void Editor::Update() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static bool isShow = true;
        ImGui::ShowDemoWindow(&isShow);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGui::EndFrame();
    }
}