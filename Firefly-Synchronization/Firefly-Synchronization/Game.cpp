#include "Game.hpp"

#include "GlobalSettings.hpp"

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

CGame::CGame() :
	m_windowTitle("Firefly Synchronization"),
	m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Firefly Synchronization")
{
}

void CGame::Run()
{
	sf::Clock deltaClock;

	ImGui::SFML::Init(m_window);

	while (m_window.isOpen())
	{
		CheckEvents();

		CheckInput();

		HandleUi(deltaClock);

		m_window.display();
	}

	ImGui::SFML::Shutdown();
}

void CGame::CheckEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);

		if (event.type == sf::Event::Closed ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			m_window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			m_network.Test();
		}
	}
}

void CGame::CheckInput()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		m_network.MouseDetection(sf::Mouse::Button::Left, sf::Mouse::getPosition(m_window));
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		m_network.MouseDetection(sf::Mouse::Button::Right, sf::Mouse::getPosition(m_window));
	}
}

void CGame::HandleUi(sf::Clock deltaClock)
{
	ImGui::SFML::Update(m_window, deltaClock.restart());

	ImGui::Begin("Menu");

	UiWidgets();

	ImGui::End();

	m_window.clear(m_backgroundColor);

	m_network.Update(m_window);

	ImGui::SFML::Render(m_window);
}

void CGame::UiWidgets()
{
	UpdateWindowTitle();

	UpdateMousePosition();

	ImGui::Separator();

	InitialiseNetwork();

	ImGui::Separator();

	UpdateBackgroundColor();

	UpdateFirefliesColor();

	UpdateShowLines();
	ImGui::SameLine();
	UpdateShowVertices();

	ImGui::Separator();

	UpdateBlinkingRate();

	UpdateBlinkingDuration();

	ResetBlinkingClock();

	ImGui::Separator();

	CreateFirefly();
	ImGui::SameLine();
	DeleteFirefly();

	ListFireflies();
}

void CGame::UpdateWindowTitle()
{
	if (ImGui::InputText("Window title", m_windowTitle, 255))
	{
		m_window.setTitle(m_windowTitle);
	}
}

void CGame::UpdateMousePosition()
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
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

void CGame::InitialiseNetwork()
{
	ImGui::InputInt("No. of fireflies", &m_numFireflies);

	if (ImGui::Button("Initialise network"))
	{
		m_network.Init(m_numFireflies);
	}
}

void CGame::UpdateBackgroundColor()
{
	if (ImGui::ColorEdit3("Background color", m_backgroundColorInput))
	{
		m_backgroundColor.r = static_cast<sf::Uint8>(m_backgroundColorInput[0] * 255.f);
		m_backgroundColor.g = static_cast<sf::Uint8>(m_backgroundColorInput[1] * 255.f);
		m_backgroundColor.b = static_cast<sf::Uint8>(m_backgroundColorInput[2] * 255.f);
	}
}

void CGame::UpdateFirefliesColor()
{
	if (ImGui::ColorEdit3("Fireflies color", m_firefliesColorInput))
	{
		sf::Color firefliesColor;
		firefliesColor.r = static_cast<sf::Uint8>(m_firefliesColorInput[0] * 255.f);
		firefliesColor.g = static_cast<sf::Uint8>(m_firefliesColorInput[1] * 255.f);
		firefliesColor.b = static_cast<sf::Uint8>(m_firefliesColorInput[2] * 255.f);

		m_network.UpdateFirefliesColor(firefliesColor);
	}
}

void CGame::UpdateShowLines()
{
	if (ImGui::Checkbox("Show Lines", &m_showLines))
	{
		m_network.ShowLines(m_showLines);
	}
}

void CGame::UpdateShowVertices()
{
	if (ImGui::Checkbox("Show Vertices", &m_showVertices))
	{
		m_network.ShowVertices(m_showVertices);
	}
}

void CGame::UpdateBlinkingRate()
{
	if (ImGui::InputFloat("Blinking rate (s)", &m_blinkingRate, 0.005f, 0.005f, "%.2f"))
	{
		m_network.UpdateFirefliesBlinkingRate(m_blinkingRate);
	}
}

void CGame::UpdateBlinkingDuration()
{
	if (ImGui::InputFloat("Blinking duration (s)", &m_blinkingDuration, 0.001f, 0.001f, "%.2f"))
	{
		m_network.UpdateFirefliesBlinkingDuration(m_blinkingDuration);
	}
}

void CGame::ResetBlinkingClock()
{
	if (ImGui::Button("Reset blinking"))
	{
		m_network.ResetBlinkingClock();
	}
}

void CGame::CreateFirefly()
{
	if (ImGui::Button("Create firefly"))
	{
		m_network.CreateFirefly();
	}
}

void CGame::DeleteFirefly()
{
	if (ImGui::Button("Delete firefly"))
	{
		m_network.DeleteFirefly();
	}
}

void CGame::ListFireflies()
{
	auto fireflies = m_network.GetFireflies();

	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Fireflies:");
	ImGui::BeginChild("Scrolling");
	for (int i = 0; i < fireflies.size(); i++)
	{
		if (fireflies[i].GetSelected())
		{
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "[%d] - Position: (%.0f, %.0f)", fireflies[i].GetId(), fireflies[i].GetPosition().x, fireflies[i].GetPosition().y);
		}
		else
		{
			ImGui::Text("[%d] - Position: (%.0f, %.0f)", fireflies[i].GetId(), fireflies[i].GetPosition().x, fireflies[i].GetPosition().y);
		}
	}
	ImGui::EndChild();
}