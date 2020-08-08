#include <SFML/Graphics.hpp>

#include "UiManager.hpp"
#include "Network.hpp"

#pragma once

class CGame
{
public:
	CGame();
	~CGame() = default;

	void Run();

private:
	sf::RenderWindow m_window;

	CUiManager m_uiManager;
	CNetwork m_network;

	void CheckEvents();
	void CheckInput();
};

