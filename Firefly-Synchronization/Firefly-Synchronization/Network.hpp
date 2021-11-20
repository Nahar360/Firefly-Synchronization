#include "Firefly.hpp"

#include <vector>

#pragma once

class CNetwork
{
public:
	CNetwork() = default;
	~CNetwork() = default;

	void Init();
	void Update(sf::RenderWindow& window);
	void Scan();
	void MouseDetection(sf::Mouse::Button mouseButton, sf::Vector2i mousePos);
	void UnselectAllFireflies();

	void UpdateFirefliesColor(sf::Color color);

	void ResetBlinkingClock();

	void CreateFirefly();
	void DeleteFirefly();
	void ClearFireflies();

	CFirefly& GetFirefly(const int id);
	std::vector<CFirefly> GetFireflies() const;

private:
	std::vector<CFirefly> m_fireflies;

	void TransmitPulse(CFirefly& firefly);
	void ShowLines(sf::RenderWindow& window);
};

