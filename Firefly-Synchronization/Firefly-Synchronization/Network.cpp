#include "Network.hpp"

#include "GlobalSettings.hpp"

#include <vector>
#include <iostream>

void CNetwork::Init(const int& numFireflies)
{
	m_fireflies.clear();

	for (int i = 0; i < numFireflies; i++)
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

	if (m_showLines)
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
	if (m_fireflies.size() > 1)
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
}

void CNetwork::UpdateFirefliesColor(sf::Color color)
{
	for (int i = 0; i < m_fireflies.size(); i++)
	{
		m_fireflies[i].UpdateColor(color);
	}
}

void CNetwork::UpdateFirefliesBlinkingRate(float blinkingRate)
{
	for (int i = 0; i < m_fireflies.size(); i++)
	{
		m_fireflies[i].SetBlinkingRate(blinkingRate);
	}
}

void CNetwork::UpdateFirefliesBlinkingDuration(float blinkingDuration)
{
	for (int i = 0; i < m_fireflies.size(); i++)
	{
		m_fireflies[i].SetBlinkingDuration(blinkingDuration);
	}
}

void CNetwork::ResetBlinkingClock()
{
	for (int i = 0; i < m_fireflies.size(); i++)
	{
		m_fireflies[i].ResetBlinking();
	}
}

void CNetwork::CreateFirefly()
{
	CFirefly firefly(m_fireflies.size() + 1);
	firefly.Init(75.0f, 75.0f); // default position, top left corner
	m_fireflies.push_back(firefly);
}

void CNetwork::DeleteFirefly()
{
	if (m_fireflies.size() > 0)
	{
		m_fireflies.pop_back();
	}
}

void CNetwork::MouseDetection(sf::Mouse::Button mouseButton, sf::Vector2i mousePos)
{
	for (int i = 0; i < m_fireflies.size(); i++)
	{
		if (m_fireflies[i].MouseDetection(mouseButton, mousePos))
		{
			m_fireflies[i].SetSelected(true);

			break;
		}
		else
		{
			m_fireflies[i].SetSelected(false);
		}
	}
}

std::vector<CFirefly> CNetwork::GetFireflies() const
{
	return m_fireflies;
}

void CNetwork::ShowLines(const bool& show)
{
	m_showLines = show;
}

void CNetwork::ShowVertices(const bool& show)
{
	for (int i = 0; i < m_fireflies.size(); i++)
	{
		m_fireflies[i].SetShowVertices(show);
	}
}