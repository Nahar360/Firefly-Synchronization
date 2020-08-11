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

private:
	sf::Clock m_deltaClock;

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
	void UpdateShowInfluenceRadius(CNetwork& network);
	void UpdateBlinkingDuration(CNetwork& network);
	void ResetBlinkingClock(CNetwork& network);
	void CreateFirefly(CNetwork& network);
	void DeleteFirefly(CNetwork& network);
	void ClearFireflies(CNetwork& network);
	void ListFireflies(CNetwork& network);
};
