#include "Firefly.hpp"

#include "GlobalSettings.hpp"

CFirefly::CFirefly(int id) :
	m_id(id),
	m_closestFirefly(-1)
{
	m_font.loadFromFile("E:/Projects/Firefly-Synchronization/Firefly-Synchronization/Firefly-Synchronization/arial.ttf");
}

void CFirefly::Init()
{
	// Firefly
	m_firefly.setRadius(50.0f);
	m_firefly.setPointCount(6);
	m_firefly.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
	m_firefly.setOutlineThickness(2.5f);
	sf::Color decoColor = sf::Color(m_firefly.getFillColor().r * 0.5, m_firefly.getFillColor().g * 0.5, m_firefly.getFillColor().b * 0.5);
	m_firefly.setOutlineColor(decoColor);
	m_firefly.setPosition(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT);
	m_firefly.setOrigin(m_firefly.getRadius(), m_firefly.getRadius());
	m_firefly.setRotation(90.0f);

	// Center
	m_center.setRadius(5.0f);
	m_center.setFillColor(decoColor);
	m_center.setPosition(m_firefly.getPosition().x, m_firefly.getPosition().y);
	m_center.setOrigin(m_center.getRadius(), m_center.getRadius());

	// Vertices
	if (m_debugShowVertices)
	{
		for (int i = 0; i < m_firefly.getPointCount(); i++)
		{
			sf::CircleShape vertex;
			vertex.setRadius(5.0f);
			vertex.setFillColor(sf::Color::Red);
			m_vertices.push_back(vertex);
		}

		SetVertices(m_firefly.getPosition());
	}

	m_idText.setString(std::to_string(GetId()));
	m_idText.setCharacterSize(24);
	m_idText.setFillColor(sf::Color::White);
	m_idText.setStyle(sf::Text::Bold);
	m_idText.setPosition(m_firefly.getPosition().x, m_firefly.getPosition().y - 37.5f);
	m_idText.setOrigin(m_center.getRadius(), m_center.getRadius());
}

void CFirefly::SetVertices(sf::Vector2f center)
{
	// TOP_LEFT
	m_vertices[0].setPosition(
		center.x - 1 * (m_firefly.getRadius() / 2),
		center.y - (sqrt(3) * m_firefly.getRadius()) / 2);
	m_vertices[0].setOrigin(m_vertices[0].getRadius(), m_vertices[0].getRadius());

	// TOP_RIGHT
	m_vertices[1].setPosition(
		center.x + 1 * (m_firefly.getRadius() / 2),
		center.y - (sqrt(3) * m_firefly.getRadius()) / 2);
	m_vertices[1].setOrigin(m_vertices[1].getRadius(), m_vertices[1].getRadius());

	// RIGHT
	m_vertices[2].setPosition(
		center.x + m_firefly.getRadius(),
		center.y);
	m_vertices[2].setOrigin(m_vertices[2].getRadius(), m_vertices[2].getRadius());

	// BOTTOM_RIGHT
	m_vertices[3].setPosition(
		center.x + 1 * (m_firefly.getRadius() / 2),
		center.y + 1 * (sqrt(3) * m_firefly.getRadius()) / 2);
	m_vertices[3].setOrigin(m_vertices[3].getRadius(), m_vertices[3].getRadius());

	// BOTTOM_LEFT
	m_vertices[4].setPosition(
		center.x - 1 * (m_firefly.getRadius() / 2),
		center.y + 1 * (sqrt(3) * m_firefly.getRadius()) / 2);
	m_vertices[4].setOrigin(m_vertices[4].getRadius(), m_vertices[4].getRadius());

	// LEFT
	m_vertices[5].setPosition(
		center.x - m_firefly.getRadius(),
		center.y);
	m_vertices[5].setOrigin(m_vertices[5].getRadius(), m_vertices[5].getRadius());
}

void CFirefly::Update(sf::RenderWindow& window)
{
	window.draw(m_firefly);
	window.draw(m_center);

	if (m_debugShowVertices)
	{
		for (int i = 0; i < m_vertices.size(); i++)
		{
			window.draw(m_vertices[i]);
		}
	}

	m_idText.setFont(m_font);
	window.draw(m_idText);

	float time = m_clock.getElapsedTime().asSeconds();
	if (time > 2.0f)
	{
		m_firefly.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
		sf::Color decoColor = sf::Color(m_firefly.getFillColor().r * 0.5, m_firefly.getFillColor().g * 0.5, m_firefly.getFillColor().b * 0.5);
		m_firefly.setOutlineColor(decoColor);
		m_center.setFillColor(decoColor);

		m_clock.restart();
	}
}

void CFirefly::UpdatePosition(float x, float y)
{
	m_firefly.setPosition(x, y);

	m_center.setPosition(x, y);

	if (m_debugShowVertices)
	{
		SetVertices(sf::Vector2f(x, y));
	}

	m_idText.setPosition(x, y - 37.5f);
}

void CFirefly::UpdateRotation()
{
	m_firefly.setRotation(m_firefly.getRotation() + 0.1f);

	if (m_debugShowVertices)
	{
		for (int i = 0; i < m_vertices.size(); i++)
		{
			m_vertices[i].setRotation(m_vertices[i].getRotation() + 0.1f);
		}
	}
}

bool CFirefly::MouseDetection(sf::Mouse::Button mouseButton, sf::Vector2i mousePos)
{
	if (m_firefly.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y)))
	{
		switch (mouseButton)
		{
		case sf::Mouse::Button::Left:
		{
			UpdatePosition(mousePos.x, mousePos.y);

			return true;
		}
		case sf::Mouse::Button::Right:
		{
			UpdateRotation();

			return true;
		}
		default:
			break;
		}
	}

	return false;
}

sf::CircleShape CFirefly::GetFirefly() const
{
	return m_firefly;
}

sf::Color CFirefly::GetColor() const
{
	return m_firefly.getFillColor();
}

int CFirefly::GetId() const
{
	return m_id;
}

sf::Vector2f CFirefly::GetPosition() const
{
	return m_firefly.getPosition();
}

void CFirefly::SetClosestFirefly(const int& closestFirefly)
{
	m_closestFirefly = closestFirefly;
}

int CFirefly::GetClosestFirefly() const
{
	return m_closestFirefly;
}