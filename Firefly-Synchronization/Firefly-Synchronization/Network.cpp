#include "Network.hpp"

#include "GlobalSettings.hpp"

#include <vector>
#include <iostream>

void CNetwork::Init()
{
	for (int i = 0; i < NUM_FIREFLIES; i++)
	{
		CFirefly firefly(i + 1);
		firefly.Init();

		m_fireflies.push_back(firefly);
	}
}

void CNetwork::Update(sf::RenderWindow& window)
{
	Scan();

	for (int i = 0; i < m_fireflies.size(); i++)
	{
		m_fireflies[i].Update(window);
	}

	if (m_debugShowLines)
	{
		std::vector<sf::Vertex> lines;
		for (int i = 0; i < m_fireflies.size(); i++)
		{
			for (int j = 1; j < m_fireflies.size(); j++)
			{
				lines.push_back(sf::Vertex(sf::Vector2f(m_fireflies[i].GetPosition().x, m_fireflies[i].GetPosition().y)));
				lines.push_back(sf::Vertex(sf::Vector2f(m_fireflies[j].GetPosition().x, m_fireflies[j].GetPosition().y)));
			}
		}

		window.draw(&lines[0], lines.size(), sf::Lines);
	}
}

void CNetwork::Scan()
{
	for (int i = 0; i < m_fireflies.size(); i++)
	{
		std::vector<std::pair<int, float>> distancesToOtherFireflies;
		for (int j = 0; j < m_fireflies.size(); j++)
		{
			if (m_fireflies[i].GetId() != m_fireflies[j].GetId())
			{
				float distance = sqrt(
					(powf((m_fireflies[i].GetPosition().x - m_fireflies[j].GetPosition().x), 2)) +
					(powf((m_fireflies[i].GetPosition().y - m_fireflies[j].GetPosition().y), 2))
				);

				distancesToOtherFireflies.push_back(std::make_pair(m_fireflies[j].GetId(), distance));
			}
		}

		int closestFirefly = -1;
		float minDistance = distancesToOtherFireflies[0].second;
		for (int j = 0; j < distancesToOtherFireflies.size(); j++)
		{
			if (distancesToOtherFireflies[j].second <= minDistance)
			{
				closestFirefly = distancesToOtherFireflies[j].first;
				minDistance = distancesToOtherFireflies[j].second;
			}
		}

		m_fireflies[i].SetClosestFirefly(closestFirefly);
	}
}

void CNetwork::Test()
{
	for (int i = 0; i < m_fireflies.size(); i++)
	{
		std::cout << "Firefly: " << m_fireflies[i].GetId() << ". Closest: " << m_fireflies[i].GetClosestFirefly() << std::endl;
	}

	std::cout << std::endl;
}

void CNetwork::MouseDetection(sf::Mouse::Button mouseButton, sf::Vector2i mousePos)
{
	for (int i = 0; i < m_fireflies.size(); i++)
	{
		if (m_fireflies[i].MouseDetection(mouseButton, mousePos))
		{
			break;
		}
	}
}

std::vector<CFirefly> CNetwork::GetFireflies() const
{
	return m_fireflies;
}