#include "RmlUiSystem.h"

#include "RmlSFMLSystemInterface.h"
#include "RmlSFMLRenderer.h"
#include "RmlPlugin.h"

#include "../../Game/Game.h"

#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>

namespace Bombmania
{

    bool RmlUiSystem::Init()
    {
        Game* pGame = GetGame();

        RMLUI_ASSERT(pGame);

        sf::RenderWindow& window = pGame->GetRenderWindow();

        m_pRendererInterface = new RmlSFMLRenderer();
        m_pRendererInterface->SetWindow(&window);

        m_pSystemInterface = new RmlSFMLSystemInterface(&window);

        Rml::SetRenderInterface(m_pRendererInterface);
        Rml::SetSystemInterface(m_pSystemInterface);

        Rml::RegisterPlugin(new RmlPlugin());

        if (!Rml::Initialise())
            return false;

        // Create a context.
        m_pMainContext = Rml::CreateContext(kMainContextName,
            Rml::Vector2i(window.getSize().x, window.getSize().y));

        if (!Rml::Debugger::Initialise(m_pMainContext))
            return false;

        return true;
    }

    void RmlUiSystem::Destroy()
    {
        Rml::Shutdown();
    }

    void RmlUiSystem::Update(const Time& time)
    {
        m_pMainContext->Update();
    }

    bool RmlUiSystem::HandleEvent(const sf::Event& ev)
    {
        switch (ev.type)
        {
        case sf::Event::MouseMoved:
            return !m_pMainContext->ProcessMouseMove(ev.mouseMove.x, ev.mouseMove.y,
                m_pSystemInterface->GetKeyModifiers());
        case sf::Event::MouseButtonPressed:
            return !m_pMainContext->ProcessMouseButtonDown(ev.mouseButton.button,
                m_pSystemInterface->GetKeyModifiers());
        case sf::Event::MouseButtonReleased:
            return !m_pMainContext->ProcessMouseButtonUp(ev.mouseButton.button,
                m_pSystemInterface->GetKeyModifiers());
        case sf::Event::MouseWheelMoved:
            return !m_pMainContext->ProcessMouseWheel(float(-ev.mouseWheel.delta),
                m_pSystemInterface->GetKeyModifiers());
        case sf::Event::TextEntered:
            if (ev.text.unicode > 32)
                return !m_pMainContext->ProcessTextInput(Rml::Character(ev.text.unicode));
            break;
        case sf::Event::KeyPressed:
            return !m_pMainContext->ProcessKeyDown(m_pSystemInterface->TranslateKey(ev.key.code),
                m_pSystemInterface->GetKeyModifiers());
        case sf::Event::KeyReleased:
            if (ev.key.code == sf::Keyboard::F8)
            {
                Rml::Debugger::SetVisible(!Rml::Debugger::IsVisible());
            };

            return !m_pMainContext->ProcessKeyUp(m_pSystemInterface->TranslateKey(ev.key.code),
                m_pSystemInterface->GetKeyModifiers());

            break;
        };

        return false;
    }

    void RmlUiSystem::Draw(sf::RenderTarget& renderTarget, const Time& time)
    {
        m_pMainContext->Render();
    }

}
