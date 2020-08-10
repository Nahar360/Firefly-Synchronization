#include "UiManager.hpp"

#include "GlobalSettings.hpp"

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

void CUiManager::Init(sf::RenderWindow& window)
{
	ImGui::SFML::Init(window);
}

void CUiManager::Shutdown()
{
	ImGui::SFML::Shutdown();
}

void CUiManager::ProcessEvent(sf::Event event)
{
	ImGui::SFML::ProcessEvent(event);
}

void CUiManager::Update(sf::RenderWindow& window)
{
	ImGui::SFML::Update(window, m_deltaClock.restart());
}

void CUiManager::Begin()
{
	ImGui::Begin("Menu");
}

void CUiManager::Run(sf::RenderWindow& window, CNetwork& network)
{
	Update(window);

	Begin();

	HandleUi(window, network);

	End();
}

void CUiManager::HandleUi(sf::RenderWindow& window, CNetwork& network)
{
	UpdateWindowTitle(window);

	UpdateMousePosition(window);

	// -------------------------
	ImGui::Separator();
	// -------------------------

	InitialiseNetwork(network);

	// -------------------------
	ImGui::Separator();
	// -------------------------

	UpdateBackgroundColor();

	UpdateFirefliesColor(network);

	UpdateShowLines(network);
	
	UpdateShowInfluenceRadius(network);
	ImGui::SameLine();
	UpdateShowVertices(network);

	// -------------------------
	ImGui::Separator();
	// -------------------------

	UpdateBlinkingDuration(network);

	ResetBlinkingClock(network);

	// -------------------------
	ImGui::Separator();
	// -------------------------

	CreateFirefly(network);
	ImGui::SameLine();
	DeleteFirefly(network);
	ImGui::SameLine();
	ClearFireflies(network);

	ListFireflies(network);
}

void CUiManager::End()
{
	ImGui::End();
}

void CUiManager::Render(sf::RenderWindow& window)
{
	ImGui::SFML::Render(window);
}

sf::Color CUiManager::GetBackgroundColor() const
{
	return m_backgroundColor;
}

void CUiManager::UpdateWindowTitle(sf::RenderWindow& window)
{
	if (ImGui::InputText("Window title", m_windowTitle, 255))
	{
		window.setTitle(m_windowTitle);
	}
}

void CUiManager::UpdateMousePosition(sf::RenderWindow& window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	if (mousePos.x >= 0 && mousePos.x <= WINDOW_WIDTH &&
		mousePos.y >= 0 && mousePos.y <= WINDOW_HEIGHT)
	{
		ImGui::Text("Mouse position: (%d, %d)", mousePos.x, mousePos.y);
	}
	else
	{
		ImGui::Text("Mouse out of window");
	}
}

void CUiManager::InitialiseNetwork(CNetwork& network)
{
	ImGui::InputInt("No. of fireflies", &m_numFireflies);

	ImGui::InputFloat("Fireflies radius", &m_firefliesRadius, 0.001f, 0.001f, "%.2f");

	ImGui::InputFloat("Influence radius", &m_influenceRadius, 0.001f, 0.001f, "%.2f");

	if (ImGui::Button("Initialise network"))
	{
		network.Init(m_numFireflies, m_firefliesRadius, m_influenceRadius);
	}
}

void CUiManager::UpdateBackgroundColor()
{
	if (ImGui::ColorEdit3("Background color", m_backgroundColorInput))
	{
		m_backgroundColor.r = static_cast<sf::Uint8>(m_backgroundColorInput[0] * 255.f);
		m_backgroundColor.g = static_cast<sf::Uint8>(m_backgroundColorInput[1] * 255.f);
		m_backgroundColor.b = static_cast<sf::Uint8>(m_backgroundColorInput[2] * 255.f);
	}
}

void CUiManager::UpdateFirefliesColor(CNetwork& network)
{
	if (ImGui::ColorEdit3("Fireflies color", m_firefliesColorInput))
	{
		sf::Color firefliesColor;
		firefliesColor.r = static_cast<sf::Uint8>(m_firefliesColorInput[0] * 255.f);
		firefliesColor.g = static_cast<sf::Uint8>(m_firefliesColorInput[1] * 255.f);
		firefliesColor.b = static_cast<sf::Uint8>(m_firefliesColorInput[2] * 255.f);

		network.UpdateFirefliesColor(firefliesColor);
	}
}

void CUiManager::UpdateShowLines(CNetwork& network)
{
	const char* showLinesOptions[] = { "None", "All", "Only neighbours" };
	static int showLinesOptionSelected = 0;
	
	if (ImGui::Combo("Show Lines", &showLinesOptionSelected, showLinesOptions, IM_ARRAYSIZE(showLinesOptions)))
	{
		network.SetShowLinesOption(showLinesOptionSelected);
	}
}

void CUiManager::UpdateShowInfluenceRadius(CNetwork& network)
{
	if (ImGui::Checkbox("Show Influence Radius", &m_showInfluenceRadius))
	{
		network.ShowInfluenceRadius(m_showInfluenceRadius);
	}
}

void CUiManager::UpdateShowVertices(CNetwork& network)
{
	if (ImGui::Checkbox("Show Vertices", &m_showVertices))
	{
		network.ShowVertices(m_showVertices);
	}
}

void CUiManager::UpdateBlinkingDuration(CNetwork& network)
{
	if (ImGui::InputFloat("Blinking duration (s)", &m_blinkingDuration, 0.001f, 0.001f, "%.2f"))
	{
		network.UpdateFirefliesBlinkingDuration(m_blinkingDuration);
	}
}

void CUiManager::ResetBlinkingClock(CNetwork& network)
{
	if (ImGui::Button("Reset blinking"))
	{
		network.ResetBlinkingClock();
	}
}

void CUiManager::CreateFirefly(CNetwork& network)
{
	if (ImGui::Button("Create firefly"))
	{
		network.CreateFirefly();
	}
}

void CUiManager::DeleteFirefly(CNetwork& network)
{
	if (ImGui::Button("Delete firefly"))
	{
		network.DeleteFirefly();
	}
}

void CUiManager::ClearFireflies(CNetwork& network)
{
	if (ImGui::Button("Clear fireflies"))
	{
		network.ClearFireflies();
	}
}

void CUiManager::ListFireflies(CNetwork& network)
{
	auto fireflies = network.GetFireflies();

	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Fireflies:");
	ImGui::BeginChild("Scrolling");
	for (int i = 0; i < fireflies.size(); i++)
	{
		if (fireflies[i].GetSelected())
		{
			ImGui::TextColored(ImVec4(0, 1, 0, 1),
				"---> [%d] - Blink. rate: %0.1f s. Closest: [%d].",
				fireflies[i].GetId(), fireflies[i].GetBlinkingRate(), fireflies[i].GetClosestFirefly());

			ImGui::SameLine();

			ImGui::TextColored(ImVec4(0, 1, 0, 1),"Neighbours: [");
			ImGui::SameLine();
			std::vector<int> neighbours = fireflies[i].GetNeighbours();
			for (int j = 0; j < neighbours.size(); j++)
			{
				if (j != neighbours.size() - 1)
				{
					ImGui::TextColored(ImVec4(0, 1, 0, 1), "%d ", neighbours[j]);
				}
				else
				{
					ImGui::TextColored(ImVec4(0, 1, 0, 1), "%d", neighbours[j]);
				}

				ImGui::SameLine();
			}
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "]");
		}
		else
		{
			ImGui::Text("[%d] - Blink. rate: %0.1f s. Closest: [%d].",
				fireflies[i].GetId(), fireflies[i].GetBlinkingRate(), fireflies[i].GetClosestFirefly());

			ImGui::SameLine();

			ImGui::Text("Neighbours: [");
			ImGui::SameLine();
			std::vector<int> neighbours = fireflies[i].GetNeighbours();
			for (int j = 0; j < neighbours.size(); j++)
			{
				if (j != neighbours.size() - 1)
				{
					ImGui::Text("%d ", neighbours[j]);
				}
				else
				{
					ImGui::Text("%d", neighbours[j]);
				}
				
				ImGui::SameLine();
			}
			ImGui::Text("]");
		}
	}
	ImGui::EndChild();
}
