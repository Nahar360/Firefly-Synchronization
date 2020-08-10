#include "Network.hpp"

#include "GlobalSettings.hpp"

#include <vector>
#include <iostream>

void CNetwork::Init(const int& numFireflies, const float& firefliesRadius, const float& influenceRadius)
{
	m_fireflies.clear();

	for (int i = 0; i < numFireflies; i++)
	{
		CFirefly firefly(i + 1);
		firefly.Init(firefliesRadius, influenceRadius);

		const float minRate = 1.0f;
		const float maxRate = 5.0f;
		float blinkingRate = minRate + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxRate - minRate)));
		firefly.SetBlinkingRate(blinkingRate);

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

	if (m_showLinesOption != 0)
	{
		std::vector<sf::Vertex> lines;
		for (int i = 0; i < m_fireflies.size(); i++)
		{
			std::vector<int> neighbours = m_fireflies[i].GetNeighbours();
			for (int j = 1; j < m_fireflies.size(); j++)
			{
				const bool isNeighbour = (std::find(neighbours.begin(), neighbours.end(), m_fireflies[j].GetId()) != neighbours.end());
				if (isNeighbour)
				{
					lines.push_back(sf::Vertex(sf::Vector2f(m_fireflies[i].GetPosition().x, m_fireflies[i].GetPosition().y), sf::Color::Green));
					lines.push_back(sf::Vertex(sf::Vector2f(m_fireflies[j].GetPosition().x, m_fireflies[j].GetPosition().y), sf::Color::Green));
				}
				else if (m_showLinesOption == 1)
				{
					lines.push_back(sf::Vertex(sf::Vector2f(m_fireflies[i].GetPosition().x, m_fireflies[i].GetPosition().y), sf::Color::Red));
					lines.push_back(sf::Vertex(sf::Vector2f(m_fireflies[j].GetPosition().x, m_fireflies[j].GetPosition().y), sf::Color::Red));
				}
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
			// Distance to other fireflies
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

			// Calculate closest firefly
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

			// Calculate neighbours (inside the influence radius)
			std::vector<int> neighbours;

			for (int j = 0; j < distancesToOtherFireflies.size(); j++)
			{
				// TODO:
				// m_fireflies[i].GetInfluenceRadius() does the job for now,
				// because all influence radii are the same.
				// I should loop all the rest of the fireflies here as well
				if (distancesToOtherFireflies[j].second <= m_fireflies[i].GetInfluenceRadius())
				{
					neighbours.push_back(distancesToOtherFireflies[j].first);
				}
			}

			m_fireflies[i].SetNeighbours(neighbours);
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
	firefly.Init(50.0f, 150.0f, 75.0f, 75.0f); // default position, top left corner

	m_fireflies.push_back(firefly);
}

void CNetwork::DeleteFirefly()
{
	if (m_fireflies.size() > 0)
	{
		m_fireflies.pop_back();
	}
}

void CNetwork::ClearFireflies()
{
	m_fireflies.clear();
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

void CNetwork::SetShowLinesOption(const int& option)
{
	m_showLinesOption = option;
}

void CNetwork::ShowInfluenceRadius(const bool& show)
{
	for (int i = 0; i < m_fireflies.size(); i++)
	{
		m_fireflies[i].SetShowInfluenceRadius(show);
	}
}

void CNetwork::ShowVertices(const bool& show)
{
	for (int i = 0; i < m_fireflies.size(); i++)
	{
		m_fireflies[i].SetShowVertices(show);
	}
}