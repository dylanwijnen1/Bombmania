#include "RmlPlugin.h"

using namespace Rml;

namespace Bombmania
{

	class PlaceholderEventListener : public EventListener
	{
	public:

		PlaceholderEventListener()
			: m_pPlaceholder(nullptr)
		{}

		virtual void OnAttach(Element* pElement) override
		{
			Variant* placeholderAttr = pElement->GetAttribute("placeholder");
			ElementPtr uniquePlaceholder = Factory::InstanceElement(pElement, "#text", "#placeholder", XMLAttributes());
			Element* pPlaceholderElement = pElement->AppendChild(std::move(uniquePlaceholder));

			m_pPlaceholder = rmlui_dynamic_cast<ElementText*>(pPlaceholderElement);
			m_pPlaceholder->SetText(placeholderAttr->GetReference<String>());
			m_pPlaceholder->SetProperty("opacity", "0.4");
		}

		virtual void ProcessEvent(Rml::Event& event) override
		{
			String textValue = event.GetParameter<String>("value", "");
			if (textValue.size() > 0)
			{
				m_pPlaceholder->SetProperty(PropertyId::Display, Property("none", Property::Unit::KEYWORD));
			}
			else
			{
				m_pPlaceholder->RemoveProperty(PropertyId::Display);
			}
		}

	private:
		ElementText* m_pPlaceholder;
	};

	void RmlPlugin::OnElementCreate(Element* element)
	{
		if (element->GetTagName() == "input" && element->HasAttribute("placeholder"))
		{
			element->AddEventListener(Rml::EventId::Change, new PlaceholderEventListener());
		}

	}

}
