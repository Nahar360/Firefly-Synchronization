#include <SFML/Graphics.hpp>

#include "Network.hpp"

class CGame
{
public:
	CGame();
	~CGame() = default;

	void Init();
	void Update();
	void CheckEvents();
	void CheckInput();

private:
	sf::RenderWindow m_window;

	CNetwork m_network;
};

