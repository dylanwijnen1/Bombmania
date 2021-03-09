#include "SceneCheckers.h"

#include "../Game.h"
#include "../../Bindings/RmlUi/RmlUiSystem.h"

#include <RmlUi/Core.h>

namespace Bombmania
{

	struct TileDragListener : Rml::EventListener
	{
		virtual void ProcessEvent(Rml::Event& event)
		{
			Rml::Element* drag_element = static_cast<Rml::Element*>(event.GetParameter< void* >("drag_element", nullptr));
			Rml::Element* dest_container = event.GetTargetElement();

			switch (event.GetId())
			{
				case Rml::EventId::Dragover:
					dest_container->SetClass("drag-over", true);
					break;
				case Rml::EventId::Dragout:
					dest_container->SetClass("drag-over", false);
					break;
				case Rml::EventId::Dragdrop:
					Rml::ElementPtr element = drag_element->GetParentNode()->RemoveChild(drag_element);
					dest_container->AppendChild(std::move(element));
					dest_container->SetClass("drag-over", false);
					break;
			}
		}
	} static s_listener;

	struct CheckersModel
	{
		Rml::String ipAddress;
	} static s_model;

	void SceneCheckers::OnSceneEntered(Game& game)
	{
		Rml::Context* pContext = game.GetSystem<RmlUiSystem>()->GetContext();

		auto pStartModel = pContext->CreateDataModel("start");
		pStartModel.Bind("ipAddress", &s_model.ipAddress);
		pStartModel.BindEventCallback("join", [this](Rml::DataModelHandle handle, Rml::Event& event, const Rml::VariantList& arguments)
		{
			Rml::String port = s_model.ipAddress.substr(s_model.ipAddress.find_first_of(':') + 1);
			int val = Rml::FromString<int>(port, 1234);
		});

		pStartModel.BindEventCallback("host", [this](Rml::DataModelHandle handle, Rml::Event& event, const Rml::VariantList& arguments)
		{
			int val = Rml::FromString<int>(s_model.ipAddress, 1234);
		});

		bool result = Rml::LoadFontFace("Assets/Fonts/space_mono_regular.ttf", true);

		auto pStartDoc = pContext->LoadDocument("Assets/Chess/Start.rml");
		pStartDoc->Show();

		auto pGameDoc = pContext->LoadDocument("Assets/Chess/Game.rml");
		//pGameDoc->Show();

		auto pBoardElement = pGameDoc->GetElementById("board");

		// Setup Board
		bool white = true;
		for (int y = 0; y < 8; ++y)
		{
			for (int x = 0; x < 8; ++x)
			{
				bool isWhite = (x + y) % 2 == 0;

				auto pTileElement = pGameDoc->CreateElement("tile");
				pTileElement->SetClass(isWhite ? "white" : "black", true);
				pTileElement->AddEventListener(Rml::EventId::Dragover, &s_listener);
				pTileElement->AddEventListener(Rml::EventId::Dragout, &s_listener);
				pTileElement->AddEventListener(Rml::EventId::Dragdrop, &s_listener);

				if (!isWhite && (y <= 2 || y >= 5))
				{
					bool playerOne = y <= 2;

					auto pPieceElement = pGameDoc->CreateElement("piece");
					pPieceElement->SetClass(playerOne ? "player1" : "player2", true);
					pTileElement->AppendChild(std::move(pPieceElement));
				}

				pBoardElement->AppendChild(std::move(pTileElement));
			}
		}
	}

	void SceneCheckers::Update(const Time& time)
	{
	}

}
