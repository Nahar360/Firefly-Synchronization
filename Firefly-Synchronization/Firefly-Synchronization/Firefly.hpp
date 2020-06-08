#include <SFML/Graphics.hpp>

class CFirefly
{
public:
	CFirefly(int id);
	~CFirefly() = default;

	void Init();
	void SetVertices(sf::Vector2f center);
	void Update(sf::RenderWindow& window);
	void UpdatePosition(float x, float y);
	void UpdateRotation();
	bool MouseDetection(sf::Mouse::Button mouseButton, sf::Vector2i mousePos);

	sf::CircleShape GetFirefly() const;

	sf::Color GetColor() const;

	int GetId() const;

	sf::Vector2f GetPosition() const;

	void SetClosestFirefly(const int& closestFirefly);
	int GetClosestFirefly() const;

private:
	sf::CircleShape m_firefly;
	sf::CircleShape m_center;
	std::vector<sf::CircleShape> m_vertices;

	int m_id;
	
	sf::Font m_font;
	sf::Text m_idText;

	int m_closestFirefly = -1;

	bool m_debugShowVertices = false;

	sf::Clock m_clock;
};
