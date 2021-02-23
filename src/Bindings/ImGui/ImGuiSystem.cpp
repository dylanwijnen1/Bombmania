#include "ImGuiSystem.h"

#include "imgui-SFML.h"
#include <imgui.h>

#include "../../Game/Game.h"

namespace Bombmania
{

    bool ImGuiSystem::Init()
    {
        Game* pGame = GetGame();

        m_pWindow = &pGame->GetWindow();
        m_pRenderTarget = &pGame->GetRenderTarget();

        return true;
    }

    void ImGuiSystem::PostInit()
    {
        ImGui::SFML::Init(*m_pWindow, *m_pRenderTarget);
    }

    void ImGuiSystem::Destroy()
    {
        ImGui::SFML::Shutdown();
    }

    void ImGuiSystem::BeginFrame(sf::RenderTarget& target, const Time& time)
    {
        sf::Time sfTime = sf::seconds(static_cast<float>(time.ElapsedSeconds()));
        ImGui::SFML::Update(*m_pWindow, *m_pRenderTarget, sfTime);

        auto context = ImGui::GetCurrentContext();
    }

    void ImGuiSystem::EndFrame(sf::RenderTarget& target, const Time& time)
    {
        ImGui::SFML::Render(target);
    }

    bool ImGuiSystem::HandleEvent(const sf::Event& ev)
    {
        // Always process window events.
        ImGui::SFML::ProcessEvent(ev);

        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureKeyboard)
            return true;

        if (io.WantCaptureMouse)
            return true;

        return false;
    }

}
