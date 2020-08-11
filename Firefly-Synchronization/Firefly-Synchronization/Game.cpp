#include "Game.hpp"

#include "GlobalSettings.hpp"

CGame::CGame() :
	m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE)
{
}

void CGame::Run()
{
	m_uiManager.Init(m_window);

	while (m_window.isOpen())
	{
		CheckEvents();

		CheckInput();

		m_uiManager.Run(m_window, m_network);

		m_window.clear(BACKGROUND_COLOR);

		m_network.Update(m_window);

		m_uiManager.Render(m_window);

		m_window.display();
	}

	m_uiManager.Shutdown();
}

void CGame::CheckEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		m_uiManager.ProcessEvent(event);

		if (event.type == sf::Event::Closed ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			m_window.close();
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
