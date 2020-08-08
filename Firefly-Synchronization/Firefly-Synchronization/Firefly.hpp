#include <SFML/Graphics.hpp>

#pragma once

class CFirefly
{
public:
	CFirefly(int id);
	~CFirefly() = default;

	void Init(float posX = 0.0f, float posY = 0.0f);
	void SetVertices(sf::Vector2f center);
	void Update(sf::RenderWindow& window);
	void UpdatePosition(float x, float y);
	void UpdateRotation();
	void UpdateColor(sf::Color color);
	bool MouseDetection(sf::Mouse::Button mouseButton, sf::Vector2i mousePos);

	void BlinkEffect();

	sf::CircleShape GetFirefly() const;

	sf::Color GetColor() const;

	int GetId() const;

	sf::Vector2f GetPosition() const;

	void SetClosestFirefly(const int& closestFirefly);
	int GetClosestFirefly() const;

	void SetSelected(const bool& selected);
	bool GetSelected() const;

	void SetBlinkingRate(const float& blinkingRate);
	void SetBlinkingDuration(const float& blinkingDuration);
	void ResetBlinking();

	void SetShowVertices(const bool& show);

private:
	sf::CircleShape m_firefly;
	sf::CircleShape m_center;
	std::vector<sf::CircleShape> m_vertices;

	int m_id;

	sf::Font m_font;
	sf::Text m_idText;

	sf::Color m_originalColor;
	float m_blinkingRate = 2.0f;
	float m_blinkingDuration = 0.1f;

	int m_closestFirefly = -1;

	bool m_showVertices = false;

	bool m_selected = false;

	sf::Clock m_clock;
};
