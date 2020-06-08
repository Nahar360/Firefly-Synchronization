#include "Firefly.hpp"

#include <vector>

class CNetwork
{
public:
	CNetwork() = default;
	~CNetwork() = default;

	void Init();
	void Update(sf::RenderWindow& window);
	void Scan();
	void MouseDetection(sf::Mouse::Button mouseButton, sf::Vector2i mousePos);

	void Test();

	std::vector<CFirefly> GetFireflies() const;

private:
	std::vector<CFirefly> m_fireflies;

	bool m_debugShowLines = false;
};

