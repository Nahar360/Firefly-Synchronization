#include <SFML/Graphics.hpp>

#include "Network.hpp"

#pragma once

class CUiManager
{
public:
	CUiManager() = default;
	~CUiManager() = default;

	void Init(sf::RenderWindow& window);
	void Shutdown();

	void ProcessEvent(sf::Event event);

	void Run(sf::RenderWindow& window, CNetwork& network);
	void Render(sf::RenderWindow& window);

	sf::Color GetBackgroundColor() const;

private:
	sf::Clock m_deltaClock;

	int m_numFireflies = 5;

	char m_windowTitle[255] = "Firefly Synchronization";

	sf::Color m_backgroundColor = { 105, 105, 105 };
	float m_backgroundColorInput[3] = { 0.41f, 0.41f, 0.41f };

	float m_firefliesColorInput[3] = { 1.f, 0.f, 0.f };

	bool m_showLines = false;
	bool m_showVertices = false;

	float m_blinkingRate = 2.0f;
	float m_blinkingDuration = 0.1f;

	void Update(sf::RenderWindow& window);
	void Begin();
	void HandleUi(sf::RenderWindow& window, CNetwork& network);
	void End();

	void UpdateWindowTitle(sf::RenderWindow& window);
	void UpdateMousePosition(sf::RenderWindow& window);
	void InitialiseNetwork(CNetwork& network);
	void UpdateBackgroundColor();
	void UpdateFirefliesColor(CNetwork& network);
	void UpdateShowLines(CNetwork& network);
	void UpdateShowVertices(CNetwork& network);
	void UpdateBlinkingRate(CNetwork& network);
	void UpdateBlinkingDuration(CNetwork& network);
	void ResetBlinkingClock(CNetwork& network);
	void CreateFirefly(CNetwork& network);
	void DeleteFirefly(CNetwork& network);
	void ListFireflies(CNetwork& network);
};
