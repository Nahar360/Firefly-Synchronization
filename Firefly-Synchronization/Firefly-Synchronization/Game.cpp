#include "Game.hpp"

#include "GlobalSettings.hpp"

CGame::CGame() :
	m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Firefly Synchronization")
{
}

void CGame::Init()
{
    m_network.Init();
}

void CGame::Update()
{
    while (m_window.isOpen())
    {
        CheckEvents();

        CheckInput();

        m_window.clear(sf::Color(105, 105, 105));

        m_network.Update(m_window);

        m_window.display();
    }
}

void CGame::CheckEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
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