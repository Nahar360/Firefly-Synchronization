#include <SFML/Graphics.hpp>

#include "Network.hpp"

class CGame
{
public:
	CGame();
	~CGame() = default;

	void Run();

	void CheckEvents();
	void CheckInput();

	void HandleUi(sf::Clock deltaClock);
	void UiWidgets();
	void UpdateWindowTitle();
	void UpdateMousePosition();
	void InitialiseNetwork();
	void UpdateBackgroundColor();
	void UpdateFirefliesColor();
	void UpdateShowLines();
	void UpdateShowVertices();
	void UpdateBlinkingRate();
	void UpdateBlinkingDuration();
	void ResetBlinkingClock();
	void CreateFirefly();
	void DeleteFirefly();
	void ListFireflies();

private:
	sf::RenderWindow m_window;

	int m_numFireflies = 5;

	char m_windowTitle[255];
	sf::Color m_backgroundColor = { 105, 105, 105 };
	float m_backgroundColorInput[3] = { 0.41f, 0.41f, 0.41f };

	float m_firefliesColorInput[3] = { 1.f, 0.f, 0.f };

	bool m_showLines = false;
	bool m_showVertices = false;

	float m_blinkingRate = 2.0f;
	float m_blinkingDuration = 0.1f;

	CNetwork m_network;
};

