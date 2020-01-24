#pragma once
#include <SFML/Graphics.hpp>

class EventHandler
{
public:
	EventHandler();
	~EventHandler();

	void ProcessEvents(sf::RenderWindow* window);

	bool IsKeyPressed(sf::Keyboard::Key keycode);
	bool IsMousePressed(sf::Mouse::Button button);
};

