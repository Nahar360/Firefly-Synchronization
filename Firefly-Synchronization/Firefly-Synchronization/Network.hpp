#include "Firefly.hpp"

#include <vector>

#pragma once

class CNetwork
{
public:
	CNetwork() = default;
	~CNetwork() = default;

	void Init(const int& numFireflies);
	void Update(sf::RenderWindow& window);
	void Scan();
	void MouseDetection(sf::Mouse::Button mouseButton, sf::Vector2i mousePos);

	void Test();

	void UpdateFirefliesColor(sf::Color color);

	void UpdateFirefliesBlinkingRate(float blinkingRate);
	void UpdateFirefliesBlinkingDuration(float blinkingDuration);
	void ResetBlinkingClock();

	void CreateFirefly();
	void DeleteFirefly();

	std::vector<CFirefly> GetFireflies() const;

	void ShowLines(const bool& show);
	void ShowVertices(const bool& show);

private:
	std::vector<CFirefly> m_fireflies;

	bool m_showLines = false;
};

