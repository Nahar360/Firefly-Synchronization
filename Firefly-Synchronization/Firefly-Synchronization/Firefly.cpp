#include "Firefly.hpp"

#include "GlobalSettings.hpp"
#include "UiSettings.hpp"

CFirefly::CFirefly(int id) :
	m_id(id),
	m_closestFirefly(-1)
{
	m_font.loadFromFile("./../Firefly-Synchronization/arial.ttf");
}

void CFirefly::Init(float posX, float posY)
{
	// Firefly
	m_firefly.setRadius(FIREFLIES_RADIUS);
	m_firefly.setPointCount(6);

	m_originalColor.r = static_cast<sf::Uint8>(FIREFLIES_COLOR_INPUT[0] * 255.f);
	m_originalColor.g = static_cast<sf::Uint8>(FIREFLIES_COLOR_INPUT[1] * 255.f);
	m_originalColor.b = static_cast<sf::Uint8>(FIREFLIES_COLOR_INPUT[2] * 255.f);
	m_firefly.setFillColor(m_originalColor);

	m_firefly.setOutlineThickness(2.5f);
	sf::Color decoColor = sf::Color(
		static_cast<sf::Uint8>(m_firefly.getFillColor().r * 0.5),
		static_cast<sf::Uint8>(m_firefly.getFillColor().g * 0.5),
		static_cast<sf::Uint8>(m_firefly.getFillColor().b * 0.5)
	);
	m_firefly.setOutlineColor(decoColor);
	if (posX == 0.0f && posY == 0.0f)
	{
		int row = 1;
		int col = m_id;
		while (col > 5)
		{
			col -= 5;
			row++;
		}
		m_firefly.setPosition(250.0f * col, 125.0f * row);
	}
	else
	{
		m_firefly.setPosition(posX, posY);
	}
	m_firefly.setOrigin(m_firefly.getRadius(), m_firefly.getRadius());
	m_firefly.setRotation(90.0f);

	// Center
	m_center.setRadius(5.0f);
	m_center.setFillColor(decoColor);
	m_center.setPosition(m_firefly.getPosition().x, m_firefly.getPosition().y);
	m_center.setOrigin(m_center.getRadius(), m_center.getRadius());

	// Vertices
	for (size_t i = 0; i < m_firefly.getPointCount(); i++)
	{
		sf::CircleShape vertex;
		vertex.setRadius(5.0f);
		vertex.setFillColor(decoColor);
		m_vertices.push_back(vertex);
	}

	SetVertices(m_firefly.getPosition());

	// Influence radius
	m_influenceRadius.setRadius(INFLUENCE_RADIUS);
	m_influenceRadius.setFillColor(sf::Color::Transparent);
	m_influenceRadius.setOutlineThickness(1.5f);
	m_influenceRadius.setOutlineColor(decoColor);
	m_influenceRadius.setPosition(m_firefly.getPosition().x, m_firefly.getPosition().y);
	m_influenceRadius.setOrigin(m_influenceRadius.getRadius(), m_influenceRadius.getRadius());

	// Text
	m_idText.setString(std::to_string(GetId()));
	m_idText.setCharacterSize(24);
	m_idText.setFillColor(sf::Color::Black);
	m_idText.setStyle(sf::Text::Bold);
	m_idText.setPosition(m_firefly.getPosition().x, m_firefly.getPosition().y - 37.5f);
	m_idText.setOrigin(m_center.getRadius(), m_center.getRadius());
}

void CFirefly::SetVertices(sf::Vector2f center)
{
	// TOP_LEFT
	m_vertices[0].setPosition(
		center.x - 1 * (m_firefly.getRadius() / 2),
		center.y - (static_cast<float>(sqrt(3)) * m_firefly.getRadius()) / 2);
	m_vertices[0].setOrigin(m_vertices[0].getRadius(), m_vertices[0].getRadius());

	// TOP_RIGHT
	m_vertices[1].setPosition(
		center.x + 1 * (m_firefly.getRadius() / 2),
		center.y - (static_cast<float>(sqrt(3)) * m_firefly.getRadius()) / 2);
	m_vertices[1].setOrigin(m_vertices[1].getRadius(), m_vertices[1].getRadius());

	// RIGHT
	m_vertices[2].setPosition(
		center.x + m_firefly.getRadius(),
		center.y);
	m_vertices[2].setOrigin(m_vertices[2].getRadius(), m_vertices[2].getRadius());

	// BOTTOM_RIGHT
	m_vertices[3].setPosition(
		center.x + 1 * (m_firefly.getRadius() / 2),
		center.y + 1 * (static_cast<float>(sqrt(3)) * m_firefly.getRadius()) / 2);
	m_vertices[3].setOrigin(m_vertices[3].getRadius(), m_vertices[3].getRadius());

	// BOTTOM_LEFT
	m_vertices[4].setPosition(
		center.x - 1 * (m_firefly.getRadius() / 2),
		center.y + 1 * (static_cast<float>(sqrt(3)) * m_firefly.getRadius()) / 2);
	m_vertices[4].setOrigin(m_vertices[4].getRadius(), m_vertices[4].getRadius());

	// LEFT
	m_vertices[5].setPosition(
		center.x - m_firefly.getRadius(),
		center.y);
	m_vertices[5].setOrigin(m_vertices[5].getRadius(), m_vertices[5].getRadius());
}

void CFirefly::Draw(sf::RenderWindow& window)
{
	window.draw(m_firefly);
	window.draw(m_center);

	if (m_selected)
	{
		for (size_t i = 0; i < m_vertices.size(); i++)
		{
			window.draw(m_vertices[i]);
		}
	}

	if (SHOW_INFLUENCE_RADIUS)
	{
		window.draw(m_influenceRadius);
	}

	m_idText.setFont(m_font);
	window.draw(m_idText);
}

void CFirefly::RunPhaseFunction()
{
	float m_currentTime = m_clock.getElapsedTime().asSeconds();
	m_phase += m_currentTime - m_previousTime;
	m_previousTime = m_currentTime;
}

bool CFirefly::HasBlinked()
{
	if (m_phase > m_blinkingRate)
	{
		// Blink
		m_firefly.setFillColor(sf::Color::Yellow);
		m_firefly.setOutlineColor(sf::Color::Black);
		m_center.setFillColor(sf::Color::Black);

		// Back to original color
		if (m_phase > m_blinkingRate + BLINKING_DURATION)
		{
			m_firefly.setFillColor(m_originalColor);
			sf::Color decoColor = sf::Color(
				static_cast<sf::Uint8>(m_firefly.getFillColor().r * 0.5),
				static_cast<sf::Uint8>(m_firefly.getFillColor().g * 0.5),
				static_cast<sf::Uint8>(m_firefly.getFillColor().b * 0.5)
			);
			m_firefly.setOutlineColor(decoColor);
			m_center.setFillColor(decoColor);

			m_phase = 0.0f;
			// m_clock.restart();

			// TODO:
			// The "return true;" here will unnecessarily sum the BLINKING_DURATION as well
			// that we are not interested about
			return true;
		}
	}

	return false;
}

// Alex's HasBlinked
/*bool CFirefly::HasBlinked()
{
	if (m_phase > m_blinkingRate - BLINKING_DURATION)
	{
		m_firefly.setFillColor(sf::Color::Yellow);
		m_firefly.setOutlineColor(sf::Color::Black);
		m_center.setFillColor(sf::Color::Black);

		if (m_phase > m_blinkingRate)
		{
			m_firefly.setFillColor(m_originalColor);
			sf::Color decoColor = sf::Color(m_firefly.getFillColor().r * 0.5, m_firefly.getFillColor().g * 0.5, m_firefly.getFillColor().b * 0.5);
			m_firefly.setOutlineColor(decoColor);
			m_center.setFillColor(decoColor);

			m_phase = m_blinkingRate - m_phase;
			return false;
		}
		return true;
	}
	return false;
}*/

void CFirefly::UpdatePosition(float x, float y)
{
	// Boundaries detection
	if (x >= 0 + m_firefly.getRadius() && x <= WINDOW_WIDTH - m_firefly.getRadius() &&
		y >= 0 + m_firefly.getRadius() && y <= WINDOW_HEIGHT - m_firefly.getRadius())
	{
		m_firefly.setPosition(x, y);

		m_center.setPosition(x, y);

		SetVertices(sf::Vector2f(x, y));

		m_influenceRadius.setPosition(x, y);

		m_idText.setPosition(x, y - 37.5f);
	}
}

void CFirefly::UpdateRotation()
{
	m_firefly.rotate(0.2f);

	for (size_t i = 0; i < m_vertices.size(); i++)
	{
		m_vertices[i].rotate(0.2f);
	}
}

void CFirefly::UpdateColor(sf::Color color)
{
	m_originalColor = color;

	m_firefly.setFillColor(m_originalColor);

	sf::Color decoColor = sf::Color(
		static_cast<sf::Uint8>(m_firefly.getFillColor().r * 0.5),
		static_cast<sf::Uint8>(m_firefly.getFillColor().g * 0.5),
		static_cast<sf::Uint8>(m_firefly.getFillColor().b * 0.5)
	);
	m_firefly.setOutlineColor(decoColor);
	m_center.setFillColor(decoColor);

	for (size_t i = 0; i < m_vertices.size(); i++)
	{
		m_vertices[i].setFillColor(decoColor);
	}

	m_influenceRadius.setOutlineColor(decoColor);
}

bool CFirefly::MouseDetection(sf::Mouse::Button mouseButton, sf::Vector2i mousePos)
{
	sf::Vector2f mousePosFloat = sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
	if (m_firefly.getGlobalBounds().contains(mousePosFloat))
	{
		switch (mouseButton)
		{
		case sf::Mouse::Button::Left:
		{
			UpdatePosition(mousePosFloat.x, mousePosFloat.y);

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
	return m_originalColor;
}

int CFirefly::GetId() const
{
	return m_id;
}

sf::Vector2f CFirefly::GetPosition() const
{
	return m_firefly.getPosition();
}

float CFirefly::GetInfluenceRadius() const
{
	return m_influenceRadius.getRadius();
}

void CFirefly::SetClosestFirefly(const int closestFirefly)
{
	m_closestFirefly = closestFirefly;
}

int CFirefly::GetClosestFirefly() const
{
	return m_closestFirefly;
}

void CFirefly::SetNeighbours(const std::vector<int>& neighbours)
{
	m_neighbours = neighbours;
}

std::vector<int> CFirefly::GetNeighbours() const
{
	return m_neighbours;
}

void CFirefly::SetSelected(const bool selected)
{
	m_selected = selected;
}

bool CFirefly::GetSelected() const
{
	return m_selected;
}

void CFirefly::SetBlinkingRate(const float blinkingRate)
{
	m_blinkingRate = blinkingRate;

	m_clock.restart();
}

float CFirefly::GetBlinkingRate() const
{
	return m_blinkingRate;
}

float CFirefly::GetElapsedTimeAsSeconds() const
{
	return m_clock.getElapsedTime().asSeconds();
}

void CFirefly::SetUrgeToBlink(const float urgeToBlink)
{
	m_urgeToBlink = urgeToBlink;
}

float CFirefly::GetUrgeToBlink() const
{
	return m_urgeToBlink;
}

void CFirefly::SetPhase(const float phase)
{
	m_phase = phase;
}

float CFirefly::GetPhase() const
{
	return m_phase;
}

void CFirefly::ResetBlinking()
{
	m_clock.restart();
}
