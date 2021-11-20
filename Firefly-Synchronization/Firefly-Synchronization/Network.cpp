#include "Network.hpp"

#include "GlobalSettings.hpp"
#include "UiSettings.hpp"

#include <vector>
#include <iostream>

void CNetwork::Init()
{
	m_fireflies.clear();

	for (int i = 0; i < NUM_FIREFLIES; i++)
	{
		CFirefly firefly(i + 1);
		firefly.Init();

		const float blinkingRate = MIN_BLINKING_RATE + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (MAX_BLINKING_RATE - MIN_BLINKING_RATE))); // 2.2f
		firefly.SetBlinkingRate(blinkingRate);

		m_fireflies.push_back(firefly);
	}
}

void CNetwork::Update(sf::RenderWindow& window)
{
	Scan();

	for (size_t i = 0; i < m_fireflies.size(); i++)
	{
		m_fireflies[i].Draw(window);

		m_fireflies[i].RunPhaseFunction();

		if (m_fireflies[i].HasBlinked())
		{
			std::cout << "Firefly " << GetFirefly(i + 1).GetId() << " has blinked." << std::endl;
			auto fireflyThatHasBlinked = GetFirefly(i + 1);
			TransmitPulse(fireflyThatHasBlinked);
		}
	}

	ShowLines(window);
}

void CNetwork::TransmitPulse(CFirefly& firefly)
{
	std::cout << "Transmitting pulse of firefly " << firefly.GetId() << " to: " << std::endl;

	// Get neighbours of firefly that has blinked
	std::vector<int> neighbours = firefly.GetNeighbours();
	for (size_t i = 0; i < neighbours.size(); i++)
	{
		std::cout << "Neighbour " << GetFirefly(neighbours[i]).GetId() << std::endl;
		auto neighbour = GetFirefly(neighbours[i]);

		float phase = neighbour.GetPhase();

		const float bDissipationFactor = BLINKING_DURATION;
		const float eAmplitudeIncrement = 0.1f;
		float alpha = exp(bDissipationFactor * eAmplitudeIncrement);
		float beta = (exp(bDissipationFactor * eAmplitudeIncrement) - 1) / (exp(bDissipationFactor) - 1);
		float phaseRespondCurve = 1;
		if (alpha * phase + beta < 1)
		{
			phaseRespondCurve = alpha * phase + beta;
		}
		float deltaPhase = phaseRespondCurve;
		phase += deltaPhase;

		std::cout << "New phase: " << phase << std::endl;

		// TODO:
		// This does not work
		// but it should!!
		// neighbour.SetPhase(phase);

		// Quick fix for this last thing...
		for (size_t j = 0; j < m_fireflies.size(); j++)
		{
			if (j + 1 == neighbour.GetId())
			{
				m_fireflies[j].SetUrgeToBlink(phase + phase * 0.1f);
			}
		}
	}

	std::cout << std::endl << std::endl;
}

// Alex's TransmitPulse
/*void CNetwork::TransmitPulse(CFirefly& firefly)
{
	std::vector<int> neighbours = firefly.GetNeighbours();
	for (size_t i = 0; i < neighbours.size(); i++)
	{
		CFirefly& neighbour = GetFirefly(neighbours[i]);
		float phase = neighbour.GetPhase();

		phase += 0.01f;
		phase = std::min(phase, firefly.GetBlinkingRate());
		neighbour.SetPhase(phase);
	}

	std::cout << std::endl << std::endl;
}*/

void CNetwork::Scan()
{
	if (m_fireflies.size() > 1)
	{
		for (size_t i = 0; i < m_fireflies.size(); i++)
		{
			std::vector<std::pair<int, float>> distancesToOtherFireflies;
			std::vector<int> neighbours;
			for (size_t j = 0; j < m_fireflies.size(); j++)
			{
				if (m_fireflies[i].GetId() != m_fireflies[j].GetId())
				{
					float distance = sqrt(
						(powf((m_fireflies[i].GetPosition().x - m_fireflies[j].GetPosition().x), 2)) +
						(powf((m_fireflies[i].GetPosition().y - m_fireflies[j].GetPosition().y), 2))
					);

					// Distance to other fireflies
					distancesToOtherFireflies.push_back(std::make_pair(m_fireflies[j].GetId(), distance));

					// Calculate neighbours (inside the influence radius)
					if (distance <= m_fireflies[i].GetInfluenceRadius())
					{
						neighbours.push_back(m_fireflies[j].GetId());
					}
				}
			}

			m_fireflies[i].SetNeighbours(neighbours);

			// Calculate closest firefly
			int closestFirefly = -1;
			float minDistance = distancesToOtherFireflies[0].second;
			for (size_t j = 0; j < distancesToOtherFireflies.size(); j++)
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

void CNetwork::ShowLines(sf::RenderWindow& window)
{
	if (m_fireflies.size() > 1 && SHOW_LINES_OPTION != 0)
	{
		std::vector<sf::Vertex> lines;
		for (size_t i = 0; i < m_fireflies.size(); i++)
		{
			std::vector<int> neighbours = m_fireflies[i].GetNeighbours();
			for (size_t j = 1; j < m_fireflies.size(); j++)
			{
				const bool isNeighbour = (std::find(neighbours.begin(), neighbours.end(), m_fireflies[j].GetId()) != neighbours.end());
				if (isNeighbour)
				{
					lines.push_back(sf::Vertex(sf::Vector2f(m_fireflies[i].GetPosition().x, m_fireflies[i].GetPosition().y), sf::Color::Green));
					lines.push_back(sf::Vertex(sf::Vector2f(m_fireflies[j].GetPosition().x, m_fireflies[j].GetPosition().y), sf::Color::Green));
				}
				else if (SHOW_LINES_OPTION == 1)
				{
					lines.push_back(sf::Vertex(sf::Vector2f(m_fireflies[i].GetPosition().x, m_fireflies[i].GetPosition().y), sf::Color::Red));
					lines.push_back(sf::Vertex(sf::Vector2f(m_fireflies[j].GetPosition().x, m_fireflies[j].GetPosition().y), sf::Color::Red));
				}
			}
		}

		if (lines.size() > 0)
		{
			window.draw(&lines[0], lines.size(), sf::Lines);
		}
	}
}

void CNetwork::UpdateFirefliesColor(sf::Color color)
{
	for (size_t i = 0; i < m_fireflies.size(); i++)
	{
		m_fireflies[i].UpdateColor(color);
	}
}

void CNetwork::ResetBlinkingClock()
{
	for (size_t i = 0; i < m_fireflies.size(); i++)
	{
		m_fireflies[i].ResetBlinking();
	}
}

void CNetwork::CreateFirefly()
{
	CFirefly firefly(m_fireflies.size() + 1);
	firefly.Init(75.0f, 75.0f); // default position, top left corner

	float blinkingRate = MIN_BLINKING_RATE + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (MAX_BLINKING_RATE - MIN_BLINKING_RATE))); // 1.0f
	firefly.SetBlinkingRate(blinkingRate);

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
	for (size_t i = 0; i < m_fireflies.size(); i++)
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

void CNetwork::UnselectAllFireflies()
{
	for (size_t i = 0; i < m_fireflies.size(); i++)
	{
		if (m_fireflies[i].GetSelected())
		{
			m_fireflies[i].SetSelected(false);
		}
	}
}

CFirefly& CNetwork::GetFirefly(const int id)
{
	return m_fireflies[id - 1];
}

std::vector<CFirefly> CNetwork::GetFireflies() const
{
	return m_fireflies;
}
