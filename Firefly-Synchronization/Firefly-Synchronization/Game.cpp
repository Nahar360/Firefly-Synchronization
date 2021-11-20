#include "Game.hpp"

#include "GlobalSettings.hpp"

CGame::CGame() :
	m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE)
{
	srand(time(NULL));
}

void CGame::Run()
{
	m_uiManager.Init(m_window);

	sf::Clock clock = sf::Clock::Clock();
	sf::Time previousTime = clock.getElapsedTime();
	sf::Time currentTime;

	while (m_window.isOpen())
	{
		currentTime = clock.getElapsedTime();
		float fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
		previousTime = currentTime;

		CheckEvents();

		m_uiManager.Run(m_window, m_network, fps);

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

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		m_mouseHasBeenPressed = true;
		m_network.MouseDetection(sf::Mouse::Button::Left, sf::Mouse::getPosition(m_window));
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		m_mouseHasBeenPressed = true;
		m_network.MouseDetection(sf::Mouse::Button::Right, sf::Mouse::getPosition(m_window));
	}

	if (m_mouseHasBeenPressed && event.type == sf::Event::MouseButtonReleased)
	{
		m_mouseHasBeenPressed = false;
		m_network.UnselectAllFireflies();
	}
}
