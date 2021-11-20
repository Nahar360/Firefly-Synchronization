#include <SFML/Graphics.hpp>

#pragma once

class CFirefly
{
public:
	CFirefly(int id);
	~CFirefly() = default;

	void Init(float posX = 0.0f, float posY = 0.0f);
	void SetVertices(sf::Vector2f center);
	void Draw(sf::RenderWindow& window);
	void RunPhaseFunction();
	bool HasBlinked();
	void UpdatePosition(float x, float y);
	void UpdateRotation();
	void UpdateColor(sf::Color color);
	bool MouseDetection(sf::Mouse::Button mouseButton, sf::Vector2i mousePos);

	sf::CircleShape GetFirefly() const;

	sf::Color GetColor() const;

	int GetId() const;

	sf::Vector2f GetPosition() const;

	float GetInfluenceRadius() const;

	void SetClosestFirefly(const int closestFirefly);
	int GetClosestFirefly() const;

	void SetNeighbours(const std::vector<int>& neighbours);
	std::vector<int> GetNeighbours() const;

	void SetSelected(const bool selected);
	bool GetSelected() const;

	void SetBlinkingRate(const float blinkingRate);
	float GetBlinkingRate() const;

	float GetElapsedTimeAsSeconds() const;

	void SetUrgeToBlink(const float urgeToBlink);
	float GetUrgeToBlink() const;

	void SetPhase(const float phase);
	float GetPhase() const;

	void ResetBlinking();

private:
	sf::CircleShape m_firefly;
	sf::CircleShape m_center;
	std::vector<sf::CircleShape> m_vertices;
	sf::CircleShape m_influenceRadius;
	sf::Font m_font;
	sf::Text m_idText;

	int m_id;

	sf::Color m_originalColor;
	float m_blinkingRate = 2.0f;

	int m_closestFirefly = -1;
	std::vector<int> m_neighbours;

	bool m_selected = false;

	sf::Clock m_clock;
	float m_previousTime = 0.0f;
	float m_currentTime = 0.0f;

	float m_urgeToBlink = 0.0f;

	float m_phase = 0.0f;
};
